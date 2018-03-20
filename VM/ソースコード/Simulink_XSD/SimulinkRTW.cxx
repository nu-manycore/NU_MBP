#include <iostream>
#include <sstream>
#include <stack>
#include <deque>

#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#include <assert.h>

#include "SimulinkRTW.h"

#ifdef __GNUC__
// TR1 using BOOST Project
// libstdc++-V3 doesn't work
#include <boost/tr1/regex.hpp>
#else
#include <tr1/regex>
#endif

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif	// USE_DMALLOC

#ifndef PARSE_CODE_DEBUG
#define PARSE_CODE_DEBUG	0
#endif	// PARSE_CODE_DEBUG

#ifndef PROCESS_VAR_DEBUG
#define PROCESS_VAR_DEBUG	0
#endif	// PROCESS_VAR_DEBUG

#ifndef PARSE_CODE_INITIALIZE_DEBUG
#define PARSE_CODE_INITIALIZE_DEBUG	0
#endif	// PARSE_CODE_INITIALIZE_DEBUG

#ifndef CHANGE_EXTERNALS_DEBUG
#define CHANGE_EXTERNALS_DEBUG	0
#endif

#ifndef CHECK_CALL_FUNCTION_DEBUG
#define CHECK_CALL_FUNCTION_DEBUG	0
#endif

#ifndef XML_USENAME_DEBUG
#define XML_USENAME_DEBUG		1
#endif

#ifndef PARSE_DATA_DEBUG
#define PARSE_DATA_DEBUG		0
#endif

#ifndef TRACK_BUS_DEBUG
#define TRACK_BUS_DEBUG			0
#endif

#define HANDLE_RATETRANSISION	0 // Do *NOT* handle RateTransision

// Mux/Demux has multiple inputs, there have  different variable name.
#define MUX_DIFFERENT_LINE_USENAME	(1<<0)
#define DEMUX_DIFFERENT_LINE_USENAME	(1<<1)

#ifndef DIFFERENT_LINE_USENAME
#define DIFFERENT_LINE_USENAME		3 // MUX=1, DEMUX=2
#endif

#if TRACK_BUS_DEBUG
    static inline void
    track_bus_debug_log(const char* func, int line,
			const std::string& VN,
			const SimulinkModel::XSD::block_T* B,
			const Simulink::RTW::NewMux* M, const char* C)
    {
      std::cout << func << ':' << line << ':' << VN
		<< ':' << B->blocktype () << ':' << B->name () << ':';
      if (M)
	std::cout << *M;
      else
	std::cout << "NOMUX";
      std::cout << '@' << C << std::endl;
    }
#define TRACK_BUS_DEBUG_LOG(VN,B,M,C)	\
  track_bus_debug_log(__func__, __LINE__, VN, B, M, C)
#else
#define TRACK_BUS_DEBUG_LOG(VN,B,M,C)	do { ; } while (0)
#endif

using namespace SimulinkModel;
using namespace SimulinkModel::XSD;

namespace Simulink
{
  namespace RTW
  {
    typedef std::pair < c_variable_map_iterator, bool > c_var_res;
    typedef std::pair < block_map_iterator, bool > block_res;
    typedef std::pair < blockname_map_iterator, bool > blockname_res;
    typedef std::pair < goto_map_iterator, bool > goto_res;
    typedef std::pair < from_map_iterator, bool > from_res;

    // File Coding System Types
    enum FileCoding { FC_NONE, FC_U8, FC_U16B, FC_U16L, FC_U32B, FC_U32L };
    static const char* const FileCodingName[] = {
      "NONE", "UTF-8", "UTF-16BE", "UTF-16LE", "UTF-32BE", "UTF-32LE",
    };

    //
    // Functions
    //

    void* mapfile (int fd, size_t& size);
    void* mapfile (const char* f, size_t& size);
    int unmapfile (void* addr, size_t size);

    FileCoding check_file_coding (void* addr, void*& skiped);

    const char* skip_empty_line (const char* line,
				 size_t size, unsigned int& lineno);

    const char* skip_space (const char* line, size_t size,
			    unsigned int& lineno);
    const char* skip_space (const char* line, size_t size);
    const char* skip_space (const char* line);

    const char*
    nextline (const char* line, size_t& size, unsigned int& lineno);

    // Greatest Common Divisor
    int
    gcd (int x, int y)
    {
      int t;

      // euclidean algorithm
      while (y != 0)
	{
	  t = x % y;
	  x = y;
	  y = t;
	}
      return x;
    }

    //
    // class BlocknameEnt
    //

    void
    BlockNameEnt::dump (void) const
    {
      std::cout << "System=" << bn
		<< ", Layer=" << ln
		<< ", Symbol=" << sn
		<< std::endl;
    }


    void
    BlockNameEnt::set (const char *name, const char *layer, const char *symbol)
    {
      bn = name;
      ln = layer;
      if (symbol == NULL)
	{
	  char buf[strlen (layer) + 1];
	  char *s = buf;
	  for (const char *p = layer; *p; p++)
	    {
#if 1
	      if (isalnum ((unsigned char)*p) || *p == '_')
		*s++ = *p;
	      else if (*p == '-' || *p == '/')
		*s++ = '_';
#else
	      if (isspace ((unsigned char)*p) || *p == '(' || *p == ')')
		;		// skip
	      else if (isalnum ((unsigned char)*p)
		       || *p == '-' || *p == '_')
		*s++ = *p;
	      else
		*s++ = '_';
#endif
	    }
	  *s++ = 0;
	  sn = buf;
	}
      else
	sn = symbol;
    }


    BlockNameEnt::BlockNameEnt()
      : top (NULL), ref (false), ubne (NULL), bi (NULL)
    {
    }

    BlockNameEnt::BlockNameEnt(const char* name, const char* layer,
			       const char *symbol)
      : top (NULL), ref (false), ubne (NULL), bi (NULL)
    {
      set (name, layer, symbol);
    }

    BlockNameEnt::~BlockNameEnt()
    {
    }

    //
    // class VariableEnt
    //

    unsigned int VariableEnt::seq = 0;


    VariableEnt::VariableEnt()
      : v_mode(V_AUTO), v_array_size (0), v_binfo (NULL), v_codeline (-1)
    {
      v_id = seq++;
    }


    VariableEnt::VariableEnt(const std::string& t, const std::string& n,
			     const std::string& p, const std::string& a,
			     unsigned int z, var_mode m)
      : v_binfo (NULL), v_codeline (-1)
    {
      v_id = seq++;
      set (t, n, p, a, z, m);
    }


    VariableEnt::VariableEnt(const std::string& t, const std::string& n,
			     const std::string& p, const std::string& a,
			     unsigned int z,
			     const std::string& s, const std::string& b,
			     var_mode m)
      : v_binfo (NULL), v_codeline (-1)
    {
      v_id = seq++;
      set (t, n, p, a, z, s, b, m);
    }


    VariableEnt::~VariableEnt()
    {
    }


    void
    VariableEnt::set(const std::string& t, const std::string& n,
		     const std::string& p, const std::string& a,
		     unsigned int z, var_mode m)
    {
      v_mode = m;
      v_type = t;
      v_name = n;
      v_ptr = p;
      v_array = a;
      v_array_size = z;
    }

    void
    VariableEnt::set(const std::string& t, const std::string& n,
		     const std::string& p, const std::string& a,
		     unsigned int z,
		     const std::string& s, const std::string& b,
		     var_mode m)
    {
      set(t, n, p, a, z, m);
      v_sys = s;
      v_block = b;
    }

    void
    VariableEnt::dump (void) const
    {
      std::cout << "Type=" << type()
		<< ", Var=" << ptr() << name() << array();
      int n = 0;
      if (is_ptr())
	n |= 1;
      if (is_array())
	n |= 2;
      if (n)
	{
	  std::cout << "<";
	  if (n & 1)
	    std::cout << "Pointer";
	  if (n == 3)
	    std::cout << ",";
	  if (n & 2)
	    std::cout << "ARRAY[" << array_size() << ']';
	  std::cout << ">";
	}
      std::cout << ", id=[" << id() << "]";
      if (system().length() && block().length())
	std::cout << ", Sys=" << system() << "/" << block();
      std::cout << std::endl;
    }


    // class BlockLink

    BlockLink::BlockLink ()
      : l_type (TYPE_NONE), l_block (NULL), l_linkpair (NULL)
    {
    }


    BlockLink::BlockLink (BlockInfo *b, link_type t)
      : l_type (t), l_block (b), l_linkpair (NULL)
    {
    }


    BlockLink::BlockLink (BlockInfo *b, std::string& l, link_type t)
      : l_type (t), l_label (l), l_block (b), l_linkpair (NULL)
    {
    }

    BlockLink::~BlockLink ()
    {
    }

    // for algorithm (find)
    bool
    BlockLink::operator== (const BlockInfo* x) const
    {
      return l_block == x;
    }

    bool
    BlockLink::operator== (const BlockLink& x) const
    {
      return l_block == x.l_block;
    }

#if 0
    // NewVecPartList
    void
    NewVecPartList::merge (void)
    {
      size_t c = size ();

      if (c <= 1)
	return;

      std::sort (begin (), end ());

      size_t p = 0;
      size_t nc = c;
      for (size_t n = 1; n < c; n++)
	{
	  NewVecPart& p_vp = (*this)[p];
	  NewVecPart& n_vp = (*this)[n];
	  size_t p_end = p_vp.index () + p_vp.count ();
	  if (n_vp.index () <= p_end)
	    {
	      // merge
	      size_t n_end = n_vp.index () + n_vp.count ();
	      if (p_end < n_end)
		p_vp.count () = n_end - p_vp.index ();
	      n_vp.count () = 0; // invalid
	      nc--;
	    }
	  else
	    p = n;
	}

      // move valid data and resize
      if (nc < c)
	{
	  size_t t = 1;
	  for (size_t f = 1; f < c; f++)
	    {
	      NewVecPart& n_vp = (*this)[f];
	      if (n_vp.count () == 0)
		continue;
	      if (t != f)
		(*this)[t] = n_vp;
	      t++;
	    }
	  resize (nc);
	}
    }
#endif

    std::ostream&
    operator<< (std::ostream& os, const NewVecElem& e)
    {
      const IOPort* iop = e.port ();
      os << "VE{";
      if (iop)
	os << iop->ioport_tag ()->line ();
      else
	os << "<none>";
      os << '[' << e.index () << "]}";
      return os;
    }

    std::ostream&
    operator<< (std::ostream& os, const NewMux& m)
    {
      os << "M[" << m.mux_size () << ','
	 << m.index () << ':' << m.count () << "]{";
      for (size_t i = 0; i < m.size (); i++)
	{
	  if (i)
	    os << ';';
	  os << i << ':' << m[i];
	}
      os << "}";
      return os;
    }

    std::ostream&
    operator<< (std::ostream& os, const NewSignal& s)
    {
      os << "S";

      if (s.get_type () == NewSignal::None)
	os << "{n}";
      else if (s.get_type () == NewSignal::Scalar)
	os << "{s}";
      else
	os << "{v}";

      const IOPort* iop = s.port ();
      if (iop)
	os << iop->ioport_tag ()->line ();

      if (s.get_type () == NewSignal::Vector)
	{
	  os << '[';
	  for (size_t i = 0; i < s.dim_size (); i++)
	    {
	      size_t x = s[i];
	      if (i)
		os << ':';
	      os << x;
	    }
	  os << ']';
	}
      return os;
    }

    std::ostream&
    operator<< (std::ostream& os, const NewMuxElem& e)
    {
      const NewSignal* s = e;
      if (s)
	return os << *s;

      const NewVecElem* ve = e;
      if (ve)
	return os << *ve;

      os << "<>";

      return os;
    }

    bool NewMux::mux_to_chunks (NewVarChunks& chunks)
    {
      NewVarChunks tmp;
      size_t pend = 0;
      int pind = -1;
      size_t mend = m_index + m_count;
      for (size_t i = m_index; i < mend; i++)
	{
	  NewMuxElem& me = (*this)[i];

	  if (me.type () == NewMuxElem::None)
	    continue;

	  if (me.type () == NewMuxElem::Signal)
	    {
	      chunks.push_back (NewVarChunkElement (-1, -1));
	      return true;
	    }

	  NewVecElem *ve = me;
	  size_t s = ve->index ();
	  if (pind != -1 && pend == s)
	    tmp[pind].end () = ++pend;
	  else
	    {
	      pend = s + 1;
	      tmp.push_back (NewVarChunkElement (s, pend));
	      pind++;
	    }
	}

      if (tmp.size () == 0)
	return false;

      if (tmp.size () > 1)
	{
	  std::sort (tmp.begin (), tmp.end ());

	  size_t tind = 0;
	  for (size_t i = 1; i < tmp.size (); i++)
	    {
	      NewVarChunkElement& n = tmp[i];

	      if (tmp[tind].do_merge (n))
		continue;

	      tind++;
	      if (tind != i)
		tmp[tind] = n;
	    }

	  if (tind + 1 < tmp.size ())
	    tmp.resize (tind + 1);
	}

      for (size_t i = 0; i < tmp.size (); i++)
	{
	  if (tmp[i].start () + 1 == tmp[i].end ())
	    tmp[i].end () = -1;
	}

      chunks.swap (tmp);
      return true;
    }

    bool NewSignal::dim_parse (const char*& p)
    {
      // dim
      char *x;
      size_t d = (size_t)strtoul (p, &x, 0);
      if (d == 0 && (*x != 0 && !isspace (*(unsigned char*)x)))
	return false;

      resize (d);

      p = skip_space (x);

      for (size_t idx = 0; idx < d; idx++)
	{
	  // size
	  size_t sz = (size_t)strtoul (p, &x, 0);
	  if (sz == 0
	      && (*x != 0 && *x != ']' && !isspace (*(unsigned char*)x)))
	    {
	      clear ();
	      return false;
	    }

	  (*this)[idx] = sz;

	  if (idx == 0)
	    {
	      if (d == 1 && sz == 1)
		set_type (NewSignal::Scalar);
	      else
		set_type (NewSignal::Vector);
	    }

	  p = skip_space (x);
	}

      return true;
    }

    //
    // NewBus
    //
    bool
    NewBus::dim_parse (const std::string& dim)
    {
      const char* p = dim.c_str ();
      if (*p++ != '[')
	return false;

      p = skip_space (p);
      if (p == NULL)
	return false;

      if (p[0] == '-' && p[1] == '2')
	{
	  // bus
	  set_type (Bus);

	  p = skip_space (p + 2);
	  if (p == NULL)
	    return false;

	  char *x;
	  size_t bus_dim = (size_t)strtoul (p, &x, 0);
	  if (bus_dim == 0 && (*x != 0 && !isspace (*(unsigned char*)x)))
	    return false;

	  resize (bus_dim);

	  p = skip_space (x);

	  size_t bus_idx = 0;
	  for (; bus_idx < bus_dim; bus_idx++)
	    {
	      if (*p == ']')
		break;

	      if (!(*this)[bus_idx].dim_parse (p))
		{
		  clear ();
		  return false;
		}
	    }

	  if (bus_idx != bus_dim)
	    {
	      clear ();
	      return false;
	    }

	  return true;
	}

      // signal
      set_type (Signal);

      resize (1);

      if (!(*this)[0].dim_parse (p) || *p != ']')
	{
	  clear ();
	  return false;
	}

      return true;
    }

    //
    // oldBusEnt (obsoleted)
    //
    bool
    oldBusEnt::operator< (const oldBusEnt& x) const
    {
      if (bi_port == x.bi_port)
	{
	  int xs = x.bi_index * bi_total;
	  int  s = bi_index * x.bi_total;
	  if (s == xs)
	    {
	      xs += x.bi_nchunk * bi_total;
	      s += bi_nchunk * x.bi_total;
	    }
	  return s < xs;
	}
      return bi_port < x.bi_port;
    }

    void
    oldBusEnt::reduction (void)
    {
      int g;

      assert (bi_nchunk > 0 && bi_total > 0);
      if (bi_index == 0)
	g = 1;
      else
	g = gcd (gcd (bi_index, bi_nchunk), bi_total);
      if (g != 1)
	{
	  bi_index /= g;
	  bi_nchunk /= g;
	  bi_total /= g;
	}
    }

    void
    oldBusEnt::dump (void) const
    {
      std::cout << bi_port->ioport_tag ()->port ()
		<< '(' << bi_index;
      if (bi_nchunk > 1)
	std::cout << ".." << bi_index + bi_nchunk;
      std::cout << '/' << bi_total << ')';
    }

    //
    // oldBusLine (obsoleted)
    //
    void
    oldBusLine::join_fragment (void)
    {
      if (size () <= 1)
	return;

      oldBusLine::iterator i (begin ());
      oldBusEnt* prev = &*i++;

      while (i != end ())
	{
	  if (prev->port () == i->port ())
	    {
	      int pend = prev->index () + prev->nchunk ();
	      int ista = i->index ();
	      const int x = gcd (prev->total (), i->total ());
	      const int pmul = i->total () / x;
	      const int imul = prev->total () / x;

	      pend *= pmul;
	      ista *= imul;
	      // join
	      if (pend == ista)
		{
		  prev->index () *= pmul;
		  prev->nchunk () = (prev->nchunk () * pmul
				     + i->nchunk () * imul);
		  prev->total () *= pmul;

		  std::string& punam = prev->usename ();
		  std::string& iunam = i->usename ();
		  if (punam.length () == 0 && iunam.length ())
		    punam = iunam;

		  i = erase (i);
		  continue;
		}
	    }

	  prev->reduction ();

	  prev = &*i++;
	}
    }

    void
    oldBusLine::defrag (oldBusLine& out) const
    {
      if (size () == 0)
	return;

      std::vector <oldBusEnt> v;

      v.reserve (size ());
      for (const_iterator i (begin ()); i != end (); i++)
	v.push_back (*i);
      std::sort (v.begin (), v.end ());

      out.clear ();

      std::vector <oldBusEnt>::iterator i (v.begin ());
      oldBusEnt* prev = &out.add (*i++);

      if (size () == 1)
	return;

      for (; i != v.end (); i++)
	{
	  if (prev->port () == i->port ())
	    {
	      int psta = prev->index ();
	      int pend = psta + prev->nchunk ();
	      int ista = i->index ();
	      int iend = ista + i->nchunk ();

	      const int x = gcd (prev->total (), i->total ());
	      const int pmul = i->total () / x;
	      const int imul = prev->total () / x;

	      psta *= pmul;
	      pend *= pmul;
	      ista *= imul;
	      iend *= imul;

	      // overlap or join
	      assert (psta <= ista);
	      if (ista <= pend)
		{
		  assert (pend <= iend);

		  prev->index () = psta;
		  prev->nchunk () = iend - psta;
		  prev->total () *= pmul;

		  std::string& punam = prev->usename ();
		  std::string& iunam = i->usename ();
		  if (punam.length () == 0 && iunam.length ())
		    punam = iunam;
		  continue;
		}
	    }

	  prev->reduction ();

	  prev = &out.add (*i);
	}
    }

    void
    oldBusLine::reduction (void)
    {
      for (iterator i (begin ()); i != end (); i++)
	i->reduction ();
    }

    //
    // oldBusPort (obsoleted)
    //
    void
    oldBusPort::dump (int indent) const
    {
      const std::string prefix (indent, ' ');

      int index = 0;
      for (const_iterator i (begin ()); i != end (); i++)
	{
	  std::cout << prefix << "[" << index << "] = ";
	  if (i->size () == 0)
	    std::cout << "*NONE*\n";
	  else
	    {
	      const std::string prefix2 (indent + 2, ' ');
	      if (i->size () > 1)
		{
		  std::cout << "{\n";
		  for (oldBusLine::const_iterator j (i->begin ());
		       j != i->end (); j++)
		    {
		      std::cout << prefix2;
		      j->dump ();
		      std::cout << "\n";
		    }
		  std::cout << prefix << "}" << std::endl;
		}
	      else
		{
		  i->begin ()->dump ();
		  std::cout << std::endl;
		}
	    }
	  index++;
	}
    }

    //
    // function_code_T
    //
    const std::string function_code_T::FuncKindName[4] = {
      "none", "init", "task", "update",
    };

    //
    // class BlockInfo
    //

    const std::string BlockLink::l_stype[5] = {
      "none", "port", "event", "data", "init",
    };


    unsigned int BlockInfo::b_seq = 1;

    BlockInfo::BlockInfo(BlockInfo* u, block_T* b, block_type t)
      : b_type (t), b_upper (u), b_tag(b), b_defined (false), b_frag (false),
	b_code_line (0),  b_update_line (0), b_init_code_line (0),
	b_init_state_line (0),
	b_any (0),
	b_trigger_input(NULL),
	b_trigger_port(NULL), b_trigger_from(NULL),
	b_enable_input(NULL),
	b_enable_port(NULL), b_enable_from(NULL),
	b_top_block(NULL), b_bus_visited(false), b_bus_search_result(false)
    {
      b_id = b_seq++;
    }


    BlockInfo::BlockInfo()
      : b_type (TYPE_NORMAL), b_upper(NULL), b_tag(NULL), b_defined (false),
	b_frag (false), b_code_line (0),  b_update_line (0),
	b_init_code_line (0), b_init_state_line (0),
	b_any (0),
	b_trigger_input(NULL),
	b_trigger_port(NULL), b_trigger_from(NULL),
	b_enable_input(NULL),
	b_enable_port(NULL), b_enable_from(NULL),
	b_top_block(NULL), b_bus_visited(false), b_bus_search_result(false)
    {
      b_id = b_seq++;
    }


    BlockInfo::~BlockInfo()
    {
    }


    void
    BlockInfo::make_link (BlockInfo* orig,
			  BlockInfo* f, BlockLinkPairList& pairlist,
			  BlockLink::link_type t)
    {
      BlockInfo* b_top = top_block ();
      BlockInfo* f_top = f->top_block ();

      if (b_top && b_top != this)
	b_top->make_link (orig, f, pairlist, t);
      else if (f_top && f_top != f)
	make_link (orig, f_top, pairlist, t);
      else if (b_top == NULL && merged_to ().size ())
	{
	  // forward先がこのブロックをマージしていない事が条件。
	  // さもないとこのブロックをマージしているforward先同士でリン
	  // クができてしまう
	  blockinfo_list_iterator ibeg = merged_to ().begin ();
	  blockinfo_list_iterator iend = merged_to ().end ();
	  if (std::find (ibeg, iend, f) == iend)
	    {
#if 0
	      // しかもその場合、単一のマージ先でないと、同じデータが二
	      // 系統から届くことになってしまう
	      assert (merged_to ().size () == 1);
#endif
	      for (blockinfo_list_iterator bli (ibeg); bli != iend; bli++)
		(*bli)->make_link (orig, f, pairlist, t);
	    }
	}
      else if (f_top == NULL && f->merged_to ().size ())
	{
	  for (blockinfo_list_iterator bli (f->merged_to ().begin ());
	       bli != f->merged_to ().end (); bli++)
	    make_link (orig, *bli, pairlist, t);
	}
      else if (this != f)
	{
#if 1
	  // 同じデータが二系統から届くことを抑止するため、ここでは既に
	  // mergeされたブロックからのリンクがあれば、警告を出してリン
	  // クせず継続することとする
	  bool already_linked = false;
	  for (BlockLinkPairList::const_iterator i (pairlist.begin ());
	       i != pairlist.end (); i++)
	    {
	      const BlockLinkPair& p = *i;
	      if (orig != this
		  && p.origin () == orig
		  && p.forward ()->blockinfo () == f)
		already_linked = true;
	    }

	  if (already_linked == true)
	    {
	      std::cout << "Warning!" << __LINE__
			<< ": Already Linked "
			<< orig->tag ()->name ()
			<< " into "
			<< tag ()->name ()
			<< " -> "
			<< f->tag ()->name ()
			<< std::endl;
	      return;
	    }
#endif

	  block_link_iterator fp = std::find (forward ().begin(),
					      forward ().end(), f);
	  block_link_iterator bp = std::find (f->backward ().begin (),
					      f->backward ().end (), this);

	  if (fp == forward ().end())
	    {
	      assert (bp == f->backward ().end ());

	      forward ().push_back (BlockLink (f, t));
	      BlockLink* fb = &forward ().back ();

	      f->backward ().push_back (BlockLink (this, t));
	      BlockLink* bb = &f->backward ().back ();

	      fb->linkpair (bb);
	      bb->linkpair (fb);

	      pairlist.push_back (BlockLinkPair (fb, bb, orig));
	    }
	  else
	    {
	      assert (bp != f->backward ().end ());
	      pairlist.push_back (BlockLinkPair (&*fp, &*bp, orig));
	    }
	}
    }

    void
    BlockInfo::make_link (BlockInfo* f, BlockLink::link_type t)
    {
      BlockLinkPairList tmp;
      return make_link (this, f, tmp, t);
    }

#if 0
    BlockLink&
    BlockInfo::add_forward (BlockInfo* f, BlockLink::link_type t)
    {
      block_link_iterator p = std::find (forward ().begin(),
					 forward ().end(), f);
      if (p != forward ().end())
	return *p;
      forward ().push_back (BlockLink (f, t));
      return forward ().back();
    }


    BlockLink&
    BlockInfo::add_backward (BlockInfo* b, BlockLink::link_type t)
    {
      block_link_iterator p = std::find (backward ().begin(),
					 backward ().end(), b);
      if (p != backward ().end())
	return *p;
      backward ().push_back (BlockLink (b, t));
      return backward ().back();
    }
#endif


    void
    BlockInfo::add_storage_if_existing (const Blocks* b, var_T& vt,
					const VariableEnt& ve) const
    {
      var_T::storage_optional so;

      if (ve.mode () == VariableEnt::V_EXTIN)
	so = b->external_input_name ();
      else if (ve.mode () == VariableEnt::V_EXTOUT)
	so = b->external_output_name ();
      else if (ve.mode () == VariableEnt::V_SIGNAL)
	so = b->signal_name();

      if (so)
	vt.storage (so);
    }

    void
    BlockInfo::add_var_tag (const Blocks* b,
			    const block_link& blink,
			    const propagate_var_list& pvl,
			    c_varentp_map& vmap, bool merge)
    {
      block_T* block = tag ();

      for (propagate_var_list_const_iterator pv (pvl.begin());
	   pv != pvl.end(); pv++)
	{
	  for (var_list_const_iterator vie (pv->varent ().begin ());
	       vie != pv->varent ().end (); vie++)
	    {
	      const VariableEnt* ve = *vie;
	      if (ve == NULL || !vie->is_used ())
		continue;

	      bool found = true;
	      if (merge)
		{
		  found = false;
		  for (block_link_const_iterator lci (blink.begin ());
		       lci != blink.end (); lci++)
		    {
		      for (pvar_ptr_list_const_iterator
			     fvi (lci->vars ().begin ());
			   fvi != lci->vars ().end (); fvi++)
			{
			  const var_list& vlist = (*fvi)->varent ();
			  var_list_const_iterator fend = vlist.end ();
			  var_list_const_iterator fbeg = vlist.begin ();
			  if (fend != std::find (fbeg, fend, ve))
			    {
			      found = true;
			      break;
			    }
			}
		    }
		}

	      if (found == true)
		{
		  std::string vn = b->storage_qualifier_variable (*ve);
		  if (vmap.find (vn) == vmap.end ())
		    {
		      block->var().push_back (pv->make_var_tag(ve));
		      add_storage_if_existing (b, block->var().back(), *ve);
		      vmap.insert (std::make_pair (vn, ve));
		    }
		}
	    }
	}
    }

    void
    BlockInfo::complete (const Blocks* b)
    {
      tag()->id (id ());

      if (type () == TYPE_ENABLED_SUBSYSTEM)
	tag()->type (block_T::type_optional ("enabled"));
      else if (type () == TYPE_TRIGGERED_SUBSYSTEM)
	tag()->type (block_T::type_optional ("triggered"));
      else if (type () == TYPE_ENABLED_TRIGGERED_SUBSYSTEM)
	tag()->type (block_T::type_optional ("enabled_triggered"));
      else if (type () == TYPE_ACTION_SUBSYSTEM)
	tag()->type (block_T::type_optional ("action"));
      else if (type () == TYPE_ITERATOR_SUBSYSTEM)
	tag()->type (block_T::type_optional ("iterator"));
      else if (type () == TYPE_STATEFLOW_SUBSYSTEM)
	tag()->type (block_T::type_optional ("stateflow"));
      else if (type () == TYPE_ATOMIC_SUBSYSTEM)
	tag()->type (block_T::type_optional ("atomic"));

      // replace symbolic value to real number
      block_T::rate_optional& ro = tag()->rate();
      if (ro)
	{
	  const std::string& rate = *ro;

	  // TODO: offset of sampletime ([m n])
	  if (rate != "inf" && rate != "0" && rate != "-1" && rate != "-2")
	    {
	      // a real number
	      char *p;
	      (void)strtod(rate.c_str (), &p);
	      if (*p != 0)
		{
		  // find in param
		  c_variable_map_const_iterator cv = b->find_param (rate);
		  if (cv != b->param_entry ().end ())
		    {
		      const VariableEnt& var = cv->second;
		      const std::string& val = var.init ();
		      if (val.length ())
			{
			  // replace value
			  tag()->rate(val);
			}
		    }
		}
	    }
	}

      if (frag ())
	{
	  for (CodeFrags::const_iterator i (codefrags ().begin ());
	       i != codefrags ().end (); i++)
	    {
	      code_T c (i->frag (), "task");
	      if (i->line () != 0)
		c.line () = code_T::line_optional (i->line ());
	      c.index () = code_T::index_optional (i->index ());
	      if (code_file().length() != 0)
		c.file () = code_T::file_optional (code_file());
	      tag()->code ().push_back (c);
	    }
	}
      if (code().length())
	{
	  code_T c(code(), "task");
	  if (code_line() != 0)
	    c.line () = code_T::line_optional (code_line());
	  if (code_file().length() != 0)
	    c.file () = code_T::file_optional (code_file());
	  tag()->code ().push_back (c);
	}
      if (update().length())
	{
	  code_T c(update(), "update");
	  if (update_line() != 0)
	    c.line () = code_T::line_optional (update_line());
	  if (update_file().length() != 0)
	    c.file () = code_T::file_optional (update_file());
	  tag()->code ().push_back (c);
	}

      if (init_code().length())
	{
	  code_T c(init_code(), "init");
	  if (init_code_line() != 0)
	    c.line () = code_T::line_optional (init_code_line());
	  if (init_code_file().length() != 0)
	    c.file () = code_T::file_optional (init_code_file());
	  tag()->code ().push_back (c);
	}

      if (init_state().length())
	{
	  code_T c(init_state(), "state");
	  if (init_state_line() != 0)
	    c.line () = code_T::line_optional (init_state_line());
	  if (init_state_file().length() != 0)
	    c.file () = code_T::file_optional (init_state_file());
	  tag()->code ().push_back (c);
	}

      // variable
      c_varentp_map out_vmap;
      if (merge_blocks ().size ())
	{
	  for (blockinfo_list_iterator bli (merge_blocks ().begin ());
	       bli != merge_blocks ().end (); bli++)
	    {
	      BlockInfo* mbi = *bli;

	      add_var_tag (b, forward (), mbi->output_var (), out_vmap, true);
	    }
	}
      add_var_tag (b, forward (), output_var (), out_vmap, false);

      if (merge_blocks ().size ())
	{
	  for (blockinfo_list_iterator bli (merge_blocks ().begin ());
	       bli != merge_blocks ().end (); bli++)
	    {
	      BlockInfo* mbi = *bli;

	      add_var_tag (b, backward (), mbi->input_var (), out_vmap, true);
	    }
	}
      add_var_tag (b, backward (), input_var (), out_vmap, false);

      // signal
      for (var_list_const_iterator vci (signal_var().begin());
	   vci != signal_var().end(); vci++)
	{
	  param_T p (b->signal_name(),
		     (*vci)->name(), (*vci)->type());
	  if ((*vci)->array_size())
	    p.size((*vci)->array_size());
	  if ((*vci)->init ().length ())
	    p.value((*vci)->init ());
	  tag()->signal().push_back(p);
	}

      // states
      for (var_list_const_iterator vci (state_var().begin());
	   vci != state_var().end(); vci++)
	{
	  param_T p (b->state_name(),
		     (*vci)->name(), (*vci)->type());
	  if ((*vci)->array_size())
	    p.size((*vci)->array_size());
	  if ((*vci)->init ().length ())
	    p.value((*vci)->init ());
	  tag()->state().push_back(p);
	}

      // trigger states
      for (var_list_const_iterator vci (trigger_state_var().begin());
	   vci != trigger_state_var().end(); vci++)
	{
	  param_T p (b->trigger_state_name(),
		     (*vci)->name(), (*vci)->type());
	  if ((*vci)->array_size())
	    p.size((*vci)->array_size());
	  if ((*vci)->init ().length ())
	    p.value((*vci)->init ());
	  tag()->trigger_state().push_back (p);
	}

      // invariant signals
      for (var_list_const_iterator vci (invariant_signal_var().begin());
	   vci != invariant_signal_var().end(); vci++)
	{
	  param_T p (b->invariant_signal_name(),
		     (*vci)->name(), (*vci)->type());
	  if ((*vci)->array_size())
	    p.size((*vci)->array_size());
	  if ((*vci)->init ().length ())
	    p.value((*vci)->init ());
	  if ((*vci)->code ().length ())
	    {
	      p.initcode(code_T ((*vci)->code (), "invariant_signal"));
	      param_T::initcode_optional& c = p.initcode ();
	      c->line ((*vci)->codeline ());
	      c->file (*b->get_filename (Blocks::XF_C_DATA));
	    }
	  tag()->invariant_signal().push_back (p);
	}

      // params
      for (var_list_const_iterator vci (params().begin());
	   vci != params().end(); vci++)
	{
	  param_T p (b->param_name(),
		     (*vci)->name(), (*vci)->type());
	  if ((*vci)->array_size())
	    p.size((*vci)->array_size());
	  if ((*vci)->init ().length ())
	    p.value((*vci)->init ());
	  if ((*vci)->code ().length ())
	    {
	      p.initcode (code_T ((*vci)->code (), "param"));
	      param_T::initcode_optional& c = p.initcode ();
	      c->line ((*vci)->codeline ());
	      c->file (*b->get_filename (Blocks::XF_C_DATA));
	    }
	  tag()->param().push_back(p);
	}

      // const params
      for (var_list_const_iterator vci (constant_params().begin());
	   vci != constant_params().end(); vci++)
	{
	  param_T p (b->constant_param_name(),
		     (*vci)->name(), (*vci)->type());
	  if ((*vci)->array_size())
	    p.size((*vci)->array_size());
	  if ((*vci)->init ().length ())
	    p.value((*vci)->init ());
	  if ((*vci)->code ().length ())
	    {
	      p.initcode(code_T ((*vci)->code (), "const"));
	      param_T::initcode_optional& c = p.initcode ();
	      c->line ((*vci)->codeline ());
	      c->file (*b->get_filename (Blocks::XF_C_DATA));
	    }
	  tag()->const_param().push_back(p);
	}

      // external outputs
      for (var_list_const_iterator vci (external_outputs().begin());
	   vci != external_outputs().end (); vci++)
	{
	  const VariableEnt* ve = *vci;
	  var_T v (ve->name(), ve->type (), "extout");

	  tag()->var().push_back (v);

	  var_T& vr = tag()->var().back ();

	  if (ve->array_size ())
	    vr.size (ve->array_size ());
	  vr.line ("");
	  vr.port ("");
	  vr.storage (b->external_output_name ());
	}

      // CSP forward link
      for (block_link_const_iterator lci (forward().begin());
	   lci != forward().end(); lci++)
	{
	  tag()->forward().push_back(link_T (lci->blockinfo ()->tag()->name(),
					     lci->type ()));
	  link_T& l = tag()->forward().back();

	  if (lci->label().length())
	    l.label (lci->label());

	  c_varentp_map forwvmap;
	  for (pvar_ptr_list_const_iterator vi (lci->vars().begin());
	       vi != lci->vars().end(); vi++)
	    {
	      const VarElement* pv = *vi;
	      for (var_list_const_iterator vie (pv->varent ().begin ());
		   vie != pv->varent ().end (); vie++)
		{
		  const VariableEnt* ve = *vie;
		  if (ve != NULL && vie->is_used ())
		    {
		      std::string vn = b->storage_qualifier_variable (*ve);
		      if (forwvmap.find (vn) == forwvmap.end ())
			{
			  l.var().push_back ((*vi)->make_var_tag(ve));
			  add_storage_if_existing (b, l.var().back(), *ve);
			  forwvmap.insert (std::make_pair (vn, ve));
			}
		    }
		}
	    }
	}

      // CSP backward link
      for (block_link_const_iterator lci (backward().begin());
	   lci != backward().end(); lci++)
	{
	  tag()->backward().push_back(link_T (lci->blockinfo ()->tag()->name(),
					      lci->type ()));
	  link_T& l = tag()->backward().back();

	  if (lci->label().length())
	    l.label (lci->label());

	  const BlockLink* back = lci->linkpair ();
	  const pvar_ptr_list& bvars = back->vars ();

	  c_varentp_map backwvmap;
	  for (pvar_ptr_list_const_iterator vi (lci->vars().begin());
	       vi != lci->vars().end(); vi++)
	    {
	      const VarElement* pv = *vi;
	      pvar_ptr_list_const_iterator bpvi = bvars.vfind (*pv);
	      if (bpvi == bvars.end ())
		{
		  std::cout << "Warning!" << __LINE__
			    << ": "
			    << tag()->name ()
			    << " <= "
			    << lci->blockinfo ()->tag ()->name ()
			    << ": pvar not existing"
			    << std::endl;
		  continue;
		}
	      else
		{
		  if (pv->name () != (*bpvi)->name ())
		    {
		      std::cout << "Warning!" << __LINE__
				<< ": "
				<< pv->name ()
				<< '@'
				<< tag()->name ()
				<< " <= "
				<< (*bpvi)->name ()
				<< '@'
				<< lci->blockinfo ()->tag ()->name ()
				<< ": pvar not same name"
				<< std::endl;
		    }
		}
	      var_list_const_iterator bbeg = (*bpvi)->varent ().begin ();
	      var_list_const_iterator bend = (*bpvi)->varent ().end ();
	      for (var_list_const_iterator vie (pv->varent ().begin ());
		   vie != pv->varent ().end (); vie++)
		{
		  const VariableEnt* ve = *vie;
		  if (ve != NULL && vie->is_used ())
		    {
		      var_list_const_iterator
			vbe = std::find (bbeg, bend, ve);
		      if (vbe != bend)
			{
			  std::string
			    vn = b->storage_qualifier_variable (*ve);
			  if (backwvmap.find (vn) == backwvmap.end ())
			    {
			      l.var().push_back ((*vi)->make_var_tag(ve));
			      add_storage_if_existing (b, l.var().back(), *ve);
			      backwvmap.insert (std::make_pair (vn, ve));
			    }
			}
		    }
		}
	    }
	}

      // Block Merge
      for (blockinfo_list_const_iterator lci (merged_to ().begin ());
	   lci != merged_to ().end (); lci++)
	{
	  const BlockInfo* bto = *lci;
	  block_name_T m(bto->tag ()->name ());
	  tag()->merged_to ().push_back (m);
	}

      for (blockinfo_list_const_iterator lci (merge_blocks ().begin ());
	   lci != merge_blocks ().end (); lci++)
	{
	  const BlockInfo* bto = *lci;
	  block_name_T m(bto->tag ()->name ());
	  tag()->merge_block ().push_back (m);
	}

      // function
      for (function_map_const_iterator fi (func_decl ().begin ());
	   fi != func_decl ().end (); fi++)
	{
	  tag ()->function ().push_back (function_T (fi->first, "decl"));
	  function_T& f = tag ()->function ().back ();
	  if (fi->second.kind () != function_code_T::FK_NONE)
	    f.kind (fi->second.kind_str ());
	}

      for (function_map_const_iterator fi (func_defn ().begin ());
	   fi != func_defn ().end (); fi++)
	{
	  tag ()->function ().push_back (function_T (fi->first, "defn"));
	  function_T& f = tag ()->function ().back ();
	  if (fi->second.kind () != function_code_T::FK_NONE)
	    f.kind (fi->second.kind_str ());
	}
    }


    bool
    BlockInfo::is_same_merged_head (const BlockInfo* info) const
    {
      if (this == info)
	return true;

      if (top_block ())
	{
	  if (top_block () == info || top_block () == info->top_block ())
	    return true;
	  return false;
	}
      else if (info->top_block ())
	{
	  if (this == info->top_block ())
	    return true;
	  return false;
	}

      if (b_merged_to.size ())
	{
	  for (blockinfo_list_const_iterator i (b_merged_to.begin ());
	       i != b_merged_to.end (); i++)
	    {
	      if ((*i)->is_same_merged_head (info))
		return true;
	    }
	}
      else if (info->b_merged_to.size ())
	{
	  for (blockinfo_list_const_iterator
		 j (info->b_merged_to.begin ());
	       j != info->b_merged_to.end (); j++)
	    {
	      if (is_same_merged_head (*j))
		return true;
	    }
	}

      return false;
    }

    void
    BlockInfo::dump (void) const
    {
      std::cout << tag()->blocktype() << "::"
		<< tag()->name() << '\n';
      if (defined ())
	std::cout << " @ Hierarchy: "
		  << system () << '/' << block () << '\n';
      else
	std::cout << " * UNDEFINED\n";

      if (upper())
	{
	  std::cout << " @ Subblock of " << upper()->tag()->blocktype()
		    << "::" << upper()->tag()->name() << '\n';
	}

      if (merged_to ().size ())
	{
	  for (blockinfo_list_const_iterator blci (merged_to ().begin ());
	       blci != merged_to ().end (); blci++)
	    {
	      const block_T* t = (*blci)->tag ();
	      std::cout << " @ Merged by "
			<< t->blocktype ()
			<< "::"
			<< t->name () << '\n';
	    }
	}

      if (input_var().size())
	{
	  std::cout << "  -- input variables --\n";
	  for (propagate_var_list_const_iterator i (input_var().begin());
	       i != input_var().end(); i++)
	    {
	      const VarElement& pv = *i;
	      if (pv.varent ().size ())
		{
		  if (i->start() != -1 && pv.varent ().size () > 1)
		    {
		      std::cout << "Warning: partial array found"
				<< " in multiple Block Signal(input)"
				<< std::endl;
		    }

		  for (var_list_const_iterator vie (pv.varent ().begin ());
		       vie != pv.varent ().end (); vie++)
		    {
		      const VariableEnt* ve = *vie;
		      std::cout << "  " << ve->type() << " " << ve->name();
		      if (i->start() != -1)
			std::cout << '[' << i->start() << ']';
		      else if (ve->array_size())
			std::cout << "[sz=" << ve->array_size() << ']';
		      std::cout << " @ [line=" << i->line()
				<< ",port=" << i->port()
				<< ']' << '\n';
		    }
		}
	      else
		std::cout << "  " << i->name() << ": unknown\n";
	    }
	}
      if (output_var().size())
	{
	  std::cout << "  -- output variables --\n";
	  for (propagate_var_list_const_iterator i (output_var().begin());
	       i != output_var().end(); i++)
	    {
	      const VarElement& pv = *i;
	      if (pv.varent ().size ())
		{
		  if (i->start() != -1 && pv.varent ().size () > 1)
		    {
		      std::cout << "Warning: partial array found"
				<< " in multiple Block Signal(output)"
				<< std::endl;
		    }

		  for (var_list_const_iterator vie (pv.varent ().begin ());
		       vie != pv.varent ().end (); vie++)
		    {
		      const VariableEnt* ve = *vie;
		      std::cout << "  " << ve->type() << " " << ve->name();
		      if (i->start() != -1)
			std::cout << '[' << i->start() << ']';
		      else if (ve->array_size())
			std::cout << "[sz=" << ve->array_size() << ']';
		      std::cout << " @ [line=" << i->line()
				<< ",port=" << i->port()
				<< ']' << '\n';
		    }
		}
	      else
		std::cout << "  " << i->name() << ": unknown\n";
	    }
	}
      if (state_var().size())
	{
	  std::cout << "  -- state variables --\n";
	  for (var_list_const_iterator i (state_var().begin());
	       i != state_var().end(); i++)
	    std::cout << "  " << (*i)->type()
		      << " " << (*i)->obj()
		      << "." << (*i)->name() << '\n';
	}
      if (trigger_state_var().size())
	{
	  std::cout << "  -- trigger state variables --\n";
	  for (var_list_const_iterator i (trigger_state_var().begin());
	       i != trigger_state_var().end(); i++)
	    std::cout << "  " << (*i)->type()
		      << " " << (*i)->obj()
		      << "." << (*i)->name() << '\n';
	}
      if (invariant_signal_var().size())
	{
	  std::cout << "  -- invariant signal variables --\n";
	  for (var_list_const_iterator i (invariant_signal_var().begin());
	       i != invariant_signal_var().end(); i++)
	    std::cout << "  " << (*i)->type()
		      << " " << (*i)->obj()
		      << "." << (*i)->name() << '\n';
	}
      if (params().size())
	{
	  std::cout << "  -- parameters --\n";
	  for (var_list_const_iterator i (params().begin());
	       i != params().end(); i++)
	    std::cout << "  " << (*i)->type()
		      << " " << (*i)->obj()
		      << "." << (*i)->name() << '\n';
	}
      if (constant_params().size())
	{
	  std::cout << "  -- constant parameters --\n";
	  for (var_list_const_iterator i (constant_params().begin());
	       i != constant_params().end(); i++)
	    std::cout << "  " << (*i)->type()
		      << " " << (*i)->obj()
		      << "." << (*i)->name() << '\n';
	}
      if (frag ())
	{
	  for (CodeFrags::const_iterator i (codefrags ().begin ());
	       i != codefrags ().end (); i++)
	    std::cout << "  -- code["
		      << i->index () << '@' << i->line ()
		      << "] --\n" << i->frag ();
	}
      else if (code().length())
	std::cout << "  -- code --\n" << code();
      if (update().length())
	std::cout << "  -- update --\n" << update();
      if (init_code().length())
	std::cout << "  -- initialize code --\n" << init_code();
      if (init_state().length())
	std::cout << "  -- initialize states --\n" << init_state();
      if (auto_var().size())
	{
	  std::cout << "  -- auto variables --\n";
	  for (var_list_const_iterator i (auto_var().begin());
	       i != auto_var().end(); i++)
	    std::cout << "  " << (*i)->type()
		      << " " << (*i)->name() << '\n';
	}
      if (backward().size())
	{
	  std::cout << "  -- input channels --\n";
	  for (block_link_const_iterator i (backward().begin());
	       i != backward().end(); i++)
	    {
	      if (i->vars().size () == 0)
		std::cout << "Warning: no variable backward from block="
			  << i->blockinfo()->tag()->name()
			  << std::endl;
	      for (pvar_ptr_list_const_iterator p (i->vars().begin());
		   p != i->vars().end(); p++)
		{
		  const VarElement* pv = *p;
		  if (pv->varent ().size ())
		    {
		      if (pv->varent ().size () > 1 && pv->start () != -1)
			{
			  std::cout << "Warning: partial array found"
				    << " in multiple Block Signal(input)"
				    << std::endl;
			}
		      for (var_list_const_iterator
			     vi (pv->varent ().begin ());
			   vi != pv->varent ().end (); vi++)
			{
			  const VariableEnt* ve = *vi;
			  std::cout << "  " << "var=" << ve->name();
			  if (pv->start() != -1)
			    std::cout << '[' << pv->start() << ']';
			  else if (ve->array_size())
			    std::cout << "[sz="
				      << ve->array_size() << ']';
			  std::cout << " <= ["
				    << "block="
				    << i->blockinfo()->tag()->name()
				    << ",line=" << pv->line()
				    << ",port=" << pv->port() << ']';
			}
		    }
		  else
		    std::cout << "  var=" << pv->name() << ": unknown";
		  std::cout << '\n';
		}
	    }
	}
      if (forward().size())
	{
	  std::cout << "  -- output channels --\n";
	  for (block_link_const_iterator i (forward().begin());
	       i != forward().end(); i++)
	    {
	      if (i->vars().size () == 0)
		std::cout << "Warning: no variable forward to block="
			  << i->blockinfo()->tag()->name()
			  << std::endl;
	      for (pvar_ptr_list_const_iterator p (i->vars().begin());
		   p != i->vars().end(); p++)
		{
		  const VarElement* pv = *p;
		  if (pv->varent ().size ())
		    {
		      if (pv->varent ().size () > 1 && pv->start () != -1)
			{
			  std::cout << "Warning: partial array found"
				    << " in multiple Block Signal(input)"
				    << std::endl;
			}
		      for (var_list_const_iterator
			     vi (pv->varent ().begin ());
			   vi != pv->varent ().end (); vi++)
			{
			  const VariableEnt* ve = *vi;
			  std::cout << "  " << "var=" << ve->name();
			  if (pv->start() != -1)
			    std::cout << '[' << pv->start() << ']';
			  else if (ve->array_size())
			    std::cout << "[sz=" << ve->array_size() << ']';
			  std::cout << " => ["
				    << "block="
				    << i->blockinfo()->tag()->name()
				    << ",line=" << pv->line()
				    << ",port=" << pv->port() << ']';
			}
		    }
		  else
		    std::cout << "  var=" << (*p)->name() << ": unknown";
		  std::cout << '\n';
		}
	    }
	}
      if (type () == TYPE_BUSCREATOR && bus_visited ())
	{
	  std::cout << "  Mux Input port (search "
		    << (bus_search_result () ? "SUCCESS" : "FAIL") << ')'
		    << std::endl;
	  busport ().dump (2);
	}
      else if (type () == TYPE_BUSSELECTOR && bus_visited ())
	{
	  std::cout << "  Demux Output port (search "
		    << (bus_search_result () ? "SUCCESS" : "FAIL") << ')'
		    << std::endl;
	  busport ().dump (2);
	}
    }

    // class ioport_deq_T

    void
    ioport_deq_T::dump (void)
    {
      std::cout << "-- Ports --" << std::endl;
      for (const_iterator i (begin ()); i != end (); i++)
	{
	  const IOPort* iop = *i;

	  const block_T* b = iop->block_tag ();
	  const ioport_T* p = iop->ioport_tag ();
	  const BlockInfo* bi = iop->blockinfo ();
	  int ind = iop->ioport_index ();

	  std::cout << b->blocktype() << ':' << b->name ()
		    << '[' << bi->id () << ']';

	  if (bi->type () == BlockInfo::TYPE_VIRTUALOUTPORT)
	    std::cout << "<VOP>";

	  std::cout << "\n    " << ind << ':'
		    << p->line () << '@' << p->port ();
	  IOPort::type t = iop->iotype ();
	  if (t == IOPort::IN)
	    std::cout << "<I>";
	  else if (t == IOPort::OUT)
	    std::cout << "<O>";
	  else
	    std::cout << "<N>";
	  std::cout << std::endl;
	}
      std::cout << "--------" << std::endl;
    }

    // class IOPort

    IOPort::IOPort ()
      : io_type (NONE), blk_tag(NULL), io_tag(NULL), io_index(-1),
	blk_info (NULL), src_outport (NULL)
    {
    }

    IOPort::IOPort (IOPort::type type, const block_T* blk,
		    const ioport_T* io, int index, BlockInfo* bi)
      : io_type (type), blk_tag (blk), io_tag (io), io_index (index),
	blk_info (bi), src_outport (NULL)
    {
    }

    IOPort::~IOPort ()
    {
    }


    // class VarElement
    VarElement::VarElement ()
    {
    }

    VarElement::VarElement (const std::string& name, int start, int end,
			    const std::string& mode,
			    const std::string& block)
      : v_start (start), v_end (end), v_mode (mode), v_block (block),
	v_switched (false), v_index (0), v_nchunk (1), v_parts (1)
    {
      c_canonical_name (v_name, name);
    }

    VarElement::VarElement (const std::string& name, int start, int end,
			    const std::string& mode,
			    const std::string& block,
			    const std::string& line,
			    const std::string& port, VariableEnt* varent)
      : v_start (start), v_end (end), v_mode (mode), v_block (block),
	v_line (line), v_port (port),
	v_switched (false), v_index (0), v_nchunk (1), v_parts (1)
    {
      c_canonical_name (v_name, name);
      if (varent)
	v_varent.add_varent(varent);
    }

    VarElement::VarElement (const oldBusEnt& be, const std::string& mode)
      : v_mode (mode), v_switched (false)
    {
      const IOPort* dop = be.port ();
      const std::string& dun = be.usename ();
      const std::string& name = (dun.length ()
				 ? dun
				 : dop->ioport_tag ()->line ());
      const std::string& block = dop->block_tag ()->name ();

      c_canonical_name (v_name, name);
      if (be.total () != 1)
	{
	  v_start = be.index ();
	  v_end = v_start + be.nchunk ();
	}
      else
	{
	  v_start = -1;
	  v_end = -1;
	}
      v_block = block;
      v_index = be.index ();
      v_nchunk = be.nchunk ();
      v_parts = be.total ();
    }

    void
    VarElement::switch_to_range (const VariableEnt* vent)
    {
      if (v_switched)
	return;

      if (!vent->is_array () || v_parts == 1)
	{
	  v_switched = true;
	  v_end = -1;
	  v_start = -1;
	  return;
	}

      int a = vent->array_size ();
      int x = a / v_parts;

      assert (x);

      int m = a % v_parts;

      const int s = v_index;
      const int e = s + v_nchunk;

      int as = s * x;
      int ae = e * x;
      if (m)
	{
	  if (s < m)
	    as += s;
	  else
	    as += m;
	  if (e < m)
	    ae += e;
	  else
	    ae += m;
	}

      v_start = as;
      if (as + 1 == ae)
	v_end = -1;
      else
	v_end = ae;

      v_switched = true;
    }


    bool
    VarElement::operator== (const VarElement& v) const
    {
      if (start() != v.start()
	  || end() != v.end())
	return false;

      if (is_same_name (v) || is_same_port (v))
	return true;		// same name or port

#if 1				// refs #6281, #6283
      return false;
#else
      var_list_const_iterator bb;
      var_list_const_iterator be;
      var_list_const_iterator sb;
      var_list_const_iterator se;

      if (varent ().size () > v.varent ().size ())
	{
	  bb = varent ().begin ();
	  be = varent ().end ();
	  sb = v.varent ().begin ();
	  se = v.varent ().end ();
	}
      else
	{
	  bb = v.varent ().begin ();
	  be = v.varent ().end ();
	  sb = varent ().begin ();
	  se = varent ().end ();
	}

      for (var_list_const_iterator vi (sb); vi != se; vi++)
	{
	  if (std::find (bb, be, *vi) == be)
	    return false;
	}

      std::cout << "Warning!" << __LINE__
		<< ": same VariableEnt: " << name ()
		<< " and " << v.name () << std::endl;

      return true;		// included all variables
#endif
    }

    var_T
    VarElement::make_var_tag (const VariableEnt *ve) const
    {
      var_T v (ve->name(), ve->type(), mode());
      if (ve->array_size())
	v.size (ve->array_size());
      if (start() != -1)
	v.start (start());
      if (end() != -1)
	v.end (end());
      if (line().length())
	v.line (line());
      if (port().length())
	v.port (port());
      return v;
    }

    void
    VarElement::dump (int ind)
    {
      std::string sind (ind * 2, ' ');
      std::cout << sind << v_name;
      if (v_start != -1)
	{
	  std::cout << "[" << v_start;
	  if (v_end != -1)
	    std::cout << ".." << v_end;
	  else
	    std::cout << "...";
	  std::cout << "]";
	}
      if (v_varent.size ())
	{
	  std::cout << '<' << v_varent.size () << ">{";
	  int n = 0;
	  for (var_list_const_iterator v (v_varent.begin ());
	       v != v_varent.end (); v++)
	    {
	      if (n++ > 0)
		std::cout << ',';
	      const std::string& o = (*v)->obj ();
	      if (o.length ())
		std::cout << o << '.';
	      std::cout << (*v)->name ();
	    }
	  std::cout << '}';
	}
      std::cout << std::endl;
    }

    //
    // class propagate_var_list
    //
    VarElement&
    propagate_var_list::add_var (const VarElement& v)
    {
      for (iterator i (begin ()); i != end (); i++)
	{
	  if (i->is_same_name (v) || i->is_same_port (v))
	    {
	      // same
	      if (i->start () == v.start ()
		  && i->end () == v.end ())
		return *i;
#if 0
	      // whole
	      if (i->start () == -1 && i->end () == -1)
		return *i;

	      // change whole
	      if (v.start () == -1 && v.end () == -1)
		{
		  i->start () = -1;
		  i->end () = -1;
		  return *i;
		}

	      // include
	      if (i->start () <= v.start ()
		  && v.end () <= i->end ())
		return *i;

	      // concat prev
	      if (i->start () <= v.end ()
		  && v.end () <= i->end ()
		  && v.start () < i->start ())
		{
		  i->start () = v.start ();
		  return *i;
		}

	      // concat post
	      if (i->start () <= v.start ()
		  && v.start () <= i->end ()
		  && i->end () < v.end ())
		{
		  i->end () = v.end ();
		  return *i;
		}
#endif
	    }
	}

      std::list < VarElement >::push_back (v);
      return back ();
    }

    //
    // class pvar_ptr_list
    //
    pvar_ptr_list::iterator
    pvar_ptr_list::vfind (pvar_ptr_list_iterator b, pvar_ptr_list_iterator e,
			  const VarElement& ve)
    {
      for (iterator i (b); i != e; i++)
	{
	  if (ve == **i)
	    return i;
	}
      return end ();
    }

    pvar_ptr_list::const_iterator
    pvar_ptr_list::vfind (pvar_ptr_list_const_iterator b,
			  pvar_ptr_list_const_iterator e,
			  const VarElement& ve) const
    {
      for (const_iterator i (b); i != e; i++)
	{
	  if (ve == **i)
	    return i;
	}
      return end ();
    }


    //
    // class SysData
    //

    int SysData::index = 0;

    //
    // class Blocks
    //

    BlockNameEnt&
    Blocks::add_blockname_entry (const std::string& s,
				 const BlockNameEnt& b)
    {
      blockname_res x = blockname_entry().insert (std::make_pair (s, b));
      return x.first->second;
    }

    c_variable_map_const_iterator
    Blocks::find_auto_var (const std::string& name) const
    {
      return auto_variable_entry().find (name);
    }

    c_variable_map_iterator
    Blocks::find_auto_var (const std::string& name)
    {
      return auto_variable_entry().find (name);
    }

    VariableEnt&
    Blocks::add_auto_variable_entry (const std::string& s,
				     const VariableEnt& v)
    {
      c_var_res x = auto_variable_entry().insert (std::make_pair (s, v));
      return x.first->second;
    }

    c_variable_map_const_iterator
    Blocks::find_ext_var (const std::string& name) const
    {
      return ext_variable_entry().find (name);
    }

    c_variable_map_iterator
    Blocks::find_ext_var (const std::string& name)
    {
      return ext_variable_entry().find (name);
    }

    VariableEnt&
    Blocks::add_ext_variable_entry (const std::string& s,
				    const VariableEnt& v)
    {
      c_var_res x = ext_variable_entry().insert (std::make_pair (s, v));
      return x.first->second;
    }


    c_variable_map_const_iterator
    Blocks::find_ext_state (const std::string& name) const
    {
      return ext_state_entry().find (name);
    }

    c_variable_map_iterator
    Blocks::find_ext_state (const std::string& name)
    {
      return ext_state_entry().find (name);
    }

    VariableEnt&
    Blocks::add_ext_state_entry (const std::string &s,
				 const VariableEnt& v)
    {
      c_var_res x = ext_state_entry().insert (std::make_pair (s, v));
      return x.first->second;
    }

    c_variable_map_const_iterator
    Blocks::find_state_var (const std::string& name) const
    {
      return state_entry().find (name);
    }

    c_variable_map_iterator
    Blocks::find_state_var (const std::string& name)
    {
      return state_entry().find (name);
    }

    VariableEnt&
    Blocks::add_state_entry (const std::string &s, const VariableEnt& v)
    {
      c_var_res x = state_entry().insert (std::make_pair (s, v));
      VariableEnt& ve = x.first->second;
      state_order().add_varent (&ve);
      return ve;
    }

    c_variable_map_const_iterator
    Blocks::find_trigger_state_var (const std::string& name) const
    {
      return trigger_state_entry().find (name);
    }

    c_variable_map_iterator
    Blocks::find_trigger_state_var (const std::string& name)
    {
      return trigger_state_entry().find (name);
    }

    VariableEnt&
    Blocks::add_trigger_state_entry (const std::string &s,
				     const VariableEnt& v)
    {
      c_var_res x = trigger_state_entry().insert (std::make_pair (s, v));
      VariableEnt& ve = x.first->second;
      trigger_state_order().add_varent (&ve);
      return ve;
    }

    c_variable_map_const_iterator
    Blocks::find_invariant_signal_var (const std::string& name) const
    {
      return invariant_signal_entry().find (name);
    }

    c_variable_map_iterator
    Blocks::find_invariant_signal_var (const std::string& name)
    {
      return invariant_signal_entry().find (name);
    }

    VariableEnt&
    Blocks::add_invariant_signal_entry (const std::string &s,
					const VariableEnt& v)
    {
      c_var_res x = invariant_signal_entry().insert (std::make_pair (s, v));
      VariableEnt& ve = x.first->second;
      invariant_signal_order().add_varent (&ve);
      return ve;
    }

    c_variable_map_const_iterator
    Blocks::find_param (const std::string& name) const
    {
      return param_entry().find (name);
    }

    c_variable_map_iterator
    Blocks::find_param (const std::string& name)
    {
      return param_entry().find (name);
    }


    VariableEnt&
    Blocks::add_param_entry (const std::string& s, const VariableEnt& v)
    {
      c_var_res x = param_entry().insert (std::make_pair (s, v));
      VariableEnt& ve = x.first->second;
      param_order().add_varent (&ve);
      return ve;
    }

    c_variable_map_const_iterator
    Blocks::find_constant_param (const std::string& name) const
    {
      return constant_param_entry().find (name);
    }

    c_variable_map_iterator
    Blocks::find_constant_param (const std::string& name)
    {
      return constant_param_entry().find (name);
    }

    VariableEnt&
    Blocks::add_constant_param_entry (const std::string s,
				      const VariableEnt& v)
    {
      c_var_res x = constant_param_entry().insert (std::make_pair (s, v));
      VariableEnt& ve = x.first->second;
      constant_param_order().add_varent (&ve);
      return ve;
    }

    c_variable_map_const_iterator
    Blocks::find_signal_var (const std::string& name) const
    {
      return signal_entry().find (name);
    }

    c_variable_map_iterator
    Blocks::find_signal_var (const std::string& name)
    {
      return signal_entry().find (name);
    }

    VariableEnt&
    Blocks::add_signal_entry (const std::string &s, const VariableEnt& v)
    {
      c_var_res x = signal_entry().insert (std::make_pair (s, v));
      VariableEnt& ve = x.first->second;
      signal_order().add_varent (&ve);
      return ve;
    }

    c_variable_map_const_iterator
    Blocks::find_external_input (const std::string& name) const
    {
      c_variable_map_const_iterator cve = external_input_entry().end ();
      c_variable_map_const_iterator cv =  external_input_entry().find (name);

      if (cv != cve)
	return cv;

      for (cv = external_input_entry ().begin (); cv != cve; cv++)
	{
	  const VariableEnt& ve = cv->second;
	  const std::string* sym = xml_systemsymbol (ve.system ());
	  if (sym == NULL)
	    continue;
	  std::string v (*sym);
	  v += "_" + ve.name ();
	  if (v == name)
	    return cv;
	}
      return cve;
    }

    c_variable_map_iterator
    Blocks::find_external_input (const std::string& name)
    {
      c_variable_map_iterator cve = external_input_entry().end ();
      c_variable_map_iterator cv =  external_input_entry().find (name);

      if (cv != cve)
	return cv;

      for (cv = external_input_entry ().begin (); cv != cve; cv++)
	{
	  const VariableEnt& ve = cv->second;
	  const std::string* sym = xml_systemsymbol (ve.system ());
	  if (sym == NULL)
	    continue;
	  std::string v (*sym);
	  v += "_" + ve.name ();
	  if (v == name)
	    return cv;
	}
      return cve;
    }

    VariableEnt&
    Blocks::add_external_input_entry (const std::string& s,
				      const VariableEnt& v)
    {
      c_var_res x = external_input_entry().insert (std::make_pair (s, v));
      VariableEnt& ve = x.first->second;
      external_input_order().add_varent (&ve);
      return ve;
    }

    c_variable_map_const_iterator
    Blocks::find_external_output (const std::string& name) const
    {
      c_variable_map_const_iterator cve = external_output_entry().end ();
      c_variable_map_const_iterator cv =  external_output_entry().find (name);

      if (cv != cve)
	return cv;

      for (cv = external_output_entry ().begin (); cv != cve; cv++)
	{
	  const VariableEnt& ve = cv->second;
	  const std::string* sym = xml_systemsymbol (ve.system ());
	  if (sym == NULL)
	    continue;
	  std::string v (*sym);
	  v += "_" + ve.name ();
	  if (v == name)
	    return cv;
	}
      return cve;
    }

    c_variable_map_iterator
    Blocks::find_external_output (const std::string& name)
    {
      c_variable_map_iterator cve = external_output_entry().end ();
      c_variable_map_iterator cv =  external_output_entry().find (name);

      if (cv != cve)
	return cv;

      for (cv = external_output_entry ().begin (); cv != cve; cv++)
	{
	  const VariableEnt& ve = cv->second;
	  const std::string* sym = xml_systemsymbol (ve.system ());
	  if (sym == NULL)
	    continue;
	  std::string v (*sym);
	  v += "_" + ve.name ();
	  if (v == name)
	    return cv;
	}
      return cve;
    }

    VariableEnt&
    Blocks::add_external_output_entry (const std::string& s,
				       const VariableEnt& v)
    {
      c_var_res x = external_output_entry().insert (std::make_pair (s, v));
      VariableEnt& ve = x.first->second;
      external_output_order().add_varent (&ve);
      return ve;
    }

    BlockInfo&
    Blocks::add_block_map (const std::string& s, const BlockInfo& b)
    {
      block_res x = block_map().insert (std::make_pair (s, b));
      return x.first->second;
    }

    IOPort&
    Blocks::add_in_port_map (const std::string& s, const IOPort& p)
    {
      ioport_map_iterator x = in_port_map().insert (std::make_pair (s, p));
      return x->second;
    }

    IOPort&
    Blocks::add_out_port_map (const std::string& s, const IOPort& p)
    {
      ioport_map_iterator x = out_port_map().insert (std::make_pair (s, p));
      return x->second;
    }


    Goto&
    Blocks::add_goto_map (const std::string& s, const Goto& g)
    {
      goto_res x = goto_map ().insert (std::make_pair (s, g));
      return x.first->second;
    }


    From&
    Blocks::add_from_map (const std::string& s, const From& f)
    {
      from_res x = from_map ().insert (std::make_pair (s, f));
      return x.first->second;
    }


    // blocknameは、対象としているブロック名。
    // in はそのブロックのinputの一つ
    int
    Blocks::find_input_var (propagate_var_list& pv,
			    const block_T& blk,
			    const ioport_T& in,
			    std::string& usename)
    {
      int var_found = 0;

#if PROCESS_VAR_DEBUG > 0
      const std::string& blockname = blk.name ();
      std::cout << "* checking line=" << in.line()
		<< " in block=" << blockname << std::endl;
#endif

      // inputのconnectを探す。たぶん一つしかないはずだ
      for (ioport_T::connect_const_iterator ci (in.connect().begin());
	   ci != in.connect().end(); ci++)
	{
	  // 接続先のブロックとoutputの探索
	  IOPort* iop = find_out_port (ci->block(), ci->port());
	  if (iop == NULL)
	    {
	      std::cout << "Warning: no connection to "
			<< ci->block() << "::" << ci->port() << std::endl;
	      continue;
	    }

	  // 出力元のブロック
	  const block_T *cb = iop->block_tag();
	  const ioport_T* out = iop->ioport_tag();
	  const BlockInfo* cbi = iop->blockinfo();
	  const BlockInfo* ubi = cbi->upper();

	  const ioport_T::usename_optional uo = out->usename ();
	  if (uo && *uo == true)
	    {
	      if (usename.length () && usename != out->line ())
		std::cout << "Warning!" << __LINE__
			  << ": name changed by usename: "
			  << usename << " to " << out->line ()
			  << std::endl;
	      usename = out->line ();
	    }

#if PROCESS_VAR_DEBUG > 0
	  std::cout << "* block name=" << cb->name()
		    << ", port=" << out->port() << std::endl;
#endif
	  // ブロック構造のブロックの場合、さらに下降する必要がある。実
	  // 際には Outportでportとブロック名が一致するものを探し、その
	  // inputと一致するoutputを探す
	  //
	  // Inportの場合、今度は上位階層に向かって探す必要がある
	  //
	  // Demuxのoutputに行き当たった場合、Demuxのinputを上位階層に
	  // 向かって探し、outputの出現順に要素を取り出す。
	  //
	  // Muxのoutputに行き当たった場合、MuxのInputを上位階層に向かっ
	  // て探し、それらを全て集める
	  //
	  // それ以外ならoutputのlineが変数である。
	  if (cb->blocks().size())
	    {
#if PROCESS_VAR_DEBUG > 0
	      std::cout << "** block " << cb->name()
			<< " is subsystem" << std::endl;
#endif
	      for (block_T::blocks_const_iterator bsi (cb->blocks().begin());
		   bsi != cb->blocks().end(); bsi++)
		{
		  if (find_subblock_input_var (pv, out->port(), *bsi, usename))
		    var_found = 1;
		}
	    }
	  else if (cb->blocktype() == "Inport")
	    {
	      if (ubi == NULL)
		{
		  /* "extin" */
		  const std::string* vnp;
		  if (usename.length ())
		    vnp = &usename;
		  else
		    vnp = &out->line ();
#if XML_USENAME_DEBUG
		  if (*vnp != out->line ())
		    std::cout << "Warning!" << __LINE__
			      << ": change input variable: "
			      << out->line () << " to " << *vnp
			      << std::endl;
#endif	// XML_USENAME_DEBUG
		  pv.add_var (VarElement (*vnp, -1, -1, "input", cb->name ()));
		  return 1;
		}
#if PROCESS_VAR_DEBUG > 0
	      std::cout << "** Inport::" << cb->name()
			<< std::endl;
#endif
	      if (find_inport_var (pv, ubi, cb->name(), usename))
		return 1;
	      // ここに到達することがあるか？
	      std::cout << "Warning: input not found " << cb->name()
			<< " in " << ubi->tag()->name()
			<< std::endl;
	    }
#if 1
	  else if (cbi->type () == BlockInfo::TYPE_DEMUX
		   || cbi->type () == BlockInfo::TYPE_BUSSELECTOR)
	    {
	      const int ind = iop->ioport_index ();
	      const oldBusLine& demux_busline = cbi->busport ()[ind];

	      if (demux_busline.size ())
		{
		  oldBusLine busline;
		  demux_busline.defrag (busline);

		  for (oldBusLine::const_iterator li (busline.begin ());
		       li != busline.end (); li++)
		    {
#if (DIFFERENT_LINE_USENAME & DEMUX_DIFFERENT_LINE_USENAME) != 0
		      pv.add_var (VarElement (*li, "input"));
#else
		      VarElement& ve = pv.add_var (VarElement (*li, "input"));
		      ve.usename () = usename;
#endif
		    }

		  return 1;
		}

	      std::cout << "Warning: input not found for Demux "
			<< cb->name() << std::endl;
	    }
	  else if (cbi->type () == BlockInfo::TYPE_MUX
		   || cbi->type () == BlockInfo::TYPE_BUSCREATOR)
	    {
	      int found = 0;
	      for (oldBusPort::const_iterator pi (cbi->busport ().begin ());
		   pi != cbi->busport ().end (); pi++)
		{
		  oldBusLine busline;
		  pi->defrag (busline);
		  for (oldBusLine::const_iterator li (busline.begin ());
		       li != busline.end (); li++)
		    {
#if (DIFFERENT_LINE_USENAME & MUX_DIFFERENT_LINE_USENAME) != 0
		      pv.add_var (VarElement (*li, "input"));
#else
		      VarElement& ve = pv.add_var (VarElement (*li, "input"));
		      ve.usename () = usename;
#endif	// !MUX_DIFFERENT_LINE_USENAME
		      found = 1;
		    }
		}

	      if (found)
		var_found = 1;

	      if (var_found == 0)
		std::cout << "Warning: input not found for Mux "
			  << cb->name() << std::endl;
	    }
#else  // !1
	  else if (cb->blocktype () == "Demux")
	    {
	      // index of element of variable
	      int ind = 0;
	      for (block_T::output_const_iterator oo (cb->output().begin());
		   oo != cb->output().end(); oo++)
		{
		  if (&*oo == out)
		    break;
		  ind++;
		}

	      // 入力を見つけて、その変数(配列)のindexを設定
	      for (block_T::input_const_iterator ii (cb->input().begin());
		   ii != cb->input().end(); ii++)
		{
		  propagate_var_list ret;
		  std::string* un;
#if (DIFFERENT_LINE_USENAME & DEMUX_DIFFERENT_LINE_USENAME) != 0
		  std::string demux_usename;
		  un = &demux_usename;
#else  // !DEMUX_DIFFERENT_LINE_USENAME
		  un = &usename;
#endif	// !DEMUX_DIFFERENT_LINE_USENAME
		  if (find_input_var (ret, *cb, *ii, *un))
		    {
		      VarElement& ve = *ret.begin();

		      if (ret.size() != 1)
			{
			  int diff = 0;
			  for (propagate_var_list_iterator pi (ret.begin ());
			       pi != ret.end (); pi++)
			    {
			      VarElement& p = *pi;
			      if (p.name () != ve.name ())
				{
				  if (diff++ == 0)
				    {
				      std::cout << "Warning: "
					"multiple input for Demux "
						<< cb->name()
						<< ", line=" << ii->line()
						<< ", port=" << ii->port()
						<< std::endl;
				    }
				  std::cout << "    " << p.name ()
					    << std::endl;
				}
			    }
			}

		      for (propagate_var_list_iterator pi (ret.begin ());
			   pi != ret.end (); pi++)
			{
			  VarElement& ve = *pi;
			  ve.start() = ind;
			  ve.end() = -1;
			  pv.add_var (ve);
			}
		      var_found = 1;
		    }
		}

	      if (var_found == 0)
		std::cout << "Warning: input not found for Demux "
			  << cb->name() << std::endl;
	    }
	  else if (cb->blocktype () == "Mux")
	    {
	      // 入力を見つけて、その変数を全て設定
	      int found = 0;
	      for (block_T::input_const_iterator ii (cb->input().begin());
		   ii != cb->input().end(); ii++)
		{
		  std::string* un;
#if (DIFFERENT_LINE_USENAME & MUX_DIFFERENT_LINE_USENAME) != 0
		  std::string mux_usename;
		  un = &mux_usename;
#else  // !MUX_DIFFERENT_LINE_USENAME
		  un = &usename;
#endif	// !MUX_DIFFERENT_LINE_USENAME
		  if (find_input_var (pv, *cb, *ii, *un))
		    found = 1;
		}

	      if (found)
		var_found = 1;

	      if (var_found == 0)
		std::cout << "Warning: input not found for Mux "
			  << cb->name() << std::endl;
	    }
#endif	// !1
	  else if (cb->blocktype () == "From")
	    {
	      const From* f = find_from (cb->name ());
	      const Goto* g = f->from_goto ();
	      if (g == NULL)
		std::cerr << "Error: [From]" << cb->name  ()
			  << ": not found [Goto]" << std::endl;
	      else
		{
		  const block_T* gb = g->goto_tag ();
		  for (block_T::input_const_iterator ii (gb->input ().begin ());
		       ii != gb->input ().end (); ii++)
		    {
		      if (find_input_var (pv, *gb, *ii, usename))
			var_found = 1;
		    }
		}
	    }
#if HANDLE_RATETRANSISION
	  else if (cb->blocktype () == "RateTransition")
	    {
	      block_T::input_const_iterator ii (cb->input ().begin ());
	      if (ii != cb->input ().end ())
		{
		  if (find_input_var (pv, *cb, *ii, usename))
		    var_found = 1;
		  else
		    std::cout << "Warning: input not found for "
			      << cb->blocktype ()
			      << " " << cb->name () << std::endl;
		}
	    }
#endif	// HANDLE_RATETRANSISION
	  else if (cb->blocktype () != "Goto")
	    {
	      const std::string* vnp;
	      if (usename.length ())
		vnp = &usename;
	      else
		vnp = &out->line ();
#if XML_USENAME_DEBUG
	      if (*vnp != out->line ())
		std::cout << "Warning!" << __LINE__
			  << ": change input variable: "
			  << out->line () << " to " << *vnp
			  << std::endl;
#endif	// XML_USENAME_DEBUG
	      pv.add_var (VarElement (*vnp, -1, -1, "input", cb->name ()));
	      return 1;
	    }
	}

      return var_found;
    }


    int
    Blocks::find_subblock_input_var (propagate_var_list& pv,
				     const std::string& port,
				     const blocks_T& bs,
				     std::string& usename)
    {
      // 入力の変数がサブブロックから伝播してくる場合、
      // その変数名は、最下層の出力変数になっているので、
      // 下位下層のOutportに結びついているinputに切り替えて
      // 探索を継続する
#if PROCESS_VAR_DEBUG > 0
      std::cout << "* searching port=" << port << std::endl;
#endif
      for (blocks_T::block_const_iterator bi (bs.block().begin());
	   bi != bs.block().end(); bi++)
	{
	  if (bi->blocktype() == "Outport" && bi->name() == port)
	    {
#if PROCESS_VAR_DEBUG > 0
	      std::cout << "* find "
			<< bi->blocktype() << "::" << bi->name()
			<< std::endl;
#endif
	      int found = 0;

	      // Outportにはinputは一つだけのはずであるので、usenameは
	      // そのまま
	      for (block_T::input_const_iterator ii (bi->input().begin());
		   ii != bi->input().end(); ii++)
		{
		  if (find_input_var (pv, *bi, *ii, usename))
		    found = 1;
		}

	      if (found)
		return 1;
	    }
	}
      return 0;
    }


    int
    Blocks::find_inport_var (propagate_var_list& pv,
			     const BlockInfo* ubi, const std::string& port,
			     std::string& usename)
    {
      // 入力の変数がInportに結びついている場合、
      // 上位階層(ubi)のinputに切り替えて探索を継続する
      const block_T* ub = ubi->tag();
      for (block_T::input_const_iterator ii (ub->input().begin());
	   ii != ub->input().end(); ii++)
	{
	  if (ii->port() == port)
	    return find_input_var (pv, *ub, *ii, usename);
	}

      return 0;
    }


    void
    Blocks::change_outport_var (const block_T& b)
    {
      for (block_T::input_const_iterator ii (b.input().begin());
	   ii != b.input().end(); ii++)
	{
	  for (ioport_T::connect_const_iterator ci (ii->connect().begin());
	       ci != ii->connect().end(); ci++)
	    {
	      IOPort* cop = find_out_port (ci->block(), ci->port());
	      if (cop == NULL)
		{
		  std::cout << "Warning: no connection to "
			    << ci->block() << "::"
			    << ci->port() << std::endl;
		  continue;
		}

	      const block_T* cb = cop->block_tag();
	      BlockInfo* bi = cop->blockinfo();

	      // 末端の場合は、一致する変数(output)をextoutに変更
	      if (cb->blocks().size() == 0)
		{
		  for (propagate_var_list_iterator
			 pv (bi->output_var().begin());
		       pv != bi->output_var().end(); pv++)
		    {
		      // check var and output line
		      if (pv->mode () == "output"
			  && pv->name() == ii->line())
			{
			  pv->mode() = "extout";
			  return;
			}
		    }
		  continue;
		}

	      // ブロックの場合 -- XXX (要確認)
	      for (block_T::blocks_const_iterator sb (cb->blocks().begin());
		   sb != cb->blocks().end(); sb++)
		{
		  for (blocks_T::block_const_iterator bi (sb->block().begin());
		       bi != sb->block().end(); bi++)
		    {
		      // Outportかつconnectのportとブロック名が一致する
		      // なら、inputを切り替えて継続
		      if (bi->blocktype() == "Outport"
			  && bi->name() == ci->port())
			{
			  change_outport_var (*bi);
			  return;
			}
		    }
		}
	    }
	}
    }


    BlockInfo*
    Blocks::check_demux_output (propagate_var_list& pv,
				const block_T& b,
				const ioport_T& out,
				const ioport_T& start_out,
				std::string& usename)
    {
      BlockInfo* ret = NULL;

      const ioport_T::usename_optional& uo = out.usename ();
      if (uo && *uo == true)
	{
	  if (usename.length () && usename != out.line ())
	    std::cout << "Warning!" << __LINE__
		      << ": name changed by usename: "
		      << usename << " to " << out.line ()
		      << std::endl;
	  usename = out.line ();
	}

      for (ioport_T::connect_const_iterator ci (out.connect().begin());
	   ci != out.connect().end(); ci++)
	{
	  IOPort* iop = find_in_port (ci->block(), ci->port());
	  if (iop == NULL)
	    {
	      std::cout << "Warning: no connection from "
			<< ci->block() << "::" << ci->port() << std::endl;
	      continue;		// no connection
	    }
	  const block_T *cb = iop->block_tag();
	  const ioport_T* in = iop->ioport_tag();
	  BlockInfo* cbi = iop->blockinfo();
	  const BlockInfo* ubi = cbi->upper ();

	  // ブロックを持つ場合は、潜る
	  //
	  // Demuxに行き当たった場合、変数を全て生成する
	  //
	  // Outportの場合でoutputがある場合は切り替えて進む
	  //
	  // それ以外なら何もしない

	  BlockInfo *x = NULL;

	  if (cb->blocks().size())
	    {
	      for (block_T::blocks_const_iterator bsi (cb->blocks().begin());
		   bsi != cb->blocks().end(); bsi++)
		{
		  x = check_subblock_demux_output (pv, in->port(), *bsi,
						   start_out, usename);
		  if (x)
		    ret = x;
		}
	    }
#if 1
	  else if (cbi->type() == BlockInfo::TYPE_DEMUX
		   || cbi->type () == BlockInfo::TYPE_BUSSELECTOR)
	    {
	      std::string mode ("output");
	      if (b.blocktype() == "Inport")
		{
		  BlockInfo* pb = find_block (b.name ());
		  assert (pb);
		  if (pb->upper() == NULL)
		    mode = "extin";
		}

	      for (oldBusPort::const_iterator pi (cbi->busport ().begin ());
		   pi != cbi->busport ().end (); pi++)
		{
		  oldBusLine busline;
		  pi->defrag (busline);
		  for (oldBusLine::const_iterator li (busline.begin ());
		       li != busline.end (); li++)
		    {
#if (DIFFERENT_LINE_USENAME & DEMUX_DIFFERENT_LINE_USENAME) != 0
		      pv.add_var (VarElement (*li, mode));
#else
		      VarElement& ve = pv.add_var (VarElement (*li, mode));
		      ve.usename () = usename;
#endif
		    }
		}

	      ret = cbi;
	    }
#else  // !1
	  else if (cb->blocktype() == "Demux")
	    {
	      int dind = 0;

	      std::string mode ("output");
	      if (b.blocktype() == "Inport")
		{
		  BlockInfo* pb = find_block (b.name ());
		  assert (pb);
		  if (pb->upper() == NULL)
		    mode = "extin";
		}

	      // check only array index (TODO: scalar)
	      for (block_T::output_const_iterator oo (cb->output().begin ());
		   oo != cb->output().end(); oo++)
		{
#if PROCESS_VAR_DEBUG
		  std::cout << "* found Demux " << start_out.line()
			    << "[" << dind << "]" << std::endl;
#endif
		  const std::string* vnp;
		  if (usename.length ())
		    vnp = &usename;
		  else
		    vnp = &start_out.line ();
#if XML_USENAME_DEBUG
		  if (*vnp != start_out.line ())
		    std::cout << "Warning!" << __LINE__
			      << ": change output variable: "
			      << start_out.line () << " to "
			      << *vnp << std::endl;
#endif	// XML_USENAME_DEBUG
		  pv.add_var (VarElement (*vnp, dind, -1, mode, b.name()));
		  dind++;
		}
	      ret = cbi;
	    }
#endif	// !1
	  else if (cb->blocktype() == "Outport" && ubi != NULL)
	    {
	      if (cb->input().size() != 0)
		{
		  x = check_outport_demux_output (pv, ubi, cb->name(),
						  start_out, usename);
		  if (x)
		    ret = x;
		}
	    }
	  else if (cb->blocktype () == "Goto")
	    {
	      Goto* g = find_goto (cb->name ());
	      for (from_list_const_iterator fi (g->from_list ().begin ());
		   fi != g->from_list ().end (); fi++)
		{
		  const From* f = *fi;

		  if (f == NULL)
		    continue;

		  const block_T* fb = f->from_tag ();
		  for (block_T::output_const_iterator
			 oo (fb->output().begin ());
		       oo != fb->output().end(); oo++)
		    {
		      x = check_demux_output (pv, b, *oo, start_out, usename);
		      if (x)
			ret = x;
		    }
		}
	    }
#if HANDLE_RATETRANSISION
	  else if (cb->blocktype () == "RateTransition")
	    {
	      block_T::input_const_iterator oo (cb->output ().begin ());
	      if (oo != cb->output ().end ())
		{
		  x = check_demux_output (pv, b, *oo, start_out, usename);
		  if (x)
		    ret = x;
		}
	    }
#endif	// HANDLE_RATETRANSISION
	  else if (cb->blocktype () == "Merge"
		   || cb->blocktype () == "Scope"
		   || cb->blocktype () == "Terminator")
	    ;
	  else
	    {
	      const std::string* vnp;
	      if (usename.length ())
		vnp = &usename;
	      else
		vnp = &start_out.line ();
#if XML_USENAME_DEBUG
	      if (*vnp != start_out.line ())
		std::cout << "Warning!" << __LINE__
			  << ": change output variable: "
			  << start_out.line () << " to "
			  << *vnp << std::endl;
#endif	// XML_USENAME_DEBUG
	      pv.add_var (VarElement (*vnp, -1, -1, "output", b.name ()));
	    }
	}

      return ret;
    }

    BlockInfo*
    Blocks::check_subblock_demux_output (propagate_var_list& pv,
					 const std::string& port,
					 const blocks_T& bs,
					 const ioport_T& start_out,
					 std::string& usename)
    {
      BlockInfo* ret = NULL;

      for (blocks_T::block_const_iterator bi (bs.block().begin());
	   bi != bs.block().end(); bi++)
	{
	  if (bi->blocktype() == "Inport" && bi->name() == port)
	    {
	      for (block_T::output_const_iterator oo (bi->output().begin ());
		   oo != bi->output().end(); oo++)
		{
		  BlockInfo* x = check_demux_output (pv, *bi, *oo,
						     start_out, usename);
		  if (x)
		    ret = x;
		}
	    }
	}

      return ret;
    }


    BlockInfo*
    Blocks::check_outport_demux_output (propagate_var_list& pv,
					const BlockInfo* up,
					const std::string& port,
					const ioport_T& start_out,
					std::string& usename)
    {
      BlockInfo* ret = NULL;

      const block_T* ub = up->tag();
      for (block_T::output_const_iterator oo (ub->output().begin());
	   oo != ub->output().end(); oo++)
	{
	  if (oo->port() == port)
	    {
	      BlockInfo* x = check_demux_output (pv, *ub, *oo,
						 start_out, usename);
	      if (x)
		ret = x;
	    }
	}

      return ret;
    }


    int
    Blocks::check_subblock_ports_output (BlockInfo& start_binfo,
					 const ioport_T& start_out,
					 std::string& usename,
					 const ioport_T& in,
					 const blocks_T& bs)
    {
      int flag = 0;

      const std::string& port = in.port();
      for (blocks_T::block_const_iterator bi (bs.block().begin());
	   bi != bs.block().end(); bi++)
	{
	  if (bi->blocktype() == "TriggerPort" && bi->name() == port)
	    {
	      BlockInfo* b = find_block (bi->name());
	      assert (b);
	      b->trigger_input () = &in; // link to <input> tag

	      const std::string* vnp;
	      if (usename.length ())
		vnp = &usename;
	      else
		vnp = &start_out.line ();
#if XML_USENAME_DEBUG
	      if (*vnp != start_out.line ())
		std::cout << "Warning!" << __LINE__
			  << ": change output variable: "
			  << start_out.line () << " to "
			  << *vnp << std::endl;
#endif	// XML_USENAME_DEBUG
	      VarElement ve (*vnp, -1, -1,
			     start_binfo.tag ()->name (),
			     "output", start_out.line(), start_out.port());

	      BlockInfo* upper = b->upper();
	      if (upper)
		{
		  upper->trigger_port() = b;
		  upper->trigger_from() = &start_binfo;
		  upper->trigger_var() = ve;

#ifndef ADD_INPUT_TO_ALL_SUBBLOCK
		  // add distination input to SubSystem
		  VarElement ive (start_out.line(), -1, -1, "input",
				  start_binfo.tag ()->name (),
				  in.line(), in.port());
		  upper->input_var ().add_var (ive);
#endif	// ADD_INPUT_TO_ALL_SUBBLOCK
		}

	      // TriggerPort is only one in SubSystem
	      return flag | BLOCKINFO_LINEOUT_TO_TRIGGERPORT;
	    }
	  else if (bi->blocktype() == "EnablePort" && bi->name() == port)
	    {
	      BlockInfo* b = find_block (bi->name());
	      assert (b);
	      b->enable_input () = &in; // link to <input> tag

	      std::string vn (start_binfo.tag()->name() + "_enable");

	      const std::string* vnp;
	      if (usename.length ())
		vnp = &usename;
	      else
		vnp = &vn;
#if XML_USENAME_DEBUG
	      if (*vnp != start_out.line ())
		std::cout << "Warning!" << __LINE__
			  << ": change output variable: "
			  << start_out.line () << " to "
			  << *vnp << std::endl;
#endif	// XML_USENAME_DEBUG
	      VarElement ve (*vnp, -1, -1, "output",
			     start_binfo.tag ()->name (),
			     start_out.line(), start_out.port());

	      BlockInfo* upper = b->upper();
	      if (upper)
		{
		  upper->enable_port() = b;
		  upper->enable_from() = &start_binfo;
		  upper->enable_var() = ve;

#ifndef ADD_INPUT_TO_ALL_SUBBLOCK
		  // add distination input to SubSystem
		  VarElement ive (start_out.line(), -1, -1, "input",
				  start_binfo.tag ()->name (),
				  in.line(), in.port());
		  upper->input_var ().add_var (ive);
#endif	// ADD_INPUT_TO_ALL_SUBBLOCK
		}

	      // EnablePort is only one in SubSystem
	      return flag | BLOCKINFO_LINEOUT_TO_ENABLEPORT;
	    }
	  else if (bi->blocktype() == "ActionPort" && bi->name () == port)
	    {
	      // No Variables, ignore this line

	      return flag | BLOCKINFO_LINEOUT_TO_ACTIONPORT;
	    }
	  else if (bi->blocktype() == "Inport" && bi->name() == port)
	    {
	      for (block_T::output_const_iterator oo (bi->output().begin ());
		   oo != bi->output().end(); oo++)
		{
		  int f = check_ports_output (start_binfo,
					      start_out, usename, port, *oo);
		  flag |= f;
		}
	    }
	  else if (bi->blocktype () != "Goto"
		   && bi->blocktype () != "Scope"
		   && bi->blocktype () != "Terminator"
		   && bi->blocktype () != "Merge") // ignore Merge
	    flag |= BLOCKINFO_LINEOUT_TO_OTHER;
	}

      return flag;
    }


    int
    Blocks::check_outport_ports_output (BlockInfo& start_binfo,
					const ioport_T& start_out,
					std::string& usename,
					const BlockInfo* up,
					const std::string& port)
    {
      const block_T* ub = up->tag();
      for (block_T::output_const_iterator oo (ub->output().begin());
	   oo != ub->output().end(); oo++)
	{
	  if (oo->port() == port)
	    return check_ports_output (start_binfo, start_out, usename,
				       ub->name(), *oo);
	}

      return 0;
    }

    int
    Blocks::check_ports_output (BlockInfo& start_binfo,
				const ioport_T& start_out,
				std::string& usename,
				const std::string& blockname,
				const ioport_T& out)
    {
      int flag = 0;

      const ioport_T::usename_optional uo = out.usename ();
      if (uo && *uo == true)
	{
	  if (usename.length () && usename != out.line ())
	    std::cout << "Warning!" << __LINE__
		      << ": name changed by usename: "
		      << usename << " to " << out.line ()
		      << std::endl;
	  usename = out.line ();
	}

      for (ioport_T::connect_const_iterator ci (out.connect().begin());
	   ci != out.connect().end(); ci++)
	{
	  IOPort* iop = find_in_port (ci->block(), ci->port());
	  if (iop == NULL)
	    {
	      std::cout << "Warning: no connection from "
			<< ci->block() << "::" << ci->port() << std::endl;
	      continue;		// no connection
	    }
	  const block_T *cb = iop->block_tag();
	  const ioport_T* in = iop->ioport_tag();
	  BlockInfo* cbi = iop->blockinfo();
	  BlockInfo* ubi = cbi->upper ();

	  if (cb->blocks().size())
	    {
	      for (block_T::blocks_const_iterator bsi (cb->blocks().begin());
		   bsi != cb->blocks().end(); bsi++)
		{
		  int f = check_subblock_ports_output (start_binfo,
						       start_out, usename,
						       *in, *bsi);
		  flag |= f;
		}
	    }
	  else if (cb->blocktype() == "Outport" && ubi != NULL)
	    {
	      if (cb->input().size() != 0)
		{
		  int f = check_outport_ports_output (start_binfo,
						      start_out, usename,
						      ubi, cb->name());
		  flag |= f;
		}
	    }
	  else if (cb->blocktype () == "Goto")
	    {
	      Goto* g = find_goto (cb->name ());
	      for (from_list_const_iterator fi (g->from_list ().begin ());
		   fi != g->from_list ().end (); fi++)
		{
		  const From* f = *fi;

		  if (f == NULL)
		    continue;

		  const block_T* fb = f->from_tag ();
		  process_variables (start_binfo, start_binfo.tag (),
				     &start_out, fb);
		}
	    }
#if HANDLE_RATETRANSISION
	  else if (cb->blocktype () == "RateTransition")
	    {
	      block_T::output_const_iterator oo (cb->output ().begin ());
	      if (oo != cb->output ().end ())
		flag |= check_ports_output (start_binfo, start_out, usename,
					    blockname, *oo);
	    }
#endif	// HANDLE_RATETRANSISION
	  else if (cb->blocktype () != "Scope"
		   && cb->blocktype () != "Terminator"
		   && cb->blocktype () != "Merge")
	    flag |= BLOCKINFO_LINEOUT_TO_OTHER;
	}

      return flag;
    }

#ifdef ADD_INPUT_TO_ALL_SUBBLOCK
    void
    Blocks::subblock_add_input (BlockInfo& subsys, const block_T& b)
    {
      if (b.blocktype() == "SubSystem" && b.blocks ().size () > 0)
	{
	  for (block_T::blocks_const_iterator bsi (b.blocks().begin());
	       bsi != b.blocks().end(); bsi++)
	    {
	      const blocks_T& bs = *bsi;
	      for (blocks_T::block_const_iterator bi (bs.block().begin());
		   bi != bs.block().end(); bi++)
		subblock_add_input (subsys, *bi);
	    }
	}
      else if (b.blocktype() != "Inport"
	       && b.blocktype() != "Outport")
	{
	  BlockInfo* binfo = find_block (b.name());
	  assert (binfo);

	  BlockInfo* trigger = subsys.trigger_from();
	  BlockInfo* triggerport = subsys.trigger_port ();
	  if (trigger)
	    {
	      const VarElement& fve = subsys.trigger_var();
	      const ioport_T* in = triggerport->trigger_input();

	      VarElement ve (fve.name(), fve.start(), fve.end(), "input",
			     in->line(), in->port());
	      binfo->input_var().add_var (ve);
	    }

	  BlockInfo* enable = subsys.enable_from();
	  BlockInfo* enableport = subsys.enable_port ();
	  if (enable)
	    {
	      const VarElement& fve = subsys.enable_var();
	      const ioport_T* in = enableport->enable_input();

	      VarElement ve (fve.name(), fve.start(), fve.end(), "input",
			     in->line(), in->port());
	      binfo->input_var().add_var (ve);
	    }
	}
    }
#endif

    void
    Blocks::process_variables (BlockInfo& binfo, const block_T* b,
			       const ioport_T* start_out,
			       const block_T* ob)
    {
      if (ob == NULL)
	ob = b;

      // 末端の出力変数
      for (block_T::output_const_iterator oi (ob->output().begin());
	   oi != ob->output().end(); oi++)
	{
	  propagate_var_list pv;
	  BlockInfo* x;
	  std::string usename;
	  const ioport_T* so;

	  if (start_out)
	    so = start_out;
	  else
	    so = &*oi;

	  if ((x = check_demux_output (pv, *b, *oi, *so, usename)) != NULL)
	    {
	      for (propagate_var_list_iterator vi (pv.begin ());
		   vi != pv.end(); vi++)
		{
		  VarElement& ve = *vi;
#if PROCESS_VAR_DEBUG
		  std::cout << b->blocktype() << ':'
			    << b->name() << " outvar=" << ve.name()
			    << "[" << ve.start() << "]"
			    << std::endl;
#endif
		  ve.line () = so->line();
		  ve.port () = so->port();
		  binfo.output_var().add_var (ve);
		}
	    }
	  else
	    {
	      usename.clear ();
	      int flag = check_ports_output (binfo, *so, usename,
					     b->name(), *oi);
	      if (flag & BLOCKINFO_LINEOUT_TO_ENABLEPORT)
		{
		  // found EnablePort
		  std::string vn (b->name() + "_enable");
#if PROCESS_VAR_DEBUG
		  std::cout << b->blocktype() << ':'
			    << b->name() << " outvar=" << vn
			    << std::endl;
#endif
		  const std::string* vnp;
		  if (usename.length ())
		    vnp = &usename;
		  else
		    vnp = &vn;
#if XML_USENAME_DEBUG
		  if (*vnp != vn)
		    std::cout << "Warning!" << __LINE__
			      << ": change output variable: "
			      << vn << " to " << *vnp
			      << std::endl;
#endif	// XML_USENAME_DEBUG
		  VarElement ve (*vnp, -1, -1, "output",
				 binfo.tag ()->name (),
				 so->line(), so->port());
		  binfo.output_var().add_var (ve);
		}
	      else if (flag & BLOCKINFO_LINEOUT_TO_ACTIONPORT)
		{
#if PROCESS_VAR_DEBUG
		  std::cout << b->blocktype() << ':'
			    << b->name() << " action var="
			    << so->line()
			    << std::endl;
#endif
		}
	      else if (flag)
		{
#if PROCESS_VAR_DEBUG
		  std::cout << b->blocktype() << ':'
			    << b->name() << " outvar=" << so->line()
			    << std::endl;
#endif
		  std::string mode ("output");

		  if (b->blocktype () == "Inport"
		      && binfo.upper() == NULL)
		    mode = "extin";

		  const std::string* vnp;
		  if (usename.length ())
		    vnp = &usename;
		  else
		    vnp = &so->line ();
#if XML_USENAME_DEBUG
		  if (*vnp != so->line ())
		    std::cout << "Warning!" << __LINE__
			      << ": change output variable: "
			      << so->line () << " to "
			      << *vnp << std::endl;
#endif	// XML_USENAME_DEBUG
		  VarElement ve (*vnp, -1, -1, mode,
				 binfo.tag ()->name (),
				 so->line(), so->port());
		  binfo.output_var().add_var (ve);
		}
	      else
		{
		  // Nothing propagate block, temporary output
		  if (so->line ().length ())
		    {
		      const std::string* vnp;
		      if (usename.length ())
			vnp = &usename;
		      else
			vnp = &so->line ();
#if XML_USENAME_DEBUG
		      if (*vnp != so->line ())
			std::cout << "Warning!" << __LINE__
				  << ": change output variable: "
				  << so->line () << " to "
				  << *vnp << std::endl;
#endif	// XML_USENAME_DEBUG
		      VarElement ve (*vnp, -1, -1, "output",
				     binfo.tag ()->name (),
				     so->line(), so->port());
		      binfo.output_var().add_var (ve);
		    }
		}
	    }
	}
    }

    void
    Blocks::process_variables (void)
    {
      for (block_map_iterator bi (block_map().begin());
	   bi != block_map().end(); bi++)
	{
	  BlockInfo& binfo = bi->second;
	  const block_T* b = binfo.tag();

	  if (binfo.type() == BlockInfo::TYPE_DEMUX
	      || binfo.type() == BlockInfo::TYPE_MUX
	      || binfo.type () == BlockInfo::TYPE_BUSCREATOR
	      || binfo.type () == BlockInfo::TYPE_BUSSELECTOR)
	    continue;

	  if (b->blocktype() == "Scope" || b->blocktype () == "Terminator"
	      || b->blocktype() == "Goto" || b->blocktype () == "From"
#if HANDLE_RATETRANSISION
	      || b->blocktype() == "RateTransition"
#endif	// HANDLE_RATETRANSISION
	      )
	    continue;		// ignore blocks

	  if (b->blocks().size())
	    continue;		// サブブロックを持つブロック

	  if (b->blocktype() != "Inport" || binfo.upper() == NULL)
	    process_variables (binfo, b);

	  if (b->blocktype() == "Outport" && binfo.upper() != NULL)
	    continue;

	  if (b->blocktype () == "Merge")
	    continue;		// no check input

	  // 入力変数の探索
	  for (block_T::input_const_iterator ii = b->input().begin();
	       ii != b->input().end(); ii++)
	    {
	      propagate_var_list pv;
	      std::string usename;
	      if (find_input_var (pv, *b, *ii, usename))
		{
		  for (propagate_var_list_iterator vi (pv.begin());
		       vi != pv.end(); vi++)
		    {
		      VarElement& ve = *vi;
#if PROCESS_VAR_DEBUG
		      std::cout << "invar=" << ve.name()
				<< "(" << ve.mode() << ")" << std::endl;
#endif
		      ve.line () = ii->line();
		      ve.port () = ii->port();
		      binfo.input_var().add_var(ve);
		    }
		}
#if PROCESS_VAR_DEBUG
	      else
		std::cout << "invar=???" << std::endl;
#endif
	    }
	}

#ifdef ADD_INPUT_TO_ALL_SUBBLOCK
      // TriggerPort/EnablePortの対応
      for (block_map_iterator bi (block_map().begin());
	   bi != block_map().end(); bi++)
	{
	  BlockInfo& binfo = bi->second;
	  block_T* b = binfo.tag();

	  if (b->blocks().size() == 0)
	    continue;

	  subblock_add_input (binfo, *b);
	}
#endif	// ADD_INPUT_TO_ALL_SUBBLOCK

      // 最外のOutportに繋がる変数をextoutにする
      for (block_map_iterator bi (block_map().begin());
	   bi != block_map().end(); bi++)
	{
	  BlockInfo& binfo = bi->second;
	  block_T* b = binfo.tag();

	  if (b->blocktype() != "Outport" || binfo.upper() != NULL)
	    continue;

	  change_outport_var (*b);
	}
    }


    void
    Blocks::process_port (void)
    {
      for (block_map_iterator bi (block_map().begin());
	   bi != block_map().end(); bi++)
	{
	  BlockInfo& binfo = bi->second;
	  block_T* b = binfo.tag();
	  int index = 0;

	  for (block_T::input_const_iterator ii (b->input().begin());
	       ii != b->input().end(); ii++)
	    add_in_port_map (ii->port(),
			     IOPort (IOPort::IN, b, &*ii, index++, &binfo));

	  index = 0;
	  for (block_T::output_const_iterator oi (b->output().begin());
	       oi != b->output().end(); oi++)
	    {
	      IOPort&
		out = add_out_port_map (oi->port(),
					IOPort (IOPort::OUT, b, &*oi, index++,
						&binfo));

	      const ioport_T::dimensions_optional& dimo = oi->dimensions ();
	      if (dimo)
		{
		  const std::string& sdim = *dimo;
		  out.businfo ().dim_parse (sdim);
		  if (!has_dimensions ())
		    has_dimensions () = true;
		}

	      const ioport_T::selectedsignal_optional&
		sso = oi->selectedsignal ();
	      if (sso)
		{
		  const std::string& ssso = *sso;
		  out.businfo ().selected_signal (ssso);
		}
	    }

	  if (b->blocktype () == "TriggerPort")
	    {
	      BlockInfo* upper = binfo.upper();
	      if (upper)
		upper->trigger_port() = &binfo;
	    }

	  if (b->blocktype () == "EnablePort")
	    {
	      BlockInfo* upper = binfo.upper();
	      if (upper)
		upper->enable_port() = &binfo;
	    }
	}

      for (ioport_map_iterator o (out_port_map ().begin ());
	   o != out_port_map ().end (); o++)
	{
	  IOPort& out_iop = o->second;
	  const ioport_T* out = out_iop.ioport_tag ();
	  ioport_deq_T& inports = out_iop.inports ();

	  for (ioport_T::connect_const_iterator
		 ci (out->connect ().begin ());
	       ci != out->connect ().end (); ci++)
	    {
	      IOPort* in_iop = find_in_port (ci->block(), ci->port());
	      if (in_iop == NULL)
		std::cout << "Warning: no connection to "
			  << ci->block() << "::" << ci->port() << std::endl;
	      else
		in_iop->outport () = &out_iop;

	      inports.push_back (in_iop);
	    }
	}

      for (block_map_iterator bi (block_map().begin());
	   bi != block_map().end(); bi++)
	{
	  BlockInfo& binfo = bi->second;
	  block_T* b = binfo.tag();

	  if (binfo.type () == BlockInfo::TYPE_MUX)
	    {
	      assert (b->output ().size () == 1);

	      block_T::output_const_iterator oi = b->output ().begin ();
	      const ioport_T& out = *oi;

	      IOPort* out_port = find_out_port (b->name (), out.port ());
	      assert (out_port);

	      NewBus& out_bus = out_port->businfo ();
	      if (out_bus.get_type () == NewBus::Bus)
		binfo.type () = BlockInfo::TYPE_BUSCREATOR;
	    }
	  else if (binfo.type () == BlockInfo::TYPE_DEMUX)
	    {
	      assert (b->input ().size () == 1);

	      block_T::input_const_iterator ii = b->input ().begin ();
	      const ioport_T& in = *ii;

	      IOPort* in_port = find_in_port (b->name (), in.port ());
	      assert (in_port);

	      NewBus& in_bus = in_port->in_businfo ();
	      if (in_bus.get_type () == NewBus::Bus)
		binfo.type () = BlockInfo::TYPE_BUSSELECTOR;
	    }
	}
    }

    void
    Blocks::process_goto_from (void)
    {
      for (block_map_iterator bi (block_map ().begin ());
	   bi != block_map ().end (); bi++)
	{
	  BlockInfo& binfo = bi->second;
	  block_T* b = binfo.tag();

	  if (b->blocktype () == "Goto")
	    add_goto_map (b->name (), Goto (b, &binfo));
	  else if (b->blocktype () == "From")
	    add_from_map (b->name (), From (b, &binfo));
	}

      for (goto_map_iterator gi (goto_map ().begin ());
	   gi != goto_map ().end (); gi++)
	{
	  Goto& g = gi->second;
	  const block_T* gb = g.goto_tag ();
	  for (block_T::input_const_iterator oi (gb->output ().begin ());
	       oi != gb->output ().end (); oi++)
	    {
	      for (ioport_T::connect_const_iterator
		     ci (oi->connect ().begin ());
		   ci != oi->connect ().end (); ci++)
		{
		  From* f = find_from (ci->block ());
		  if (f)
		    {
		      BlockInfo* finfo = f->from_blockinfo ();
		      block_T* fb = finfo->tag ();

		      g.add_from (f);
		      Goto* fg = f->from_goto ();
		      if (fg != NULL && fg != &g)
			{
			  std::cerr << "Error: From["
				    << fb->name ()
				    << "]: has multple Goto: "
				    << fg->goto_blockinfo ()->tag ()->name ()
				    << ", "
				    << ci->block ()
				    << std::endl;
			}
		      else
			f->from_goto () = &g;
#if 0
		      std::cout << "Goto["
				<< gb->name ()
				<< "] -> From["
				<< f->from_blockinfo ()->tag ()->name ()
				<< "]" << std::endl;
#endif
		    }
		  else
		    {
		      std::cout << "Warning: Goto["
				<< gb->name ()
				<< "]: has no From" << std::endl;
		    }
		}
	    }
	}
    }

    size_t
    Blocks::mux_out_index (IOPort* out_port, IOPort* in_port)
    {
      const NewBus& obus = out_port->businfo ();
      assert (obus.get_type () != NewBus::Bus);

      const block_T* b = in_port->block_tag ();
      size_t out_index = 0;
      for (block_T::input_const_iterator i (b->input ().begin ());
	   i != b->input ().end (); i++)
	{
	  const IOPort* in = find_in_port (b->name (), i->port ());
	  if (in_port == in)
	    break;

	  const NewBus& ibus = in->in_businfo ();
	  assert (ibus.get_type () == NewBus::Signal);

	  const NewSignal& s = ibus[0];
	  assert (s.dim_size () > 0);
	  out_index += s[0];
	}

      return out_index;
    }

    bool
    Blocks::track_bus_inport (IOPort* start_port,
			      int chunk_start, int chunk_size,
			      ioport_deq_T& pass_ports,
			      IOPort* in_port, NewMux* mux,
			      std::string& usename,
			      track_mode mode)
    {
      BlockInfo* binfo = in_port->blockinfo ();
      const block_T* b = binfo->tag ();

      assert (b->blocktype () != "From");

      NewBus& start_bus = start_port->businfo ();
      NewSignal& start_signal = start_bus[0];
      NewSignal::Type signal_type = start_signal.get_type ();

      bool used = false;

      if (b->blocktype () == "SubSystem" && b->blocks ().size () > 0)
	{
	  const ioport_T* in = in_port->ioport_tag ();
	  BlockInfo* n_binfo = find_block (in->port ());
	  assert (n_binfo);
	  const block_T* n_b = n_binfo->tag ();
	  if (n_b->blocktype () == "Inport")
	    return track_bus_output (start_port, chunk_start, chunk_size,
				     pass_ports, *n_b, mux, usename, mode);

	  assert (n_b->blocktype () == "TriggerPort"
		  || n_b->blocktype () == "EnablePort"
		  || n_b->blocktype () == "ActionPort");
	}

      if (b->blocktype () == "Goto")
	{
	  Goto* g = find_goto (b->name ());
	  for (from_list_const_iterator fi (g->from_list ().begin ());
	       fi != g->from_list ().end (); fi++)
	    {
	      const From* f = *fi;
	      if (f == NULL)
		continue;

	      const block_T* fb = f->from_tag ();
	      bool u = track_bus_output (start_port, chunk_start, chunk_size,
					 pass_ports, *fb, mux, usename, mode);
	      if (u)
		used = u;
	    }
	}
      else if (b->blocktype () == "Outport" && binfo->upper () != NULL)
	{
	  const block_T* ub = binfo->upper ()->tag ();
	  for (block_T::output_const_iterator out (ub->output ().begin ());
	       out != ub->output ().end (); out++)
	    {
	      if (out->port () == b->name ())
		{
		  IOPort* out_port = find_out_port (ub->name (), out->port ());
		  assert (out_port);

		  const ioport_T::usename_optional& uo = out->usename ();
		  if (uo && *uo)
		    {
		      assert (out_port->businfo ().get_type ()
			      == NewBus::Signal);
		      if (usename.length () && usename != out->line ())
			{
			  std::cout << "Warning!" << __LINE__
				    << ": name changed by usename: "
				    << usename << " to " << out->line ()
				    << std::endl;
			}
		      usename = out->line ();
		    }

		  // 条件付き Subsystem の初期値付き VirtualOutport か
		  // も知れないのでここで pass に追加
		  pass_ports.push_back (in_port);

		  bool f = track_bus_outport (start_port,
					      chunk_start, chunk_size,
					      pass_ports, out_port, mux,
					      usename, mode);
		  pass_ports.pop_back ();

		  return f;
		}
	    }
	}
      else if (binfo->type () == BlockInfo::TYPE_BUSCREATOR)
	{
	  assert (b->output ().size () == 1);

	  block_T::output_const_iterator oi (b->output ().begin ());
	  const ioport_T& out = *oi;

	  // 接続先がなければ何もしない
	  if (out.connect ().size () == 0)
	    return used;

	  // ここに usename があってはならない。なぜならばここを通る全
	  // てのシグナルが同じになってしまう
	  const ioport_T::usename_optional& uo = out.usename ();
	  assert (!uo || !*uo);

	  IOPort* out_port = find_out_port (b->name (), out.port ());
	  assert (out_port);

	  TRACK_BUS_DEBUG_LOG (line_varname (usename, start_port),
			       b, mux, "track_bus_outport");

	  return track_bus_outport (start_port, chunk_start, chunk_size,
				    pass_ports, out_port, mux, usename, mode);
	}
      else if (binfo->type () == BlockInfo::TYPE_BUSSELECTOR)
	{
	  for (block_T::output_const_iterator oi (b->output ().begin ());
	       oi != b->output ().end (); oi++)
	    {
	      const ioport_T& out = *oi;

	      // 接続先がなければ何もしない
	      if (out.connect ().size () == 0)
		continue;

	      // ここにusenameがあってはならない
	      const ioport_T::usename_optional& uo = out.usename ();
	      assert (!uo || !*uo);

	      IOPort* out_port = find_out_port (b->name (), out.port ());
	      assert (out_port);

	      NewBus& out_bus = out_port->businfo ();
	      assert (out_bus.bus_size () > 0);

	      const std::string& varname = line_varname (usename, start_port);

	      //
	      // 出力がシグナルならselectedsignalをチェック
	      // そうでなければ、そのまま先に進む
	      //
	      if (out_bus.get_type () == NewBus::Signal)
		{
		  //
		  // BusをSignalかベクタへSelect
		  //

		  const std::string& this_signal = out_bus.selected_signal ();

		  // 選択されるべきシグナルがない
		  if (this_signal.length () == 0)
		    continue;

		  // selectedsignalと不一致
		  if (varname != this_signal
		      && !match_line_name (pass_ports, this_signal))
		    continue;

		  // ここではDemuxされていればそのままの状態で、次
		  // へ送る
		}

	      TRACK_BUS_DEBUG_LOG (varname, b, mux, "track_bus_outport");

	      bool u = track_bus_outport (start_port,
					  chunk_start, chunk_size,
					  pass_ports, out_port, mux,
					  usename, mode);
	      if (u)
		used = u;
	    }
	}
      else if (binfo->type () == BlockInfo::TYPE_MUX)
	{
	  assert (b->output ().size () == 1);

	  block_T::output_const_iterator oi (b->output ().begin ());
	  const ioport_T& out = *oi;

	  // 接続先がなければ何もしない
	  if (out.connect ().size () == 0)
	    return used;

	  // ここに usename があってはならない。なぜならばここを通る全
	  // てのシグナルが同じになってしまう
	  const ioport_T::usename_optional& uo = out.usename ();
	  assert (!uo || !*uo);

	  IOPort* out_port = find_out_port (b->name (), out.port ());
	  assert (out_port);

	  NewBus& in_bus = in_port->in_businfo ();
	  NewBus& out_bus = out_port->businfo ();
	  NewMux out_mux;
	  NewMux* out_mux_ptr = &out_mux;

#if TRACK_BUS_DEBUG
	  const std::string& varname = line_varname (usename, start_port);
#endif

	  // 入力はスカラかベクタなのでMuxする
	  assert (in_bus.get_type () == NewBus::Signal);

	  NewMux sig_elem_mux;
	  std::vector < NewVecElem > vec_elem;

	  assert (out_bus.bus_size () > 0);

	  NewSignal& out_vec = out_bus[0];

	  assert (out_vec.dim_size () > 0);

	  size_t out_vec_size = out_vec[0];

	  // このinputに対するMuxの位置を算出
	  size_t out_mux_idx = mux_out_index (out_port, in_port);

	  out_mux.resize (out_vec_size);

	  if (mux)
	    {
	      //
	      // 入力が既にMux/Demuxされているベクタの要素の場合
	      //

	      // 出力がベクタなので、元のmuxを新たなMuxの位置へ、
	      // フラットに格納する
	      assert (out_mux_idx + mux->count () < out_vec_size);

	      size_t c = mux->index ();
	      size_t out_mux_end = out_mux_idx + mux->count ();
	      for (size_t i = out_mux_idx; i < out_mux_end; i++)
		{
		  NewMuxElem& me = (*mux)[c++];
		  if (me.type () != NewMuxElem::None)
		    out_mux[i] = &me;
		}
	      out_mux.region (out_mux_idx, mux->count ());
	    }
	  else if (signal_type == NewSignal::Vector)
	    {
	      //
	      // VectorのMux
	      //

	      size_t in_vec_size;
	      size_t in_vec_idx;

	      if (chunk_start == -1)
		{
		  in_vec_idx = 0;
		  in_vec_size = start_signal[0];
		}
	      else
		{
		  in_vec_idx = chunk_start;
		  in_vec_size = chunk_size;
		}

	      assert (out_mux_idx + in_vec_size <= out_vec_size);

	      // Muxを作る
	      vec_elem.resize (in_vec_size);
	      for (size_t i = 0; i < in_vec_size; i++)
		{
		  NewVecElem& ve = vec_elem[i];
		  ve.port () = start_port;
		  ve.index () = in_vec_idx + i;
		  out_mux[out_mux_idx + i] = &ve;
		}
	      out_mux.region (out_mux_idx, in_vec_size);
	    }
	  else
	    {
	      //
	      // ScalarのMux
	      assert (in_bus.bus_size () > 0);

	      NewSignal& in_signal = in_bus[0];

	      if (b->input ().size () == 1)
		{
		  // 1:1 の Mux で入力がスカラの場合は、そのまま通す。
		  //
		  // 1次元でサイズが1の場合はスカラであり、
		  // 1次元でサイズが1のベクタは存在しないため。
		  out_mux_ptr = NULL;
		}
	      else
		{
		  assert (out_mux_idx < out_vec_size);

		  out_mux.resize (out_vec_size);
		  in_signal.port () = start_port;
		  out_mux[out_mux_idx] = &in_signal;
		  out_mux.region (out_mux_idx, 1);
		}
	    }

	  TRACK_BUS_DEBUG_LOG (varname, b, &out_mux, "track_bus_outport");

	  return track_bus_outport (start_port, chunk_start, chunk_size,
				    pass_ports, out_port, out_mux_ptr,
				    usename, mode);
	}
      else if (binfo->type () == BlockInfo::TYPE_DEMUX)
	{
	  size_t in_demux_idx = 0;

	  for (block_T::output_const_iterator oi (b->output ().begin ());
	       oi != b->output ().end (); oi++)
	    {
	      const ioport_T& out = *oi;

	      // 接続先がなければ何もしない
	      if (out.connect ().size () == 0)
		continue;

	      const ioport_T::usename_optional& uo = out.usename ();
#if 1
	      // usename があったらおかしいと思う
	      assert (!uo || !*uo);
#else
	      if (uo && *uo)
		{
		  if (usename.length () && usename != out.line ())
		    {
		      std::cout << "Warning!" << __LINE__
				<< ": name changed by usename: "
				<< usename << " to " << out.line ()
				<< std::endl;
		    }
		  usename = out.line ();
		}
#endif

	      IOPort* out_port = find_out_port (b->name (), out.port ());
	      assert (out_port);

	      NewBus& out_bus = out_port->businfo ();
	      assert (out_bus.bus_size () > 0);
	      assert (out_bus.get_type () != NewBus::Bus);

	      // Signal(スカラ、ベクタ)またはMux/Demuxされたベクタ
	      // をDemux

	      NewSignal& out_signal = out_bus[0];

	      assert (out_signal.dim_size () > 0);

	      size_t out_vec_size = out_signal[0];

	      size_t in_demux_next_idx = in_demux_idx + out_vec_size;

	      NewMux out_mux;

#if TRACK_BUS_DEBUG
	      const std::string&
		varname = line_varname (usename, start_port);
#endif

	      if (mux)
		{
		  bool demuxed = false;
		  out_mux.resize (out_vec_size);
		  size_t c = in_demux_idx;
		  for (size_t i = 0; i < out_vec_size; i++)
		    {
		      NewMuxElem& me = (*mux)[c++];
		      if (me.type () == NewMuxElem::None)
			continue;

		      out_mux[i] = &me;
		      demuxed = true;
		    }

		  // 到達するものはないのでもう呼ばない
		  if (demuxed)
		    {
		      out_mux.region (0, out_vec_size);

		      TRACK_BUS_DEBUG_LOG (varname, b, &out_mux,
					   "track_bus_outport");

		      bool u = track_bus_outport (start_port,
						  chunk_start, chunk_size,
						  pass_ports, out_port,
						  &out_mux, usename, mode);
		      if (u)
			used = u;
		    }
		}
	      else if (signal_type == NewSignal::Scalar)
		{
		  // スカラの場合は、1:1 のDemux以外は受け付けない
		  assert (out_vec_size == 1);

		  // そのまま通す
		  bool u = track_bus_outport (start_port,
					      chunk_start, chunk_size,
					      pass_ports, out_port,
					      NULL, usename, mode);
		  if (u)
		    used = u;
		}
	      else
		{
		  assert (signal_type == NewSignal::Vector);

		  int demux_chunk_start;
		  if (chunk_start != -1)
		    {
		      assert ((size_t)chunk_size >= out_vec_size);
		      demux_chunk_start = chunk_start + in_demux_idx;
		    }
		  else
		    {
		      assert (start_signal.dim_size () > 0);
		      assert (start_signal[0] >= out_vec_size);
		      demux_chunk_start = in_demux_idx;
		    }

		  TRACK_BUS_DEBUG_LOG (varname, b, NULL, "track_bus_outport");

		  bool u = track_bus_outport (start_port,
					      demux_chunk_start, out_vec_size,
					      pass_ports, out_port, NULL,
					      usename, mode);
		  if (u)
		    used = u;
		}

	      in_demux_idx = in_demux_next_idx;
	    }
	}
      else if (b->blocktype () == "Merge"
	       || b->blocktype () == "Scope"
	       || b->blocktype () == "Terminator")
	;			// 何もしない
      else
	{
	  //
	  // ここは処理ブロックに到達した状態
	  //

	  used = true;

	  // TODO ブロック間のデータフローを作る

	  BlockInfo* s_binfo = start_port->blockinfo ();
	  const block_T* s_btag = start_port->block_tag ();

	  const std::string& s_linenam = start_port->ioport_tag ()->line ();
	  const std::string& s_portnam = start_port->ioport_tag ()->port ();

	  BlockInfo* in_binfo = in_port->blockinfo ();
	  const block_T* in_btag = in_port->block_tag ();

	  const std::string& varname = line_varname (usename, start_port);

#if TRACK_BUS_DEBUG
	  std::cout << "% " << varname;
	  if (signal_type == NewSignal::Vector)
	    std::cout << '[' << start_signal[0] << ']';

	  std::cout << '@'
		    << s_btag->name ()
		    << " -> "
		    << in_btag->name ()
		    << ':';
#endif

	  NewBus& in_bus = in_port->in_businfo ();

	  NewVarChunks chunks;

	  if (mux)
	    {
#if TRACK_BUS_DEBUG
	      ::std::cout << *mux;
#endif

	      assert (in_bus.get_type () != NewBus::Bus);

	      mux->mux_to_chunks (chunks);
	    }
	  else if (chunk_start != -1)
	    {
	      int chunk_end = -1;
	      if (chunk_size > 1)
		chunk_end = chunk_start + chunk_size;

#if TRACK_BUS_DEBUG
	      if (chunk_end == -1)
		std::cout << varname
			  << '[' << chunk_start << ']';
	      else
		std::cout << varname
			  << '[' << chunk_start << ".." << chunk_end << ']';
#endif

	      chunks.push_back (NewVarChunkElement (chunk_start, chunk_end));
	    }
	  else
	    {
#if TRACK_BUS_DEBUG
	      std::cout << varname;
#endif

	      chunks.push_back (NewVarChunkElement (-1, -1));
	    }

#if TRACK_BUS_DEBUG
	  std::cout << std::endl;
#endif

	  std::string vinmode ("input");
	  std::string voutmode ("output");
	  if (s_btag->blocktype () == "Inport"
	      && s_binfo->upper () == NULL)
	    vinmode = "extin";

	  // 後からusenameが指定された場合、それ以前に現れている信号名
	  // を更新する必要がある
	  for (propagate_var_list_iterator
		 ov (s_binfo->output_var ().begin ());
	       ov != s_binfo->output_var ().end (); ov++)
	    {
	      VarElement& ve = *ov;
	      if (ve.port () == s_portnam
		  && ve.line () == s_linenam
		  && ve.name () == s_linenam
		  && varname != s_linenam)
		{
		  // 信号名を更新
		  ve.name () = varname;

		  // 関連付けられたものを同様に置換。これには入力ブロッ
		  // クの変数の他、条件付きSubsystemのOutportが含まれる
		  for (pvar_ptr_list_iterator ipv (ve.velist ().begin ());
		       ipv != ve.velist ().end (); ipv++)
		    {
		      VarElement *ive = *ipv;
		      assert (ive->name () == s_linenam);
		      ive->name () = varname;
		    }
		}
	    }

	  if (mode == TRACK_BUS_PV)
	    {
	      for (size_t i = 0; i < chunks.size (); i++)
		{
		  int s = chunks[i].start ();
		  int e = chunks[i].end ();

		  // 名前とポートの情報を設定する
		  VarElement ve (varname, s, e, "", s_btag->name (),
				 s_linenam, s_portnam);

		  VarElement& sve = s_binfo->output_var ().add_var (ve);

		  sve.mode () = voutmode;
		  sve.switched () = true;

		  // input_varの方は、line/portは一致することはないので、
		  // 名前だけ判定するようにする
		  ve.line ().clear ();
		  ve.port ().clear ();

		  VarElement& dve = in_binfo->input_var ().add_var (ve);
		  dve.mode () = vinmode;
		  dve.line () = in_port->ioport_tag ()->line ();
		  dve.port () = in_port->ioport_tag ()->port ();
		  dve.switched () = true;

		  // 出力変数と関連付ける
		  sve.velist ().add_pvar (&dve);
		}
	    }
	  else
	    {
	      BlockLink::link_type ltype = BlockLink::TYPE_DATA;
	      if ((s_btag->blocktype () == "Inport"
		   && s_binfo->upper () == NULL)
		  || (in_btag->blocktype () == "Outport"
		      && in_binfo->upper () == NULL))
		ltype = BlockLink::TYPE_PORT;

	      // 信号名(変数名)の適正化
	      const std::string* varp = &varname;
	      VarElement* ovp = NULL;
	      for (propagate_var_list_iterator
		     pv (s_binfo->output_var ().begin ());
		   pv != s_binfo->output_var ().end (); pv++)
		{
		  VarElement& ve = *pv;
		  if (ve.port () == s_portnam && ve.line () == s_linenam)
		    {
		      ovp = &ve;
		      if (ve.name () != varname)
			varp = &ve.name ();
		      break;
		    }
		}

	      // 変数検索用の正規表現
	      std::tr1::regex vpat ("\\b" + *varp + "\\b");

	      // 直近の条件付きSubsystemのOutportからの初期値送信のため
	      // のリンク生成
	      for (ioport_deq_T::const_iterator p (pass_ports.begin ());
		   p != pass_ports.end (); p++)
		{
		  const IOPort* io = *p;
		  if (io->iotype () == IOPort::OUT)
		    continue;

		  BlockInfo* bi = io->blockinfo ();
		  if (bi->type () != BlockInfo::TYPE_VIRTUALOUTPORT)
		    continue;

		  // 出力変数が初期設定されていなければ終了
		  std::tr1::smatch mr;
		  if (!std::tr1::regex_search (bi->init_code (), mr, vpat))
		    break;

		  VarElement ve (*varp, -1, -1, "", s_btag->name ());

		  // 処理済みであればやめる
		  if (std::find (bi->output_var ().begin (),
				 bi->output_var ().end (), ve)
		      != bi->output_var ().end ())
		    break;

		  // VirtialOutportの出力変数を登録する
		  VarElement& ive = bi->output_var ().add_var (ve);
		  ive.mode () = voutmode;
		  ive.switched () = true;

		  // VirtualOutport(forward)から
		  // 出力(start)ブロック(backward)へのリンク作成
		  block_link& vpforw = bi->forward ();
		  block_link& stback = s_binfo->backward ();

		  // forward側
		  block_link_iterator
		    fli = std::find (vpforw.begin (), vpforw.end (), s_binfo);

		  // backward側
		  block_link_iterator
		    bli = std::find (stback.begin (), stback.end (), bi);

		  BlockLink* fbl;
		  BlockLink* bbl;

		  if (fli == vpforw.end ())
		    {
		      // リンクがなければ、初期値送信のリンクを作る
		      assert (bli == stback.end ());
		      vpforw.push_back (BlockLink (s_binfo,
						   BlockLink::TYPE_INIT));
		      fbl = &vpforw.back ();
		      stback.push_back (BlockLink (bi,
						   BlockLink::TYPE_INIT));
		      bbl = &stback.back ();
		      fbl->linkpair (bbl);
		      bbl->linkpair (fbl);
		    }
		  else
		    {
		      // リンクがある、これは複数の信号がある事を意味し
		      // ているはず
		      assert (fli->type () == BlockLink::TYPE_INIT);
		      assert (bli != stback.end ());

		      fbl = &*fli;
		      bbl = &*bli;
		    }

		  // 出力側ブロックの変数を作成
		  // ここでは変数そのものが必要だが、出力側ブロックには
		  // 関係ないので、作業領域にline/portを指定せず登録
		  ve.mode () = vinmode;
		  VarElement& ove = bi->work_var ().add_var (ve);

		  // VarElementに実際のVariableEntを設定
		  var_list& ovpvl = ovp->varent ();
		  for (var_list_iterator ovpvli (ovpvl.begin ());
		       ovpvli != ovpvl.end (); ovpvli++)
		    {
		      VariableEnt* vent = *ovpvli;
		      ive.varent ().add_varent (vent).used (true);
		      ove.varent ().add_varent (vent).used (true);
		    }

		  // 変数を設定
		  fbl->vars ().push_back (&ove);
		  bbl->vars ().push_back (&ive);
		}

	      // 末端同士のリンクを作成
	      BlockLinkPairList pairlist;
	      s_binfo->make_link (s_binfo, in_binfo, pairlist, ltype);

	      // リンクがなければ何もしない
	      if (pairlist.size () == 0)
		return used;

	      for (size_t i = 0; i < chunks.size (); i++)
		{
		  int s = chunks[i].start ();
		  int e = chunks[i].end ();
		  if (s != -1 && e != -1 && s + 1 == e)
		    e = -1;

		  // 名前の他、ポートの情報も設定する
		  VarElement ve (*varp, s, e, vinmode, s_btag->name (),
				 s_linenam, s_portnam);

		  propagate_var_list_iterator
		    ovi = std::find (s_binfo->output_var ().begin (),
				     s_binfo->output_var ().end (), ve);
		  if (ovi == s_binfo->output_var ().end ())
		    continue;

		  VarElement& ove = *ovi;
		  var_list& ovl = ove.varent ();

		  // 入力側は、変数名のみで探す
		  ve.line ().clear ();
		  ve.port ().clear ();

		  propagate_var_list_iterator
		    ivi = std::find (in_binfo->input_var ().begin (),
				     in_binfo->input_var ().end (), ve);
		  if (ivi == in_binfo->input_var ().end ())
		    continue;

		  VarElement& ive = *ivi;
		  var_list& ivl = ive.varent ();

		  // 出力側ブロックの出力信号に対する変数が、入力側ブロッ
		  // クの入力変数として現れていればusedとする。ただし、
		  // 既に通常はparse_complete()にてusedである。
		  // これはブロックの変数が複数ある場合のチェックである
		  for (var_list_iterator ovli (ovl.begin ());
		       ovli != ovl.end (); ovli++)
		    {
		      VariableEnt* ovent = *ovli;
		      var_list_iterator ivli = std::find (ivl.begin (),
							  ivl.end (), ovent);
		      if (ivli == ivl.end ())
			continue;
		      ivli->used (true);
		    }

		  // 入力側ブロックの入力信号に対する変数が、出力側ブロッ
		  // クの出力変数として現れていればusedとする。ただし、
		  // 既に通常はparse_complete()にてusedである。
		  // これはブロックの変数が複数ある場合のチェックである
		  for (var_list_iterator ivli (ivl.begin ());
		       ivli != ivl.end (); ivli++)
		    {
		      VariableEnt* ivent = *ivli;
		      var_list_iterator ovli = std::find (ovl.begin (),
							  ovl.end (), ivent);
		      if (ovli == ovl.end ())
			continue;
		      ovli->used (true);
		    }

		  // ブロックのリンクに、伝播する信号を登録
		  for (BlockLinkPairList::iterator p (pairlist.begin ());
		       p != pairlist.end (); p++)
		    {
		      BlockLink* fb = p->forward ();
		      fb->vars ().push_back (&ive);

		      BlockLink* bb = p->backward ();
		      bb->vars ().push_back (&ove);
		    }
		}
	    }
	}

      return used;
    }

    bool
    Blocks::track_bus_output (IOPort* start_port,
			      int chunk_start, int chunk_size,
			      ioport_deq_T& pass_ports,
			      const block_T& block, NewMux* mux,
			      std::string& usename,
			      track_mode mode)
    {
      bool used = false;

      for (block_T::output_const_iterator oi (block.output ().begin ());
	   oi != block.output ().end (); oi++)
	{
	  IOPort* oi_outport = find_out_port (block.name (), oi->port ());
	  assert (oi_outport);

	  const ioport_T::usename_optional& uo = oi->usename ();
	  if (uo && *uo)
	    {
	      assert (oi_outport->businfo ().get_type () == NewBus::Signal);
	      if (usename.length () && usename != oi->line ())
		{
		  std::cout << "Warning!" << __LINE__
			    << ": name changed by usename: "
			    << usename << " to " << oi->line ()
			    << std::endl;
		}
	      usename = oi->line ();
	    }

	  bool u = track_bus_outport (start_port, chunk_start, chunk_size,
				      pass_ports, oi_outport, mux,
				      usename, mode);
	  if (u)
	    used = u;
	}

      return used;
    }

    bool
    Blocks::track_bus_outport (IOPort* start_port,
			       int chunk_start, int chunk_size,
			       ioport_deq_T& pass_ports,
			       IOPort* out_port, NewMux* mux,
			       std::string& usename,
			       track_mode mode)
    {
      bool used = false;

      pass_ports.push_back (out_port);

      for (ioport_deq_T::iterator ii (out_port->inports ().begin ());
	   ii != out_port->inports ().end (); ii++)
	{
	  IOPort* in_port = *ii;

	  if (in_port == NULL)
	    continue;

	  const block_T *cb = in_port->block_tag ();
	  if (cb->blocks ().size () == 0)
	    {
	      bool u = track_bus_inport (start_port, chunk_start, chunk_size,
					 pass_ports, in_port, mux,
					 usename, mode);
	      if (u)
		used = u;

	      continue;
	    }

	  // sub block
	  const ioport_T* in = in_port->ioport_tag();
	  for (block_T::blocks_const_iterator bsi (cb->blocks ().begin ());
	       bsi != cb->blocks ().end (); bsi++)
	    {
	      for (blocks_T::block_const_iterator bi (bsi->block ().begin ());
		   bi != bsi->block ().end (); bi++)
		{
		  if (bi->name () != in->port ())
		    continue;

		  if (bi->blocktype () == "Inport")
		    {
		      bool u = track_bus_output (start_port,
						 chunk_start, chunk_size,
						 pass_ports, *bi, mux,
						 usename, mode);
		      if (u)
			used = u;
		    }
		  else if (bi->blocktype () == "ActionPort"
			   || bi->blocktype () == "TriggerPort"
			   || bi->blocktype () == "EnablePort")
		    {
		      bool u = track_bus_inport (start_port,
						 chunk_start, chunk_size,
						 pass_ports, in_port, mux,
						 usename, mode);
		      if (u)
			used = u;
		    }
		}
	    }
	}

      pass_ports.pop_back ();

      return used;
    }

    // should be signal only
    void
    Blocks::track_bus (BlockInfo& start, track_mode mode)
    {
      const block_T* b = start.tag ();
      for (block_T::output_const_iterator oi (b->output ().begin ());
	   oi != b->output ().end (); oi++)
	{
	  std::string usename;
	  const ioport_T& out = *oi;

	  IOPort* out_iop = find_out_port (b->name (), out.port ());
	  assert (out_iop);

	  NewBus& out_bus = out_iop->businfo ();

	  if (out_bus.bus_size () == 0)
	    continue;		// no dimensions

	  assert (out_bus.get_type () == NewBus::Signal);

	  const ioport_T::usename_optional& uo = out.usename ();
	  if (uo && *uo)
	    usename = out.line ();

	  ioport_deq_T pass;
	  bool f = track_bus_outport (out_iop, -1, -1, pass, out_iop, NULL,
				      usename, mode);

	  // no propagate variable, but maybe define
	  if (mode == TRACK_BUS_PV && !f)
	    {
	      const std::string& varname = line_varname (usename, out_iop);
	      VarElement ve (varname, -1, -1, "output", b->name (),
			     out.line (), out.port ());
	      VarElement& sve = start.output_var ().add_var (ve);
	      sve.switched () = true;
	    }
	}
    }

    void
    Blocks::track_bus (track_mode mode)
    {
      for (block_map_iterator bi (block_map ().begin ());
	   bi != block_map ().end (); bi++)
	{
	  BlockInfo& binfo = bi->second;
	  const block_T* b = binfo.tag ();

	  if (binfo.type () == BlockInfo::TYPE_DEMUX
	      || binfo.type () == BlockInfo::TYPE_MUX
	      || binfo.type () == BlockInfo::TYPE_BUSCREATOR
	      || binfo.type () == BlockInfo::TYPE_BUSSELECTOR)
	    continue;

	  if (b->blocktype () == "Scope" || b->blocktype () == "Terminator"
	      || b->blocktype () == "Goto" || b->blocktype () == "From")
	    continue;

	  if (b->blocktype () == "SubSystem" && b->blocks ().size () > 0)
	    continue;

	  if ((b->blocktype () == "Inport" || b->blocktype () == "Outport")
	      && binfo.upper () != NULL)
	    continue;

	  track_bus (binfo, mode);
	}
    }

    int
    Blocks::search_bus_input (BlockInfo& start)
    {
      if (start.bus_visited ())
	return 0;

      int ind = 0;
      const block_T* b = start.tag ();

      int err = 0;
      for (block_T::input_const_iterator ii (b->input ().begin ());
	   ii != b->input ().end (); ii++)
	{
	  std::string usename;
	  if (search_bus_input (start, ind++, *ii, usename))
	    err = -1;
	}

      start.bus_visited () = true;
      if (err == 0)
	start.bus_search_result () = true;

      return err;
    }

    // この機能は誤った仕様に基づくため、誤った結果となる。また、track
    // busが新しいバス追跡機能なので、この関数はもう利用すべきではない
    int
    Blocks::search_bus_input (BlockInfo& start,
			      int index, const ioport_T& in,
			      std::string& usename)
    {
      // no connection
      if (in.connect ().size () == 0)
	return -1;

      assert (in.connect ().size () == 1);

      ioport_T::connect_const_iterator ci (in.connect ().begin ());
      IOPort* cop = find_out_port (ci->block (), ci->port ());
      if (cop == NULL)
	{
	  std::cout << "Warning: no connection to "
		    << ci->block() << "::"
		    << ci->port() << std::endl;
	  return -1;
	}

      const block_T* cb = cop->block_tag ();
      BlockInfo* cbi = cop->blockinfo ();
      const ioport_T* out = cop->ioport_tag ();

      const ioport_T::usename_optional uo = out->usename ();
      if (uo && *uo == true)
	{
	  if (usename.length () && usename != out->line ())
	    std::cout << "Warning!" << __LINE__
		      << ": name changed by usename: "
		      << usename << " to " << out->line ()
		      << std::endl;
	  usename = out->line ();
	}

      // Subsystemの場合
      if (cb->blocks ().size ())
	{
	  for (block_T::blocks_const_iterator bsi (cb->blocks ().begin ());
	       bsi != cb->blocks ().end (); bsi++)
	    {
	      for (blocks_T::block_const_iterator
		     bi (bsi->block ().begin ());
		   bi != bsi->block ().end (); bi++)
		{
		  if (bi->blocktype () != "Outport"
		      || bi->name () != out->port ())
		    continue;

		  if (bi->input ().size () == 0)
		    {
		      std::cerr << "Error!" << __LINE__
				<< ": " << bi->blocktype ()
				<< ":" << bi->name ()
				<< ": no input" << std::endl;
		      return -1;
		    }

		  // <input>はあれば1である
		  assert (bi->input ().size () == 1);


		  block_T::input_const_iterator ii (bi->input ().begin ());
		  if (search_bus_input (start, index, *ii, usename))
		    return -1;
		  return 0;
		}
	    }

	  std::cerr << "Error!" << __LINE__
		    << ": Outport: " << out->port ()
		    << ": not found" << std::endl;
	  return -1;
	}

      assert (cb->blocktype () != "Goto");

      if (cbi->type () == BlockInfo::TYPE_MUX
	  || cbi->type () == BlockInfo::TYPE_BUSCREATOR)
	{
	  if (search_bus_input (*cbi))
	    return -1;

	  const oldBusPort& mux_busport = cbi->busport ();

	  if (start.type() == BlockInfo::TYPE_MUX
	      || start.type () == BlockInfo::TYPE_BUSCREATOR)
	    {
	      oldBusLine& busline = start.busport ()[index];

	      for (oldBusPort::const_iterator bci (mux_busport.begin ());
		   bci != mux_busport.end (); bci++)
		{
		  for (oldBusLine::const_iterator bli (bci->begin ());
		       bli != bci->end (); bli++)
		    busline.add (*bli).usename () = usename;
		}
	    }
	  else
	    {
	      assert (start.type () == BlockInfo::TYPE_DEMUX
		      || start.type () == BlockInfo::TYPE_BUSSELECTOR);

	      size_t mux_in_bus = mux_busport.bus_count ();
	      size_t mux_in_ports = cb->input ().size ();

	      size_t out_size = start.tag()->output ().size ();
	      oldBusPort& busport = start.busport ();
	      oldBusPort::iterator pi (busport.begin ());

	      if (out_size == mux_in_ports)
		{
		  // ポート数が一致する場合、Muxしたポートをそのまま
		  // Demuxのポートにする
		  for (oldBusPort::const_iterator mi (mux_busport.begin ());
		       mi != mux_busport.end (); mi++)
		    {
		      for (oldBusLine::const_iterator mbi (mi->begin ());
			   mbi != mi->end (); mbi++)
			{
			  // pi->add (*mbi).usename () = usename; -- XXX
			  pi->add (*mbi);
			}
		      pi++;
		    }
		}
	      else if (out_size == mux_in_bus)
		{
		  // ポート数が不一致だが、バスの数が一致している場合は
		  // Muxの入力バスを全てDemuxのポートに分配する
		  for (oldBusPort::const_iterator mi (mux_busport.begin ());
		       mi != mux_busport.end (); mi++)
		    {
		      for (oldBusLine::const_iterator mbi (mi->begin ());
			   mbi != mi->end (); mbi++)
			{
			  // pi->add (*mbi).usename () = usename; -- XXX
			  pi->add (*mbi);
			  pi++;
			}
		    }
		}
	      else if (out_size < mux_in_bus)
		{
		  // Busを分配する(p < n のルール)
		  size_t bus = 0;
		  size_t x = mux_in_bus / out_size;
		  size_t m = mux_in_bus % out_size;
		  if (m)
		    x++;
		  for (oldBusPort::const_iterator mi (mux_busport.begin ());
		       mi != mux_busport.end (); mi++)
		    {
		      for (oldBusLine::const_iterator mbi (mi->begin ());
			   mbi != mi->end (); mbi++)
			{
			  // pi->add (*mbi).usename () = usename; -- XXX
			  pi->add (*mbi);
			  if (pi->size () >= x)
			    {
			      pi++;
			      if (++bus == m)
				x--;
			    }
			}
		    }
		}
	      else
		{
		  // TODO: 複数のバスのベクタの分割
		  if (mux_in_bus != 1)
		    {
		      std::cerr << "Error!" << __LINE__
				<< ": Demux: "
				<< start.tag()->name ()
				<< ": multiple vector input "
				<< "not support yet"
				<< std::endl;
		    }

		  const oldBusEnt& bus = *mux_busport.begin ()->begin ();
		  int bind = bus.index () * out_size;
		  int bttl = bus.total () * out_size;
		  for (; pi != busport.end (); pi++)
		    pi->add (bus.port (),
			     bind++, 1, bttl).usename () = usename;
		}
	    }
	}
      else if (cbi->type () == BlockInfo::TYPE_DEMUX
	       || cbi->type () == BlockInfo::TYPE_BUSSELECTOR)
	{
	  const int demux_index = cop->ioport_index ();
	  const oldBusLine& demux_busline = cbi->busport()[demux_index];

	  if (start.type() == BlockInfo::TYPE_MUX
	      || start.type () == BlockInfo::TYPE_BUSCREATOR)
	    {
	      oldBusLine& busline = start.busport ()[index];
	      for (oldBusLine::const_iterator di (demux_busline.begin ());
		   di != demux_busline.end (); di++)
		busline.add (*di).usename () = usename;
	    }
	  else
	    {
	      assert (start.type () == BlockInfo::TYPE_DEMUX
		      || start.type () == BlockInfo::TYPE_BUSSELECTOR);

	      const size_t demux_out_bus = demux_busline.size ();
	      const size_t out_size = start.tag ()->output ().size ();

	      oldBusPort& busport = start.busport ();
	      oldBusPort::iterator pi (busport.begin ());

	      if (out_size == demux_out_bus)
		{
		  for (oldBusLine::const_iterator di (demux_busline.begin ());
		       di != demux_busline.end (); di++)
		    {
		      // pi->add (*di).usename () = usename; -- XXX
		      pi->add (*di);
		      pi++;
		    }
		}
	      else if (out_size < demux_out_bus)
		{
		  // Busを分配する(p < n のルール)
		  size_t bus = 0;
		  size_t x = demux_out_bus / out_size;
		  size_t m = demux_out_bus % out_size;
		  if (m)
		    x++;
		  for (oldBusLine::const_iterator dbi (demux_busline.begin ());
		       dbi != demux_busline.end (); dbi++)
		    {
		      // pi->add (*dbi).usename () = usename; -- XXX
		      pi->add (*dbi);
		      if (pi->size () >= x)
			{
			  pi++;
			  if (++bus == m)
			    x--;
			}
		    }
		}
	      else
		{
		  // TODO: 複数のバスのベクタの分割
		  if (demux_out_bus != 1)
		    {
		      std::cerr << "Error!" << __LINE__
				<< ": Demux: "
				<< start.tag()->name ()
				<< ": multiple vector input "
				<< "not support yet"
				<< std::endl;
		    }

		  const oldBusEnt& bus = *demux_busline.begin ();
		  int bind = bus.index () * out_size;
		  int bttl = bus.total () * out_size;
		  for (; pi != busport.end (); pi++)
		    pi->add (bus.port (),
			     bind++, 1, bttl).usename () = usename;
		}
	    }
	}
      else if (cb->blocktype () == "Inport")
	{
	  const BlockInfo* ubi = cbi->upper ();
	  if (ubi == NULL)
	    {
	      oldBusLine& busline = start.busport ()[index];
	      busline.add (cop).usename () = usename;
	      return 0;
	    }

	  const block_T* ub = ubi->tag ();
	  for (block_T::input_const_iterator ii (ub->input ().begin ());
	       ii != ub->input ().end (); ii++)
	    {
	      if (ii->port () != cb->name ())
		continue;

	      return search_bus_input (start, index, *ii, usename);
	    }

	  std::cerr << "Error!" << __LINE__
		    << ": " << cb->name ()
		    << ": input not found"
		    << std::endl;
	  return -1;		// not found
	}
      else if (cb->blocktype () == "From")
	{
	  const From* f = find_from (cb->name ());
	  const Goto* g = f->from_goto ();
	  if (g)
	    {
	      const block_T* gb = g->goto_tag ();

	      assert (gb->input ().size () <= 1);

	      block_T::input_const_iterator ii (gb->input ().begin ());
	      if (ii != gb->input ().end ())
		return search_bus_input (start, index, *ii, usename);
	      else
		std::cerr << "Error!" << __LINE__
			  << ": [Goto]" << cb->name ()
			  << ": no input" << std::endl;
	    }
	  else
	    std::cerr << "Error!" << __LINE__
		      << ": [From]" << cb->name  ()
		      << ": not found [Goto]" << std::endl;
	  return -1;
	}
#if HANDLE_RATETRANSISION
      else if (cb->blocktype () == "RateTransition")
	{
	  block_T::input_const_iterator ii (cb->input ().begin ());
	  if (ii != cb->input ().end ())
	    return search_bus_input (start, index, *ii, usename);
	}
#endif	// HANDLE_RATETRANSISION

      if (start.type() == BlockInfo::TYPE_MUX
	  || start.type () == BlockInfo::TYPE_BUSCREATOR)
	{
	  oldBusLine& busline = start.busport ()[index];
	  busline.add (cop).usename () = usename;
	}
      else
	{
	  assert (start.type () == BlockInfo::TYPE_DEMUX
		  || start.type () == BlockInfo::TYPE_BUSSELECTOR);

	  int bindex = 0;
	  int btotal = start.tag ()->output ().size ();
	  for (oldBusPort::iterator pi (start.busport ().begin ());
	       pi != start.busport ().end (); pi++)
	    pi->add (cop, bindex++, 1, btotal).usename () = usename;
	}

      return 0;
    }

    void
    Blocks::process_bus (void)
    {
      for (block_map_iterator bi (block_map ().begin ());
	   bi != block_map ().end (); bi++)
	{
	  BlockInfo& binfo = bi->second;
	  if ((binfo.type () == BlockInfo::TYPE_MUX
	       || binfo.type () == BlockInfo::TYPE_DEMUX
	       || binfo.type () == BlockInfo::TYPE_BUSCREATOR
	       || binfo.type () == BlockInfo::TYPE_BUSSELECTOR)
	      && !binfo.bus_visited ())
	    (void)search_bus_input (binfo);
	}
    }

    void
    Blocks::process_all (void)
    {
      process_blocks ();
      process_one_subsystem ();
      process_port ();
      process_goto_from ();

      if (has_dimensions ())
	track_bus (TRACK_BUS_PV);
      else
	{
	  std::cout << "Warning: No `dimensions' in <output>: obsoleted BLXML"
		    << std::endl;
	  process_bus ();
	  process_variables ();
	}
    }


    void
    Blocks::process_blocks (void)
    {
      process_blocks (NULL, *b_xml);
    }


    void
    Blocks::process_blocks (BlockInfo* upper,
			    blocks_T& blks)
    {
      int type = 0;
      int act = 0;
      int iter = 0;

      for (blocks_T::block_iterator bs (blks.block().begin());
	   bs != blks.block().end(); bs++)
	{
	  if (find_block (bs->name ()))
	    {
	      std::cout << "Warning: Duplicate block in XML: "
			<< bs->blocktype () << ":" << bs->name ()
			<< std::endl;
	      continue;
	    }

	  if (bs->blocktype() == "EnablePort")
	    type |= 1;
	  else if (bs->blocktype () == "TriggerPort")
	    type |= 2;
	  if (bs->blocktype() == "ActionPort")
	    act = 1;
	  if (bs->blocktype () == "ForIterator"
	      || bs->blocktype () == "WhileIterator")
	    iter = 1;

	  BlockInfo& x = add_block_map (bs->name(), BlockInfo (upper, &*bs));

	  if (bs->blocktype () == "Mux")
	    {
	      x.busport ().resize (bs->input ().size ());
	      x.type () = BlockInfo::TYPE_MUX;
	    }
	  else if (bs->blocktype () == "Demux")
	    {
	      x.busport ().resize (bs->output ().size ());
	      x.type () = BlockInfo::TYPE_DEMUX;
	    }
	  else if (bs->blocktype () == "BusCreator")
	    {
	      x.busport ().resize (bs->input ().size ());
	      x.type () = BlockInfo::TYPE_BUSCREATOR;
	    }
	  else if (bs->blocktype () == "BusSelector")
	    {
	      x.busport ().resize (bs->output ().size ());
	      x.type () = BlockInfo::TYPE_BUSSELECTOR;
	    }
	  else if (upper == NULL
		   && (bs->blocktype () == "Inport"
		       || bs->blocktype () == "Outport"))
	    x.type () = BlockInfo::TYPE_EXTIOPORT;

	  if (bs->blocktype () == "SubSystem")
	    {
	      // Atomic Subsystem: low priority
	      const block_T::TreatAsAtomicUnit_optional&
		ao = bs->TreatAsAtomicUnit();
	      if (ao && *ao == "on")
		x.type () = BlockInfo::TYPE_ATOMIC_SUBSYSTEM;
	    }

	  for (block_T::blocks_iterator i (bs->blocks().begin());
	       i != bs->blocks().end(); i++)
	    process_blocks (&x, *i);

	  if (bs->blocktype () == "SubSystem")
	    {
	      // Stateflow: high priority
	      const block_T::stateflow_optional& so = bs->stateflow ();
	      if (so && (*so == "chart" || *so == "truthtable"))
		x.type () = BlockInfo::TYPE_STATEFLOW_SUBSYSTEM;
	    }
	}

      // middle priority
      if (type == 1)
	upper->type () = BlockInfo::TYPE_ENABLED_SUBSYSTEM;
      else if (type == 2)
	upper->type () = BlockInfo::TYPE_TRIGGERED_SUBSYSTEM;
      else if (type == 3)
	upper->type () = BlockInfo::TYPE_ENABLED_TRIGGERED_SUBSYSTEM;
      else if (act)
	upper->type () = BlockInfo::TYPE_ACTION_SUBSYSTEM;
      else if (iter)
	upper->type () = BlockInfo::TYPE_ITERATOR_SUBSYSTEM;
    }


    void
    Blocks::process_one_subsystem (void)
    {
      process_one_subsystem (NULL, *b_xml);
    }


    void
    Blocks::process_one_subsystem (BlockInfo* top, blocks_T& blks)
    {
      for (blocks_T::block_iterator bs (blks.block().begin());
	   bs != blks.block().end(); bs++)
	{
	  BlockInfo* binfo = find_block (bs->name ());
	  BlockInfo* t = top;

	  assert (binfo);

	  if (t == NULL
	      && (binfo->type () == BlockInfo::TYPE_ITERATOR_SUBSYSTEM
		  || binfo->type () == BlockInfo::TYPE_STATEFLOW_SUBSYSTEM))
	    t = binfo;

	  if (t)
	    {
	      binfo->top_block () = t;
#if 0
	      const BlockInfo* u = binfo;
	      std::vector < const BlockInfo* > l;
	      while (u)
		{
		  l.push_back (u);
		  u = u->upper ();
		}
	      for (std::vector < const BlockInfo* >::const_reverse_iterator
		     r (l.rbegin ()); r != l.rend (); r++)
		{
		  const BlockInfo* xb = *r;
		  if (r != l.rbegin ())
		    std::cout << "->";
		  std::cout << xb->tag()->name();
		}
	      std::cout << ": " << t->tag()->name() << std::endl;
#endif
	    }

	  for (block_T::blocks_iterator i (bs->blocks().begin());
	       i != bs->blocks().end(); i++)
	    process_one_subsystem (t, *i);
	}
    }


    void
    Blocks::xml (blocks_T_PTR& x)
    {
      b_xml.reset (x.release ());
      block_map().clear();
      process_all ();
    }

    const std::string*
    Blocks::xml_systemsymbol (const std::string& s) const
    {
      blockname_map_const_iterator bi (blockname_entry().find (s));
      if (bi == blockname_entry().end())
	return NULL;
      return &bi->second.symbol ();
    }

    std::string
    Blocks::xml_blockname (const std::string& s, const std::string& b)
      const
    {
      std::string r;

      const std::string* symbol = xml_systemsymbol (s);
      if (symbol != NULL)
	{
	  std::string t (b);
	  xml_canonicalize (t);
	  r = *symbol + "_" + t;
	}
      return r;
    }


    const BlockNameEnt*
    Blocks::find_blockname_entry (const std::string& s) const
    {
      blockname_map_const_iterator bi (blockname_entry().find (s));
      if (bi == blockname_entry().end())
	return NULL;
      return &bi->second;
    }

    BlockNameEnt*
    Blocks::find_blockname_entry (const std::string& s)
    {
      blockname_map_iterator bi (blockname_entry().find (s));
      if (bi == blockname_entry().end())
	return NULL;
      return &bi->second;
    }

    BlockInfo*
    Blocks::find_block (const std::string& n)
    {
      block_map_iterator i = block_map().find (n);
      if (i == block_map().end ())
	return NULL;
      return &i->second;
    }


    const BlockInfo*
    Blocks::find_block (const std::string& n) const
    {
      block_map_const_iterator i = block_map().find (n);
      if (i == block_map().end ())
	return NULL;
      return &i->second;
    }


    BlockInfo*
    Blocks::find_block (const std::string& s, const std::string& b)
    {
      std::string blk = xml_blockname (s, b);
      BlockInfo* binfo = find_block (blk);
      if (binfo)
	{
	  if (binfo->system() == "" && binfo->block () == "")
	    {
	      binfo->system () = s;
	      binfo->block () = b;
	      binfo->defined () = true;
	    }
	}
      else
	{
	  const BlockNameEnt* bne = find_blockname_entry (s);

	  // not found in the system hierarchy at header file.
	  assert (bne);

	  const BlockNameEnt* top = bne->find_top ();
	  if (top == NULL)
	    return NULL;

	  binfo = top->blockinfo ();

#if PARSE_DATA_DEBUG > 0
	  std::cout << "* " << s << '/' << b;
	  if (bne->reference ())
	    std::cout << " => Reference: ";
	  else
	    std::cout << " => SubSystem: ";
	  std::cout << top->blockname ()
		    << '(' << top->symbol () << ')'
		    << std::endl;
#endif
	}

      return binfo;
    }

    const BlockInfo*
    Blocks::find_block (const std::string& s, const std::string& b) const
    {
      std::string blk = xml_blockname (s, b);
      const BlockInfo* binfo = find_block (blk);
      if (binfo == NULL)
	{
	  const BlockNameEnt* bne = find_blockname_entry (s);

	  // not found in the system hierarchy at header file.
	  assert (bne);

	  const BlockNameEnt* top = bne->find_top ();
	  if (top == NULL)
	    return NULL;

	  binfo = top->blockinfo ();

#if PARSE_DATA_DEBUG > 0
	  std::cout << "* " << s << '/' << b;
	  if (bne->reference ())
	    std::cout << " => Reference: ";
	  else
	    std::cout << " => SubSystem: ";
	  std::cout << top->blockname ()
		    << '(' << top->symbol () << ')'
		    << std::endl;
#endif
	}

      return binfo;
    }


    IOPort*
    Blocks::find_in_port (const std::string& b, const std::string& p)
    {
      ioport_map_iterator_pair pi = in_port_map().equal_range (p);
      for (ioport_map_iterator i = pi.first; i != pi.second; i++)
	{
	  if (i->second.match (b, p))
	    return &i->second;
	}
      return NULL;
    }


    const IOPort*
    Blocks::find_in_port (const std::string& b, const std::string& p) const
    {
      ioport_map_const_iterator_pair pi = in_port_map().equal_range (p);
      for (ioport_map_const_iterator i = pi.first; i != pi.second; i++)
	{
	  if (i->second.match (b, p))
	    return &i->second;
	}
      return NULL;
    }


    IOPort*
    Blocks::find_out_port (const std::string& b, const std::string& p)
    {
      ioport_map_iterator_pair pi = out_port_map().equal_range (p);
      for (ioport_map_iterator i = pi.first; i != pi.second; i++)
	{
	  if (i->second.match (b, p))
	    return &i->second;
	}
      return NULL;
    }


    const IOPort*
    Blocks::find_out_port (const std::string& b, const std::string& p) const
    {
      ioport_map_const_iterator_pair pi = out_port_map().equal_range (p);
      for (ioport_map_const_iterator i = pi.first; i != pi.second; i++)
	{
	  if (i->second.match (b, p))
	    return &i->second;
	}
      return NULL;
    }


    Goto*
    Blocks::find_goto (const std::string& b)
    {
      goto_map_iterator i = goto_map ().find (b);
      if (i == goto_map ().end ())
	return NULL;
      return &i->second;
    }


    const Goto*
    Blocks::find_goto (const std::string& b) const
    {
      goto_map_const_iterator i = goto_map ().find (b);
      if (i == goto_map ().end ())
	return NULL;
      return &i->second;
    }


    From*
    Blocks::find_from (const std::string& b)
    {
      from_map_iterator i = from_map ().find (b);
      if (i == from_map ().end ())
	return NULL;
      return &i->second;
    }


    const From*
    Blocks::find_from (const std::string& b) const
    {
      from_map_const_iterator i = from_map ().find (b);
      if (i == from_map ().end ())
	return NULL;
      return &i->second;
    }


    BlockInfo*
    Blocks::previous_block (const ioport_T& in)
    {
      for (ioport_T::connect_const_iterator ci (in.connect ().begin ());
	   ci != in.connect ().end (); ci++)
	{
	  IOPort* iop = find_out_port (ci->block (), ci->port ());
	  if (iop == NULL)
	    return NULL;

	  const block_T *cb = iop->block_tag ();
	  const ioport_T* out = iop->ioport_tag ();
	  BlockInfo* cbi = iop->blockinfo ();
	  const BlockInfo* ubi = cbi->upper ();

	  if (cb->blocks ().size ())
	    {
	      for (block_T::blocks_const_iterator i (cb->blocks ().begin ());
		   i != cb->blocks ().end (); i++)
		{
		  for (blocks_T::block_const_iterator
			 bi (i->block ().begin ());
		       bi != i->block ().end (); bi++)
		    {
		      if (bi->blocktype () == "Outport"
			  && bi->name () == out->port ())
			{
			  for (block_T::input_const_iterator
				 ii (bi->input ().begin ());
			       ii != bi->input ().end (); ii++)
			    {
			      BlockInfo* p = previous_block (*ii);
			      if (p)
				return p;
			    }
			}
		    }
		}
	    }

	  if (cb->blocktype () == "Inport")
	    {
	      if (ubi == NULL)
		return cbi;

	      const block_T* ub = ubi->tag ();
	      for (block_T::input_const_iterator i (ub->input ().begin ());
		   i != ub->input ().end (); i++)
		{
		  if (i->port () == cb->name ())
		    {
		      BlockInfo* bi = previous_block (*i);
		      if (bi)
			return bi;
		    }
		}
	    }
	  else if (cbi->type () == BlockInfo::TYPE_DEMUX
		   || cbi->type () == BlockInfo::TYPE_MUX
		   || cbi->type () == BlockInfo::TYPE_BUSSELECTOR
		   || cbi->type () == BlockInfo::TYPE_BUSCREATOR)
	    {
	      const block_T* ub = ubi->tag ();
	      for (block_T::input_const_iterator i (ub->input ().begin ());
		   i != ub->input ().end (); i++)
		{
		  BlockInfo* bi = previous_block (*i);
		  if (bi)
		    return bi;
		}
	    }
	  else if (cb->blocktype () == "From")
	    {
	      const From* f = find_from (cb->name ());
	      const Goto* g = f->from_goto ();
	      if (g == NULL)
		continue;

	      const block_T* gb = g->goto_tag ();
	      for (block_T::input_const_iterator i (gb->input ().begin ());
		   i != gb->input ().end (); i++)
		{
		  BlockInfo* bi = previous_block (*i);
		  if (bi)
		    return bi;
		}
	    }
#if HANDLE_RATETRANSISION
	  else if (cb->blocktype () == "RateTransition")
	    {
	      block_T::input_const_iterator i (cb->input ().begin ());
	      if (i != cb->input ().end ())
		{
		  BlockInfo* bi = previous_block (*i);
		  if (bi)
		    return bi;
		}
	    }
#endif	// HANDLE_RATETRANSISION
	  else if (cb->blocktype () == "Merge")
	    {
	      // ignore
	      continue;
	    }
	  else
	    return cbi;
	}

      return NULL;
    }

    void
    Blocks::subblock_input_from (BlockInfo* head,
				 BlockInfo* sbinfo,
				 const std::string& port)
    {
      const block_T* btag = sbinfo->tag ();
      for (block_T::blocks_const_iterator bi (btag->blocks ().begin ());
	   bi != btag->blocks ().end (); bi++)
	{
	  for (blocks_T::block_const_iterator i (bi->block ().begin ());
	       i != bi->block ().end (); i++)
	    {
	      if (i->blocktype () == "Outport" && i->name () == port)
		{
		  for (block_T::input_const_iterator ii (i->input ().begin ());
		       ii != i->input ().end (); ii++)
		    input_from (head, *ii);
		  return;
		}
	    }
	}
    }


    void
    Blocks::input_from (BlockInfo* head, const ioport_T& in)
    {
      for (ioport_T::connect_const_iterator ci (in.connect ().begin ());
	   ci != in.connect ().end (); ci++)
	{
	  IOPort* iop = find_out_port (ci->block(), ci->port ());
	  if (iop == NULL)
	    {
	      std::cout << "Warning: no connection to "
			<< ci->block() << "::" << ci->port() << std::endl;
	      continue;		// no connection
	    }
	  const block_T *cb = iop->block_tag ();
	  const ioport_T* out = iop->ioport_tag ();
	  BlockInfo* cbi = iop->blockinfo ();
	  const BlockInfo* ubi = cbi->upper ();

	  // subblock
	  if (cb->blocks ().size ())
	    {
	      subblock_input_from (head, cbi, out->port ());
	      continue;
	    }

	  if (cb->blocktype () == "Inport")
	    {
	      // external input
	      if (ubi == NULL)
		{
#if 0
		  head->add_backward (cbi, BlockLink::TYPE_PORT);
		  cbi->add_forward (head, BlockLink::TYPE_PORT);
#else
		  cbi->make_link (head, BlockLink::TYPE_PORT);
#endif
		  continue;
		}

	      // change port
	      const block_T* ub = ubi->tag ();
	      for (block_T::input_const_iterator ii (ub->input ().begin ());
		   ii != ub->input ().end (); ii++)
		{
		  if (ii->port() == cb->name ())
		    input_from (head, *ii);
		}
	    }
	  else if (head->is_same_merged_head (cbi))
	    continue;
	  else if (cbi->type() == BlockInfo::TYPE_DEMUX
		   || cbi->type() == BlockInfo::TYPE_MUX
		   || cbi->type () == BlockInfo::TYPE_BUSSELECTOR
		   || cbi->type () == BlockInfo::TYPE_BUSCREATOR)
	    {
	      for (block_T::input_const_iterator ii (cb->input ().begin ());
		   ii != cb->input ().end (); ii++)
		{
		  input_from (head, *ii);
		}
	    }
	  else if (cb->blocktype () == "From")
	    {
	      const From* f = find_from (cb->name ());
	      const Goto* g = f->from_goto ();
	      if (g == NULL)
		continue;

	      const block_T* gb = g->goto_tag ();
	      for (block_T::input_const_iterator ii (gb->input ().begin ());
		   ii != gb->input ().end (); ii++)
		{
		  input_from (head, *ii);
		}
	    }
#if HANDLE_RATETRANSISION
	  else if (cb->blocktype () == "RateTransition")
	    {
	      block_T::input_const_iterator ii (cb->input ().begin ());
	      if (ii != cb->input ().end ())
		input_from (head, *ii);
	    }
#endif	// HANDLE_RATETRANSISION
	  else if (cb->blocktype () == "Merge")
	    {
	      // ignore
	      continue;
	    }
	  else
	    {
#if 0
	      head->add_backward (cbi, BlockLink::TYPE_DATA);
	      cbi->add_forward (head, BlockLink::TYPE_DATA);
#else
	      cbi->make_link (head, BlockLink::TYPE_DATA);
#endif
	    }
	}
    }


#ifdef ADD_INPUT_TO_ALL_SUBBLOCK
    void
    Blocks::subblock_add_flow (BlockInfo* head, BlockInfo* sbinfo)
    {
      const block_T* btag = sbinfo->tag ();
      for (block_T::blocks_const_iterator
	     xi (btag->blocks ().begin ());
	   xi != btag->blocks ().end (); xi++)
	{
	  for (blocks_T::block_const_iterator bi (xi->block ().begin ());
	       bi != xi->block ().end (); bi++)
	    {
	      if (bi->blocks ().size ())
		{
		  BlockInfo* ssbinfo = find_block (bi->name ());
		  assert (ssbinfo);
		  subblock_add_flow (head, ssbinfo);
		}
	      else if (bi->blocktype () != "Inport"
		       && bi->blocktype () != "Outport"
		       && bi->blocktype () != "TriggerPort"
		       && bi->blocktype () != "EnablePort"
		       && bi->blocktype () != "ActionPort"
		       && bi->blocktype () != "ForIterator"
		       && bi->blocktype () != "WhileIterator"
		       && bi->blocktype () != "From"
		       && bi->blocktype () != "Goto")
		{
		  BlockInfo* cbi = find_block (bi->name());
		  assert (cbi);
#if 0
		  head->add_forward (cbi, BlockLink::TYPE_DATA);
		  cbi->add_backward (head, BlockLink::TYPE_DATA);
#else
		  head->make_link (cbi, BlockLink::TYPE_DATA);
#endif
		}
	    }
	}
    }
#endif	// ADD_INPUT_TO_ALL_SUBBLOCK

    void
    Blocks::subblock_output_to (BlockInfo* head,
				BlockInfo* sbinfo,
				const std::string& port)
    {
      const block_T* btag = sbinfo->tag ();
      for (block_T::blocks_const_iterator sb (btag->blocks ().begin ());
	   sb != btag->blocks ().end (); sb++)
	{
	  for (blocks_T::block_const_iterator bi (sb->block ().begin ());
	       bi != sb->block ().end (); bi++)
	    {
	      if (bi->blocktype () == "Inport" && bi->name () == port)
		{
		  for (block_T::output_const_iterator
			 oo (bi->output ().begin ());
		       oo != bi->output ().end (); oo++)
		    output_to (head, *oo);
		  return;
		}
	      else if ((bi->blocktype () == "TriggerPort"
			|| bi->blocktype () == "EnablePort"
			|| bi->blocktype () == "ActionPort")
		       && bi->name () == port)
		{
#ifdef ADD_INPUT_TO_ALL_SUBBLOCK
		  subblock_add_flow (head, sbinfo);
#else
		  // link to Triggered/Enabled SubSystem
#if 0
		  head->add_forward (sbinfo, BlockLink::TYPE_DATA);
		  sbinfo->add_backward (head, BlockLink::TYPE_DATA);
#else
		  head->make_link (sbinfo, BlockLink::TYPE_DATA);
#endif
#endif
		}
	    }
	}
    }


    void
    Blocks::output_to (BlockInfo* head, const ioport_T& out)
    {
      for (ioport_T::connect_const_iterator ci (out.connect ().begin ());
	   ci != out.connect ().end (); ci++)
	{
	  IOPort* iop = find_in_port (ci->block(), ci->port ());
	  if (iop == NULL)
	    {
	      std::cout << "Warning: no connection from "
			<< ci->block() << "::" << ci->port() << std::endl;
	      continue;		// no connection
	    }
	  const block_T *cb = iop->block_tag ();
	  const ioport_T* in = iop->ioport_tag ();
	  BlockInfo* cbi = iop->blockinfo ();
	  const BlockInfo* ubi = cbi->upper ();

	  // subblock
	  if (cb->blocks ().size ())
	    {
	      subblock_output_to (head, cbi, in->port ());
	      continue;
	    }

	  if (cb->blocktype () == "Outport")
	    {
	      // external output
	      if (ubi == NULL)
		{
#if 0
		  head->add_forward (cbi, BlockLink::TYPE_PORT);
		  cbi->add_backward (head, BlockLink::TYPE_PORT);
#else
		  head->make_link (cbi, BlockLink::TYPE_PORT);
#endif
		  continue;
		}

	      // change port
	      const block_T* ub = ubi->tag ();
	      for (block_T::output_const_iterator oo (ub->output ().begin ());
		   oo != ub->output ().end (); oo++)
		{
		  if (oo->port() == cb->name ())
		    output_to (head, *oo);
		}
	    }
	  else if (head->is_same_merged_head (cbi))
	    continue;
	  else if (cbi->type() == BlockInfo::TYPE_DEMUX
		   || cbi->type() == BlockInfo::TYPE_MUX
		   || cbi->type () == BlockInfo::TYPE_BUSSELECTOR
		   || cbi->type () == BlockInfo::TYPE_BUSCREATOR)
	    {
	      for (block_T::output_const_iterator oo (cb->output ().begin ());
		   oo != cb->output ().end (); oo++)
		{
		  output_to (head, *oo);
		}
	    }
	  else if (cb->blocktype () == "Goto")
	    {
	      Goto* g = find_goto (cb->name ());
	      for (from_list_const_iterator fi (g->from_list ().begin ());
		   fi != g->from_list ().end (); fi++)
		{
		  const From* f = *fi;
		  const block_T* fb = f->from_tag ();
		  for (block_T::output_const_iterator
			 oo (fb->output ().begin ());
		       oo != fb->output ().end (); oo++)
		    {
		      output_to (head, *oo);
		    }
		}
	    }
#if HANDLE_RATETRANSISION
	  else if (cb->blocktype () == "RateTransition")
	    {
	      block_T::output_const_iterator oo (cb->output ().begin ());
	      if (oo != cb->output ().end ())
		output_to (head, *oo);
	    }
#endif	// HANDLE_RATETRANSISION
	  else if (cb->blocktype () == "Scope"
		   || cb->blocktype () == "Terminator")
	    {
	      // ignore
	      continue;
	    }
	  else
	    {
#if 0
	      head->add_forward (cbi, BlockLink::TYPE_DATA);
	      cbi->add_backward (head, BlockLink::TYPE_DATA);
#else
	      head->make_link (cbi, BlockLink::TYPE_DATA);
#endif
	    }
	}
    }


    void
    Blocks::trace_flow_forward (BlockInfo& tail, BlockInfo& head,
				pvar_ptr_list& pvpl)
    {
      if (tail.merge_blocks ().size ())
	{
	  for (blockinfo_list_iterator hbli (tail.merge_blocks ().begin ());
	       hbli != tail.merge_blocks ().end (); hbli++)
	    {
	      BlockInfo* mhb = *hbli;
	      trace_flow_forward (*mhb, head, pvpl);
	    }
	}
      else if (head.merge_blocks ().size ())
	{
	  for (blockinfo_list_iterator tbli (head.merge_blocks ().begin ());
	       tbli != head.merge_blocks ().end (); tbli++)
	    {
	      BlockInfo* mtb = *tbli;
	      trace_flow_forward (tail, *mtb, pvpl);
	    }
	}

      if (tail.output_var ().size ())
	{
	  for (propagate_var_list_iterator
		 vo (tail.output_var().begin());
	       vo != tail.output_var().end(); vo++)
	    {
	      VarElement& ve = *vo;
	      propagate_var_list_iterator
		vi = std::find (head.input_var().begin(),
				head.input_var().end(), ve);
	      if (vi != head.input_var().end())
		{
		  var_list& vli = vi->varent ();
		  var_list& vlo = vo->varent ();
		  for (var_list_iterator vlie (vli.begin ());
		       vlie != vli.end (); vlie++)
		    {
		      VariableEnt* ive = *vlie;
		      var_list_iterator vloe = std::find (vlo.begin (),
							  vlo.end (), ive);
		      if (vloe == vlo.end ())
			continue;

		      // tail output, head input
		      vloe->used (true);
		      // vlie->used (true);
		    }

		  if (pvpl.vfind (ve) == pvpl.end ())
		    pvpl.push_back (&*vi);
		}
	    }
	}
    }

    void
    Blocks::trace_flow_backward (BlockInfo& head, BlockInfo& tail,
				 pvar_ptr_list& pvpl)
    {
      if (tail.merge_blocks ().size ())
	{
	  for (blockinfo_list_iterator tbli (tail.merge_blocks ().begin ());
	       tbli != tail.merge_blocks ().end (); tbli++)
	    {
	      BlockInfo* mtb = *tbli;
	      trace_flow_backward (head, *mtb, pvpl);
	    }
	}
      else if (head.merge_blocks ().size ())
	{
	  for (blockinfo_list_iterator hbli (head.merge_blocks ().begin ());
	       hbli != head.merge_blocks ().end (); hbli++)
	    {
	      BlockInfo* mhb = *hbli;
	      trace_flow_backward (*mhb, tail, pvpl);
	    }
	}

      if (head.input_var ().size ())
	{
	  for (propagate_var_list_iterator
		 vi (head.input_var ().begin());
	       vi != head.input_var ().end(); vi++)
	    {
	      VarElement& ve = *vi;
	      propagate_var_list_iterator
		vo = std::find (tail.output_var().begin(),
				tail.output_var().end(), ve);
	      if (vo != tail.output_var().end())
		{
		  var_list& vli = vi->varent ();
		  var_list& vlo = vo->varent ();
		  for (var_list_iterator vlie (vli.begin ());
		       vlie != vli.end (); vlie++)
		    {
		      VariableEnt* ive = *vlie;
		      var_list_iterator vloe = std::find (vlo.begin (),
							  vlo.end (), ive);
		      if (vloe == vlo.end ())
			continue;

		      // head input, tail output
		      vlie->used (true);
		      // vloe->used (true);
		    }
		  if (pvpl.vfind (ve) == pvpl.end ())
		    pvpl.push_back (&*vo);
		}
	    }
	}
    }

    void
    Blocks::trace_flow (void)
    {
      for (block_map_iterator bi (block_map().begin());
	   bi != block_map().end (); bi++)
	{
	  BlockInfo& binfo = bi->second;
	  block_T* btag = binfo.tag();

	  if (!binfo.defined ()
	      && binfo.type () != BlockInfo::TYPE_EXTIOPORT)
	    continue;

	  if ((btag->blocktype () == "Inport"
	       || btag->blocktype () == "Outport")
	      && binfo.upper () != NULL)
	    continue;

	  if (binfo.type() == BlockInfo::TYPE_DEMUX
	      || binfo.type() == BlockInfo::TYPE_MUX
	      || binfo.type () == BlockInfo::TYPE_BUSSELECTOR
	      || binfo.type () == BlockInfo::TYPE_BUSCREATOR)
	    continue;

	  if (btag->blocktype () == "From"
	      || btag->blocktype () == "Goto"
#if HANDLE_RATETRANSISION
	      || btag->blocktype () == "RateTransition"
#endif	// HANDLE_RATETRANSISION
	      || btag->blocktype () == "Terminator"
	      || btag->blocktype () == "Scope")
	    continue;

	  // subblock top
	  // TODO: Triggered/Enabled SubSystem
	  if (btag->blocks ().size ())
	    continue;

	  // no check all inputs of "Merge"
	  if (btag->blocktype () != "Inport"
	      && btag->blocktype () != "Merge")
	    {
	      // input from leaf
	      for (block_T::input_const_iterator ii (btag->input ().begin ());
		   ii != btag->input ().end (); ii++)
		input_from (&binfo, *ii);
	    }

	  // output to leaf
	  if (btag->blocktype () != "Outport")
	    {
	      for (block_T::output_const_iterator oo (btag->output ().begin ());
		   oo != btag->output ().end (); oo++)
		output_to (&binfo, *oo);
	    }
	}

      // TODO
      // forward/backwardに変数が入らないものがある
      for (block_map_iterator bi (block_map().begin());
	   bi != block_map().end (); bi++)
	{
	  BlockInfo& binfo = bi->second;

	  for (block_link_iterator bl (binfo.forward().begin());
	       bl != binfo.forward().end(); bl++)
	    {
	      BlockInfo* fb = bl->blockinfo();

	      trace_flow_forward (binfo, *fb, bl->vars ());
	    }

	  for (block_link_iterator bl (binfo.backward().begin());
	       bl != binfo.backward().end(); bl++)
	    {
	      BlockInfo* bb = bl->blockinfo();

	      trace_flow_backward (binfo, *bb, bl->vars ());
	    }
	}

      change_externals ();
    }

    void
    Blocks::xml_complete (void)
    {
      for (block_map_iterator i (block_map().begin());
	   i != block_map().end (); i++)
	{
	  i->second.complete(this);
	}

      blocks_T& bs = *xml();

      // program specific types
      for (sysdata_list_iterator s (sysdata_list ().begin ());
	   s != sysdata_list ().end (); s++)
	{
	  SysData& sd = *s;

	  std::string tmpnam;
	  std::string* nam;

	  BlockInfo* binfo = find_block (sd.system (), sd.block ());
	  if (binfo)
	    {
	      sd.blockinfo () = binfo;
	      nam = &binfo->tag ()-> name ();
	    }
	  else
	    {
	      tmpnam = xml_blockname (sd.system (), sd.block ());
	      nam = &tmpnam;
	    }

	  bs.deftype ().push_back (object_T (sd.code (), sd.vtype (), *nam));
	  object_T& d = bs.deftype ().back ();

	  if (sd.has_struct ())
	    d.has_struct (true);

	  switch (sd.stype ())
	    {
	    case SysData::SD_BSIG:
	      d.kind ("bsignal");
	      break;
	    case SysData::SD_BSTATE:
	      d.kind ("bstate");
	      break;
	    case SysData::SD_PARAM:
	      d.kind ("param");
	      break;
	    }
	}

      if (common_code ().length ())
	bs.code ().push_back (code_T (common_code (), "common"));
      if (common_init_code ().length ())
	bs.code ().push_back (code_T (common_init_code (), "common-init"));

      if (code_macros ().size ())
	{
	  int index = 0;
	  for (std::vector < file_code_T >::const_iterator
		 i (code_macros ().begin ());
	       i != code_macros ().end (); i++)
	    {
	      code_T c (i->first, "c_macro");
	      c.index () = index++;
	      c.line () = i->second;
	      bs.code ().push_back (c);
	    }
	}

      // Real-Time Model Object


#if 0
      if (rtm_extern_has_struct())
	{
	  std::string d ("struct " + rtm_extern_type ());
	  bs.deftype().push_back (object_T (d, rtm_object_type ()));
	}
#endif

      // from Header
      if (header_preamble ().length ())
	bs.code().push_back (code_T (header_preamble (), "preamble"));
      if (rtm_macro ().length ())
	bs.code().push_back (code_T (rtm_macro(), "rt-macro"));
      if (rtm_struct ().length ())
	bs.code().push_back (code_T (rtm_struct(), "rt-type"));

      std::string t;

      if (rtm_extern_name ().length ())
	{
	  t = "extern";
	  if (rtm_extern_has_struct())
	    t += " struct";
	  t += " " + rtm_extern_type() + " *";
	  if (rtm_extern_has_const())
	    t += " const";

	  bs.object().push_back (object_T (t, rtm_extern_name()));
	  object_T& o = bs.object().back();
	  o.kind ("rtm-extern");
	}

      if (rtm_object_type ().length () && rtm_object_name ().length ())
	{
	  // from C
	  bs.object().push_back (object_T (rtm_object_type(),
					   rtm_object_name()));
	  t = rtm_object_type () + " *";
	  if (rtm_object_has_const ())
	    t += " const";
	  object_T& o = bs.object().back();
	  o.kind ("rtm-object");
	}

      if (rtm_object_pointer ().length ())
	{
	  bs.object().push_back (object_T (t, rtm_object_pointer()));
	  object_T& o = bs.object().back();


	  std::string v ("&" + rtm_object_name ());
	  o.value(v);
	  o.kind("rtm-pointer");
	}

      if (rtm_code().length ())
	bs.code().push_back (code_T (rtm_code(), "rt-code"));
      if (rtm_init_code ().length ())
	bs.code().push_back (code_T (rtm_init_code(), "rt-init"));

      for (function_map_const_iterator fi (function_decl ().begin ());
	   fi != function_decl ().end (); fi++)
	{
	  const function_code_T& f = fi->second;

	  bs.function ().push_back (function_T (fi->first, "decl"));

	  function_T& xf = bs.function ().back ();

	  code_T xc (f, "func");
	  xc.line (f.line ());
	  xf.code (xc);

	  for (blockinfo_list_const_iterator blci (f.blocks ().begin ());
	       blci != f.blocks ().end (); blci++)
	    {
	      const BlockInfo* bi = *blci;
	      xf.block ().push_back (block_name_T (bi->tag ()->name ()));
	    }
	}

      for (function_map_iterator fi (function_defn ().begin ());
	   fi != function_defn ().end (); fi++)
	{
	  const function_code_T& f = fi->second;

	  bs.function ().push_back (function_T (fi->first, "defn"));

	  function_T& xf = bs.function ().back ();

	  code_T xc (f, "func");
	  xc.line (f.line ());
	  xf.code (xc);

	  for (std::vector < std::string >::const_iterator
		 cf (f.call ().begin ()); cf != f.call ().end (); cf++)
	    {
	      const std::string& fn = *cf;

	      xf.function ().push_back (function_T (fn, "defn"));
	    }

	  for (blockinfo_list_const_iterator blci (f.blocks ().begin ());
	       blci != f.blocks ().end (); blci++)
	    {
	      const BlockInfo* bi = *blci;
	      xf.block ().push_back (block_name_T (bi->tag ()->name ()));
	    }
	}

      if (step_func_auto_vars ().length ())
	bs.code().push_back (code_T (step_func_auto_vars(), "autovars"));

      // include files

      std::string hfile;	// basename of header file name
      std::string::size_type p = header_filename ().rfind ('/');
      if (p == std::string::npos)
	hfile = header_filename ();
      else
	hfile = header_filename ().substr (p + 1);

      int ix = 0;
      const include_files_T* cif = include_files (source_filename ());
      if (cif)
	{
	  for (include_files_T::const_iterator i (cif->begin ());
	       i != cif->end (); i++)
	    {
	      // extract processed header file
	      if (*i == hfile)
		{
		  const include_files_T*
		    hif = include_files (header_filename ());

		  if (hif)
		    {
		      for (include_files_T::const_iterator j (hif->begin ());
			   j != hif->end (); j++)
			{
			  include_T xinc (ix, j->incfile ());
			  xinc.from (header_filename ());
			  bs.includes ().push_back (xinc);
			  ix++;
			}
		    }

		  include_T xinc (ix, i->incfile ());
		  xinc.from (source_filename ());
		  xinc.mark (true);
		  bs.includes ().push_back (xinc);
		  ix++;
		  continue;
		}

	      // included from C source
	      include_T xinc (ix, i->incfile ());
	      xinc.from (source_filename ());
	      bs.includes ().push_back (xinc);
	      ix++;
	    }
	}
    }

    void
    Blocks::out_xml (std::ostream &os)
    {
      xml_schema::namespace_infomap map;
      map["sm"].name = SIMULINKMODEL_NS;
      map["sm"].schema = "SimulinkModel.xsd";

      blocks (os, *xml(), map);
    }


    void
    Blocks::out_xml (const char* file)
    {
      std::ofstream os (file);
      out_xml (os);
    }


    void
    Blocks::out_xml (void)
    {
      out_xml (std::cout);
    }


    void
    Blocks::change_externals (void)
    {
      // correct externals
      if (external_output_order ().size () == 0)
	return;

      for (block_map_iterator bi (block_map().begin());
	   bi != block_map().end(); bi++)
	{
	  BlockInfo& binfo = bi->second;
	  block_T* btag = binfo.tag ();

	  if (btag->blocktype () == "Outport"
	      && binfo.upper () == NULL)
	    {
	      VariableEnt* ve = NULL;

	      for (var_list_iterator vi (external_output_order ().begin ());
		   vi != external_output_order ().end (); vi++)
		{
		  if ((*vi)->system () == binfo.system ()
		      && (*vi)->block () == binfo.block ())
		    {
		      ve = *vi;
		      break;
		    }
		}

	      if (ve == NULL)
		{
		  std::cout << "Warning: Outport: "
			    << btag->name () << ": ";
		  if (binfo.system ().length () == 0)
		    {
		      if (binfo.block ().length ())
			std::cout << "'<**UNKNOWN**>/"
				  << binfo.block () << "'";
		    }
		  else
		    {
		      std::cout << "'" << binfo.system() << "/";
		      if (binfo.block ().length () == 0)
			std::cout << "**UNKNOWN**";
		      else
			std::cout << binfo.block();
		      std::cout << "'";
		    }
		  std::cout << " not have External Output" << std::endl;
		}
	      else
		{
		  ve->tag () = external_output_type ();
		  ve->obj () = external_output_name ();
		  ve->blockinfo () = &binfo;
		  binfo.external_outputs().add_varent (ve);
		}
	    }

#if CHANGE_EXTERNALS_DEBUG
	  std::cout << "---- " << btag->blocktype()
		    << ":" << btag->name () << " ----" << std::endl;
#endif	// CHANGE_EXTERNALS_DEBUG
	  for (block_link_iterator l (binfo.backward ().begin ());
	       l != binfo.backward ().end (); l++)
	    {
	      for (pvar_ptr_list_iterator pvi (l->vars ().begin ());
		   pvi != l->vars ().end (); pvi++)
		{
		  VarElement* v = *pvi;
#if CHANGE_EXTERNALS_DEBUG
		  std::cout << v->name ();
#endif	// CHANGE_EXTERNALS_DEBUG
		  if (v->mode () == "extout")
		    {
#if CHANGE_EXTERNALS_DEBUG
		      std::cout << " change to output";
#endif	// CHANGE_EXTERNALS_DEBUG
		      v->mode () = "output";
		    }
#if CHANGE_EXTERNALS_DEBUG
		  std::cout << std::endl;
#endif	// CHANGE_EXTERNALS_DEBUG
		}
	    }
	}
    }


    class fname
    {
    public:
      fname (const std::string& s) : n (&s) {}
      const std::string& get (void) const { return *n; }
      const std::string* n;
      bool operator< (const fname& x) const
      {
	return get ().length () < x.get ().length ();
      }
    };


    void
    Blocks::check_call_function (void)
    {
      try
	{
	  std::string spat;

	  std::vector < fname > list;

	  for (function_map_const_iterator fi (function_defn ().begin ());
	       fi != function_defn ().end (); fi++)
	    {
	      // ignore Block function (eg. atomic subsystem funtion)
	      if (fi->second.blocks ().size ())
		continue;

	      list.push_back (fname (fi->first));
	    }

	  if (list.size () == 0)
	    return;

	  std::sort (list.begin (), list.end ());

	  for (std::vector < fname >::iterator i = list.begin ();
	       i != list.end (); i++)
	    {
	      fname& f = *i;
	      if (spat.length () == 0)
		spat = "(";
	      else
		spat += "|";
	      spat += "(" + f.get () + ")";
	    }

	  spat += ")[[:space:]]*\\(";

	  std::tr1::regex pat (spat);

	  std::tr1::cmatch mr;

	  for (function_map_iterator fi (function_defn ().begin ());
	       fi != function_defn ().end (); fi++)
	    {
	      function_code_T& f = fi->second;

	      const char *text = f.c_str ();
	      size_t size = f.length ();
	      unsigned int l = 1;

	      // skip to body
	      while (*text)
		{
		  if (*text == '{')
		    break;
		  text = nextline (text, size, l);
		}

	      // check functions in body
	      while (*text)
		{
		  if (std::tr1::regex_search (text, mr, pat))
		    {
#if CHECK_CALL_FUNCTION_DEBUG
		      std::cout << "* " << fi->first
				<< " call " << mr[1] << std::endl;
#endif
		      f.call ().push_back (mr[1]);

		      text = mr[0].second;
		    }
		  else
		    break;
		}
	    }
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}
    }


    void
    Blocks::parse_complete (void)
    {
      // Signal (order)
      for (var_list_iterator vi (signal_order().begin());
	   vi != signal_order().end(); vi++)
	{
	  VariableEnt* ve = *vi;
	  BlockInfo* binfo = find_block (ve->system(), ve->block());
	  ve->tag () = signal_type();
	  ve->obj () = signal_name();
	  if (binfo)
	    {
	      ve->blockinfo () = binfo;
	      binfo->signal_var().add_varent (ve);
	    }
	  else
	    std::cerr << "Warning: unknown block "
		      << ve->system () << '/' << ve->block ()
		      << " for Signal: "
		      << ve->name () << std::endl;
	}

      // Variables
      for (block_map_iterator bi (block_map().begin());
	   bi != block_map().end(); bi++)
	{
	  BlockInfo& binfo = bi->second;

	  for (propagate_var_list_iterator vi (binfo.input_var().begin());
	       vi != binfo.input_var().end(); vi++)
	    {
	      VarElement& ve = *vi;
	      c_variable_map_iterator cvi;
	      std::string& bname = ve.block();
	      BlockInfo* obinfo = NULL;

	      cvi = find_external_input (ve.name ());
	      if (cvi != external_input_entry ().end ())
		goto input_ok;

	      cvi = find_ext_var (ve.name());
	      if (cvi != ext_variable_entry().end())
		goto input_ok;

	      obinfo = find_block(bname);
	      if (obinfo != NULL)
		{
		  int found = 0;
		  if (&binfo != obinfo && obinfo->signal_var ().size () > 0)
		    {
		      for (var_list_iterator
			     svi (obinfo->signal_var ().begin ());
			   svi != obinfo->signal_var ().end (); svi++)
			{
			  VariableEnt* sve = *svi;
			  if (has_variable (binfo, sve))
			    {
			      ve.varent().add_varent(sve);
			      ve.switch_to_range (sve);
			      found = 1;
			    }
			}
		    }

		  if (found == 0 && obinfo->merged_to ().size ())
		    {
		      for (blockinfo_list_iterator
			     mbli (obinfo->merged_to ().begin ());
			   mbli != obinfo->merged_to ().end (); mbli++)
			{
			  BlockInfo* mbl = *mbli;
			  if (mbl == &binfo)
			    continue;

			  for (var_list_iterator
				 svi (mbl->signal_var ().begin ());
			       svi != mbl->signal_var ().end (); svi++)
			    {
			      VariableEnt* sve = *svi;
			      if (has_variable (binfo, sve))
				{
				  ve.varent().add_varent(sve);
				  ve.switch_to_range (sve);
				  found = 1;
				}
			    }
			}
		    }

		  if (found)
		    continue;
		}

	      std::cout << "Warning: unknown variable "
			<< binfo.tag ()->name ()
			<< ": "
			<< vi->name()
			<< '@'
			<< bname
			<< "(" << vi->mode() << ")"
			<< std::endl;
	      continue;

	    input_ok:
	      ve.varent ().add_varent(&cvi->second).used (true);
	      ve.switch_to_range (&cvi->second);
	    }

	  for (propagate_var_list_iterator vi (binfo.output_var().begin());
	       vi != binfo.output_var().end(); vi++)
	    {
	      VarElement& ve = *vi;
	      c_variable_map_iterator cvi;
	      int found = 0;

	      cvi = find_external_input (ve.name ());
	      if (cvi != external_input_entry ().end ())
		goto output_ok;

	      cvi = find_external_output (ve.name ());
	      if (cvi != external_output_entry ().end ())
		goto output_ok;

	      cvi = find_ext_var (ve.name());
	      if (cvi != ext_variable_entry().end())
		goto output_ok;

	      if (binfo.signal_var ().size () > 0)
		{
		  for (var_list_iterator
			 svi (binfo.signal_var ().begin ());
		       svi != binfo.signal_var ().end (); svi++)
		    {
		      VariableEnt* sve = *svi;
		      ve.varent ().add_varent(sve);
		      ve.switch_to_range (sve);
		      found = 1;
		    }
		}

	      if (found == 0 && binfo.merged_to ().size ())
		{
		  for (blockinfo_list_iterator
			 mbli (binfo.merged_to ().begin ());
		       mbli != binfo.merged_to ().end (); mbli++)
		    {
		      BlockInfo *mbl = *mbli;
		      if (mbl == &binfo)
			continue;

		      for (var_list_iterator
			     svi (mbl->signal_var ().begin ());
			   svi != mbl->signal_var ().end (); svi++)
			{
			  VariableEnt* sve = *svi;
			  ve.varent().add_varent(sve);
			  ve.switch_to_range (sve);
			  found = 1;
			}
		    }
		}

	      if (found == 0)
		{
		  std::cout << "Warning: unknown variable "
			    << binfo.tag ()->name ()
			    << ": "
			    << vi->name()
			    << '@'
			    << "(" << vi->mode() << ")"
			    << std::endl;
		}

	      continue;

	    output_ok:
	      ve.varent ().add_varent(&cvi->second).used (true);
	      ve.switch_to_range (&cvi->second);
	    }
	}

      // States (order)
      for (var_list_iterator vi (state_order().begin());
	   vi != state_order().end(); vi++)
	{
	  VariableEnt* ve = *vi;
	  BlockInfo* binfo = find_block (ve->system(), ve->block());
	  ve->tag () = state_type();
	  ve->obj () = state_name();
	  if (binfo)
	    {
	      ve->blockinfo () = binfo;
	      binfo->state_var().add_varent (ve);
	    }
	  else
	    std::cerr << "Warning: unknown block "
		      << ve->system () << '/' << ve->block ()
		      << " for State: "
		      << ve->name () << std::endl;
	}

      // Trigger States (order)
      for (var_list_iterator vi (trigger_state_order().begin());
	   vi != trigger_state_order().end(); vi++)
	{
	  VariableEnt* ve = *vi;
	  BlockInfo* binfo = find_block (ve->system(), ve->block());
	  ve->tag () = trigger_state_type();
	  ve->obj () = trigger_state_name();
	  if (binfo)
	    {
	      ve->blockinfo () = binfo;
	      binfo->trigger_state_var().add_varent (ve);
	    }
	  else
	    std::cerr << "Warning: unknown block "
		      << ve->system () << '/' << ve->block ()
		      << " for Trigger State: "
		      << ve->name () << std::endl;
	}

      // Invariant Signal (order)
      for (var_list_iterator vi (invariant_signal_order().begin());
	   vi != invariant_signal_order().end(); vi++)
	{
	  VariableEnt* ve = *vi;
	  BlockInfo* binfo = find_block (ve->system(), ve->block());
	  ve->tag () = invariant_signal_type();
	  ve->obj () = invariant_signal_name();
	  if (binfo)
	    {
	      ve->blockinfo () = binfo;
	      binfo->invariant_signal_var().add_varent (ve);
	    }
	  else
	    std::cerr << "Warning: unknown block "
		      << ve->system () << '/' << ve->block ()
		      << " for Invariant Signal: "
		      << ve->name () << std::endl;
	}

      // Parameters (order)
      for (var_list_iterator vi (param_order().begin());
	   vi != param_order().end(); vi++)
	{
	  VariableEnt* ve = *vi;

	  ve->tag () = param_type();
	  ve->obj () = param_name();

	  if (ve->multiple_system ().size ())
	    {
	      for (multiple_system_iterator
		     mi (ve->multiple_system ().begin ());
		   mi != ve->multiple_system ().end (); mi++)
		{
		  BlockInfo* binfo = find_block (mi->system (), mi->block ());
		  if (binfo)
		    {
		      mi->blockinfo () = binfo;
		      binfo->params ().add_varent (ve);
		    }
		  else
		    std::cerr << "Warning: unknown block "
			      << ve->system () << '/' << ve->block ()
			      << " for Parameter: "
			      << ve->name () << std::endl;
		}
	    }
	  else
	    {
	      BlockInfo* binfo = find_block (ve->system(), ve->block());
	      if (binfo)
		{
		  ve->blockinfo () = binfo;
		  binfo->params().add_varent (ve);
		}
	      else
		std::cerr << "Warning: unknown block "
			  << ve->system () << '/' << ve->block ()
			  << " for Parameter: "
			  << ve->name () << std::endl;
	    }
	}

      // Constant parameters (order)
      for (var_list_iterator vi (constant_param_order().begin());
	   vi != constant_param_order().end(); vi++)
	{
	  VariableEnt* ve = *vi;

	  ve->tag () = constant_param_type();
	  ve->obj () = constant_param_name();

	  if (ve->multiple_system ().size ())
	    {
	      for (multiple_system_iterator
		     mi (ve->multiple_system ().begin ());
		   mi != ve->multiple_system ().end (); mi++)
		{
		  BlockInfo* binfo = find_block (mi->system (), mi->block ());
		  if (binfo)
		    {
		      mi->blockinfo () = binfo;
		      binfo->constant_params ().add_varent (ve);
		    }
		  else
		    std::cerr << "Warning: unknown block "
			      << ve->system () << '/' << ve->block ()
			      << " for Constant Parameter: "
			      << ve->name () << std::endl;
		}
	    }
	  else
	    {
	      BlockInfo* binfo = find_block (ve->system(), ve->block());
	      if (binfo)
		{
		  ve->blockinfo () = binfo;
		  binfo->constant_params().add_varent (ve);
		}
	      else
		std::cerr << "Warning: unknown block "
			  << ve->system () << '/' << ve->block ()
			  << " for Constant Parameter: "
			  << ve->name () << std::endl;
	    }
	}

      // functions
      check_call_function ();
    }

    void
    Blocks::set_filename (file_type type, const char* file)
    {
      struct stat st;

      if (stat (file, &st) == -1)
	{
	  std::cerr << "Couldn't get status: " << file
		    << ": " << strerror (errno) << std::endl;
	  return;
	}

      struct tm *tmp = localtime (&st.st_mtime);
      if (tmp == NULL)
	{
	  std::cerr << "Couldn't convert localtime " << st.st_mtime
		    << ": " << strerror (errno) << std::endl;
	  return;
	}

      char iso8061[64];
      memset(iso8061, 0, sizeof iso8061);
#if defined(__MINGW32__) || defined(__MINGW64__)
      size_t s = strftime (iso8061, sizeof iso8061, "%Y-%m-%dT%H:%M:%S", tmp);
      if (s == 0)
	{
	  std::cerr << "Couldn't convert ISO8061 time " << st.st_mtime
		    << " -> " << iso8061 << ": " << strerror (errno)
		    << std::endl;
	  return;
	}
      if (timezone == 0)
	{
	  iso8061[s] = 'Z';
	  iso8061[s + 1] = 0;
        }
      else
	{
	  long m;
	  if (timezone < 0)
	    m = (-timezone) / 60;
	  else
	    m = timezone / 60;
	  long h = m / 60;
	  m %= 60;
	  snprintf (&iso8061[s], sizeof iso8061 - s, "%c%02ld%02ld",
		    timezone > 0 ? '-' : '+', h, m);
	}
#else
      if (strftime (iso8061, sizeof iso8061, "%FT%T%z", tmp) == 0)
	{
	  std::cerr << "Couldn't convert ISO8061 time " << st.st_mtime
		    << " -> " << iso8061 << ": " << strerror (errno)
		    << std::endl;
	  return;
	}
#endif

      std::string ftype;
      switch (type)
	{
	case XF_C_SOURCE:
	  ftype = "c-source";
	  break;
	case XF_C_HEADER:
	  ftype = "c-header";
	  break;
	case XF_C_DATA:
	  ftype = "c-data";
	  break;
	case XF_XML:
	  ftype = "xml";
	  break;
	}

      blocks_T* bs = xml();

      if (bs)
	{

	  // replace
	  for (blocks_T::file_iterator fi (bs->file().begin());
	       fi != bs->file().end(); fi++)
	    {
	      file_T& f = *fi;
	      if (f.type() == ftype)
		{
		  f.name() = file;
		  f.timestamp() = iso8061;
		  f.size() = st.st_size;
		  return;
		}
	    }

	  // add
	  const file_T nf (ftype, file, iso8061, st.st_size);
	  bs->file().push_back(nf);
	}
    }

    const file_T::name_type*
    Blocks::get_filename (file_type type) const
    {
      std::string ftype;
      switch (type)
	{
	case XF_C_SOURCE:
	  ftype = "c-source";
	  break;
	case XF_C_HEADER:
	  ftype = "c-header";
	  break;
	case XF_C_DATA:
	  ftype = "c-data";
	  break;
	case XF_XML:
	  ftype = "xml";
	  break;
	}

      blocks_T& bs = *xml();

      for (blocks_T::file_const_iterator fi (bs.file().begin());
	   fi != bs.file().end(); fi++)
	{
	  const file_T& f = *fi;
	  if (f.type() == ftype)
	    return &f.name();
	}
      return NULL;
    }

    void
    Blocks::add_include (const std::string& source,
			 const std::string& include, char mode)
    {
      include_files_T* ifs = include_files (source);

      include_files_T::iterator
	i = std::find (ifs->begin (), ifs->end (), include);

      if (i == ifs->end ())
	ifs->push_back (inc_file_T (include, mode));
    }

    const include_files_T*
    Blocks::include_files (const std::string& source) const
    {
      const include_map_T& im = include_map ();
      include_map_T::const_iterator i (im.find (source));
      if (i != im.end ())
	return &i->second;
      return NULL;
    }

    include_files_T*
    Blocks::include_files (const std::string& source)
    {
      include_map_T& im = include_map ();
      include_map_T::iterator i (im.find (source));
      if (i != im.end ())
	return &i->second;

      std::pair < include_map_T::iterator, bool >
	x = im.insert (std::make_pair (source, include_files_T()));
      return &x.first->second;
    }

    std::string
    Blocks::storage_qualifier_variable (const VariableEnt& ve) const
    {
      std::string s = "\\b";

      if (ve.mode () == VariableEnt::V_EXTIN)
	s += external_input_name ();
      else if (ve.mode () == VariableEnt::V_EXTOUT)
	s += external_output_name ();
      else if (ve.mode () == VariableEnt::V_SIGNAL)
	s += signal_name();

      if (s.length () > 1)
	s += "\\.";
      s += ve.name ();
      s += "\\b";

      return s;
    }

    bool
    Blocks::has_variable (const BlockInfo& binfo, const VariableEnt* ve) const
    {
      return has_variable (binfo, NULL, ve);
    }

    bool
    Blocks::has_variable (const BlockInfo& binfo, const BlockInfo* top,
			  const VariableEnt* ve) const
    {
      const BlockInfo* cur_top = binfo.top_block ();
      if (cur_top != NULL && cur_top != top && cur_top != &binfo)
	{
	  if (top == NULL)
	    top = cur_top;
	  return has_variable (*cur_top, top, ve);
	}

      if (binfo.merged_to ().size ())
	{
	  for (blockinfo_list_const_iterator
		 blci (binfo.merged_to ().begin ());
	       blci != binfo.merged_to ().end (); blci++)
	    {
	      if (top == NULL)
		{
		  if (cur_top)
		    top = cur_top;
		  else
		    top = &binfo;
		}
	      const BlockInfo* mbinfo = *blci;
	      if (mbinfo != &binfo && has_variable (*mbinfo, top, ve))
		return true;
	    }
	  return false;
	}

      try
	{
	  std::tr1::regex pat (storage_qualifier_variable (*ve));
	  std::tr1::smatch mr;

	  if (binfo.frag ())
	    {
	      for (CodeFrags::const_iterator i (binfo.codefrags ().begin ());
		   i != binfo.codefrags ().end (); i++)
		{
		  if (std::tr1::regex_search (i->frag (), mr, pat))
		    return true;
		}
	    }

	  const std::string& cd = binfo.code ();
	  if (cd.length ())
	    {
	      if (std::tr1::regex_search (cd, mr, pat))
		return true;
	    }
	  const std::string& upd = binfo.update ();
	  if (upd.length ())
	    {
	      if (std::tr1::regex_search (upd, mr, pat))
		return true;
	    }
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}

      return false;
    }

    //
    // Header
    //

    static const std::tr1::regex inc_pat ("^"
					  "[[:blank:]]*"
					  "#"
					  "[[:blank:]]*"
					  "include"
					  "[[:blank:]]*"
					  "([\"<])"
					  "([^\"<>]+)"
					  "([\">])"
					  );

    static const std::tr1::regex define_pat ("^"
					     "[[:blank:]]*"
					     "#"
					     "[[:blank:]]*"
					     "define"
					     "[[:blank:]]+"
					     "(" // %1 macro ident
					     "[_[:alpha:]][_[:alnum:]]*"
					     ")"
					     );

    static const std::tr1::regex ifdef_pat ("^"
					    "[[:blank:]]*"
					    "#"
					    "[[:blank:]]*"
					    "(" // %1 control macro
					    "(" // %2 start macro
					    "ifndef"
					    "|"
					    "ifdef"
					    "|"
					    "if"
					    ")"
					    "|"
					    "elif"
					    "|"
					    "else"
					    "|"
					    "(" // %3 end macro
					    "endif"
					    ")"
					    ")\\b"
					    "(" // %4 trail
					    "[[:blank:]]+"
					    "(" // %5 maybe ident
					    "[_[:alpha:]][_[:alnum:]]*"
					    ")"
					    ")?"
					    );

    void
    parse_root_storage (Blocks& bn, const char*& line, size_t& size,
			unsigned int& lineno, VariableEnt::var_mode mode)
    {
      try
	{
	  std::tr1::regex mdef ("^"
				"[[:space:]]*"
				"([_[:alpha:]][_[:alnum:]]*"
				"([[:space:]]+[_[:alpha:]][_[:alnum:]]*)?"
				")"			// -- %1 type
				"[[:space:]]+"
				"("			// -- %3
				"(\\*+)?"		// -- %4 (pointer)
				"[[:space:]]*"
				"([_[:alpha:]][_[:alnum:]]*)" // -- %5 name
				"[[:space:]]*"
				"("			// -- %6 array
				"(\\["
				"[[:space:]]*"
				"([[:digit:]]+)"	// -- %8 dem(int)
				"[[:space:]]*"
				"\\])+"			// -- %7 dem
				")?"			// -- %6
				")"			// -- %3
				"[[:space:]]*"
				";"
				"[[:space:]]*"
				"/\\*"
				"[[:space:]]*"
				"'"
				"("			// -- %9 <Sys>/block
				"(<[^>]+>)"		// -- %10 <Sys>
				"/"
				"([^':*/]+)"		// -- %11 block
				")"			// -- %9
				"'"
				"[[:space:]]*"
				"\\*/");

	  std::tr1::regex tdef("^"
			       "\\}"
			       "[[:space:]]*"
			       "([_[:alpha:]][_[:alnum:]]*)"
			       "[[:space:]]*"
			       ";");

	  std::tr1::regex sdef("^"
			       "[[:space:]]*"
			       "struct"
			       "[[:space:]]*"
			       "\\{"
			       "[[:space:]]*$");
	  std::tr1::regex send("^"
			       "([[:space:]]*"
			       "\\})"			// -- %1
			       "[[:space:]]*"
			       "([_[:alpha:]][_[:alnum:]]*)" // -- %2 var
			       "[[:space:]]*"
			       ";"
			       "[[:space:]]*"
			       "/\\*"
			       "[[:space:]]*"
			       "'"
			       "("			// -- %3 <Sys>/block
			       "(<[^>]+>)"		// -- %4 <Sys>
			       "/"
			       "([^':*/]+)"		// -- %5 block
			       ")"			// -- %3
			       "'"
			       "[[:space:]]*"
			       "\\*/");

	  std::tr1::cmatch mr;

	  const char* end = line + size;

	  int in_struct = 0;

	  std::string sbuf;

	  while (line < end)
	    {
	      size_t psize = size;
	      const char* pline = line;
	      line = nextline (line, size, lineno);
	      size_t z = line - pline;

	      const char* x = skip_space (pline, z);
	      if (x == NULL)
		continue;

	      if (in_struct == 0)
		{
		  if (psize >= 16
		      && strncmp (pline, "typedef struct {", 16) == 0)
		    continue;
		  else if (std::tr1::regex_search (pline, line, mr, sdef))
		    {
		      sbuf.assign (pline, z);
		      in_struct = 1;
		      continue;
		    }
		  else if (std::tr1::regex_search (pline, line, mr, mdef))
		    {
		      std::string type (mr[1]);
		      std::string ptr (mr[4]);
		      std::string var (mr[5]);
		      std::string array (mr[6]);
		      std::string sys (mr[10]);
		      std::string blk (mr[11]);

		      unsigned int sz = 0;
		      if (array.length())
			{
			  std::istringstream ss (mr[8]);
			  ss >> sz;
			}

		      const VariableEnt v(type, var, ptr, array, sz,
					  sys, blk, mode);
		      switch (mode)
			{
			case VariableEnt::V_STATE:
			  bn.add_state_entry (var, v);
			  break;
			case VariableEnt::V_TRIGGER:
			  bn.add_trigger_state_entry (var, v);
			  break;
			case VariableEnt::V_INVARIANT:
			  bn.add_invariant_signal_entry (var, v);
			  break;
			default:
			  break;
			}
		      continue;
		    }
		  else if (std::tr1::regex_search (pline, line, mr, tdef))
		    {
		      switch (mode)
			{
			case VariableEnt::V_STATE:
			  bn.state_type () = mr[1];
			  break;
			case VariableEnt::V_TRIGGER:
			  bn.trigger_state_type () = mr[1];
			  break;
			case VariableEnt::V_INVARIANT:
			  bn.invariant_signal_type () = mr[1];
			  break;
			default:
			  break;
			}
		    }
		  break;
		}
	      else
		{
		  if (std::tr1::regex_search (pline, line, mr, send))
		    {
		      std::string type (sbuf + mr[1]);
		      std::string var (mr[2]);
		      std::string sys (mr[4]);
		      std::string blk (mr[5]);
		      const VariableEnt v (type, var, "", "", 0,
					   sys, blk, mode);
		      switch (mode)
			{
			case VariableEnt::V_STATE:
			  bn.add_state_entry (var, v);
			  break;
			case VariableEnt::V_TRIGGER:
			  bn.add_trigger_state_entry (var, v);
			  break;
			case VariableEnt::V_INVARIANT:
			  bn.add_invariant_signal_entry (var, v);
			  break;
			default:
			  break;
			}
		      in_struct = 0;
		      continue;
		    }

		  sbuf += std::string (pline, z);
		}
	    }
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}
    }


    void
    parse_states (Blocks& bn, const char*& line, size_t& size,
		  unsigned int& lineno)
    {
      try
	{
	  std::tr1::regex vardef ("^"
				  "extern([[:space:]]+const)?"
				  "[[:space:]]+"
				  "([_[:alpha:]][_[:alnum:]]*)" // %2 -- type
				  "[[:space:]]+"
				  "([_[:alpha:]][_[:alnum:]]*)" // %3 -- name
				  ";");

	  std::tr1::cmatch mr;

	  const char* end = line + size;
	  while (line < end)
	    {
	      const char* nline = skip_empty_line (line, size, lineno);
	      if (nline != line)
		{
		  size -= (nline - line);
		  line = nline;
		}

	      if (strncmp ("extern", line, 6) != 0)
		break;

	      if (std::tr1::regex_search (line, end, mr, vardef))
		{
		  if (bn.state_type() == mr[2])
		    bn.state_name () = mr[3];
		  else if (bn.invariant_signal_type () == mr[2])
		    bn.invariant_signal_name () = mr[3];
		  else
		    {
		      std::cout << "Warning: "
				   "State/Invariant Signal type missmatch: "
				<< mr[2] << std::endl;
		    }

		  while (line < mr[0].second)
		    line = nextline (line, size, lineno);
		}
	    }
	}

      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}
    }

    void
    parse_param_storage (Blocks& bn, const char*& line, size_t& size,
			 unsigned int& lineno)
    {
      try
	{
	  std::tr1::regex stdef ("^"
				 "struct"
				 "[[:space:]]+"
				 "([_[:alpha:]][_[:alnum:]]*)_" // %1 -- tag
				 "[[:space:]]+"
				 "\\{");

	  std::tr1::regex mdef ("^"
				"[[:space:]]*"
				"([_[:alpha:]][_[:alnum:]]*"
				"([[:space:]]+[_[:alpha:]][_[:alnum:]]*)?"
				")"			// -- %1 type
				"[[:space:]]+"
				"("			// -- %3
				"(\\*+)?"		// -- %4 (pointer)
				"[[:space:]]*"
				"([_[:alpha:]][_[:alnum:]]*)" // -- %5 name
				"[[:space:]]*"
				"("			// -- %6 array
				"(\\["
				"[[:space:]]*"
				"([[:digit:]]+)"	// -- %8 dem(int)
				"[[:space:]]*"
				"\\])+"			// -- %7 dem
				")?"			// -- %6
				")"			// -- %3
				"[[:space:]]*"
				";"
				"[[:space:]]*"
				"("			// -- %9
				"/\\*"
				"[[:space:]]*"
				"'"
				"("			// -- %10 <Sys>/block
				"(<[^>]+>)"		// -- %11 <Sys>
				"/"
				"([^':*/]+)"		// -- %12 block
				")"			// -- %10
				"'"
				"[[:space:]]*"
				"\\*/"
				")?"			// -- %9
				);

	  std::tr1::regex sysdef("^"
				 "[[:space:]]*"
				 "\\*"
				 "[[:space:]]*"
				 "Referenced[[:space:]]+by:"
				 "("		// -- %1 (single block)
				 "[[:space:]]+"
				 "'"
				 "("		// -- %2 <Sys>/block
				 "(<[^>]+>)"	// -- %3 <Sys>
				 "/"
				 "([^':*/]+)"	// -- %4 block
				 ")"		// -- %2 (close)
				 "'"
				 ")?"
				 );

	  std::tr1::regex msysdef ("^"
				   "[[:space:]]*"
				   "\\*"
				   "[[:space:]]*"
				   "'"
				   "("		// -- %1 <Sys>/block
				   "(<[^>]+>)"	// -- %2 <Sys>
				   "/"
				   "([^':*/]+)"	// -- %3 block
				   ")"		// -- %1 (close)
				   "'"
				   );

	  std::tr1::cmatch mr;

	  std::string type;
	  std::string ptr;
	  std::string var;
	  std::string array;
	  std::string sys;
	  std::string block;
	  unsigned int sz;

	  const char* end = line + size;

	  VariableEnt* ve = NULL;

	  bool multi = false;

	  while (line < end)
	    {
	      const char* pline = line;
	      line = nextline (line, size, lineno);

	      if (multi == true
		  && std::tr1::regex_search (pline, line, mr, msysdef))
		{
		  if (ve == NULL)
		    {
		      const VariableEnt v (type, var, ptr, array, sz,
					   mr[2], mr[3],
					   VariableEnt::V_PARAM);
		      ve = &bn.add_param_entry(var, v);
		    }

		  ve->multiple_system().push_back (SystemBlockEnt (mr[2],
								   mr[3]));
		}
	      else if (std::tr1::regex_search (pline, line, mr, stdef))
		{
		  bn.param_type () = mr[1];
		}
	      else if (std::tr1::regex_search (pline, line, mr, mdef))
		{
		  ve = NULL;
		  multi = false;

		  type = mr[1];
		  ptr = mr[4];
		  var = mr[5];
		  array = mr[6];
		  if (array.length())
		    {
		      std::istringstream ss (mr[8]);
		      ss >> sz;
		    }
		  else
		    sz = 0;

		  if (mr[9].matched)
		    {
		      const VariableEnt v (type, var, ptr,
					   array, sz,
					   mr[11], mr[12],
					   VariableEnt::V_PARAM);
		      bn.add_param_entry(var, v);
		    }
		}
	      else if (std::tr1::regex_search (pline, line, mr, sysdef))
		{
		  if (mr[1].matched)
		    {
		      const VariableEnt v (type, var, ptr, array, sz,
					   mr[3], mr[4],
					   VariableEnt::V_PARAM);
		      bn.add_param_entry(var, v);
		      multi = false;
		    }
		  else
		    {
		      multi = true;
		      ve = NULL;
		    }
		}
	      else if (pline[0] == '}' && pline[1] == ';')
		break;
	    }
	}

      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}
    }

    void
    parse_params (Blocks& bn, const char*& line, size_t& size,
		  unsigned int& lineno)
    {
      try
	{
	  std::tr1::regex vardef ("^"
				  "extern"
				  "[[:space:]]+"
				  "([_[:alpha:]][_[:alnum:]]*)" // %1 -- type
				  "[[:space:]]+"
				  "([_[:alpha:]][_[:alnum:]]*)" // %2 -- name
				  ";");

	  std::tr1::cmatch mr;

	  const char* end = line + size;
	  while (line < end)
	    {
	      const char* pline = line;

	      line = nextline (line, size, lineno);

	      if (std::tr1::regex_search (pline, line, mr, vardef))
		{
		  if (bn.param_type() != mr[1])
		    {
		      std::cout << "Warning: Parameter type missmatch: "
				<< bn.param_type() << " != "
				<< mr[1] << std::endl;
		    }
		  bn.param_name () = mr[2];
		  break;
		}
	    }
	}

      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}
    }

    typedef std::pair < std::string, std::string > sysblk_T;
    typedef std::vector < sysblk_T > sysblk_list_T;

    void
    parse_const_params (Blocks& bn, const char*& line, size_t& size,
			unsigned int& lineno)
    {
      try
	{
	  std::tr1::regex mdef ("^"
				"[[:space:]]*"
				"([_[:alpha:]][_[:alnum:]]*"
				"([[:space:]]+[_[:alpha:]][_[:alnum:]]*)?"
				")"			// -- %1 type
				"[[:space:]]+"
				"("			// -- %3
				"(\\*+)?"		// -- %4 (pointer)
				"[[:space:]]*"
				"([_[:alpha:]][_[:alnum:]]*)" // -- %5 name
				"[[:space:]]*"
				"("			// -- %6 array
				"(\\["
				"[[:space:]]*"
				"([[:digit:]]+)"	// -- %8 dem(int)
				"[[:space:]]*"
				"\\])+"			// -- %7 dem
				")?"			// -- %6
				")"			// -- %3
				"[[:space:]]*"
				";");

	  std::tr1::regex sysdef("^"
				 "[[:space:]]*"
				 "\\*"
				 "[[:space:]]*"
				 "Referenced[[:space:]]+by:"
				 "("		// -- %1 (single block)
				 "[[:space:]]+"
				 "'"
				 "("		// -- %2 <Sys>/block
				 "(<[^>]+>)"	// -- %3 <Sys>
				 "/"
				 "([^':*/]+)"	// -- %4 block
				 ")"		// -- %2 (close)
				 "'"
				 ")?"		// -- %1 (close)
				 );

	  std::tr1::regex msysdef ("^"
				   "[[:space:]]*"
				   "\\*"
				   "[[:space:]]*"
				   "'"
				   "("		// -- %1 <Sys>/block
				   "(<[^>]+>)"	// -- %2 <Sys>
				   "/"
				   "([^':*/]+)"	// -- %3 block
				   ")"		// -- %1 (close)
				   "'"
				   );

	  std::tr1::regex tdef("^"
			       "\\}"
			       "[[:space:]]*"
			       "([_[:alpha:]][_[:alnum:]]*)"
			       "[[:space:]]*"
			       ";");

	  std::tr1::regex sdef("^"
			       "extern[[:space:]]+const[[:space:]]+"
			       "([_[:alpha:]][_[:alnum:]]*)"
			       "[[:space:]]+"
			       "([_[:alpha:]][_[:alnum:]]*)"
			       "[[:space:]]*"
			       ";");

	  std::tr1::cmatch mr;

	  std::string sys;
	  std::string block;

	  const char* end = line + size;

	  bool multi = false;

	  sysblk_list_T msystem;

	  while (line < end)
	    {
	      const char* pline = line;
	      size_t psize = size;
	      line = nextline (line, size, lineno);

	      if (psize > 16
		  && strncmp (pline, "typedef struct {", 16) == 0)
		continue;
	      else if (std::tr1::regex_search (pline, line, mr, sysdef))
		{
		  if (mr[1].matched)
		    {
		      sys = mr[3];
		      block = mr[4];
		      multi = false;
		    }
		  else
		    {
		      multi = true;
		      msystem.clear ();
		    }
		}
	      else if (multi
		       && std::tr1::regex_search (pline, line, mr, msysdef))
		{
		  msystem.push_back (std::make_pair (mr[2], mr[3]));
		}
	      else if (std::tr1::regex_search (pline, line, mr, mdef))
		{
		  std::string type = mr[1];
		  std::string ptr = mr[4];
		  std::string var = mr[5];
		  std::string array = mr[6];
		  unsigned int sz = 0;
		  if (array.length())
		    {
		      std::istringstream ss (mr[8]);
		      ss >> sz;
		    }

		  if (multi)
		    {
		      sysblk_list_T::iterator i = msystem.begin ();
		      const VariableEnt ve (type, var, ptr, array, sz,
					    i->first, i->second,
					    VariableEnt::V_CONST);
		      VariableEnt*
			vp = &bn.add_constant_param_entry (var, ve);
		      // i++;
		      for (; i != msystem.end (); i++)
			{
			  const SystemBlockEnt sb(i->first, i->second);
			  vp->multiple_system ().push_back (sb);
			}
		      multi = false;
		    }
		  else
		    {
		      VariableEnt ve (type, var, ptr, array, sz, sys, block,
				      VariableEnt::V_CONST);
		      bn.add_constant_param_entry(var, ve);
		    }
		}
	      else if (std::tr1::regex_search (pline, line, mr, tdef))
		{
		  bn.constant_param_type (mr[1]);
		  break;
		}
	      else if (std::tr1::regex_search (pline, line, mr, sdef))
		{
		  if (bn.constant_param_type() != mr[1])
		    {
		      std::cout
			<< "Warning: Constant Parameter type missmatch: "
			<< bn.constant_param_type()
			<< " != " << mr[1] << std::endl;
		    }
		  bn.constant_param_name (mr[2]);
		  break;
		}
	    }
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}
    }

    void
    parse_signals (Blocks& bn, const char*& line, size_t& size,
		   unsigned int& lineno)
    {
      try
	{
	  std::tr1::regex vardef ("^"
				  "extern"
				  "[[:space:]]+"
				  "([_[:alpha:]][_[:alnum:]]*)" // %1 -- type
				  "[[:space:]]+"
				  "([_[:alpha:]][_[:alnum:]]*)" // %2 -- name
				  ";");

	  std::tr1::regex mdef ("^"
				"[[:space:]]*"
				"([_[:alpha:]][_[:alnum:]]*"
				"([[:space:]]+[_[:alpha:]][_[:alnum:]]*)?"
				")"			// -- %1 type
				"[[:space:]]+"
				"("			// -- %3
				"(\\*+)?"		// -- %4 (pointer)
				"[[:space:]]*"
				"([_[:alpha:]][_[:alnum:]]*)" // -- %5 name
				"[[:space:]]*"
				"("			// -- %6 array
				"(\\["
				"[[:space:]]*"
				"([[:digit:]]+)"	// -- %8 dem(int)
				"[[:space:]]*"
				"\\])+"			// -- %7 dem
				")?"			// -- %6
				")"			// -- %3
				"[[:space:]]*"
				";"
				"[[:space:]]*"
				"/\\*"
				"[[:space:]]*"
				"'"
				"("			// -- %9 <Sys>/block
				"(<[^>]+>)"		// -- %10 <Sys>
				"/"
				"([^':*/]+)"		// -- %11 block
				")"			// -- %9
				"'"
				"[[:space:]]*"
				"\\*/");

	  std::tr1::regex tdef("^"
			       "\\}"
			       "[[:space:]]*"
			       "([_[:alpha:]][_[:alnum:]]*)"
			       "[[:space:]]*"
			       ";");

	  std::tr1::cmatch mr;

	  const char* end = line + size;

	  while (line < end)
	    {
	      const char* pline = line;
	      size_t psize = size;

	      line = nextline (line, size, lineno);

	      if (psize > 16
		  && strncmp (pline, "typedef struct {", 16) == 0)
		continue;
	      else if (std::tr1::regex_search (pline, line, mr, mdef))
		{
		  std::string type = mr[1];
		  std::string ptr = mr[4];
		  std::string var = mr[5];
		  std::string array = mr[6];
		  std::string sys = mr[10];
		  std::string blk = mr[11];
		  unsigned int sz = 0;
		  if (array.length ())
		    {
		      std::istringstream ss (mr[8]);
		      ss >> sz;
		    }
		  VariableEnt ve (type, var, ptr, array, sz, sys, blk,
				  VariableEnt::V_SIGNAL);
		  bn.add_signal_entry(var, ve);
		  continue;
		}
	      else if (std::tr1::regex_search (pline, line, mr, tdef))
		{
		  bn.signal_type () = mr[1];
		  break;
		}
	      else if (std::tr1::regex_search (pline, line, mr, vardef))
		{
		  if (bn.signal_type() != mr[1])
		    {
		      std::cout
			<< "Warning: Block Signal type missmatch: "
			<< bn.signal_type()
			<< " != " << mr[1] << std::endl;
		    }
		  bn.signal_name () = mr[2];
		  break;
		}
	    }
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}
    }

    void
    parse_externals (Blocks& bn,
		     const char*& line, size_t& size, unsigned int& lineno,
		     int io)
    {
      try
	{
	  std::tr1::regex vardef ("^"
				  "extern"
				  "[[:space:]]+"
				  "([_[:alpha:]][_[:alnum:]]*)" // %1 -- type
				  "[[:space:]]+"
				  "([_[:alpha:]][_[:alnum:]]*)" // %2 -- name
				  ";");

	  std::tr1::regex mdef ("^"
				"[[:space:]]*"
				"([_[:alpha:]][_[:alnum:]]*"
				"([[:space:]]+[_[:alpha:]][_[:alnum:]]*)?"
				")"			// -- %1 type
				"[[:space:]]+"
				"("			// -- %3
				"(\\*+)?"		// -- %4 (pointer)
				"[[:space:]]*"
				"([_[:alpha:]][_[:alnum:]]*)" // -- %5 name
				"[[:space:]]*"
				"("			// -- %6 array
				"(\\["			// -- %7 dem
				"[[:space:]]*"
				"([[:digit:]]+)"	// -- %8 dem(int)
				"[[:space:]]*"
				"\\])+"			// -- %7
				")?"			// -- %6
				")"			// -- %3
				"[[:space:]]*"
				";"
				"[[:space:]]*"
				"/\\*"
				"[[:space:]]*"
				"'"
				"("			// -- %9 <Sys>/block
				"(<[^>]+>)"		// -- %10 <Sys>
				"/"
				"([^':*/]+)"		// -- %11 block
				")"			// -- %9
				"'"
				"[[:space:]]*"
				"\\*/");

	  std::tr1::regex tdef("^"
			       "\\}"
			       "[[:space:]]*"
			       "([_[:alpha:]][_[:alnum:]]*)"
			       "[[:space:]]*"
			       ";");

	  std::tr1::cmatch mr;

	  const char* end = line + size;

	  while (line < end)
	    {
	      const char* pline = line;
	      size_t psize = size;

	      line = nextline (line, size, lineno);

	      if (psize > 16
		  && strncmp (pline, "typedef struct {", 16) == 0)
		continue;
	      else if (std::tr1::regex_search (pline, line, mr, mdef))
		{
		  std::string type = mr[1];
		  std::string ptr = mr[4];
		  std::string var = mr[5];
		  std::string array = mr[6];
		  std::string sys = mr[10];
		  std::string blk = mr[11];
		  unsigned int sz = 0;
		  if (array.length ())
		    {
		      std::istringstream ss (mr[8]);
		      ss >> sz;
		    }

		  VariableEnt ve (type, var, ptr, array, sz, sys, blk,
				  io ? VariableEnt::V_EXTIN
				  : VariableEnt::V_EXTOUT);
		  if (io)
		    bn.add_external_input_entry(var, ve);
		  else
		    bn.add_external_output_entry(var, ve);
		  continue;
		}
	      else if (std::tr1::regex_search (pline, line, mr, tdef))
		{
		  if (io)
		    bn.external_input_type (mr[1]);
		  else
		    bn.external_output_type (mr[1]);
		  break;
		}
	      else if (std::tr1::regex_search (pline, line, mr, vardef))
		{
		  if (io)
		    {
		      if (bn.external_input_type() != mr[1])
			{
			  std::cout
			    << "Warning: External Input type missmatch: "
			    << bn.external_input_type()
			    << " != " << mr[1] << std::endl;
			}
		      bn.external_input_name (mr[2]);
		    }
		  else
		    {
		      if (bn.external_output_type() != mr[1])
			{
			  std::cout
			    << "Warning: External Output type missmatch: "
			    << bn.external_output_type()
			    << " != " << mr[1] << std::endl;
			}
		      bn.external_output_name (mr[2]);
		    }
		  break;
		}
	    }
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}
    }

    void
    parse_block (Blocks& bn,
		 const char*& line, size_t& size, unsigned int& lineno)
    {
      try
	{
	  const char* end = line + size;
	  const char* p;
	  const char* q;

	  std::string b;
	  std::string l;

	  while (line < end)
	    {
	      const char* pline = line;
	      size_t psize = size;

	      line = nextline (line, size, lineno);

	      if ((psize >= 3 && strncmp (pline, " */", 3) == 0)
		  || (psize >= 2 && strncmp (pline, " *", 2) != 0))
		break;

	      if (psize < 6)
		continue;

	      // find RTW-EC Block prefix
	      if (pline[3] == '\'' && pline[4] == '<')
		{
		  q = &pline[4]; // block name
		  for (p = q + 1; p < end; p++)
		    {
		      if (*p == '\'')
			{
			  b.assign (q, p - q);
			  p++;
			  break;
			}
		    }

		  q = NULL;

		  // skip to Simulink Block layer
		  for (; p < end; p++)
		    {
		      if (*p == '\'')
			{
			  q = ++p;
			  break;
			}
		    }

		  // break if not found
		  if (q == NULL)
		    break;

		  // find end mark
		  for (; p < end; p++)
		    {
		      if (*p == '\'')
			{
			  l.assign (q, p - q);
			  break;
			}
		    }

		  BlockNameEnt&
		    be = bn.add_blockname_entry(b,
						BlockNameEnt (b.c_str(),
							      l.c_str()));
		  BlockInfo* binfo = bn.find_block (be.symbol ());
		  if (binfo)
		    {
		      be.blockinfo (binfo);

		      // Top Reference block
		      if (binfo->tag ()->blocktype () == "Reference")
			{
			  be.reference (true);
			  be.set_top (&be);
#if PARSE_DATA_DEBUG > 0
			  std::cout << "* " << be.blockname () << ": "
				    << be.layer () << "[Reference:"
				    << be.symbol ()
				    << ']' << std::endl;
#endif
			}
		    }
		  else
		    {
		      // a under block of Reference block
		      std::string::size_type ll = l.length ();
		      for (blockname_map_iterator
			     bni (bn.blockname_entry ().begin ());
			   bni != bn.blockname_entry ().end (); bni++)
			{
			  BlockNameEnt& bne = bni->second;

			  if (!bne.reference ())
			    continue;

			  const std::string& u = bne.layer ();
			  std::string::size_type ul = u.length ();
			  if (ll > ul
			      && l.compare (0, ul, u) == 0
			      && l[ul] == '/')
			    {
			      be.reference (true);
			      be.set_top (bne.set_top ());
#if PARSE_DATA_DEBUG > 0
			      std::cout << "* " << be.blockname () << ": "
					<< be.layer () << "[Reference:"
					<< bne.symbol ()
					<< ']' << std::endl;
#endif
			      break;
			    }
			}
		    }

		  // make lower-upper relation, except `<Root>'
		  std::string::size_type p = l.rfind ('/');
		  if (p != std::string::npos)
		    {
		      for (blockname_map_iterator
			     bni (bn.blockname_entry ().begin ());
			   bni != bn.blockname_entry ().end (); bni++)
			{
			  BlockNameEnt& bne = bni->second;
			  const std::string& u = bne.layer ();
			  std::string::size_type ul = u.length ();
			  if (ul == p && l.compare (0, ul, u) == 0)
			    {
			      // relate lower-upper
			      be.upper (&bne);
			      bne.lowers ().push_back (&be);

			      // set Top block,
			      // when block is not in BLXML,
			      // and is not Reference.
			      //
			      // (this is a lower block of
			      // Functionalized Atomic Sybsystem)
			      if (binfo == NULL && !be.reference ())
				be.set_top (bne.find_top ());
			      break;
			    }
			}
		    }
		}
	    }
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}
    }

    void
    parse_header_rtm_macro (Blocks& bn, const char*& line, size_t& size,
			    unsigned int& lineno)
    {
      try
	{
	  const char* start = line;
	  const char* end = line + size;

	  line = nextline (line, size, lineno); // skip comment

	  while (line < end)
	    {
	      // found next head comment
	      if (size >= 2 && line[0] == '/' && line[1] == '*')
		{
		  bn.rtm_macro().assign (start, line - start);
		  break;
		}

	      line = nextline (line, size, lineno);
	    }
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}
    }


    void
    parse_header_rtm_struct (Blocks& bn, const char*& line, size_t& size,
			     unsigned int& lineno)
    {
      try
	{
	  const char* start = line;
	  const char* end = line + size;

	  // skip comment
	  line = nextline (line, size, lineno);

	  while (line < end)
	    {
	      const char* pline = line;
	      size_t psize = size;
	      line = nextline (line, size, lineno);

	      // found terminator of structure
	      if (psize >= 2 && pline[0] == '}' && pline[1] == ';')
		{
		  bn.rtm_struct().assign (start, line - start);
		  break;
		}
	    }
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}
    }


    void
    parse_header_rtm_object (Blocks& bn, const char*& line, size_t& size,
			     unsigned int& lineno)
    {
      try
	{
	  std::tr1::regex edef ("^"
				"extern"
				"[[:space:]]+"
				"(struct[[:space:]]+)?"	// -- %1 struct?
				"([_[:alpha:]][_[:alnum:]]*)" // -- %2 tag
				"[[:space:]]*"
				"\\*"
				"([[:space:]]*const)?" // -- %3 const
				"[[:space:]]*"
				"([_[:alpha:]][_[:alnum:]]*)" // -- %4 name
				"[[:space:]]*"
				";");

	  std::tr1::cmatch mr;

	  const char* end = line + size;
	  while (line < end)
	    {
	      if (std::tr1::regex_search (line, end, mr, edef))
		{
		  if (mr[1].matched)
		    bn.rtm_extern_has_struct() = true;
		  bn.rtm_extern_type() = mr[2];
		  if (mr[3].matched)
		    bn.rtm_extern_has_const() = true;
		  bn.rtm_extern_name() = mr[4];

		  while (line < mr[0].second)
		    line = nextline (line, size, lineno);
		}
	      break;
	    }
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}
    }


    void
    parse_header_for_system (Blocks& bn, const char*& line, size_t& size,
			     unsigned int& lineno, SysData::sd_type stype)
    {
      try
	{
	  std::tr1::regex cdef ("/\\*"
				"[[:space:]]*"
				"("		// %1 -- kind
				"("		// %2 -- parameters
				"Parameters"
				"[[:space:]]+"
				"for"
				"[[:space:]]+"
				"system:"
				")"		// %2 -- (close)
				"|"
				"("		// %3 -- block signals
				"Block"
				"[[:space:]]+"
				"("		// %4 -- signal/state
				"signals"
				"|"
				"states"
				"[[:space:]]+"
				"\\("
				"[[:space:]]*"
				"auto"
				"[[:space:]]+"
				"storage"
				"[[:space:]]*"
				"\\)"
				")"		// %4 -- (close)
				"[[:space:]]+"
				"for"
				"[[:space:]]+"
				"system"
				")"		// %3 -- (close)
				")"		// %1 -- (close)
				"[[:space:]]+"
				"'"
				"("		// %5 -- <Sys>/block
				"(<[^>]+>)"	// %6 -- <Sys>
				"/"
				"([^':*/]+)"	// %7 -- block
				")"		// %5 -- (close)
				"'"
				"[[:space:]]*"
				"\\*/");

	  std::tr1::regex bdef ("("		// %1 typedef / struct
				"("		// %2 -- typedef (block)
				"typedef"
				"[[:space:]]+"
				"struct"
				")"		// %2 -- close
				"|"
				"("		// %3 -- struct (param)
				"struct"
				"[[:space:]]+"
				"("		// %4 -- struct tag
				"[_[:alpha:]][_[:alnum:]]*"
				")"		// %4 -- close
				")"		// %3 -- close
				")"		// %1 -- close
				"[[:space:]]*"
				"\\{");

	  std::tr1::regex edef ("^"
				"\\}"
				"[[:space:]]*"
				"("		// %1 -- typedef (block)
				"[_[:alpha:]][_[:alnum:]]*"
				")?"		// %1 -- close
				"[[:space:]]*"
				";");

	  std::tr1::cmatch mr;

	  const char* t_beg = line;
	  const char* t_end = NULL;

	  std::string system;
	  std::string block;
	  std::string type;
	  std::string tag;
	  bool has_struct = false;

	  int state = 0;

	  const char* end = line + size;
	  while (line < end)
	    {
	      size_t s = size;
	      unsigned int l = lineno;
	      const char* n = nextline (line, s, l);

	      if (state == 0
		  && std::tr1::regex_search (line, n, mr, cdef))
		{
		  system = mr[6];
		  block = mr[7];
		  state = 1;
		}
	      else if (state == 1
		       && std::tr1::regex_search (line, n, mr, bdef))
		{
		  if (mr[3].matched)
		    {
		      type = mr[4];
		      has_struct = true;
		    }
		  state = 2;
		}
	      else if (state == 2
		       && std::tr1::regex_search (line, n, mr, edef))
		{
		  if (mr[1].matched)
		    type = mr[1];
		  state = 3;
		}
	      else if (state == 3)
		{
		  t_end = line;

		  const std::string code (t_beg, t_end - t_beg);
		  bn.add_sysdata_list (SysData (stype, system, block, NULL,
						type, has_struct, tag, code));
		  break;
		}

	      line = n;
	      lineno = l;
	      size = s;
	    }
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}
    }

    int
    parse_header (Blocks& bn, void *addr, size_t size)
    {
      try {
	const char* line = static_cast <const char*> (addr);
	const char* end = line + size;

	std::string& hname = bn.header_filename ();
	if (hname.length () == 0)
	  hname = "h-none";

	enum {
	  PARSE_NONE,
	  PARSE_BLOCK,
	  PARSE_STATEDEF, PARSE_STATES,
	  PARSE_TRIGGERDEF,
	  PARSE_INVARIANTDEF,
	  PARSE_PARAMDEF, PARSE_PARAMS,
	  PARSE_CONSTPARAMS,
	  PARSE_SIGNAL,
	  PARSE_EXTINS,
	  PARSE_EXTOUTS,
	  PARSE_RTM_MACRO,
	  PARSE_RTM_STRUCT,
	  PARSE_RTM_OBJ,
	  PARSE_PARAM_FOR_SYSTEM,
	  PARSE_BSIGNAL_FOR_SYSTEM,
	  PARSE_BSTATE_FOR_SYSTEM
	} state = PARSE_NONE;

	std::tr1::cmatch mr;

	unsigned int lineno = 1;

	while (line < end)
	  {
	    size_t s = size;
	    unsigned int ln = lineno;
	    const char *n = nextline (line, s, ln);

	    if (state == PARSE_NONE)
	      {
		if (size >= 46
		    && strncmp (line,
				" * Here is the system hierarchy "
				"for this model",
				46) == 0)
		  state = PARSE_BLOCK;
		else if (size >= 53
			 && strncmp (line,
				     "/* Block states (auto storage) "
				     "for system '<Root>' */", 53) == 0)
		  state = PARSE_STATEDEF;
		else if (size >= 33
			 && strncmp (line,
				     "/* Block states (auto storage) */",
				     33) == 0)
		  state = PARSE_STATES;
		else if (size >= 35
			 && strncmp (line,
				     "/* Zero-crossing (trigger) state */",
				     35) == 0)
		  state = PARSE_TRIGGERDEF;
		else if (size >= 44
			 && strncmp (line,
				     "/* Invariant block signals "
				     "(auto storage) */",
				     44) == 0)
		  state = PARSE_INVARIANTDEF;
		else if (size >= 31
			 && strncmp (line,
				     "/* Parameters (auto storage) */",
				     31) == 0)
		  state = PARSE_PARAMDEF;
		else if (size > 37
			 && strncmp (line,
				     "/* Block parameters (auto storage) */",
				     37) == 0)
		  state = PARSE_PARAMS;
		else if (size > 40
			 && strncmp (line,
				     "/* Constant parameters "
				     "(auto storage) */",
				     40) == 0)
		  state = PARSE_CONSTPARAMS;
		else if (size >= 34
			 && strncmp (line,
				     "/* Block signals (auto storage) */",
				     34) == 0)
		  state = PARSE_SIGNAL;
		else if (size >= 61
			 && strncmp (line,
				     "/* External inputs "
				     "(root inport signals with "
				     "auto storage) */",
				     61) == 0)
		  state = PARSE_EXTINS;
		else if (size >= 71
			 && strncmp (line,
				     "/* External outputs "
				     "(root outports fed by signals with "
				     "auto storage) */",
				     71) == 0)
		  state = PARSE_EXTOUTS;
		else if (size >= 57
			 && strncmp (line,
				     "/* Macros for accessing real-time model "
				     "data structure */",
				     57) == 0)
		  {
		    state = PARSE_RTM_MACRO;

		    // retstart from this comment
		    continue;
		  }
		else if (size >= 36
			 && strncmp (line,
				     "/* Real-time Model Data Structure */",
				     36) == 0)
		  {
		    state = PARSE_RTM_STRUCT;

		    // retstart from this comment
		    continue;
		  }
		else if (size >= 28
			 && strncmp (line,
				     "/* Real-time Model object */",
				     28) == 0)
		  state = PARSE_RTM_OBJ;
		else if (size >= 26
			 && strncmp (line,
				     "/* Parameters for system: ", 26) == 0)
		  {
		    state = PARSE_PARAM_FOR_SYSTEM;

		    // restart from this comment
		    continue;
		  }
		else if (size >= 29
			 && strncmp (line,
				     "/* Block signals for system ", 29) == 0)
		  {
		    state = PARSE_BSIGNAL_FOR_SYSTEM;

		    // restart from this comment
		    continue;
		  }
		else if (size >= 42
			 && strncmp (line,
				     "/* Block states (auto storage) "
				     "for system ", 42) == 0)
		  {
		    state = PARSE_BSTATE_FOR_SYSTEM;

		    // restart from this comment
		    continue;
		  }
		else if (bn.header_preamble ().length () == 0
			 && std::tr1::regex_search (line, n, mr, ifdef_pat)
			 && mr[2].matched
			 && mr[2] == "ifndef"
			 && mr[5].matched)
		  {
		    std::string macro_ident (mr[5]);

		    size_t n_s = s;
		    unsigned int n_ln = ln;
		    const char* n_n = nextline (n, n_s, n_ln);

		    if (std::tr1::regex_search (n, n_n, mr, define_pat)
			&& mr[1] == macro_ident)
		      {
			bn.header_preamble () = std::string(line, n_n - line);

			n = n_n;
			s = n_s;
			ln = n_ln;
		      }
		  }
		else if (std::tr1::regex_search (line, n, mr, inc_pat))
		  {
		    const std::string& ifm = mr[1];
		    const std::string& inc = mr[2];
		    char m = ifm[0];

		    bn.add_include (hname, inc, m);
		  }

		line = n;
		size = s;
		lineno = ln;
	      }
	    else if (state == PARSE_BLOCK)
	      {
		parse_block (bn, line, size, lineno);
		state = PARSE_NONE;
	      }
	    else if (state == PARSE_STATEDEF)
	      {
		parse_root_storage (bn, line, size, lineno,
				    VariableEnt::V_STATE);
		state = PARSE_NONE;
	      }
	    else if (state == PARSE_STATES)
	      {
		parse_states (bn, line, size, lineno);
		state = PARSE_NONE;
	      }
	    else if (state == PARSE_TRIGGERDEF)
	      {
		parse_root_storage (bn, line, size, lineno,
				    VariableEnt::V_TRIGGER);
		state = PARSE_NONE;
	      }
	    else if (state == PARSE_INVARIANTDEF)
	      {
		parse_root_storage (bn, line, size, lineno,
				    VariableEnt::V_INVARIANT);
		state = PARSE_NONE;
	      }
	    else if (state == PARSE_PARAMDEF)
	      {
		parse_param_storage (bn, line, size, lineno);
		state = PARSE_NONE;
	      }
	    else if (state == PARSE_PARAMS)
	      {
		parse_params (bn, line, size, lineno);
		state = PARSE_NONE;
	      }
	    else if (state == PARSE_CONSTPARAMS)
	      {
		parse_const_params (bn, line, size, lineno);
		state = PARSE_NONE;
	      }
	    else if (state == PARSE_SIGNAL)
	      {
		parse_signals (bn, line, size, lineno);
		state = PARSE_NONE;
	      }
	    else if (state == PARSE_EXTINS)
	      {
		parse_externals (bn, line, size, lineno, 1);
		state = PARSE_NONE;
	      }
	    else if (state == PARSE_EXTOUTS)
	      {
		parse_externals (bn, line, size, lineno, 0);
		state = PARSE_NONE;
	      }
	    else if (state == PARSE_RTM_MACRO)
	      {
		parse_header_rtm_macro (bn, line, size, lineno);
		state = PARSE_NONE;
	      }
	    else if (state == PARSE_RTM_STRUCT)
	      {
		parse_header_rtm_struct (bn, line, size, lineno);
		state = PARSE_NONE;
	      }
	    else if (state == PARSE_RTM_OBJ)
	      {
		parse_header_rtm_object (bn, line, size, lineno);
		state = PARSE_NONE;
	      }
	    else if (state == PARSE_BSIGNAL_FOR_SYSTEM)
	      {
		parse_header_for_system (bn, line, size, lineno,
					 SysData::SD_BSIG);
		state = PARSE_NONE;
	      }
	    else if (state == PARSE_BSTATE_FOR_SYSTEM)
	      {
		parse_header_for_system (bn, line, size, lineno,
					 SysData::SD_BSTATE);
		state = PARSE_NONE;
	      }
	    else if (state == PARSE_PARAM_FOR_SYSTEM)
	      {
		parse_header_for_system (bn, line, size, lineno,
					 SysData::SD_PARAM);
		state = PARSE_NONE;
	      }
	  }
      }
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	  return -1;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	  return -1;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	  return -1;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	  return -1;
	}

      return 0;
    }

    Blocks_PTR
    parse_header (void* addr, size_t size, blocks_T_PTR& xml)
    {
      Blocks_PTR bn (new Blocks (xml));

      if (parse_header (*bn, addr, size) == 0)
	return bn;
      return Blocks_PTR();
    }


    Blocks_PTR
    parse_header (const char* f, blocks_T_PTR& xml)
    {
      size_t size = 0;
      void* addr = mapfile (f, size);
      void* start = NULL;

      if (addr == NULL)
	return Blocks_PTR();

      FileCoding fc = check_file_coding (addr, start);
      if (fc != FileCoding::FC_U8 && fc != FileCoding::FC_NONE)
	{
	  std::cerr << "Error: " << f
		    << ": Invalid CodingSystem: " << FileCodingName[fc]
		    << std::endl;
	  (void)unmapfile (addr, size);
	  return Blocks_PTR();
	}

      Blocks_PTR bn (new Blocks (xml));

      if (f)
	{
	  bn->set_filename (Blocks::XF_C_HEADER, f);
	  bn->header_filename () = f;
	}

      if (parse_header (*bn, start, size) == 0)
	{
	  (void)unmapfile (addr, size);
	  return bn;
	}

      (void)unmapfile (addr, size);
      return Blocks_PTR();
    }


    Blocks_PTR
    parse_header (const char* f)
    {
      size_t size = 0;
      void* addr = mapfile (f, size);
      void* start = NULL;

      if (addr == NULL)
	return Blocks_PTR();

      FileCoding fc = check_file_coding (addr, start);
      if (fc != FileCoding::FC_U8 && fc != FileCoding::FC_NONE)
	{
	  std::cerr << "Error: " << f
		    << ": Invalid CodingSystem: " << FileCodingName[fc]
		    << std::endl;
	  (void)unmapfile (addr, size);
	  return Blocks_PTR();
	}

      Blocks_PTR bn (new Blocks);

      if (f)
	{
	  bn->set_filename (Blocks::XF_C_HEADER, f);
	  bn->header_filename () = f;
	}

      if (parse_header (*bn, start, size) == 0)
	{
	  (void)unmapfile (addr, size);
	  return bn;
	}

      (void)unmapfile (addr, size);
      return Blocks_PTR();
    }


    //
    // C Source code
    //

    const char*
    nextline (const char* line, size_t& size, unsigned int& lineno)
    {
      const char* end = line + size;
      while (line < end)
	{
	  --size;
	  if (*line++ == '\n')
	    {
	      lineno++;
	      break;
	    }
	}
      return line;
    }


    const char*
    nextline (const char* line, size_t& size)
    {
      unsigned int tmp = 0;
      return nextline (line, size, tmp);
    }

    const char*
    find_comment (const char* beg, size_t size, const char*& end,
		  unsigned int& lineno, unsigned int& endno)
    {
      const char* start = NULL;
      const char* c = beg;
      const char* e = beg + size;
      size_t s = size;
      unsigned int l = lineno;

      while (c < e)
	{
	  const char* p = c;
	  unsigned int ll = l;
	  const char* n = nextline (c, s, ll);

	  for (; p + 1 < n && p + 1 < e; p++)
	    {
	      if (start && p[0] == '*' && p[1] == '/')
		{
		  end = n;
		  endno = ll;
		  return start;
		}
	      if (p[0] == '/' && p[1] == '*')
		{
		  if (start == NULL)
		    {
		      start = c;
		      lineno = l;
		    }
		}
	    }

	  if (start == NULL)
	    return NULL;
	  c = n;
	  l = ll;
	}
      if (start)
	{
	  end = e;
	  endno = l;
	}
      return start;
    }

    const char*
    skip_empty_line (const char* line, size_t size, unsigned int& lineno)
    {
      const char* end = line + size;
      const char* prev = line;

      while (line < end)
	{
	  unsigned int c = *line++;
	  if (!isspace (c))
	    break;
	  if (c == '\n')
	    {
	      lineno++;
	      prev = line;
	    }
	}

      return prev;
    }


    const char*
    skip_space (const char* line)
    {
      while (*line)
	{
	  if (!isspace (*line))
	    return line;
	  line++;
	}
      return NULL;
    }

    const char*
    skip_space (const char* line, size_t size, unsigned int& lineno)
    {
      const char* end = line + size;
      while (line < end)
	{
	  if (!isspace (*line))
	    return line;
	  if (*line++ == '\n')
	    lineno++;
	}
      return NULL;
    }

    const char*
    skip_space (const char* line, size_t size)
    {
      unsigned int l = 0;
      return skip_space (line, size, l);
    }

    const char*
    skip_space_and_comment (const char *x, size_t size, unsigned int& lineno)
    {
      const char* end = x + size;
      int c = 0;
      while (x < end)
	{
	  if (c == 0)
	    {
	      if (isspace (*x))
		;		// ok skip
	      else if (end - x > 1 && x[0] == '/' && x[1] == '*')
		{
		  c = 1;
		  x += 2;
		  continue;
		}
	      else
		return x;	// found any
	    }
	  else if (end - x > 1 && x[0] == '*' && x[1] == '/')
	    {
	      c = 0;
	      x += 2;
	      continue;
	    }
	  if (*x++ == '\n')
	    lineno++;
	}
      return NULL;
    }

    const char*
    find_c_block_end (const char *beg, size_t size, unsigned int& lineno)
    {
      const char* e = beg + size;
      int n = 1;
      unsigned int l = lineno;

      while (beg < e)
	{
	  if (*beg == '{')
	    n++;
	  else if (*beg == '}')
	    {
	      if (--n == 0)
		{
		  size_t s = e - beg;
		  lineno = l;
		  return nextline (beg, s, lineno);
		}
	    }
	  if (*beg++ == '\n')
	    l++;
	}
      return NULL;
    }

    const char*
    find_c_block (const char *line, size_t size, unsigned int& lineno,
		  const char*& end)
    {
      const char* c = line;
      unsigned int ln = lineno;

      c = skip_space (line, size, ln);
      if (c == NULL || *c != '{')
	return NULL;

      end = find_c_block_end (c + 1, size - (c - line) - 1, ln);
      lineno = ln;
      return line;
    }


    const char*
    skip_empty_statement (const char *line, size_t size, unsigned int& lineno)
    {
      const char* end = line + size;
      unsigned int l = lineno;
      int b1 = 0;

      while (line < end)
	{
	  if (line + 1 < end && line[0] == '/' && line[1] == '*')
	    {
	      unsigned int wl = l;
	      const char* n = skip_space_and_comment (line, end - line, wl);
	      if (n == NULL)
		break;

	      line = n;
	      l = wl;
	      continue;
	    }

	  if (*line == '\n')
	    l++;
	  else if (*line == '(')
	    b1++;
	  else if (*line == ')')
	    b1--;
	  else if (b1 == 0)
	    {
	      if (*line == '{' || *line == '}' || *line == ';')
		{
		  lineno = l;
		  return line;
		}
	    }
	  line++;
	}
      return NULL;
    }


    class c_block_beg_ent
    {
    public:
      c_block_beg_ent() {}
      c_block_beg_ent(const char* p, unsigned int l) : ptr (p), line (l) {}
      operator const char*() const { return ptr; }
      operator unsigned int () const { return line; }

      const char* ptr;
      unsigned int line;
    };

    typedef std::deque< c_block_beg_ent > c_block_beg_list;
    typedef c_block_beg_list::iterator c_block_beg_list_iterator;

    const char*
    skip_statement (const char* line, const char* head, size_t size,
		    unsigned int& lineno, c_block_beg_list& bl)
    {
      int b1 = 0;
      int b2 = 0;
      const char* end = line + size;
      const char* a = head;
      unsigned int aln = lineno;
      unsigned int ln = lineno;

      while (line < end)
	{
	  if (b1 == 0 && b2 == 0 && *line == ';')
	    {
	    next_check:
	      unsigned int wln = ln;
	      const char* w = skip_space_and_comment (line + 1,
						      end - line - 1, wln);
	      if (w && end - w > 5)
		{
		  size_t l = 0;
		  if (strncmp ("else", w, 4) == 0)
		    l = 4;
#if 0
		  // TODO: "do ; while ()"
		  else if (strncmp ("while", w, 5) == 0)
		    l = 5;
#endif

		  if (l && !isalnum (w[l]) && w[l] != '_')
		    w += l;
		  else
		    w = NULL;
		}
	      else
		w = NULL;

	      if (w == NULL)
		{
		  line++;
		  lineno = ln;
		  size_t s = end - line;
		  return skip_empty_line (line, s, lineno);
		}
	      while (line < w)
		{
		  if (*line++ == '\n')
		    {
		      ln++;
		      a = line;
		    }
		}
	      continue;
	    }
	  else if (*line == '(')
	    b1++;
	  else if (*line == ')')
	    {
	      if (b1 == 0)
		goto out;
	      b1--;
	    }
	  else if (*line == '{')
	    {
	      const char* n = line + 1;
	      size_t s = end - n;
	      unsigned int l = ln;
	      const char *e = line;
	      const char *b;
	      const char *h = head;

	      b = skip_empty_line (n, s, l);
	      if (b != n)
		{
		  s -= b - n;
		  n = b;
		  h = n;
		}

	      while (n < end)
		{
		  unsigned int lb = l;
		  b = skip_empty_statement (n, s, lb);
		  if (b)
		    {
		      s -= b - n;
		      n = b;
		      l = lb;
		    }

		  if (n >= end)
		    break;

		  lb = l;
		  b = skip_space_and_comment (n, s, lb);
		  if (b && *b == '}')
		    {
		      line = b;
		      ln = lb;
		      if (b2 == 0)
			goto next_check;
		      goto closed_brace;
		    }

		  c_block_beg_list t;
		  e = skip_statement (n, h, s, l, t);
		  if (e == NULL)
		    {
		      line = n + s;
		      n = line;
		      for (c_block_beg_list_iterator i (t.begin());
			   i != t.end(); i++)
			bl.push_back (*i);
		      break;
		    }

		  s -= e - n;
		  n = e;
		  line = n;
		  ln = l;
		}

	      bl.push_front (c_block_beg_ent (a, aln));
	      a = n;
	      aln = l;
	      b2++;
	    }
	  else if (*line == '}')
	    {
	    closed_brace:
	      if (bl.size() == 0)
		{
		  bl.push_front (c_block_beg_ent (a, aln));
		  goto out;	// error
		}
	      bl.pop_front ();
	      if (--b2 == 0)
		goto next_check;
	    }
	  if (*line++ == '\n')
	    ln++;
	}
    out:
      return NULL;		// incomplete
    }

    const char*
    skip_statement (const char* line, size_t size, unsigned int& lineno)
    {
      c_block_beg_list tmp;
      return skip_statement (line, line, size, lineno, tmp);
    }

    const char*
    skip_statement (const char* line, size_t size)
    {
      c_block_beg_list tmp;
      unsigned int l = 0;
      return skip_statement (line, line, size, l, tmp);
    }

    bool
    check_empty_code_easily (const char* beg, const char* end)
    {
      while (beg < end)
	{
	  unsigned int tl = 0;
	  beg = skip_space_and_comment (beg, end - beg, tl);
	  if (beg == NULL)
	    break;		// none
	  else if (*beg == '{' || *beg == '}' || *beg == ';')
	    beg++;		// skip brace and delimiter
	  else
	    return false;	// found any effective code
	}

      return true;
    }


    int
    parse_code_macros (Blocks& bn, const char*& line, size_t& size,
		       unsigned int& lineno)
    {
      try
	{
	  const char* start = line;
	  const char* end = line + size;

	  line = nextline (line, size, lineno); // skip comment

	  while (line < end)
	    {
	      // found next head comment
	      if (size >= 2 && line[0] == '/' && line[1] == '*')
		{
		  bn.code_macros()
		    .push_back (std::make_pair (std::string (start,
							     line - start),
						lineno));
		  break;
		}

	      line = nextline (line, size, lineno);
	    }

	  return 0;
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}

      return -1;
    }


    int
    parse_code_export (Blocks& bn, const char*& line, size_t& size,
		       unsigned int& lineno, int extvar)
    {
      const char* end = line + size;
      try
	{
	  // export variable
	  std::tr1::regex exvar ("^"
				 "[[:space:]]*"
				 "([_[:alpha:]][_[:alnum:]]*"
				 "([[:space:]]+[_[:alpha:]][_[:alnum:]]*)?"
				 ")"			// -- %1
				 "[[:space:]]+"
				 "("			// -- %3
				 "(\\*+)?"		// -- %4
				 "[[:space:]]*"
				 "([_[:alpha:]][_[:alnum:]]*)" // -- %5 name
				 "[[:space:]]*"
				 "("			// -- %6 array
				 "(\\["			// -- %7 dem
				 "[[:space:]]*"
				 "([[:digit:]]+)"	// -- %8 dem(int)
				 "[[:space:]]*"
				 "\\])+"		// -- %7
				 ")?"			// -- %6
				 ")"			// -- %3
				 "[[:space:]]*"
				 ";"
				 "[[:space:]]*"
				 "/\\*"
				 "[[:space:]]*"
				 "'"
				 "("			// -- %9 <Sys>/block
				 "(<[^>]+>)"		// -- %10 <Sys>
				 "/"
				 "([^':*/]+)"		// -- %11 block
				 ")"			// -- %9
				 "'"
				 "[[:space:]]*"
				 "\\*/");

	  std::tr1::cmatch mr;

	  while (line < end)
	    {
	      if (std::tr1::regex_search (line, end, mr, exvar))
		{
		  std::string type (mr[1]);
		  std::string ptr (mr[4]);
		  std::string var (mr[5]);
		  std::string array (mr[6]);
		  std::string sys (mr[10]);
		  std::string blk (mr[11]);
		  unsigned int sz = 0;
		  if (array.length())
		    {
		      std::istringstream ss (mr[8]);
		      ss >> sz;
		    }

		  VariableEnt v(type, var, ptr, array, sz, sys, blk,
				VariableEnt::V_VAR);

		  VariableEnt& ve = (extvar
				     ? bn.add_ext_variable_entry (var, v)
				     : bn.add_ext_state_entry (var, v));

		  // set BlockInfo into VariableEnt
		  BlockInfo* binfo = bn.find_block (sys, blk);
		  if (binfo == NULL)
		    {
		      std::cerr << "Warning: '" << sys << "/" << blk
				<< "' not found " << std::endl;
		    }
		  else
		    ve.blockinfo () = binfo;

		  while (line < mr[0].second)
		    line = nextline (line, size, lineno);
		}
	      else
		break;
	    }
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error: " << e.what() << std::endl;
	  return -1;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	  return -1;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	  return -1;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	  return -1;
	}

      return 0;
    }

    const char*
    search_next_simulink_block (const char* line, size_t size,
				const std::tr1::regex& block_mark,
				unsigned int& lineno,
				std::tr1::cmatch *n_mr = NULL,
				const std::string *end_mark = NULL,
				const std::string *sys = NULL,
				const std::string *blk = NULL)
    {
      const char* end = line + size;
      std::tr1::cmatch tmr;
      unsigned int ln = lineno;

      std::tr1::cmatch& mr = n_mr ? *n_mr : tmr;

      while (line < end)
	{
	  if (*line == '}')
	    return line;

	  size_t s = size;
	  unsigned int l = ln;
	  const char* n = nextline (line, s, l);
	  const char* e;
	  unsigned int lc = ln;
	  unsigned int le = ln;
	  const char* c = find_comment (line, size, e, lc, le);
	  if (c)
	    {
	      if (std::tr1::regex_search (c, e, mr, block_mark))
		{
		  std::string pre (mr[1]);
		  if (end_mark == NULL
		      || *end_mark == ""
		      || (pre == *end_mark && mr[4] == *sys && mr[5] == *blk))
		    {
		      lineno = lc;
		      return c;
		    }
		}
	      line = e;
	      size -= e - line;
	      ln = le;
	    }
	  else
	    {
	      line = n;
	      size = s;
	      ln = l;
	    }
	}

      lineno = ln;
      return end;
    }

    typedef std::vector < std::string* > subblock_list;
    typedef subblock_list::iterator subblock_list_iterator;
    typedef subblock_list::const_iterator subblock_list_const_iterator;

    class subsystem_ent
    {
    public:
      subsystem_ent () {}
      subsystem_ent (const std::string& n,
		     const std::string& s, const std::string& b)
	: name (n), sys (s), blk (b) {}

      void add_code (std::string& s) const;

      subblock_list subblock;

      const std::string name;
      const std::string sys;
      const std::string blk;
    };

    void
    subsystem_ent::add_code (std::string& s) const
    {
      for (subblock_list_const_iterator si (subblock.begin ());
	   si != subblock.end (); si++)
	{
	  std::string* x = *si;
	  *x += s;
	}
    }

    typedef std::deque < subsystem_ent > subsystem_stack;
    typedef subsystem_stack::iterator subsystem_stack_iterator;

    typedef std::deque < std::string* > text_list;
    typedef text_list::iterator text_list_iterator;

    class c_block_info
    {
    public:
      c_block_info ()
	: id (seq++), beg (NULL), end (NULL), block (NULL),
	  brace (0), line (0), text(NULL), is_subsys (false), is_loop(false)
      {
	gen_mark();
      }

      c_block_info (const char* b, const char* e, BlockInfo* bi,
		    int f, unsigned int l, std::string* c, bool s,
		    bool lp)
	: id (seq++), beg (b), end (e), str (beg, end - beg), block (bi),
	  brace (f), line (l), text (c), is_subsys (s), is_loop (lp)
      {
	id = seq++;
	gen_mark ();
      }

      void add_close_list (std::string* c);
      void gen_mark (void);

      static int seq;

      int id;
      const char* beg;
      const char* end;
      std::string str;
      BlockInfo* block;		// enabled or triggerd SubSystem
      int brace;
      unsigned int line;
      std::string* text;
      text_list close_list;
      bool is_subsys;
      bool is_loop;
      std::string mark;
    };

    int c_block_info::seq = 0;

    void
    c_block_info::add_close_list (std::string* c)
    {
      text_list_iterator i = std::find (close_list.begin (),
					close_list.end (), c);
      if (i == close_list.end ())
	close_list.push_back (c);
    }

    void
    c_block_info::gen_mark (void)
    {
      std::ostringstream oid;
      oid << id << " ";
      mark = oid.str();
      if (block)
	mark += (block->tag()->name()
		 + " '" + block->system() + "/" + block->block() + "'");
      else
	mark += "none";
    }

    typedef std::vector< c_block_info > c_block_stack;
    typedef c_block_stack::iterator c_block_stack_iterator;

    // merge lower blocks to their SubSystem
    static void
    merge_lower (Blocks& bn, BlockInfo* top, BlockInfo* binfo)
    {
      const block_T* b = binfo->tag ();

      for (block_T::blocks_const_iterator bsi (b->blocks ().begin ());
	   bsi != b->blocks ().end (); bsi++)
	{
	  const blocks_T& bs = *bsi;
	  for (blocks_T::block_const_iterator bi (bs.block ().begin ());
	       bi != bs.block ().end (); bi++)
	    {
	      const block_T& lb = *bi;
	      BlockInfo* lbinfo = bn.find_block (lb.name ());
	      assert (lbinfo);
	      merge_lower (bn, top, lbinfo);
	      top->merge_blocks ().addinfo (lbinfo);
	      lbinfo->merged_to ().addinfo (top);
	    }
	}
      // not add binfo
    }


    // depth first search, and merge blocks to SwitchCase/If
    static bool
    dfs_switchcase_if (Blocks& bn, BlockInfo* tail,
		       BlockInfo* current, int *color)
    {
      int index = current->id ();
      const block_T* b = current->tag ();
      bool reached = false;

      color[index] = -1;

      for (block_T::output_const_iterator oi (b->output ().begin ());
	   oi != b->output ().end (); oi++)
	{
	  const ioport_T& out = *oi;
	  for (ioport_T::connect_const_iterator ci (out.connect ().begin ());
	       ci != out.connect ().end (); ci++)
	    {
	      const connection_T& c = *ci;
	      BlockInfo* cbinfo = bn.find_block (c.block ());

	      assert (cbinfo);

	      const block_T* cb = cbinfo->tag ();
	      int cindex = cbinfo->id ();

	      // found `Merge'
	      if (cbinfo->upper () == tail->upper ()
		  && cb->blocktype () == "Merge")
		{
		  if (color[cindex] == 0)
		    {
		      color[cindex] = 1;
		      tail->merge_blocks ().addinfo (cbinfo);
		      cbinfo->merged_to ().addinfo (tail);
		    }
		  reached = true;
		  continue;
		}

	      // already merged, or connected-block reach to `Merge'
	      if (cbinfo->merged_to ().isexist (tail)
		  || (color[cindex] == 0
		      && dfs_switchcase_if (bn, tail, cbinfo, color)))
		reached = true;
	    }
	}

      color[index] = 1;

      if (reached && current != tail)
	{
	  if (b->blocktype () == "SubSystem")
	    merge_lower (bn, tail, current);
	  tail->merge_blocks ().addinfo (current);
	  current->merged_to ().addinfo (tail);
	}

      return reached;
    }


    // merge blocks to tail(e.g. SwitchCase/If)
    static void
    dfs_switchcase_if (Blocks& bn, BlockInfo* tail)
    {
      std::vector < int > color (bn.block_map ().size () + 1, 0);
      (void)dfs_switchcase_if (bn, tail, tail, color.data ());
    }

    unsigned int parse_options = 0;

    bool no_cut_triggered_subsystem (void)
    {
      if (parse_options & PARSE_OPTION_CUT_TRIGGERED_SUBSYSTEM)
	return false;
      return true;
    }

    bool no_cut_atomic_subsystem (void)
    {
      if (parse_options & PARSE_OPTION_CUT_ATOMIC_SUBSYSTEM)
	return false;
      return true;
    }

    static void
    handle_incorporates_block (Blocks& bn, const std::string& sys,
			       const char* i_beg, const char* com_end,
			       unsigned int lineno,
			       BlockInfo* current_block,
			       std::tr1::regex& block_mark,
			       std::tr1::regex& ccatpat)
    {
      while (i_beg < com_end)
	{
	  size_t i_n = com_end - i_beg;
	  i_beg = nextline (i_beg, i_n);

	  std::tr1::cmatch i_mr;
	  if (std::tr1::regex_search (i_beg, com_end, i_mr, block_mark) == 0)
	    break;

	  // ignore following:
	  //   external Inport/Outport
	  //   Enabled/Triggerd Subsystem
	  //   SwitchCase/If, and Iterator SubSystem
	  //   Goto/From/Mux/Demux
	  //   Merge
	  //   and sink block
	  if (i_mr[2] == "Inport"
	      || i_mr[2] == "Outport"
	      || i_mr[2] == "EnablePort"
	      || i_mr[2] == "TriggerPort"
	      || i_mr[2] == "ActionPort"
	      || i_mr[2] == "WhileIterator"
	      || i_mr[2] == "ForIterator"
	      || i_mr[2] == "Goto"
	      || i_mr[2] == "From"
	      || i_mr[2] == "Mux"
	      || i_mr[2] == "Demux"
	      || i_mr[2] == "Merge"
	      || i_mr[2] == "Scope"
	      || i_mr[2] == "ToWorkspace"
	      || i_mr[2] == "FloatingScope"
	      || i_mr[2] == "Terminator"
	      || i_mr[2] == "Display"
	      || i_mr[2] == "ToFile"
	      || i_mr[2] == "BusCreator"
	      || i_mr[2] == "BusSelector")
	    continue;

	  std::string i_sys = i_mr[4];
	  std::string i_blk = i_mr[5];

	  BlockInfo* i_binfo = bn.find_block (i_sys, i_blk);
	  if (i_binfo == NULL)
	    {
	      // try check Concatenate
	      if (i_mr[2] == "SignalConversion")
		{
		  std::tr1::smatch ccatmr;
		  if (regex_search (i_blk, ccatmr, ccatpat))
		    {
		      std::string ccatblk = ccatmr[1];
		      i_binfo = bn.find_block (sys, ccatblk);
		    }
		}
	    }

	  if (i_binfo == NULL)
	    {
	      std::cerr << "Warning:"
			<< lineno
			<< ": unknown incorporated Block "
			<< i_sys << '/' << i_blk
			<< std::endl;
	      continue;
	    }

	  // ignore myself
	  if (i_binfo == current_block)
	    continue;

	  current_block->merge_blocks ().addinfo (i_binfo);
	  i_binfo->merged_to ().addinfo (current_block);
	}
    }


    int
    parse_code_step_func (Blocks& bn, const char*& line, size_t& size,
			  unsigned int& lineno)
    {
      try
	{
	  // block mark
	  std::tr1::regex block_mark ("^"
				      "[[:space:]]*"
				      "/?"
				      "\\*"
				      "[[:space:]]+"
				      "([^:]+[[:space:]]+)?"	// -- %1
				      "([^:[:space:]]+)"	// -- %2
				      ":"
				      "[[:space:]]+"
				      "'?"
				      "((<[^>]+>)/([^':*/]+))" // -- %3(%4/%5)
				      "'?"
				      "[[:space:]]*"
				      "("
				      "(incorporates:)"		// -- %7
				      "|"
				      "\\*/?"
				      ")?"
				      ".*$");

	  // for loop
	  std::tr1::regex for_loop ("^"
				    "[[:space:]]*"
				    "for[[:space:]]*\\("
				    "[[:space:]]*"
				    "([_[:alpha:]][_[:alnum:]]*)"
				    "[[:space:]]*"
				    "="
				    "[[:space:]]*"
				    "([[:digit:]]+)"
				    "[[:space:]]*"
				    ";"
				    "[[:space:]]*"
				    "([_[:alpha:]][_[:alnum:]]*)"
				    "[[:space:]]*"
				    "(<|>|<=|>=|==|!=)"
				    "[[:space:]]*"
				    "([[:digit:]]+)"
				    "[[:space:]]*"
				    ";");

	  // TaskCounters (different rate)
	  std::tr1::regex if_tid ("^"
				  "[[:space:]]*"
				  "if[[:space:]]*\\("
				  "[[:space:]]*"
				  + bn.rtm_object_pointer ()
				  +
				  "[[:space:]]*"
				  "->"
				  "Timing"
				  "\\."
				  "TaskCounters"
				  "\\."
				  "TID"
				  "\\["
				  "[[:digit:]]+"
				  "\\]"
				  "[[:space:]]*"
				  "(<|>|<=|>=|==|!=)"
				  "[[:space:]]*"
				  "[[:digit:]]+"
				  "[[:space:]]*"
				  "\\)"
				  "[[:space:]]*"
				  "\\{");

	  std::tr1::regex ccatpat ("^ConcatBufferAt(.*)In([[:digit:]]+)$");

	  std::tr1::regex tscatpat ("^TmpSignal\\s+ConversionAt"
				    "(.*)Inport([[:digit:]]+)$");

	  std::tr1::regex call_rs_pat ("^"
				       "[[:space:]]*"
				       "rate_scheduler"
				       "[[:space:]]*"
				       "\\("
				       "[[:space:]]*"
				       "\\)"
				       "[[:space:]]*"
				       ";");

	  std::tr1::cmatch mr;

	  const char* end = line + size;
	  c_block_stack nesting;

	  std::string& cname = bn.source_filename ();
	  if (cname.length () == 0)
	    cname = "c-none";

	  enum com_type {
	    com_type_none,
	    com_type_output,
	    com_type_update,
	    com_type_end,
	    com_type_end_update,
	    com_type_sfunc,
	    com_type_sfunc_update,
	    com_type_matlab,
	    com_type_refer,
	    com_type_enable,
	    com_type_trigger,
	    com_type_init,
	    com_type_base_rate,
	    com_type_iterator,
	    com_type_atomic,
	    com_type_chart,
	    com_type_func_call,
	    com_type_atomic_subsys_update,
	    com_type_other_subsys_update,
	    com_type_other
	  } com_type = com_type_none;

	  enum code_mode {
	    code_mode_step,
	    code_mode_rtm,
	    code_mode_other
	  } code_mode = code_mode_step;

	  // skip for function body
	  while (line < end)
	    {
	      int c = *line;
	      line = nextline (line, size, lineno);
	      if (c == '{')
		break;
	    }

	  if (line >= end)
	    return 0;		// none

	  const char* c_block_beg = line;
	  const char* c_block_end = NULL;
	  unsigned int c_block_line;

	  const char* next_block = NULL;
	  unsigned int next_block_line;

	  BlockInfo* current_block = NULL;
	  std::string* current_text = NULL;
	  int current_text_begin = -1;

	  unsigned int* current_line = NULL;
	  std::string* current_file = NULL;

	  subsystem_stack subsys_stack;
	  subsystem_ent* current_subsystem = NULL;
	  bool is_subsystem = false;

	  int block_count = 0;
	  bool block_changed = false;

	  const char* rtm_beg = NULL;
	  const char* rtm_end = NULL;
	  unsigned int rtm_line = 0;

	  bool skip_to_end = false;
	  std::string skip_to_end_sys;
	  std::string skip_to_end_blk;

	  int loop_in_block = 0;

	  enum { m_NONE, m_DotProduct, m_ToEndBlock } merged_type = m_NONE;
	  BlockInfo* merged_block = NULL;
	  BlockInfo* merge_to_block = NULL;

	  bool done_autovars = false;

	  bool in_chart = false;
	  BlockInfo* chart_block = NULL;

	  bool is_ratetransition = false;

	  while (line < end)
	    {
	      if (*line == '}')	// end step function
		{
		  // skip this line
		  line = nextline (line, size, lineno);
		  break;
		}

	      size_t s = size;
	      unsigned int l = lineno;
	      const char* n = nextline (line, s, l);

	      if (code_mode == code_mode_rtm)
		{
		  rtm_end = n;
		  line = n;
		  size = s;
		  lineno = l;
		  continue;
		}

	      const char* first_char = skip_space (line, n - line);

	      // continue if blank line
	      if (first_char == NULL)
		{
		  line = n;
		  size = s;
		  lineno = l;
		  continue;
		}

	      const char* com_beg;
	      const char* com_end = NULL;
	      unsigned int com_blineno = lineno;
	      unsigned int com_elineno = lineno;

	      std::tr1::cmatch rs_mr;

	      com_beg = find_comment (line, size, com_end,
				      com_blineno, com_elineno);
	      if (com_beg)
		{
#if PARSE_CODE_DEBUG > 0
		  std::string x(com_beg, com_end - com_beg);
		  std::cout << "<<<<<<<< COMMENT at " << com_blineno << "\n"
			    << x
			    << ">>>>>>>>"
			    << std::endl;
#endif

		  BlockInfo* comment_block = NULL;

		  if (std::tr1::regex_search (com_beg, com_end,
					      mr, block_mark) == 0)
		    {
		      if (com_end - com_beg >= 42
			  && strncmp ("  /* Update absolute "
				      "time for base rate */",
				      com_beg, 42) == 0)
			{
			  code_mode = code_mode_rtm;
			  rtm_beg = line;
			  rtm_line = lineno;
			  continue;
			}

		      if (current_text)
			*current_text += std::string (com_beg,
						      com_end - com_beg);

		      // ignore others
		      line = n;
		      size = s;
		      lineno = l;
		      continue;
		    }

		  block_changed = false;
		  block_count++;

		  com_type = com_type_none;

		  std::string sys = mr[4];
		  std::string blk = mr[5];

		  if (mr[1].matched)
		    {
		      std::string pre(mr[1]); // update/end ...

		      if (pre.compare (0, 18, "Update for Atomic ") == 0
			  && mr[2] == "SubSystem")
			com_type = com_type_atomic_subsys_update;
		      else if (pre.compare (0, 6, "Update") == 0)
			com_type = com_type_update;
		      else if (pre.compare (0, 18, "End of Update for ") == 0)
			com_type = com_type_end_update;
		      else if (pre.compare (0, 3, "End") == 0
			       || pre.compare (0, 3, "end") == 0)
			com_type = com_type_end;
		      else if (pre.compare (0, 20,
					    "Outputs for Enabled ") == 0)
			com_type = com_type_enable;
		      else if (pre.compare (0, 22,
					    "Outputs for Triggered ") == 0)
			com_type = com_type_trigger;
		      else if (pre.compare (0, 21,
					    "Outputs for Iterator ") == 0)
			com_type = com_type_iterator;
		      else if (pre.compare (0, 19,
					    "Outputs for Atomic ") == 0)
			com_type = com_type_atomic;
		      else if (pre.compare (0, 26,
					    "Outputs for Function Call ") == 0)
			com_type = com_type_func_call;
		      else if (pre.compare (0, 7, "Outputs") == 0)
			com_type = com_type_output;
		      else if (pre.compare (0, 10, "S-Function") == 0)
			{
			  if (mr[2] == "Block")
			    com_type = com_type_sfunc_update;
			  else
			    com_type = com_type_sfunc;
			}
		      else if (pre.compare (0, 15,
					    "Embedded MATLAB") == 0)
			com_type = com_type_matlab;
		      else if (pre.compare (0, 7,
					    "MATLAB ") == 0)
			com_type = com_type_matlab;
		      else if (pre.compare (0, 10, "Referenced") == 0)
			com_type = com_type_refer;
		      else if (pre.compare (0, 25,
					    "InitializeConditions for ") == 0
			       || pre.compare (0, 11, "Enable for ") == 0
			       || pre.compare (0, 24,
					       "VirtualOutportStart for ") == 0)
			com_type = com_type_init;
		      else if (pre.compare (0, 6, "Truth ") == 0)
			com_type = com_type_none; // sateflow: truth table
		      else if (pre.compare (0, 22,
					    "Dynamic Look-Up Table ") == 0)
			com_type = com_type_none; // lookup table
		      else
			{
			  std::cout << "Warning: Unknown word("
				    << pre
				    << ") in step function"
				    << std::endl;
			  com_type = com_type_other;
			}
		    }

		  // ignore
		  if (com_type == com_type_refer)
		    {
		      line = n;
		      size = s;
		      lineno = l;
		      continue;
		    }

		  if (mr[2].matched)
		    is_subsystem = (mr[2] == "SubSystem");
		  else
		    is_subsystem = false;

		  if (is_subsystem)
		    {
		      BlockInfo* tmp = bn.find_block (sys, blk);
		      if (tmp)
			{
			  if (tmp->type () == BlockInfo::TYPE_ATOMIC_SUBSYSTEM
			      && com_type == com_type_none)
			    com_type = com_type_atomic;
			  else if (com_type == com_type_update)
			    com_type = com_type_other_subsys_update;
			}
		      else
			std::cerr << "Warning: '" << sys << "/" << blk
				  << "' not found " << std::endl;
		    }

		  // Chart parser state
		  if (com_type == com_type_none
		      && mr[2].matched
		      && (mr[2] == "Chart"
			  || (mr[1].matched && mr[1] == "Truth "
			      && mr[2] == "Table")))
		    com_type = com_type_chart;

		  if (in_chart)
		    {
		      if ((com_type == com_type_end
			   || com_type == com_type_end_update)
			  && chart_block->system () == sys
			  && chart_block->block () == blk)
			{
			  // End of Chart
			  in_chart = false;
			  chart_block = NULL;
			}
		      else if (com_type == com_type_func_call)
			{
			  BlockInfo* tmp = bn.find_block (sys, blk);
			  if (tmp != NULL
			      && !tmp->merged_to ().isexist (chart_block))
			    {
			      // merge called SubSystem
			      merge_lower (bn, chart_block, tmp);
			      chart_block->merge_blocks ().addinfo (tmp);
			      tmp->merged_to ().addinfo (chart_block);
			    }
			  sys = chart_block->system ();
			  blk = chart_block->block ();
			  is_subsystem = false;
			  com_type = com_type_none;
			}
		      else
			{
			  BlockInfo* tmp = bn.find_block (sys, blk);
			  if (tmp == NULL
			      || tmp->merged_to ().isexist (chart_block))
			    {
			      // block of Chart SubSystem
			      com_type = com_type_none;
			      sys = chart_block->system ();
			      blk = chart_block->block ();
			    }
			  else
			    {
			      // block is not in Stateflow SubSystem
			      in_chart = false;
			      chart_block = NULL;
			    }
			}
		    }

		  bool found_skip_block = false;

		  if (skip_to_end)
		    {
		      if ((com_type == com_type_end
			   || com_type == com_type_end_update)
			  && skip_to_end_sys == sys
			  && skip_to_end_blk == blk)
			skip_to_end = false;
		      else
			{
			  // handle `incorporates:'
			  if (mr[7].matched)
			    handle_incorporates_block (bn, sys,
						       mr[7].second, com_end,
						       lineno,
						       current_block,
						       block_mark,
						       ccatpat);

			  com_type = com_type_none; // normal
			}
		    }
		  else if ((mr[2].matched
			    && (mr[2] == "If"
				|| mr[2] == "SwitchCase"
				|| mr[2] == "Switch"
				|| mr[2] == "MultiPortSwitch"))
			   || com_type == com_type_iterator
			   || (no_cut_atomic_subsystem ()
			       && (com_type == com_type_atomic
				   || com_type == com_type_atomic_subsys_update))
			   || (no_cut_triggered_subsystem ()
			       && (com_type == com_type_trigger
				   || com_type == com_type_enable
				   || com_type == com_type_other_subsys_update)))
		    {
		      found_skip_block = true;
		      skip_to_end = true;
		      skip_to_end_sys = sys;
		      skip_to_end_blk = blk;
		    }
		  else if (mr[2].matched)
		    {
		      if (mr[2] == "RateTransition")
			is_ratetransition = true;
		      else
			is_ratetransition = false;
		    }

		  if (c_block_end < c_block_beg)
		    c_block_end = line;

		  int need_open = 1;
		  bool separate_code = false;
		  size_t input_index = 0;

		  if (found_skip_block == false && skip_to_end == true)
		    {
		      if (current_text)
			goto add_to_block_code;
		      else
			goto skip_code; // fail-safe, block not existing
		    }

		  if (skip_to_end == false
		      && (com_type == com_type_end
			  || com_type == com_type_end_update))
		    {
		      need_open = 0;

		      comment_block = bn.find_block (sys, blk);
		      if (comment_block == NULL)
			std::cerr << "Warning: unknown End block "
				  << sys << "/" << blk << std::endl;

		      if (current_block != comment_block
			  || current_text == NULL)
			{
			  if (merged_type == m_DotProduct)
			    {
			      if (current_block)
				merge_to_block = current_block;
			      else
				{
				  // previous block
				  merge_to_block = NULL;
				  const block_T* tag = merged_block->tag ();
				  for (block_T::input_const_iterator
					 in (tag->input ().begin ());
				       in != tag->input ().end (); in++)
				    {
				      BlockInfo* xi = bn.previous_block (*in);
				      if (merge_to_block == NULL)
					merge_to_block = xi;
				      else if (merge_to_block != xi)
					{
					  const block_T*
					    d1 = merge_to_block->tag ();
					  const block_T* d2 = xi->tag ();
					  std::cerr << "Error: "
						    << "Merged DotProduct "
						    << "has different inputs:"
						    << std::endl
						    << "  "
						    << tag->name ()
						    << std::endl
						    << "    "
						    << d1->blocktype ()
						    << ':'
						    << d1->name ()
						    << std::endl
						    << "    "
						    << d2->blocktype ()
						    << ':'
						    << d2->name ()
						    << std::endl;
					}
				    }

				  if (merge_to_block == NULL)
				    {
				      std::cerr << "Error: "
						<< "Merged DotProduct "
						<< "has no inputs:"
						<< std::endl
						<< "  "
						<< tag->name ()
						<< std::endl;
				      merged_block = NULL;
				      merged_type = m_NONE;
				    }
				}

			      std::cout << "Warning:"
					<< com_blineno
					<< ": DotProdct "
					<< merged_block->system ()
					<< "/"
					<< merged_block->block ()
					<< " is marged to "
					<< merge_to_block->system ()
					<< "/"
					<< merge_to_block->block ()
					<< std::endl;
			      merged_block->merged_to ()
				.addinfo (merge_to_block);
			      merge_to_block->merge_blocks ()
				.addinfo (merged_block);
			    }
			  else if (current_block == NULL)
			    std::cout << "Warning:"
				      << com_blineno
				      << ": Current block is unknown"
				      << " switch to End block "
				      << sys << "/" << blk << std::endl;
			  else
			    std::cout << "Warning:"
				      << com_blineno
				      << ": Current block "
				      << current_block->system ()
				      << "/"
				      << current_block->block ()
				      << " is not End block "
				      << sys
				      << "/"
				      << blk
				      << std::endl;

			  if (comment_block)
			    {
			      if (com_type == com_type_end_update)
				current_text = &comment_block->update();
			      else
				current_text = &comment_block->code();
			    }
			}
		      else
			{
			  if (merged_type == m_ToEndBlock
			      && current_text_begin != -1)
			    {
			      const char* bcode = current_text->c_str ();
			      const char *bend = (bcode
						  + current_text->length ());
			      std::tr1::cmatch cmr;

			      bcode += current_text_begin;
			      while (std::tr1::regex_search (bcode, cmr,
							     block_mark))
				{
				  std::string msys = cmr[4];
				  std::string mblk = cmr[5];

				  BlockInfo* x = bn.find_block (msys, mblk);
				  if (x)
				    {
				      x->merged_to ()
					.addinfo (current_block);
				      current_block->merge_blocks ()
					.addinfo (x);
				      if (x->merged_to().size () > 1)
					std::cout << "Warning:"
						  << com_blineno
						  << ": "
						  << sys
						  << '/'
						  << blk
						  << " merge "
						  << msys
						  << '/'
						  << mblk
						  << ", but "
						  << "it is already merged"
						  << std::endl;
				    }
				  else
				    std::cerr << "Warning: unknown block "
					      << sys << '/' << blk
					      << std::endl;

				  bcode = cmr[0].second;
				  size_t xn = bend - bcode;
				  bcode = nextline (bcode, xn);
				}
			    }
			}

		      if (merged_type != m_NONE
			  && merged_block == comment_block)
			{
			  merged_block = NULL;
			  merged_type = m_NONE;
			  merge_to_block = NULL;
			}

		      if (current_text)
			{
#if PARSE_CODE_DEBUG > 0
			  std::ostringstream o;
			  o << "/*### " << __LINE__ << " ###*/\n";
			  *current_text += o.str();
#endif	// PARSE_CODE_DEBUG
			  const std::string x (line, com_end - line);
			  *current_text += x;
			}

		      if (is_subsystem)
			{
			  if (current_subsystem == NULL)
			    {
			      std::cerr << "Error:"
					<< com_blineno
					<< ": Not in Any SubSystem"
					<< std::endl;
			    }
			  else
			    {
			      const std::string& s = current_subsystem->sys;
			      const std::string& b = current_subsystem->blk;

			      if (s != sys || b != blk)
				{
				  std::cerr << "Error:"
					    << com_blineno
					    << " End for "
					    << sys << "/" << blk
					    << " but not "
					    << s << "/" << b
					    << std::endl;
				}

			      subsys_stack.pop_back();
			      if (subsys_stack.size () > 0)
				current_subsystem = &subsys_stack.back();
			      else
				current_subsystem = NULL;
			      is_subsystem = false;
			    }
			}
		    }

		  current_text = NULL;
		  current_block = NULL;

		  if (need_open == 0)
		    {
		      size -= com_end - line;
		      line = com_end;
		      lineno = com_elineno;
		      continue;
		    }

#if PARSE_CODE_DEBUG > 0
		  std::cout << "change block: "
			    << sys << "/" << blk << std::endl;
#endif	// PARSE_CODE_DEBUG

		  current_block = bn.find_block (sys, blk);
		  if (current_block == NULL)
		    {
		      // TOD: check Concatenate block
		      if (mr[2] == "SignalConversion")
			{
			  std::tr1::smatch ccatmr;
			  if (regex_search (blk, ccatmr, ccatpat)
			      || regex_search (blk, ccatmr, tscatpat))
			    {
			      std::string ccatblk = ccatmr[1];

			      current_block = bn.find_block (sys, ccatblk);
			      if (current_block)
				{
				  separate_code = true;
				  std::istringstream ss (ccatmr[2]);
				  ss >> input_index;
				}
			    }
			}
		    }

		  if (current_block == NULL)
		    {
		      std::string x(com_beg, com_end - com_beg);
		      std::cerr << "Warning:"
				<< lineno
				<< ": Unknown block\n"
				<< "<<<<<<<<\n"
				<< x
				<< ">>>>>>>>" << std::endl;

		      sys.clear();
		      blk.clear();

#if PARSE_CODE_DEBUG > 0
		      // not found
		      std::cout << "--------" << std::endl;
		      std::cout << "[1]=" << mr[1]
				<< ", [3]=" << mr[3] << ", bn=***"
				<< ", [2]=" << mr[2]
				<< ", [4]=" << mr[4] << std::endl;
#endif	// PARSE_CODE_DEBUG
		      size -= com_end - line;
		      line = com_end;
		      lineno = com_elineno;
		      continue;
		    }

		  // handle `incorporates:'
		  if (mr[7].matched)
		    handle_incorporates_block (bn, sys,
					       mr[7].second, com_end,
					       lineno,
					       current_block,
					       block_mark,
					       ccatpat);

		  // Iterator SubSystem and Stateflow(SubSystem)
		  // merge lower blocks
		  if (com_type == com_type_iterator
		      || (no_cut_atomic_subsystem ()
			  && com_type == com_type_atomic)
		      || (no_cut_triggered_subsystem ()
			  && (com_type == com_type_trigger
			      || com_type == com_type_enable))
		      || (in_chart == false
			  && com_type == com_type_chart))
		    merge_lower (bn, current_block, current_block);

		  // start Chart Now!!
		  if (in_chart == false && com_type == com_type_chart)
		    {
		      in_chart = true;
		      chart_block = current_block;
		      current_block->type () = BlockInfo::TYPE_STATEFLOW_SUBSYSTEM;
		    }

		  // merge from SwitchCase/If to Merge block
		  if (mr[2].matched
		      && (mr[2] == "SwitchCase" || mr[2] == "If"))
		    dfs_switchcase_if (bn, current_block);

		  // DotProduct maybe merged
		  if (com_type == com_type_none
		      && mr[2].matched && mr[2] == "DotProduct")
		    {
		      merged_block = current_block;
		      merged_type = m_DotProduct;
		    }

		  // a block like Switch maybe merged
		  if (com_type == com_type_none
		      && mr[2].matched
		      && (mr[2] == "Switch" || mr[2] == "MultiPortSwitch"))
		    {
		      merged_block = current_block;
		      merged_type = m_ToEndBlock;
		      current_text_begin = -1;
		    }

		  if (com_type == com_type_update
		      || com_type == com_type_end_update
		      || com_type == com_type_sfunc_update
		      || com_type == com_type_atomic_subsys_update
		      || com_type == com_type_other_subsys_update)
		    {
		      current_text = &current_block->update();
		      current_line = &current_block->update_line();
		      current_file = &current_block->update_file();
		    }
		  else if (separate_code)
		    {
		      current_block->frag () = true;

		      code_frag_T&
			f = current_block->codefrags ().create (input_index);
		      current_text = &f.frag ();
		      current_line = &f.line ();
		      current_file = &current_block->code_file();
		    }
		  else
		    {
		      current_text = &current_block->code();
		      current_line = &current_block->code_line();
		      current_file = &current_block->code_file();
		    }

		  if (is_subsystem
		      && (com_type == com_type_enable
			  || com_type == com_type_trigger
			  || com_type == com_type_output
			  || com_type == com_type_iterator
			  || com_type == com_type_atomic
			  || com_type == com_type_atomic_subsys_update
			  || com_type == com_type_other_subsys_update))
		    {
		      if (com_type == com_type_atomic
			  && current_block->type () == BlockInfo::TYPE_NORMAL)
			current_block->type () = BlockInfo::TYPE_ATOMIC_SUBSYSTEM; // XXX

		      std::string& bname = current_block->tag ()->name ();
		      subsys_stack.push_back (subsystem_ent (bname, sys, blk));
		      current_subsystem = &subsys_stack.back();
		    }
		  else
		    {
		      if (current_subsystem)
			{
			  int found = 0;
			  subblock_list& sbl = current_subsystem->subblock;
			  for (subblock_list_iterator si (sbl.begin());
			       si != sbl.end(); si++)
			    {
			      std::string* x = *si;
			      if (x == current_text)
				{
				  found = 1;
				  break;
				}
			    }
			  if (found == 0)
			    sbl.push_back (current_text);
			}
		    }

		  // save auto variables in step-function
		  if (done_autovars == false)
		    {
		      if (c_block_beg < c_block_end)
			{
			  std::string& p = bn.step_func_auto_vars ();
#if PARSE_CODE_DEBUG > 0
			  std::ostringstream o;
			  o << "/*### " << __LINE__ << " ###*/\n";
			  p += o.str();
#endif	// PARSE_CODE_DEBUG
			  p += std::string (c_block_beg,
					    c_block_end - c_block_beg);
			}
		      done_autovars = true;

		      c_block_end = line;
		      c_block_beg = line;
		      c_block_line = lineno;
		    }

		  if (current_text->length () == 0)
		    {
		      for (c_block_stack_iterator bi (nesting.begin ());
			   bi != nesting.end(); bi++)
			{
			  c_block_info& x = *bi;

#if PARSE_CODE_DEBUG > 0
			  std::ostringstream o;
			  o << "/*### " << __LINE__ << " ###*/\n";
			  *current_text += o.str();
#endif	// PARSE_CODE_DEBUG
			  *current_text += "/*<<< S " + x.mark + " <<<*/\n";
			  *current_text += x.str;
			  *current_text += "/*>>> S " + x.mark + " >>>*/\n";
			  if (x.brace)
			    x.add_close_list (current_text);
			}
		    }

		add_to_block_code:

#if PARSE_CODE_DEBUG > 0
		  {
		    std::ostringstream o;
		    o << "/*### " << __LINE__ << " ###*/\n";
		    *current_text += o.str();
		  }
#endif	// PARSE_CODE_DEBUG

		  if (in_chart == false || *current_line == 0)
		    *current_line = com_blineno;
		  *current_file = cname;
		  *current_text += std::string (com_beg,
						com_end - com_beg);
		  if (current_text_begin == -1)
		    current_text_begin = current_text->length ();

		skip_code:
		  size -= com_end - line;
		  line = com_end;
		  lineno = com_elineno;
		}
	      else if (std::tr1::regex_search (line, n, rs_mr, call_rs_pat))
		{
		  code_mode = code_mode_rtm;
		  rtm_beg = line;
		  rtm_line = lineno;
		  continue;
		}
	      else if (skip_to_end)
		{
		  const char* x = skip_statement (line, end - line, lineno);

		  if (current_text)
		    *current_text += std::string (line, x - line);
		  size -= x - line;
		  line = x;
		}
	      else
		{
		  int else_block = 0;
		  const char* x = skip_space (line, n - line);
		  if (x)
		    {
		      // c-block end unless `} else {'
		      if (*x == '}')
			{
			  const char* w;
			  unsigned int wl = lineno;
			  w = skip_space_and_comment (x + 1,
						      end - x - 1, wl);
			  if (w && end - x > 4
			      && strncmp ("else", w, 4) == 0)
			    {
			      w = skip_space (w + 4, end - w - 4);
			      if (w && *w == '{')
				{
				  if (w > n)
				    {
				      size_t x = end - w;
				      n = nextline (w, x);
				      s = x;
				    }
				  std::string a (line, n - line);
				  c_block_info& b = nesting.back();
				  b.str += a;
				  for (text_list_iterator
					 ti (b.close_list.begin());
				       ti != b.close_list.end(); ti++)
				    {
				      std::string* t = *ti;
				      *t += "/*<<< E " + b.mark + " <<<*/\n";
				      *t += a;
				      *t += "/*>>> E " + b.mark + " >>>*/\n";
				    }
#if PARSE_CODE_DEBUG > 0
				  std::cout << "* " << __LINE__
					    << " add nesting:\n<<<\n"
					    << nesting.back().str
					    << ">>>" << std::endl;
#endif	// PARSE_CODE_DEBUG
				  else_block = 1;
				}
			    }

			  if (else_block == 0)
			    {
			      if (nesting.size() == 0)
				{
				  std::cerr << "Error: empty block at "
					    << lineno << std::endl;
				}
			      else
				{
				  c_block_info& b = nesting.back();
				  if (b.brace)
				    {
				      std::string a (line, n - line);
#if PARSE_CODE_DEBUG > 0
				      std::ostringstream o;
				      o << "/*### " << __LINE__ << " ###*/\n";
#endif	// PARSE_CODE_DEBUG
				      for (text_list_iterator
					     ti (b.close_list.begin());
					   ti != b.close_list.end(); ti++)
					{
					  std::string* ct = *ti;
					  if (ct)
					    {
#if PARSE_CODE_DEBUG > 0
					      *ct += o.str();
#endif	// PARSE_CODE_DEBUG
					      *ct += ("/*<<< E "
						      + b.mark + " <<<*/\n");
					      *ct += a;
					      *ct += ("/*>>> E "
						      + b.mark + " >>>*/\n");
					    }
					}
				    }
				  else
				    {
				      std::cerr << "not C Block pushed"
						<< std::endl;
				    }

#if PARSE_CODE_DEBUG > 0
				  std::cout << "* " << __LINE__
					    << " pop["
					    << (b.brace
						? "brace" : "nonbrace")
					    << "] at " << lineno
					    << std::endl;

				  std::string fs ("none");
				  std::string fb ("none");
				  std::string ts ("none");
				  std::string tb ("none");

				  if (current_block)
				    {
				      fs = current_block->system();
				      fb = current_block->block();
				    }
				  if (b.text)
				    {
				      ts = b.block->system();
				      tb = b.block->block();
				    }

				  std::cout << "* " << __LINE__
					    << " change from "
					    << fs << "/" << fb
					    << " to "
					    << ts << "/" << tb;
				  if (b.is_subsys)
				    std::cout << " [SubSystem]";
				  std::cout << " at " << lineno
					    << std::endl;
#endif

				  std::string* last_text = current_text;
				  unsigned int* last_line = current_line;
				  std::string* last_file = current_file;

				  current_block = b.block;
				  current_text = b.text;
				  is_subsystem = b.is_subsys;

				  block_changed = true;

				  if (nesting.back ().is_loop)
				    loop_in_block--;

				  nesting.pop_back ();

				  if (current_text == NULL
				      && merged_type == m_DotProduct)
				    {
				      current_text = last_text;
				      current_line = last_line;
				      current_file = last_file;
				    }
				}
			    }
			  line = n;
			  size = s;
			  lineno = l;
			  continue;
			}
		      else if (std::tr1::regex_search (line, n,
						       mr, for_loop))
			{
			  const char* fl_nb;
			  unsigned int fl_nbl;
			  const char* fl_end;
			  unsigned int fl_endl;
			  std::tr1::cmatch fl_mr;
			  bool is_for_loop = false;

			  // check Simulink Block in for loop

			  // a statement after for loop
			  fl_endl = lineno;
			  fl_end = skip_statement (line, end - line, fl_endl);

			  // next Simulink Block
			  fl_nbl = l;
			  fl_nb = search_next_simulink_block (line, size,
							      block_mark,
							      fl_nbl,
							      &fl_mr);

			  // Simulink Block before next-statement
			  if (fl_nb < fl_end)
			    {
			      std::string fl_nsys (fl_mr[4]);
			      std::string fl_nblk (fl_mr[5]);
			      std::cout << "Warning:"
					<< lineno
					<< ": block in for loop: "
					<< fl_nsys << "/" << fl_nblk
					<< std::endl;
			      current_text = NULL;
			      current_block = NULL;
			      loop_in_block++;
			      is_for_loop = true;
			    }

			  // start for loop
			  nesting.push_back (c_block_info (line, n,
							   current_block,
							   1,
							   lineno,
							   current_text,
							   is_subsystem,
							   is_for_loop));
#if PARSE_CODE_DEBUG > 0
			  std::cout << "* " << __LINE__
				    << " push[loop brace] at " << lineno
				    << " <<<\n"
				    << nesting.back().str
				    << ">>>\n" << std::endl;
#endif	// PARSE_CODE_DEBUG
			  if (current_text)
			    {
			      c_block_info& b = nesting.back();
			      b.add_close_list (current_text);
			      std::string x (line, n - line);
			      *current_text += ("/*<<< S "
						+ b.mark + " <<<*/\n");
			      *current_text += x;
			      *current_text += ("/*>>> S "
						+ b.mark + " >>>*/\n");
			      if (is_subsystem && current_subsystem != NULL
				  && current_subsystem->subblock.size() > 0)
				current_subsystem->add_code (x);
			    }
			  else if (is_for_loop == false)
			    {
			      std::cerr << "Error: "
				"loop found, but no block at " << lineno
					<< std::endl;
			    }
			  line = n;
			  size = s;
			  lineno = l;
			  continue;
			}
		    }

		  const char* st_end;
		  unsigned int st_end_line = lineno;

		  if (next_block < line)
		    {
		      next_block_line = lineno;
		      next_block = search_next_simulink_block (line, size,
							       block_mark,
							       next_block_line);
		    }

		  c_block_beg_list bl;
		  st_end = skip_statement (line, line, next_block - line,
					   st_end_line, bl);

		  // continuous statement
		  if (st_end)
		    {
		      if (std::tr1::regex_search(line, n, mr, if_tid))
			{
			  // no block(empty) in sub-rate...
			  // eg `if (...TID[N] == 0) { <empty> }'
			  if (check_empty_code_easily (mr[0].second, st_end))
			    {
			      current_text = NULL;
			      current_block = NULL;
			      s = st_end - line;
			      size -= s;
			      line = st_end;
			      lineno = st_end_line;
			      continue;
			    }

			  nesting.push_back (c_block_info (line, n,
							   NULL, // XXX
							   1,
							   lineno,
							   current_text,
							   is_subsystem,
							   false));
			  if (current_text)
			    {
			      if (!is_ratetransition)
				std::cout << "Warning: "
					  << lineno
					  << ": block has multirate code"
					  << std::endl;
			      c_block_info& b = nesting.back();
			      b.add_close_list (current_text);
			      std::string x (line, n - line);
			      *current_text += ("/*<<< S "
						+ b.mark + " <<<*/\n");
			      *current_text += x;
			      *current_text += ("/*>>> S "
						+ b.mark + " >>>*/\n");
			      if (is_subsystem && current_subsystem != NULL
				  && current_subsystem->subblock.size() > 0)
				current_subsystem->add_code (x);
			    }
			  else
			    {
			      std::cerr << "Warning: "
				"TID control found, but no block at " << lineno
					<< std::endl;
			    }
			  line = n;
			  size = s;
			  lineno = l;
			  continue;
			}

		      s = st_end - line;
		      if (current_text)
			{
#if PARSE_CODE_DEBUG > 0
			  std::ostringstream o;
			  o << "/*### " << __LINE__ << " ###*/\n";
			  *current_text += o.str();
			  if (is_subsystem
			      && current_subsystem != NULL
			      && current_subsystem->subblock.size () > 0)
			    {
			      const std::string& s = current_subsystem->sys;
			      const std::string& b = current_subsystem->blk;
			      o.clear();
			      o << "/*### " << __LINE__
				<< " by " << s << "/" << b << " ###*/\n";
			      std::string xstr = o.str();
			      current_subsystem->add_code (xstr);
			    }
#endif	// PARSE_CODE_DEBUG
			  std::string x;
			  if (block_changed)
			    {
			      if (is_subsystem && current_subsystem != NULL)
				{
				  const std::string&
				    bn = current_subsystem->name;
				  const std::string&
				    sy = current_subsystem->sys;
				  const std::string&
				    bl = current_subsystem->blk;
				  int id = 0;
				  if (nesting.size ())
				    id = nesting.back ().id;
				  std::ostringstream os;
				  os << id << " " << bn
				     << " '" << sy << "/" << bl << "'";
				  x = "/*<<< X " + os.str() + " <<<*/\n";
				}
			      block_changed = false;
			    }

			  x += std::string (line, s);
			  *current_text += x;
			  if (is_subsystem && current_subsystem != NULL
			      && current_subsystem->subblock.size () > 0)
			    current_subsystem->add_code (x);
			}
		      else if (block_count > 0)
			{
			  std::cerr << "Error:" << lineno
				    << ": continuous statement existing, "
				    << "but missing block"
				    << std::endl;
			}
		      size -= s;
		      line = st_end;
		      lineno = st_end_line;
		      continue;
		    }

		  //
		  // comment break in statement
		  //

		  // special case: found TaskCounters
		  //   -- begin different rate block
		  //
		  // Only RateTransigion has outside block-definition
		  //
		  const char* inside_block_code = NULL;
		  unsigned int inside_block_lineno = 0;
		  BlockInfo* outside_block = NULL;

		  if (std::tr1::regex_search (line, next_block, mr, if_tid))
		    {
		      if (is_ratetransition)
			{
			  // search inside statement
			  const char* tmp_line = line;
			  size_t tmp_size = size;
			  unsigned int tmp_lineno = lineno;
			  while (tmp_line < mr[0].second)
			    tmp_line = nextline (tmp_line, tmp_size,
						 tmp_lineno);

			  // check effective code
			  unsigned int tmp_sc_lineno = tmp_lineno;
			  const char*
			    skip_sc = skip_space_and_comment (tmp_line,
							      tmp_size,
							      tmp_sc_lineno);
			  // ok, has effective code
			  if (skip_sc < next_block)
			    {
			      inside_block_code = tmp_line;
			      inside_block_lineno = tmp_lineno;
			      outside_block = current_block;
			    }
			}
		      else
			current_text = NULL;
		      current_block = NULL;
		    }

		  if (c_block_end < c_block_beg)
		    c_block_end = line;

		  if (c_block_beg < c_block_end)
		    nesting.push_back (c_block_info (c_block_beg,
						     c_block_end,
						     current_block,
						     0,
						     lineno,
						     current_text,
						     is_subsystem,
						     false));

		  c_block_beg = line;
		  c_block_line = lineno;

		  if (inside_block_code)
		    bl.push_back (c_block_beg_ent (inside_block_code,
						   inside_block_lineno));
		  else
		    bl.push_back (c_block_beg_ent (next_block,
						   next_block_line));

		  for (c_block_beg_list_iterator i (bl.begin());
		       i != bl.end(); i++)
		    {
		      c_block_end = *i;
		      unsigned int cb_line = *i;

		      if (c_block_beg < c_block_end)
			{
			  nesting.push_back (c_block_info (c_block_beg,
							   c_block_end,
							   current_block,
							   1,
							   c_block_line,
							   current_text,
							   is_subsystem,
							   false));
			  if (current_text)
			    {
#if PARSE_CODE_DEBUG > 0
			      std::ostringstream o;
			      o << "/*### " << __LINE__ << " ###*/\n";
			      *current_text += o.str();
			      if (is_subsystem && current_subsystem != NULL
				  && current_subsystem->subblock.size () > 0)
				{
				  const std::string&
				    s = current_subsystem->sys;
				  const std::string&
				    b = current_subsystem->blk;
				  o.clear();
				  o << "/*### " << __LINE__
				    << " by " << s << "/" << b << " ###*/\n";
				  std::string xstr = o.str();
				  current_subsystem->add_code (xstr);
				}
#endif	// PARSE_CODE_DEBUG
			      c_block_info& b = nesting.back();
			      *current_text += ("/*<<< S "
						+ b.mark + " <<<*/\n");
			      *current_text += b.str;
			      *current_text += ("/*>>> S "
						+ b.mark + " >>>*/\n");
			      if (is_subsystem && current_subsystem != NULL
				  && current_subsystem->subblock.size () > 0)
				current_subsystem->add_code (b.str);
			      b.add_close_list (current_text);
			    }
#if PARSE_CODE_DEBUG > 0
			  std::cout << "* " << __LINE__
				    << " push[brace] at " << lineno
				    << " <<<\n"
				    << nesting.back().str
				    << ">>>\n" << std::endl;
#endif	// PARSE_CODE_DEBUG
			}
		      c_block_beg = c_block_end;
		      c_block_line = cb_line;
		    }

		  if (inside_block_code != NULL && current_text != NULL)
		    {
		      size_t len = next_block - inside_block_code;
		      *current_text += std::string (inside_block_code, len);
		      current_block = outside_block;
		    }

		  size -= next_block - line;
		  line = next_block;
		  lineno = next_block_line;

		  c_block_beg = line;
		  c_block_end = NULL;
		  c_block_line = lineno;
		}
	    }

	  if (rtm_beg)
	    {
	      bn.rtm_code ().assign (rtm_beg, rtm_end - rtm_beg);
	      bn.rtm_line () = rtm_line;
	    }
	}

      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error: " << e.what() << std::endl;
	  return -1;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	  return -1;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	  return -1;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	  return -1;
	}

      return 0;
    }

    enum parse_code_initialize_fstate
      {
	P_C_I_NONE,
	P_C_I_BLOCKIO,
	P_C_I_GLOBAL,
	P_C_I_GLOBAL_STATE,
	P_C_I_STATE,
	P_C_I_EXTIN,
	P_C_I_EXTOUT,
	P_C_I_UNITDELAY,
	P_C_I_MEMORY,
	P_C_I_SFUNCTION,
	P_C_I_SUBSYSTEM,
	P_C_I_DISCRETEINTEGRATOR,
	P_C_I_PULSEGENERATOR,
	P_C_I_DELAY,
	P_C_I_CHART,
	P_C_I_COMMON,
	P_C_I_RT_ASSGIN,
	P_C_I_RTM
      };

#if PARSE_CODE_INITIALIZE_DEBUG
    const char* fstate_sym (parse_code_initialize_fstate s)
    {
      switch (s)
	{
	case P_C_I_NONE:
	  return "P_C_I_NONE";
	case P_C_I_BLOCKIO:
	  return "P_C_I_BLOCKIO";
	case P_C_I_GLOBAL:
	  return "P_C_I_GLOBAL";
	case P_C_I_GLOBAL_STATE:
	  return "P_C_I_GLOBAL_STATE";
	case P_C_I_STATE:
	  return "P_C_I_STATE";
	case P_C_I_EXTIN:
	  return "P_C_I_EXTIN";
	case P_C_I_EXTOUT:
	  return "P_C_I_EXTOUT";
	case P_C_I_UNITDELAY:
	  return "P_C_I_UNITDELAY";
	case P_C_I_MEMORY:
	  return "P_C_I_MEMORY";
	case P_C_I_SFUNCTION:
	  return "P_C_I_SFUNCTION";
	case P_C_I_SUBSYSTEM:
	  return "P_C_I_SUBSYSTEM";
	case P_C_I_DISCRETEINTEGRATOR:
	  return "P_C_I_DISCRETEINTEGRATOR";
	case P_C_I_PULSEGENERATOR:
	  return "P_C_I_PULSEGENERATOR";
	case P_C_I_DELAY:
	  return "P_C_I_DELAY";
	case P_C_I_CHART:
	  return "P_C_I_CHART";
	case P_C_I_COMMON:
	  return "P_C_I_COMMON";
	case P_C_I_RT_ASSGIN:
	  return "P_C_I_RT_ASSGIN";
	case P_C_I_RTM:
	  return "P_C_I_RTM";
	}
      return "*UNKNOWN*";
    }
#endif

    void
    open_c_blocks (c_block_stack& nesting, std::string* c)
    {
      for (c_block_stack_iterator i (nesting.begin());
	   i != nesting.end(); i++)
	{
	  c_block_info& b = *i;
	  text_list_iterator ti = std::find (b.close_list.begin (),
					     b.close_list.end(),
					     c);
	  if (ti == b.close_list.end ())
	    {
#if PARSE_CODE_INITIALIZE_DEBUG > 0
	      std::ostringstream o;

	      o << "/*### " << __LINE__ << " ###*/\n";
	      *c += o.str();
#endif
	      *c += b.str;
	      b.close_list.push_back (c);
	    }
	}
    }


    int
    parse_code_initialize (Blocks& bn, const char*& line, size_t& size,
			   unsigned int& lineno)
    {
      const char* end = line + size;
      enum parse_code_initialize_fstate fstate = P_C_I_NONE;

      const file_T::name_type*
	filename = bn.get_filename (Blocks::XF_C_SOURCE);
      const file_T::name_type fnone ("c-none");
      if (filename == NULL)
	{
	  filename = &fnone;
	  bn.source_filename () = fnone;
	}

      try
	{
	  // regex for comment
	  std::tr1::regex comdef ("/\\*"
				  "[[:space:]]*"

				  "("			 // -- %1

				  "("			 // -- %2
				  "InitializeConditions"
				  "|"
				  "Enable"
				  "|"
				  "VirtualOutportStart"
				  ")"			// -- %2(close)
				  "[[:space:]]+"
				  "for"
				  "[[:space:]]+"
				  "("			 // -- %3
				  "UnitDelay"
				  "|"
				  "Memory"
				  "|"
				  "S-Function"
				  "|"
				  "Delay"
				  "|"
				  "[[:alpha:]]+[[:space:]]+SubSystem"
				  "|"
				  "DiscreteIntegrator"
				  "|"
				  "Chart"
				  "|"
				  "Truth Table"
				  ")"			// -- %3(close)
				  "|"			// -- %1(or)
				  "("			// -- %4
				  "[Ee]nd"
				  "[[:space:]]+"
				  "of"
				  "[[:space:]]+"
				  ")?"			// -- %4(close)
				  "("			// -- %5
				  "Start"
				  "|"
				  "InitializeConditions"
				  "|"
				  "Enable"
				  "|"
				  "VirtualOutportStart"
				  "|"
				  "SystemInitialize"
				  ")"			// -- %5(close)
				  "[[:space:]]+"
				  "for"
				  "[[:space:]]+"
				  "("			// -- %6
				  "S-Function[[:space:]]+\\([^:]+\\)"
				  "|"
				  "[_[:alpha:]][_[:alnum:]]*" // XXX
				  "|"
				  "[_[:alpha:]][_[:alnum:]]*" // XXX
				  "[[:space:]]+"
				  "[_[:alpha:]][_[:alnum:]]*"
				  ")"			// -- %6(close)
				  "|"			// -- %1(or)
				  "("			// -- %7
				  "S-Function"
				  "[[:space:]]+"
				  "Block"
				  ")"			// -- %7(close)
				  "|"
				  "("			// -- %8
				  "ConstCode"
				  "[[:space:]]+"
				  "for"
				  "[[:space:]]+"
				  "[_[:alpha:]][_[:alnum:]]*"
				  ")"			// -- %8(close)
				  ")"			// -- %1(close)
				  ":"
				  "[[:space:]]+"
				  "'?"
				  "((<[^>]+>)/([^':*/]+))" // -- %9 (%10/%11)
				  "'?"
				  "[[:space:]]*"
				  "("			// -- %12
				  "incorporates:"
				  "|"
				  "\\*/"
				  ")"			// -- %12(close)
				  );

	  // regex for auto variables
	  std::tr1::regex avar ("^"
				"[[:space:]]*"
				"([_[:alpha:]][_[:alnum:]]*"
				"([[:space:]]+[_[:alpha:]][_[:alnum:]]*)?"
				")"			// -- %1
				"[[:space:]]+"
				"("
				"(\\*+)?[[:space:]]*"	// -- %4
				"([_[:alpha:]][_[:alnum:]]*)" // -- %5
				"[[:space:]]*"
				"((\\["
				"[[:space:]]*"
				"([[:digit:]]+)"	// -- %8 dem(int)
				"[[:space:]]*"
				"\\])+)?"		// -- %6
				")"			// -- %3
				"[[:space:]]*"
				";");

	  // regex for `for' loop
	  std::tr1::regex for_loop ("^"
				    "[[:space:]]*"
				    "for"
				    "[[:space:]]*"
				    "\\("
				    "[[:space:]]*"
				    "([_[:alpha:]][_[:alnum:]]*)"
				    "[[:space:]]*"
				    "=");

	  // memset((void *)&...) or memset(((void *)&...))
	  std::tr1::regex mmm ("^"
			       "[[:space:]]*"
			       "\\(void\\)"
			       "[[:space:]]*"
			       "memset"
			       "[[:space:]]*"
			       "\\("
			       "[[:space:]]*"
			       "\\(?"
			       "("
			       "[[:space:]]*"
			       "\\("
			       "[[:space:]]*"
			       "void"
			       "[[:space:]]*"
			       "\\*"
			       "[[:space:]]*"
			       "\\)"
			       ")?"		// -- %1 -- (void*)
			       "[[:space:]]*"
			       "&"
			       "[[:space:]]*"
			       "([_[:alpha:]][_[:alnum:]]*)" // %2
			       "\\)?"
			       "[[:space:]]*"
			       ",");

	  std::string storages;

	  if (bn.state_name().length ())
	    {
	      if (storages.length () == 0)
		storages += "((";
	      storages += bn.state_name ();
	    }
	  if (bn.trigger_state_name ().length ())
	    {
	      if (storages.length () == 0)
		storages += "((";
	      else
		storages += "|";
	      storages += bn.trigger_state_name ();
	    }
	  if (bn.invariant_signal_name ().length ())
	    {
	      if (storages.length () == 0)
		storages += "((";
	      else
		storages += "|";
	      storages += bn.invariant_signal_name ();
	    }
	  if (bn.param_name ().length ())
	    {
	      if (storages.length () == 0)
		storages += "((";
	      else
		storages += "|";
	      storages += bn.param_name ();
	    }
	  if (bn.constant_param_name ().length ())
	    {
	      if (storages.length () == 0)
		storages += "((";
	      else
		storages += "|";
	      storages += bn.constant_param_name ();
	    }
	  if (bn.constant_param_name ().length ())
	    {
	      if (storages.length () == 0)
		storages += "((";
	      else
		storages += "|";
	      storages += bn.constant_param_name ();
	    }
	  if (bn.signal_name ().length ())
	    {
	      if (storages.length () == 0)
		storages += "((";
	      else
		storages += "|";
	      storages += bn.signal_name ();
	    }
	  if (bn.external_input_name ().length ())
	    {
	      if (storages.length () == 0)
		storages += "((";
	      else
		storages += "|";
	      storages += bn.external_input_name ();
	    }
	  if (bn.external_output_name ().length ())
	    {
	      if (storages.length () == 0)
		storages += "((";
	      else
		storages += "|";
	      storages += bn.external_output_name ();
	    }
	  if (storages.length () == 0)
	    storages = "((= = = u n d e f i n e d = = =)\\.)?";
	  else
	    storages += ")\\.)?";

	  // regex for assinment (VAR = VAL;)
	  std::tr1::regex adef ("^"
				"[[:space:]]*"
				"("
				+ storages		// %3 (st)
				+ "([_[:alpha:]][_[:alnum:]]*)" // %4 (var)
				"(\\.[_[:alpha:]][_[:alnum:]]*)*" // %5 (rest)
				"[[:space:]]*"
				"("
				"\\["
				"[[:space:]]*"
				"[_[:alnum:]]+"
				"[[:space:]]*"
				"\\]"
				")*"			// %6 var[index]
				"(\\.[_[:alpha:]][_[:alnum:]]*)*" // %7 (mb)
				")"			//  %1 (var)
				"[[:space:]]*"
				"="
				"[[:space:]]*"
				"([_.[:alnum:]]+)"	// %8 (val)
				"[[:space:]]*"
				";");

	  // regex for assignment with expression (VAR = ...;)
	  std::tr1::regex aexp ("^"
				"[[:space:]]*"
				"("
				+ storages		// %3 (st)
				+
				"([_[:alpha:]][_[:alnum:]]*)" // %4 (var)
				"(\\.[_[:alpha:]][_[:alnum:]]*)*" // %5 (rest)
				"[[:space:]]*"
				"("
				"\\["
				"[[:space:]]*"
				"[_[:alnum:]]+"
				"[[:space:]]*"
				"\\]"
				")*"			// %6 var[index]
				"(\\.[_[:alpha:]][_[:alnum:]]*)*" // %7 (mb)
				")"			//  %1 (var)
				"[[:space:]]*"
				"="
				"[[:space:]]*"
				"(.*)"			// %8
				"(;)");			// %9

	  std::tr1::cmatch mr;

	  BlockInfo* cur_blk = NULL;

	  c_block_stack nesting;
	  std::string function_head;

	  const char* c_beg = NULL;
	  const char* c_end = NULL;
	  const char* var_beg = NULL;
	  const char* var_end = NULL;
	  std::string varname;
	  std::string storage_name;

	  const char* com_end = NULL;
	  const char* com_beg = NULL;

	  enum parse_code_initialize_fstate main_fstate = P_C_I_NONE;

	  line = nextline (line, size, lineno); // function name
	  line = nextline (line, size, lineno); // start block

	  while (line < end)
	    {
	      size_t s = size;
	      unsigned int l = lineno;
	      const char *n = nextline (line, s, l);
	      unsigned int cl;
	      unsigned int el;

	      if (*line == '}')	// end initialize function
		{
		  line = n;
		  size = s;
		  lineno = l;
		  break;
		}

	      varname.clear();

	      int add_code_and_next = 0;
	      const char* x = skip_space (line, n - line);
	      if (x)
		{
		  if (*x == '}')
		    {
		      // check c-block end (no check `} else {')
		      if (nesting.size() > 0)
			{
			  std::string z (line, n - line);
			  for (text_list_iterator
				 ti (nesting.back().close_list.begin());
			       ti != nesting.back().close_list.end(); ti++)
			    {
			      std::string* t = *ti;
#if PARSE_CODE_INITIALIZE_DEBUG > 0
			      std::ostringstream o;

			      o << "/*### " << __LINE__ << " ###*/\n";
			      *t += o.str();
#endif
			      *t += z;
			    }
			  nesting.pop_back();
			}
		      // add_code_and_next = 1;
		      goto try_next;
		    }
		  else if (*x == '{')
		    {
		      // c-block starting
		      nesting.push_back(c_block_info(line, n,
						     NULL, 1, lineno, NULL,
						     false, false));
		      add_code_and_next = 1;
		    }
		  else if (std::tr1::regex_search (line, n, mr, for_loop))
		    {
		      // found foo-loop
		      nesting.push_back(c_block_info(line, n,
						     NULL, 1, lineno, NULL,
						     false, false));
		      add_code_and_next = 1;
		    }
		}

	      // found auto variable
	      if (std::tr1::regex_search (line, n, mr, avar))
		{
		  std::string x (line, n - line);
		  if (nesting.size())
		    nesting.back().str += x;
		  else
		    function_head += x;
		  add_code_and_next = 1;
		}

	      // add code and restart from next line
	      if (add_code_and_next == 1)
		{
		  if (cur_blk)
		    {
		      std::string x (line, n - line);
		      std::string* c;
		      if (fstate == P_C_I_GLOBAL_STATE
			  || fstate == P_C_I_SFUNCTION
			  || fstate == P_C_I_SUBSYSTEM)
			c = &cur_blk->init_code();
		      else
			c = &cur_blk->init_state();
#if PARSE_CODE_INITIALIZE_DEBUG > 0
		      std::ostringstream oo;
		      oo << "/*### " << __LINE__ << " ###*/\n";
		      *c += oo.str();
#endif	// PARSE_CODE_INITIALIZE_DEBUG
		      *c += x;
		      if (nesting.size ())
			nesting.back().add_close_list (c);
		    }
		  goto try_next;
		}

	      // check comment
	      cl = lineno;
	      el = lineno;
	      com_beg = find_comment (line, size, com_end, cl, el);
	      if (com_beg)
		{
		  if (com_end - com_beg > 17
		      && strncmp (com_beg,
				  "  /* block I/O */", 17) == 0)
		    main_fstate = fstate = P_C_I_BLOCKIO;
		  else if (com_end - com_beg > 31
			   && strncmp (com_beg,
				       "  /* exported global signals */",
				       31) == 0)
		    main_fstate = fstate = P_C_I_GLOBAL;
		  else if (com_end - com_beg > 30
			   && strncmp (com_beg,
				       "  /* exported global states */",
				       30) == 0)
		    main_fstate = fstate = P_C_I_GLOBAL_STATE;
		  else if (com_end - com_beg > 22
			   && strncmp (com_beg, "  /* states (dwork) */",
				       22) == 0)
		    main_fstate = fstate = P_C_I_STATE;
		  else if (com_end - com_beg > 23
			   && strncmp (com_beg, "  /* external inputs */",
				       23) == 0)
		    main_fstate = fstate = P_C_I_EXTIN;
		  else if (com_end - com_beg > 24
			   && strncmp (com_beg, "  /* external outputs */",
				       24) == 0)
		    main_fstate = fstate = P_C_I_EXTOUT;
		  else if (com_end - com_beg > 30
			   && strncmp (com_beg,
				       "  /* initialize non-finites */",
				       30) == 0)
		    main_fstate = fstate = P_C_I_COMMON;
		  else if (com_end - com_beg > 41
			   && strncmp (com_beg,
				       "  /* non-finite (run-time) "
				       "assignments */",
				       41) == 0)
		    main_fstate = fstate = P_C_I_RT_ASSGIN;
		  else if (com_end - com_beg > 34
			   && strncmp (com_beg,
				       "  /* initialize real-time model */",
				       34) == 0)
		    {
		      main_fstate = fstate = P_C_I_RTM;
		      bn.rtm_init_code () += std::string (line, n - line);
		    }
		  else if (std::tr1::regex_search (line, n, mr, comdef))
		    {
		      enum parse_code_initialize_fstate bup_state = fstate;
		      BlockInfo* bup_blk = cur_blk;

		      fstate = P_C_I_NONE;
		      cur_blk = NULL;

		      bool is_virtualoutportstart = false;
		      bool is_end = false;

		      if (mr[2].matched)
			{
			  std::string str (mr[2]);
			  if (str == "VirtualOutportStart")
			    is_virtualoutportstart = true;
			}
		      if (mr[5].matched)
			{
			  std::string str (mr[5]);
			  if (str == "VirtualOutportStart")
			    is_virtualoutportstart = true;
			}

		      // check comment type
		      if (mr[3].matched)
			{
			  std::string type (mr[3]);
			  if (type == "UnitDelay")
			    fstate = P_C_I_UNITDELAY;
			  else if (type == "Memory")
			    fstate = P_C_I_MEMORY;
			  else if (type.length () >= 9
				   && type.compare (type.length () - 9,
						    9, "SubSystem") == 0)
			    fstate = P_C_I_SUBSYSTEM;
			  else if (type == "S-Function")
			    fstate = P_C_I_SFUNCTION;
			  else if (type == "DiscreteIntegrator")
			    fstate = P_C_I_DISCRETEINTEGRATOR;
			  else if (type == "Delay")
			    fstate = P_C_I_DELAY;
			  else if (type == "Chart" || type == "Truth Table")
			    fstate = P_C_I_CHART;
			}
		      else if (mr[4].matched) // End of ...
			{
			  fstate = bup_state; // recovery
			  cur_blk = bup_blk;
			  is_end = true;
			}
		      else if (mr[6].matched)
			{
			  std::string type = mr[6];
			  if (type.compare (0, 10, "S-Function") == 0)
			    fstate = P_C_I_SFUNCTION;
			  else if (type == "DiscretePulseGenerator")
			    fstate = P_C_I_PULSEGENERATOR;
			  else
			    fstate = P_C_I_SUBSYSTEM; // and other blocks
			}
		      else if (mr[7].matched)
			fstate = P_C_I_SFUNCTION;
		      else if (mr[8].matched)
			fstate = P_C_I_GLOBAL;

		      // try to find simulink block
		      std::string sys (mr[10]);
		      std::string blk (mr[11]);

		      if (is_virtualoutportstart)
			std::cout << "Warning: " << lineno
				  << ": VirtualOutport initializer for "
				  << mr[9] << std::endl;

		      cur_blk = bn.find_block (sys, blk);
		      if (cur_blk == NULL)
			{
			  std::cout << "Warning: "
				    << lineno
				    << ": unknown system in " << mr[1]
				    << ": " << sys << std::endl;
			}

		      // add function header, c-block header, and comment
		      if (cur_blk)
			{
			  if (is_virtualoutportstart)
			    {
			      BlockInfo::block_type& t = cur_blk->type ();
			      if (t == BlockInfo::TYPE_NORMAL)
				t = BlockInfo::TYPE_VIRTUALOUTPORT;
			      else
				assert (t == BlockInfo::TYPE_VIRTUALOUTPORT);
			    }

			  std::string* c;
			  // TODO: P_C_I_PULSEGENERATORの検討
			  if (fstate == P_C_I_GLOBAL_STATE
			      || fstate == P_C_I_SFUNCTION
			      || fstate == P_C_I_SUBSYSTEM
			      || fstate == P_C_I_GLOBAL
			      || fstate == P_C_I_CHART)
			    {
			      c = &cur_blk->init_code(); // same as code
			      cur_blk->init_code_appearence (cl, filename);
			      if ((cur_blk->any() & 0x10) == 0)
				{
				  cur_blk->any() |= 0x10;
#if PARSE_CODE_INITIALIZE_DEBUG > 0
				  std::ostringstream oo;
				  oo << "/*### " << __LINE__ << " ###*/\n";
				  *c += oo.str();
#endif	// PARSE_CODE_INITIALIZE_DEBUG
				  *c += function_head;
				}
			    }
			  else
			    {
			      c = &cur_blk->init_state();
			      cur_blk->init_state_appearence (cl, filename);
			      if ((cur_blk->any() & 0x20) == 0)
				{
				  cur_blk->any() |= 0x20;
#if PARSE_CODE_INITIALIZE_DEBUG > 0
				  std::ostringstream oo;
				  oo << "/*### " << __LINE__ << " ###*/\n";
				  *c += oo.str();
#endif	// PARSE_CODE_INITIALIZE_DEBUG
				  *c += function_head;
				}
			    }

#if PARSE_CODE_INITIALIZE_DEBUG > 0
			  std::ostringstream oo;
			  oo << "/*### " << __LINE__
			     << "[" << fstate_sym (fstate) << "] ###*/\n";
			  *c += oo.str();
#endif	// PARSE_CODE_INITIALIZE_DEBUG

			  std::string h;
			  open_c_blocks (nesting, c);

			  *c += std::string (com_beg, com_end - com_beg);
			}

		      if (is_end)
			{
			  fstate = main_fstate;
			  cur_blk = NULL;
			}
		    }
		  else if (fstate == P_C_I_RTM)
		    {
		      open_c_blocks (nesting, &bn.rtm_init_code ());
		      bn.rtm_init_code () += std::string (com_beg,
							  com_end - com_beg);
		    }

		  line = com_end;
		  size -= com_end - com_beg;
		  lineno = el;
		  continue;
		}

	      // none
	      if (fstate == P_C_I_NONE)
		{
		  line = n;
		  size = s;
		  lineno = l;
		  continue;
		}

	      // skip empty line
	      if (skip_space (line, n - line) == NULL)
		{
		  line = n;
		  size = s;
		  lineno = l;
		  continue;
		}

#if PARSE_CODE_INITIALIZE_DEBUG
	      {
		std::string x (line, n - line);
		std::cout << "line: " << x << std::endl;
	      }
#endif

	      // no check variable name
	      if (fstate == P_C_I_GLOBAL_STATE
		  || fstate == P_C_I_MEMORY
		  || fstate == P_C_I_SFUNCTION
		  || fstate == P_C_I_SUBSYSTEM
		  || fstate == P_C_I_UNITDELAY
		  || fstate == P_C_I_DELAY
		  || fstate == P_C_I_DISCRETEINTEGRATOR
		  || fstate == P_C_I_CHART
		  || fstate == P_C_I_COMMON
		  || fstate == P_C_I_RTM)
		goto add_code;

	      if (std::tr1::regex_search (line, n, mr, mmm))
		{
		  // memset() type initialization
		  c_beg = line;
		  l = lineno;
		  n = skip_statement (line, size, l);
		  s = size - (n - line);
		  c_end = n;
		  // XXX
		  // この varname は例えば状態変数全体の場合もあるので、
		  // マッチしないかもしれない。その場合は無視してよい。
		  // 初期化式では0を入れることにする。
		  varname = mr[2];
#if PARSE_CODE_INITIALIZE_DEBUG
		  std::string x (line, n - line);
		  std::cout << "* memset statement: " << x << std::endl;
#endif
		}
	      else
		{
		  // normal code
		  var_beg = c_beg = line;
		  l = lineno;
		  n = skip_statement (line, size, l);
		  var_end = c_end = n;
		  s = size - (n - line);
#if PARSE_CODE_INITIALIZE_DEBUG
		  std::string x (var_beg, var_end - var_beg);
		  std::cout << "* Default statement: " << x << std::endl;
#endif
		}

	      if (varname.length() == 0)
		{
		  storage_name.clear ();

		  // check assinment (VAR = VAL;)
		  if (std::tr1::regex_search (var_beg, var_end, mr, adef))
		    {
		      std::string assign_storage (mr[3]);
		      std::string assign_var (mr[4]);
		      std::string assign_value (mr[8]);

		      varname = assign_var;
		      if (mr[3].matched)
			storage_name = assign_storage;
#if PARSE_CODE_INITIALIZE_DEBUG
		      std::cout << "* [" << fstate_sym (fstate)
				<< "] storage=" << assign_storage
				<< ", var=" << assign_var
				<< ", val = " << assign_value
				<< std::endl;
#endif
		    }
		  else if (std::tr1::regex_search (var_beg, var_end,
						   mr, aexp))
		    {
		      std::string assign_storage (mr[3]);
		      std::string assign_var (mr[4]);
		      std::string assign_value (mr[8].first,
						mr[9].first
						- mr[8].first - 1);

		      varname = assign_var;
		      if (mr[3].matched)
			storage_name = assign_storage;

#if PARSE_CODE_INITIALIZE_DEBUG
		      std::cout << "* [" << fstate_sym (fstate)
				<< "] storage=" << assign_storage
				<< ", var=" << assign_var
				<< ", val = " << assign_value
				<< std::endl;
#endif
		    }
		  else
		    {
#if PARSE_CODE_INITIALIZE_DEBUG
		      std::string x (var_beg, var_end - var_beg);
		      std::cout << "* Unmatch assignment: "
				<< x << std::endl;
#endif
		    }
		}

	      // couldn't found variable...
	      if (varname.length() == 0)
		{
		  line = n;
		  size = s;
		  lineno = l;
		  continue;
		}

	    add_code:
	      if (fstate == P_C_I_BLOCKIO)
		{
		  // TODO Block I/O
#if PARSE_CODE_INITIALIZE_DEBUG
		  std::string x (line, n - line);
		  std::cout << "* BIO: " << x << std::flush;
#endif
		  c_variable_map_iterator v;

		  // signale objects
		  if (bn.signal_name() == varname)
		    goto try_next;

		  v = bn.find_signal_var (varname);
		  if (v == bn.signal_entry().end())
		    {
		      std::cout << "Warning: unknown varname "
				<< varname << std::endl;
		      goto try_next;
		    }

		  cur_blk = bn.find_block (v->second.system(),
					   v->second.block());
		  if (cur_blk == NULL)
		    {
		      std::cout << "Warning: signal " << varname
				<< ": unknown system/block "
				<< v->second.system() << "/"
				<< v->second.block() << std::endl;
		      goto try_next;
		    }

		  std::string* c = &cur_blk->init_code();
#if PARSE_CODE_INITIALIZE_DEBUG > 0
		  std::ostringstream oo;
		  oo << "/*### " << __LINE__ << " ###*/\n";
		  *c += oo.str();
#endif	// PARSE_CODE_INITIALIZE_DEBUG
		  open_c_blocks (nesting, c);
		  *c += std::string (c_beg, c_end - c_beg);
		  cur_blk->init_code_appearence (lineno, filename);
		  cur_blk = NULL;
		  goto try_next;
		}
	      else if (fstate == P_C_I_GLOBAL
		       || fstate == P_C_I_EXTIN
		       || fstate == P_C_I_EXTOUT
		       || fstate == P_C_I_STATE)
		{
		  c_variable_map_iterator v;
		  const char* vartype = "signal";
		  bool is_state = false;

		  switch (fstate)
		    {
		    case P_C_I_EXTIN:
		      // all of external input storage
		      if (bn.external_input_name () == varname)
			goto try_next; // XXX
		      break;

		    case P_C_I_EXTOUT:
		      // all of external output storage
		      if (bn.external_output_name () == varname)
			goto try_next; // XXX
		      break;

		    case P_C_I_STATE:
		      // all of state storage
		      if (bn.state_name () == varname)
			goto try_next; // XXX
		      break;

		    default:
		      break;
		    }

		  if (storage_name.length () == 0)
		    {
		      // exported block signal
		      v = bn.find_ext_var (varname);
		      if (v == bn.ext_variable_entry().end())
			goto unknown_var;
		    }
		  else if (storage_name == bn.external_input_name ())
		    {
		      vartype = "extin";
		      v = bn.find_external_input (varname);
		      if (v == bn.external_input_entry().end ())
			goto unknown_var;
		    }
		  else if (storage_name == bn.external_output_name ())
		    {
		      vartype = "extout";
		      v = bn.find_external_output (varname);
		      if (v == bn.external_output_entry().end ())
			goto unknown_var;
		    }
		  else if (storage_name == bn.signal_name ())
		    {
		      v = bn.find_signal_var (varname);
		      if (v == bn.signal_entry().end())
			goto unknown_var;
		    }
		  else if (storage_name == bn.state_name ())
		    {
		      vartype = "state";
		      v = bn.find_state_var (varname);
		      if (v == bn.state_entry ().end ())
			goto unknown_var;
		      is_state = true;
		    }
		  else if (storage_name == bn.invariant_signal_name ())
		    {
		      vartype = "invariant signal";
		      v = bn.find_invariant_signal_var (varname);
		      if (v == bn.invariant_signal_entry().end())
			goto unknown_var;
		      is_state = true;
		    }
		  else if (storage_name == bn.trigger_state_name ())
		    {
		      vartype = "trigger state";
		      v = bn.find_trigger_state_var (varname);
		      if (v == bn.trigger_state_entry().end())
			goto unknown_var;
		      is_state = true;
		    }
		  else
		    {
		    unknown_var:
		      std::cout << "Warning: unknown " << vartype << ' ';
		      if (storage_name.length ())
			std::cout << storage_name << '.';
		      std::cout << varname << std::endl;
		      goto try_next;
		    }

		  cur_blk = bn.find_block (v->second.system(),
					   v->second.block());
		  if (cur_blk == NULL)
		    {
		      std::cout << "Warning: " << vartype << ' ' << varname
				<< ": unknown system/block "
				<< v->second.system() << "/"
				<< v->second.block() << std::endl;
		      goto try_next;
		    }

		  std::string* c;
		  if (is_state)
		    {
		      c = &cur_blk->init_state();
		      cur_blk->init_state_appearence (lineno, filename);
		    }
		  else
		    {
		      c = &cur_blk->init_code();
		      cur_blk->init_code_appearence (lineno, filename);
		    }
#if PARSE_CODE_INITIALIZE_DEBUG > 0
		  std::ostringstream oo;
		  oo << "/*### " << __LINE__ << " ###*/\n";
		  *c += oo.str();
#endif	// PARSE_CODE_INITIALIZE_DEBUG
		  open_c_blocks (nesting, c);
		  *c += std::string (c_beg, c_end - c_beg);
		  cur_blk = NULL;
		  goto try_next;
		}
	      else if (fstate == P_C_I_PULSEGENERATOR)
		{
		  // check state variable
		  c_variable_map_iterator v = bn.find_state_var (varname);

		  if (v == bn.state_entry().end())
		    {
		      v = bn.find_trigger_state_var (varname);
		      if (v == bn.trigger_state_entry().end())
			{
			  std::cout << "Warning: unknown trigger state "
				    << varname
				    << std::endl;
			  goto try_next;
			}
		    }

		  cur_blk = bn.find_block (v->second.system(),
					   v->second.block());
		  if (cur_blk == NULL)
		    {
		      std::cout << "Warning: trigger state " << varname
				<< ": unknown system/block "
				<< v->second.system() << "/"
				<< v->second.block() << std::endl;
		      goto try_next;
		    }

		  std::string* c = &cur_blk->init_state();
#if PARSE_CODE_INITIALIZE_DEBUG > 0
		  std::ostringstream oo;
		  oo << "/*### " << __LINE__ << " ###*/\n";
		  *c += oo.str();
#endif	// PARSE_CODE_INITIALIZE_DEBUG
		  open_c_blocks (nesting, c);
		  *c += std::string (c_beg, c_end - c_beg);
		  cur_blk->init_state_appearence (lineno, filename);
		  cur_blk = NULL;
		  goto try_next;
		}
	      else if (fstate == P_C_I_GLOBAL_STATE
		       || fstate == P_C_I_SFUNCTION
		       || fstate == P_C_I_SUBSYSTEM
		       || fstate == P_C_I_CHART)
		{
		  // same as code
		  l = lineno;
		  n = skip_statement (line, size, l);
		  s = size - (n - line);

#if PARSE_CODE_INITIALIZE_DEBUG > 0
		  std::string x (line, n - line);
		  std::cout << "* [" << fstate_sym (fstate) << "]\n<<<\n"
			    << x << ">>>" << std::endl;
#endif	// PARSE_CODE_INITIALIZE_DEBUG
		  if (cur_blk == NULL)
		    goto try_next;

#if PARSE_CODE_INITIALIZE_DEBUG > 0
		  std::ostringstream oo;
		  oo << "/*### " << __LINE__ << " ###*/\n";
		  cur_blk->init_code() += oo.str();
#endif	// PARSE_CODE_INITIALIZE_DEBUG
		  cur_blk->init_code() += std::string (line, n - line);
		  cur_blk->init_code_appearence (lineno, filename);
		}
	      else if (fstate == P_C_I_UNITDELAY
		       || fstate == P_C_I_MEMORY
		       || fstate == P_C_I_DELAY
		       || fstate == P_C_I_DISCRETEINTEGRATOR)
		{
		  // same as state
		  l = lineno;
		  n = skip_statement (line, size, l);
		  s = size - (n - line);

#if PARSE_CODE_INITIALIZE_DEBUG
		  std::string x (line, n - line);
		  std::cout << "* [" << fstate_sym (fstate) << "]\n<<<\n"
			    << x << ">>>" << std::endl;
#endif

		  if (cur_blk == NULL)
		    goto try_next;

#if PARSE_CODE_INITIALIZE_DEBUG > 0
		  std::ostringstream oo;
		  oo << "/*### " << __LINE__ << " ###*/\n";
		  cur_blk->init_state() += oo.str();
#endif	// PARSE_CODE_INITIALIZE_DEBUG
		  cur_blk->init_state() += std::string (line, n - line);
		  cur_blk->init_state_appearence (lineno, filename);
		}
	      else if (fstate == P_C_I_COMMON)
		{
		  if (bn.common_init_line () == 0)
		    bn.common_init_line () = lineno;
		  open_c_blocks (nesting, &bn.common_init_code ());
		  bn.common_init_code () += std::string (line, n - line);
		}
	      else if (fstate == P_C_I_RTM)
		{
		  if (bn.rtm_init_line () == 0)
		    bn.rtm_init_line () = lineno;
		  open_c_blocks (nesting, &bn.rtm_init_code ());
		  bn.rtm_init_code () += std::string (line, n - line);
		}
	      else if (fstate == P_C_I_RT_ASSGIN)
		{
		  if (storage_name.length ())
		    {
		      VariableEnt* v = NULL;
		      std::string param_target;
		      if (storage_name == bn.param_name ())
			{
			  c_variable_map_iterator
			    vi = bn.find_param (varname);
			  if (vi == bn.param_entry ().end ())
			    {
			      std::cout << "Warning: unknown param "
					<< varname
					<< std::endl;
			      goto try_next;
			    }
			  param_target = "param";
			  v = &vi->second;
			}
		      else if (storage_name == bn.constant_param_name ())
			{
			  // XXX -- need constant params?
			  c_variable_map_iterator
			    vi = bn.find_constant_param (varname);
			  if (vi == bn.constant_param_entry ().end ())
			    {
			      std::cout << "Warning: unknown constant param "
					<< varname
					<< std::endl;
			      goto try_next;
			    }
			  param_target = "constant param";
			  v = &vi->second;
			}

		      if (v->multiple_system ().size () == 0)
			{
			  cur_blk = bn.find_block (v->system (), v->block ());
			  if (cur_blk == NULL)
			    {
			      std::cout << "Warning: " << param_target
					<< varname
					<< ": unknown system/block "
					<< v->system() << "/"
					<< v->block() << std::endl;
			      goto try_next;
			    }
			  std::string* c = &cur_blk->init_code();
#if PARSE_CODE_INITIALIZE_DEBUG > 0
			  std::ostringstream oo;
			  oo << "/*### " << __LINE__ << " ###*/\n";
			  *c += oo.str();
#endif	// PARSE_CODE_INITIALIZE_DEBUG
			  open_c_blocks (nesting, c);
			  *c += std::string (c_beg, c_end - c_beg);
			  cur_blk->init_code_appearence (lineno, filename);
			  cur_blk = NULL;
			  goto try_next;
			}
		      else
			{
			  for (multiple_system_const_iterator
				 i (v->multiple_system ().begin ());
			       i != v->multiple_system ().end (); i++)
			    {
			      const SystemBlockEnt& sb = *i;
			      cur_blk = bn.find_block (sb.system (),
						       sb.block ());
			      if (cur_blk == NULL)
				{
				  std::cout << "Warning: param " << varname
					    << ": unknown system/block "
					    << v->system() << "/"
					    << v->block() << std::endl;
				  continue;
				}
			      std::string* c = &cur_blk->init_code();
#if PARSE_CODE_INITIALIZE_DEBUG > 0
			      std::ostringstream oo;
			      oo << "/*### " << __LINE__ << " ###*/\n";
			      *c += oo.str();
#endif	// PARSE_CODE_INITIALIZE_DEBUG
			      open_c_blocks (nesting, c);
			      *c += std::string (c_beg, c_end - c_beg);
			      cur_blk->init_code_appearence (lineno, filename);
			      cur_blk = NULL;
			    }
			}
		    }
		  else
		    {
		    }
		}
	      else
		{
		  std::cerr << "* BUG: " << __FUNCTION__
			    << ": " << __LINE__ << ": unknown state"
			    << std::endl;
		  goto try_next;
		}

	    try_next:
	      line = n;
	      size = s;
	      lineno = l;
	    }
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error: " << e.what() << std::endl;
	  return -1;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	  return -1;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	  return -1;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	  return -1;
	}

      return 0;
    }

    int
    parse_code_trigger_state (Blocks& bn, const char*& line, size_t& size,
			      unsigned int& lineno)
    {
      try
	{
	  std::string vpat;

	  vpat = ("^" "[[:space:]]*"
		  + bn.trigger_state_type()
		  + "[[:space:]]+"
		  "([_[:alpha:]][_[:alnum:]]*)"
		  "[[:space:]]*;");

	  std::tr1::regex vdef (vpat);
	  std::tr1::cmatch mr;

	  if (std::tr1::regex_search (line, mr, vdef))
	    {
	      bn.trigger_state_name() = mr[1];
	      while (line < mr[0].second)
		line = nextline (line, size, lineno);
	    }
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error: " << e.what() << std::endl;
	  return -1;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	  return -1;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	  return -1;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	  return -1;
	}

      return 0;
    }

    int
    parse_code_real_time_model (Blocks& bn, const char*& line, size_t& size,
				unsigned int& lineno)
    {
      try
	{
	  std::tr1::regex opat ("^"
				"([_[:alpha:]][_[:alnum:]]*)" // %1 -- type
				"[[:space:]]+"
				"([_[:alpha:]][_[:alnum:]]*)" // %2 -- obj
				"[[:space:]]*"
				";");

	  std::tr1::regex vpat ("^"
				"([_[:alpha:]][_[:alnum:]]*)" // %1 -- type
				"[[:space:]]*"
				"\\*"
				"([[:space:]]*const)?"        // %2 -- const?
				"[[:space:]]"
				"([_[:alpha:]][_[:alnum:]]*)" // %3 -- var
				"[[:space:]]*"
				"="
				"[[:space:]]*"
				"&"
				"[[:space:]]*"
				"([_[:alpha:]][_[:alnum:]]*)" // %4 -- obj
				"[[:space:]]*"
				";");

	  std::tr1::cmatch mr;

	  const char* end = line + size;

	  while (line < end)
	    {
	      if (std::tr1::regex_search (line, mr, opat))
		{
		  std::string& ctype = bn.rtm_object_type();
		  std::string& cobj = bn.rtm_object_name();
		  const std::string& htype = bn.rtm_extern_type();

		  if ((ctype.length () && ctype != mr[1])
		      || (cobj.length () && cobj != mr[2])
		      || (htype.length () && htype != mr[1]))
		    {
		      std::cout << "Warning: "
			"unmatch Real-Time Model in H/C" << std::endl;
		    }
		  ctype = mr[1];
		  cobj = mr[2];
		}
	      else if (std::tr1::regex_search (line, mr, vpat))
		{
		  std::string& ctype = bn.rtm_object_type();
		  std::string& cobj = bn.rtm_object_name();
		  std::string& cvar = bn.rtm_object_pointer();
		  const std::string& htype = bn.rtm_extern_type();
		  const std::string& hvar = bn.rtm_extern_name();

		  if ((ctype.length () && ctype != mr[1])
		      || (htype.length () && htype != mr[1])
		      || (cvar.length () && cvar != mr[3])
		      || (hvar.length () && hvar != mr[3])
		      || (cobj.length () && cobj != mr[4]))
		    {
		      std::cout << "Warning: "
			"unmatch Real-Time Model in H/C" << std::endl;
		    }

		  ctype = mr[1];
		  cobj = mr[4];
		  cvar = mr[3];

		  if (mr[2].matched)
		    bn.rtm_object_has_const() = true;
		}
	      else
		break;

	      while (line < mr[0].second)
		line = nextline (line, size, lineno);
	    }
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error: " << e.what() << std::endl;
	  return -1;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	  return -1;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	  return -1;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	  return -1;
	}

      return 0;
    }

    struct check_block_func_result
    {
#if PARSE_CODE_DEBUG
      check_block_func_result (BlockInfo* pbi, function_code_T* pfc,
			       const std::string& psys,
			       const std::string& pblk)
	: bi (pbi), fc (pfc), sys (psys), blk (psys)
      {
      }
#else
      check_block_func_result (BlockInfo* pbi, function_code_T* pfc)
	: bi (pbi), fc (pfc)
      {
      }
#endif
      BlockInfo* bi;
      function_code_T* fc;
#if PARSE_CODE_DEBUG
      std::string sys;
      std::string blk;
#endif
    };

    check_block_func_result
    check_block_function (Blocks& bn,
			  const std::string& fname,
			  const char* com_beg, const char* com_end,
			  unsigned int com_line,
			  std::tr1::regex& task_regex,
			  std::tr1::regex& other_regex,
			  bool decl)
    {
      function_code_T::FuncKind fk = function_code_T::FK_NONE;
      std::string sys;
      std::string blk;

      std::tr1::cmatch cmr;
      if (std::tr1::regex_search (com_beg, com_end, cmr, task_regex))
	{
	  sys = cmr[9];
	  blk = cmr[10];
	  if (cmr[3].matched)
	    fk = function_code_T::FK_INIT;
	  else if (cmr[4].matched)
	    fk = function_code_T::FK_TASK;
	  else if (cmr[7].matched)
	    fk = function_code_T::FK_UPDATE;
	}
      else if (std::tr1::regex_search (com_beg, com_end, cmr, other_regex))
	{
	  sys = cmr[4];
	  blk = cmr[5];
	}
      else
	{
#if PARSE_CODE_DEBUG
	  return check_block_func_result (NULL, NULL, "", "");
#else
	  return check_block_func_result (NULL, NULL);
#endif
	}

      function_code_T* fc = NULL;
      BlockInfo* fb = bn.find_block (sys, blk);
      if (fb)
	{
	  function_map_T& fmap = decl ? fb->func_decl () : fb->func_defn ();
	  function_map_T::iterator m = fmap.add (fname, "");
	  function_code_T& fct = m->second;
	  if (fk != function_code_T::FK_NONE)
	    fct.kind () = fk;
	  fct.line (com_line);
	  fc = &fct;
	}

#if PARSE_CODE_DEBUG
      return check_block_func_result (fb, fc, sys, blk);
#else
      return check_block_func_result (fb, fc);
#endif
    }


    int
    parse_code (Blocks& bn, void* addr, size_t size)
    {
      const char* line = static_cast<const char*> (addr);
      const char* end = line + size;
      int ret = -1;

      std::string& cname = bn.source_filename ();
      if (cname.length () == 0)
	cname = "c-none";

      unsigned int lineno = 1;

      try
	{
	  std::tr1::regex fdecl ("^"
				 "[[:space:]]*"
				 "("			// %1 -- type
				 "[_[:alpha:]][_[:alnum:]]*"
				 "([[:space:]]+[_[:alpha:]][_[:alnum:]]*)*"
				 ")"
				 "[[:space:]]+"
				 "("			// %3 -- func
				 "[_[:alpha:]][_[:alnum:]]*"
				 ")"
				 "[[:space:]]*"
				 "\\("
				 "[[:space:]]*"
				 "([^()]*)"		// %4 --param
				 "[[:space:]]*"
				 "\\)"
				 "[[:space:]]*"
				 ";"
				 );

	  std::tr1::regex fdef ("^"
				"[[:space:]]*"
				"("			// %1 -- type
				"[_[:alpha:]][_[:alnum:]]*"
				"([[:space:]]+[_[:alpha:]][_[:alnum:]]*)*"
				")"
				"[[:space:]]+"
				"("			// %3 -- func
				"[_[:alpha:]][_[:alnum:]]*"
				")"
				"[[:space:]]*"
				"\\("
				"[[:space:]]*"
				"([^)]*)"		// %4 --param
				"[[:space:]]*"
				"\\)"
				"[[:space:]]*"
				"^"
				"(\\{)"			// %5 -- beg
				);

	  std::tr1::regex fcomm ("^/\\*"
				 "[[:space:]]+"

				 "(\\*[[:space:]]+)?" // %1

				 "("

				 // %3 -- initialize function
				 "("
				 "Start[[:space:]]+for[[:space:]]+"
				 "function-call"
				 "|"
				 "System[[:space:]]+initialize[[:space:]]+"
				 "for[[:space:]]+[^[:space:]]+"
				 ")"
				 "[[:space:]]+system"

				 "|"

				 // %4 -- task function
				 "("
				 "Function[[:space:]]+for[[:space:]]+"
				 "([^:]+)" // %5
				 "|"
				 "Output"
				 "([[:space:]]+and[[:space:]]+update)?" // %6
				 "[[:space:]]+for[[:space:]]+"
				 "[^[:space:]]+"
				 "[[:space:]]+system"
				 ")"

				 "|"

				 // %7 -- update function
				 "("
				 "Update[[:space:]]+for[[:space:]]+"
				 "[^[:space:]]+"
				 "[[:space:]]+system"
				 ")"

				 ")"

				 // %8(%9/%10)
				 "[[:space:]]*:[[:space:]]+"
				 "'?((<[^>/]+>)/([^':*/]+))'?"

				 "[[:space:]]+"
				 "(\\*/)?" // %11
				 );

	  std::tr1::regex fcomm_blk ("^/\\*"
				     "[[:space:]]+"

				     "(\\*[[:space:]]+)?" // %1

				     // %2
				     "(.*):"

				     "[[:space:]]+"

				     // %3(%4/%5)
				     "'?((<[^>/]+>)/([^':*/]+))'?"

				     "[[:space:]]+"
				     "(\\*/)?" // %6
				     );

	  std::tr1::cmatch mr;

	  const char* com_beg = NULL;
	  const char* com_end = NULL;
	  unsigned int com_line = 0;

	  while (line < end)
	    {
	      if (size >= 28
		  && strncmp ("/* Exported block signals */", line, 28) == 0)
		{
		  // skip this comment
		  line = nextline (line, size, lineno);
		  ret = parse_code_export (bn, line, size, lineno, 1);
		  if (ret)
		    goto end;
		  com_beg = com_end = NULL;
		  continue;
		}
	      if (size >= 27
		  && strncmp ("/* Exported block states */", line, 27) == 0)
		{
		  // skip this comment
		  line = nextline (line, size, lineno);
		  ret = parse_code_export (bn, line, size, lineno, 0);
		  if (ret)
		    goto end;
		  com_beg = com_end = NULL;
		  continue;
		}
	      if (size >= 25
		  && strncmp ("/* Model step function */", line, 25) == 0)
		{
		  // skip this comment
		  line = nextline (line, size, lineno);
		  ret = parse_code_step_func (bn, line, size, lineno);
		  if (ret)
		    goto end;
		  com_beg = com_end = NULL;
		  continue;
		}
	      if (size >= 31
		  && strncmp ("/* Model initialize function */", line, 31) == 0)
		{
		  // skip this comment
		  line = nextline (line, size, lineno);
		  ret = parse_code_initialize (bn, line, size, lineno);
		  if (ret)
		    goto end;
		  com_beg = com_end = NULL;
		  continue;
		}
	      if (size >= 30
		  && strncmp ("/* Model terminate function */", line, 30) == 0)
		{
		  line = nextline (line, size, lineno);
		  while (line < end)
		    {
		      const char* p = line;
		      line = nextline (line, size, lineno);
		      if (*p == '}')
			break;
		    }
		  com_beg = com_end = NULL;
		  continue;
		}
	      if (size >= 46
		  && strncmp ("/* Previous zero-crossings "
			      "(trigger) states */", line, 46) == 0)
		{
		  // skip this comment
		  line = nextline (line, size, lineno);
		  ret = parse_code_trigger_state (bn, line, size, lineno);
		  if (ret)
		    goto end;
		  com_beg = com_end = NULL;
		  continue;
		}
	      if (size >= 21
		  && strncmp ("/* Real-time model */", line, 21) == 0)
		{
		  line = nextline (line, size, lineno);
		  ret = parse_code_real_time_model (bn, line, size, lineno);
		  if (ret)
		    goto end;
		  com_beg = com_end = NULL;
		  continue;
		}

	      if (size >= 23
		  && strncmp ("/* Named constants for ", line, 23) == 0)
		{
		  ret = parse_code_macros (bn, line, size, lineno);
		  if (ret)
		    goto end;
		  com_beg = com_end = NULL;
		  continue;
		}

	      // process common code (there are functions, includes)
	      unsigned int wl = lineno;
	      size_t s = end - line;
	      const char* n = nextline (line, s, wl);

	      if (std::tr1::regex_search (line, n, mr, inc_pat))
		{
		  const std::string& ifm = mr[1];
		  const std::string& inc = mr[2];
		  char m = ifm[0];

		  bn.add_include (cname, inc, m);

		  line = n;
		  size = end - line;
		  lineno = wl;
		  com_beg = com_end = NULL;
		  continue;
		}

	      const char* w = skip_space (line, n - line);

	      if (com_beg == NULL || (com_end < line))
		{
		  com_line = lineno;
		  unsigned int el = lineno;
		  com_beg = find_comment (line, end - line, com_end,
					  com_line, el);
		}

	      // must be check first function declaration
	      if (std::tr1::regex_search (line, end, mr, fdecl))
		{
		  if (w == mr[0].first)
		    {
#if PARSE_CODE_DEBUG
		      std::cout << "<<< " << lineno << " Function Decl"
				<< std::endl;
#endif
		      const char* pt = line;
		      unsigned int pl = lineno;
		      BlockInfo* fb = NULL;
		      function_code_T* fbf = NULL;
#if PARSE_CODE_DEBUG
		      std::string sys;
		      std::string blk;
#endif

		      // just has previous comment, include it.
		      if (com_end == pt)
			{
			  pt = com_beg;
			  pl = com_line;

			  check_block_func_result
			    bf = check_block_function (bn, mr[3],
						       com_beg, com_end,
						       com_line,
						       fcomm, fcomm_blk,
						       true);
			  fb = bf.bi;
			  fbf = bf.fc;
#if PARSE_CODE_DEBUG
			  sys = bf.sys;
			  blk = bf.blk;
#endif
			}

		      while (line < mr[0].second)
			line = nextline (line, size, lineno);

		      function_map_T::iterator
			m = bn.function_decl ().add (mr[3], pt, line);

		      function_code_T& fct = m->second;
		      fct.line (pl);

		      if (fb)
			{
			  fct.blocks ().addinfo (fb);
			  fct.kind () = fbf->kind ();
			}

#if PARSE_CODE_DEBUG
		      if (fb)
			std::cout << "# " << sys << '/' << blk
				  << '(' << fbf->kind_str () << ')' << '\n';
		      std::cout << m->second << ">>> " << std::endl;
#endif
		      continue;
		    }
		}
	      else if (std::tr1::regex_search (line, end, mr, fdef))
		{
		  if (w == mr[0].first)
		    {
#if PARSE_CODE_DEBUG
		      std::cout << "<<< " << lineno << " Function Define"
				<< std::endl;
#endif
		      const char* pt = line;
		      unsigned int pl = lineno;
		      BlockInfo* fb = NULL;
		      function_code_T* fbf = NULL;
#if PARSE_CODE_DEBUG
		      std::string sys;
		      std::string blk;
#endif

		      while (line < mr[0].second)
			line = nextline (line, size, lineno);

		      // just has previous comment, include it.
		      if (com_end == pt)
			{
			  pt = com_beg;
			  pl = com_line;

			  check_block_func_result
			    bf = check_block_function (bn, mr[3],
						       com_beg, com_end,
						       com_line,
						       fcomm, fcomm_blk,
						       false);
			  fb = bf.bi;
			  fbf = bf.fc;
#if PARSE_CODE_DEBUG
			  sys = bf.sys;
			  blk = bf.blk;
#endif
			}

		      while (line < end)
			{
			  const char* x = line;
			  line = nextline (line, size, lineno);
			  if (*x == '}')
			    break;
			}

		      function_map_T::iterator
			m = bn.function_defn ().add (mr[3], pt, line);

		      function_code_T& fct = m->second;
		      fct.line (pl);

		      if (fb)
			{
			  fct.blocks ().addinfo (fb);
			  fct.kind () = fbf->kind ();
			}

#if PARSE_CODE_DEBUG
		      if (fb)
			std::cout << "# " << sys << '/' << blk
				  << '(' << fbf->kind_str () << ')' << '\n';
		      std::cout << m->second << ">>> " << std::endl;
#endif
		      continue;
		    }
		}

	      line = nextline (line, size, lineno);
	    }

	  bn.parse_complete ();
	}

      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	  return -1;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	  return -1;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	  return -1;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	  return -1;
	}

    end:
      return ret;
    }


    int
    parse_code (Blocks& bn, int fd)
    {
      size_t size;
      void* addr = mapfile (fd, size);
      if (addr == NULL)
	return -1;

      void* start = NULL;
      FileCoding fc = check_file_coding (addr, start);
      if (fc != FileCoding::FC_U8 && fc != FileCoding::FC_NONE)
	{
	  std::cerr << "Error: Invalid CodingSystem: " << FileCodingName[fc]
		    << std::endl;
	  (void)unmapfile (addr, size);
	  return -1;
	}

      int ret = parse_code (bn, start, size);
      (void)unmapfile (addr, size);
      return ret;
    }


    int
    parse_code (Blocks& bn, const char* file)
    {
      size_t size;
      void* addr = mapfile (file, size);
      if (addr == NULL)
	return -1;

      void* start = NULL;
      FileCoding fc = check_file_coding (addr, start);
      if (fc != FileCoding::FC_U8 && fc != FileCoding::FC_NONE)
	{
	  std::cerr << "Error: " << file
		    << ": Invalid CodingSystem: " << FileCodingName[fc]
		    << std::endl;
	  (void)unmapfile (addr, size);
	  return -1;
	}

      bn.set_filename (Blocks::XF_C_SOURCE, file);
      bn.source_filename () = file;

      int ret = parse_code (bn, start, size);
      (void)unmapfile (addr, size);
      return ret;
    }

    //
    // _data.c (parameters/Constant parameters initializer)
    //

    void
    c_param_value_check (const char* p,
			 int& in_comment, int& in_block,
			 int& has_value, int& has_comma, int& has_comment)
    {
      // checking
      while (*p)
	{
	  if (in_comment)
	    {
	      if (p[0] == '*' && p[1] == '/')
		{
		  in_comment = 0;
		  p++;
		}
	      p++;
	      continue;
	    }

	  if (p[0] == '/' && p[1] == '*')
	    {
	      in_comment = 1;
	      has_comment = 1;
	      p += 2;
	      continue;
	    }

	  if (*p == '{')
	    {
	      in_block++;
	      p++;
	      continue;
	    }

	  if (*p == '}')
	    {
	      in_block--;
	      p++;
	      continue;
	    }

	  if (*p != ',' && !isspace (*p))
	    has_value = 1;

	  if (in_block == 0 && *p == ',')
	    has_comma = 1;
	  p++;
	}
    }

    bool
    set_init_value (VariableEnt* ve, const std::string& ival)
    {
      unsigned const char* ucp = (unsigned const char*)ival.c_str ();
      int in_comment = 0;
      char buf[ival.length () + 1];
      char *bp = buf;
      bool has_last_comma = false;

      while (*ucp)
	{
	  unsigned char uc = *ucp++;
	  if (isspace (uc))
	    continue;
	  if (in_comment)
	    {
	      if (uc == '*' && ucp[0] == '/')
		{
		  ucp++;
		  in_comment--;
		}
	      continue;
	    }
	  else
	    {
	      if (uc == '/' && ucp[0] == '*')
		{
		  ucp++;
		  in_comment++;
		  continue;
		}

	      if (uc == '/' && ucp[0] == '/')
		{
		  while (*++ucp && *ucp == '\n')
		    ;
		  continue;
		}
	    }

	  *bp++ = uc;
	}
      if (bp != buf && *(bp-1) == ',')
	{
	  bp--;
	  has_last_comma = true;
	}
      *bp = 0;
      ve->init () = buf;
      return has_last_comma;
    }

    int
    parse_data (Blocks& bn, std::istream& is, const char* filename)
    {
      try {
	char buf[1024];
	enum {
	  PARSE_DATA_NONE,
	  PARSE_DATA_PARAMS,
	  PARSE_DATA_IN_PARAMS,
	  PARSE_DATA_CONSTPARAMS,
	  PARSE_DATA_IN_CONSTPARAMS,
	  PARSE_DATA_INVARIANTS,
	  PARSE_DATA_IN_INVARIANTS,
	} state = PARSE_DATA_NONE;

	std::string param_pattern;
	std::string const_param_pattern;
	std::string invariant_pattern;

	if (bn.param_type().length())
	  {
	    param_pattern = "^"
	      "[[:space:]]*"
	      + bn.param_type()
	      + "[[:space:]]*"
	      + bn.param_name()
	      + "[[:space:]]*" "=" "[[:space:]]*" "\\{";
	  }
	else
	  param_pattern = "= = = u n d e f i n e d = = =";

	if (bn.constant_param_type().length())
	  {
	    const_param_pattern = "^"
	      "[[:space:]]*" "const" "[[:space:]]*"
	      + bn.constant_param_type()
	      + "[[:space:]]*"
	      + bn.constant_param_name()
	      + "[[:space:]]*" "=" "[[:space:]]*" "\\{";
	  }
	else
	  const_param_pattern = "= = = u n d e f i n e d = = =";

	if (bn.invariant_signal_type().length())
	  {
	    invariant_pattern = "^"
	      "[[:space:]]*" "const" "[[:space:]]+"
	      + bn.invariant_signal_type()
	      + "[[:space:]]+"
	      + bn.invariant_signal_name()
	      + "[[:space:]]*" "=" "[[:space:]]*" "\\{";
	  }
	else
	  invariant_pattern = "= = = u n d e f i n e d = = =";

	std::tr1::regex paramdef (param_pattern);
	std::tr1::regex constparamdef (const_param_pattern);
	std::tr1::regex invariantdef (invariant_pattern);

	std::tr1::regex refdef("\\*"
			       "[[:space:]]*"
			       "Referenced[[:space:]]+by:"
			       "("		// -- %1 (single block)
			       "[[:space:]]+"
			       "'"
			       "("		// -- %2 <Sys>/block
			       "(<[^>]+>)"	// -- %3 <Sys>
			       "/"
			       "([^':*/]+)"	// -- %4 block
			       ")"		// -- %2 (close)
			       "'"
			       ")?"		// -- %1 (close)
			       );

	std::tr1::regex msysdef ("\\*"
				 "[[:space:]]*"
				 "'"
				 "("		// -- %1 <Sys>/block
				 "(<[^>]+>)"	// -- %2 <Sys>
				 "/"
				 "([^':*/]+)"	// -- %3 block
				 ")"		// -- %1 (close)
				 "'"
				 );

	std::tr1::regex sysdef("/\\*"
			       "[[:space:]]*"
			       "(Start|End)"	// -- %1
			       "[[:space:]]*"
			       "of"
			       "[[:space:]]*"
			       "'"
			       "("		// -- %2 <Sys>/block
			       "(<[^>]+>)"	// -- %3 <Sys>
			       "/"
			       "([^':*/]+)"	// -- %4 block
			       ")"		// -- %2
			       "'"
			       "[[:space:]]*"
			       "\\*/"
			       );


	std::tr1::cmatch cmr;
	std::tr1::smatch smr;

	var_list_const_iterator pi (bn.param_order().begin());
	var_list_const_iterator cpi (bn.constant_param_order().begin());
	var_list_const_iterator isi (bn.invariant_signal_order().begin());
	var_list_const_iterator pend (bn.param_order().end());
	var_list_const_iterator cpend (bn.constant_param_order().end());
	var_list_const_iterator isend (bn.invariant_signal_order().end());
	int in_block = 0;
	int in_comment = 0;
	int has_comma = 0;
	int has_value = 0;
	int has_comment = 0;
	int has_sysdef = 0;

	std::string value;

	unsigned int lineno = 0;
	unsigned int mark_lineno = 0;

	std::string system;
	std::string block;
	int valid = 0;

	bn.set_filename (Blocks::XF_C_DATA, filename);

	bool multi = false;

	sysblk_list_T msystem;

	while (!is.eof ())
	  {
	    char *text;
	    is.getline (buf, sizeof buf);
	    if (lineno++ == 0)
	      {
		void* ptr;
		FileCoding fc = check_file_coding (buf, ptr);
		if (fc != FileCoding::FC_U8 && fc != FileCoding::FC_NONE)
		  {
		    std::cerr << "Error: " << filename
			      << ": Invalid CodingSystem: "
			      << FileCodingName[fc]
			      << std::endl;
		    return -1;
		  }
		text = static_cast< char* > (ptr);
	      }
	    else
	      text = buf;

	    if (state == PARSE_DATA_NONE)
	      {
		if (strncmp (text,
			     "/* Block parameters (auto storage) */",
			     37) == 0)
		  state = PARSE_DATA_PARAMS;
		else if (strncmp (text,
				  "/* Constant parameters (auto storage) */",
				  40) == 0)
		  state = PARSE_DATA_CONSTPARAMS;
		else if (strncmp (text,
				  "/* Invariant block signals "
				  "(auto storage) */",
				  44) == 0)
		  state = PARSE_DATA_INVARIANTS;
	      }
	    else if (state == PARSE_DATA_PARAMS)
	      {
		if (std::tr1::regex_search (text, cmr, paramdef))
		  state = PARSE_DATA_IN_PARAMS;
		else
		  {
		    std::cerr << "Error: no match parameters"
			      << std::endl;
		    return -1;
		  }
	      }
	    else if (state == PARSE_DATA_CONSTPARAMS)
	      {
		if (std::tr1::regex_search (text, cmr, constparamdef))
		  state = PARSE_DATA_IN_CONSTPARAMS;
		else
		  {
		    std::cerr << "Error: no match Constant parameters"
			      << std::endl;
		    return -1;
		  }
	      }
	    else if (state == PARSE_DATA_INVARIANTS)
	      {
		if (std::tr1::regex_search (text, cmr, invariantdef))
		  state = PARSE_DATA_IN_INVARIANTS;
		else
		  {
		    std::cerr << "Error: no match Invariant block signals"
			      << std::endl;
		    return -1;
		  }
	      }
	    else if ((state == PARSE_DATA_IN_PARAMS
		      || state == PARSE_DATA_IN_CONSTPARAMS
		      || state == PARSE_DATA_IN_INVARIANTS)
		     && (text[0] == '}' && text[1] == ';'))
	      state = PARSE_DATA_NONE;
	    else if (state == PARSE_DATA_IN_PARAMS
		     || state == PARSE_DATA_IN_CONSTPARAMS
		     || state == PARSE_DATA_IN_INVARIANTS)
	      {
		if (std::tr1::regex_search (text, cmr, sysdef))
		  {
		    if (cmr[1] == "Start")
		      has_sysdef++;
		    else if (cmr[1] == "End")
		      {
			if (has_sysdef > 0 && --has_sysdef == 0)
			  {
			    has_sysdef = -1;
			    has_value = 1;
			    has_comma = 0;
			    system = cmr[3];
			    block = cmr[4];
			    valid = 1;
			  }
		      }
		  }

		if (has_sysdef <= 0)
		  {
		    // checking value
		    c_param_value_check (text,
					 in_comment, in_block,
					 has_value, has_comma, has_comment);

		    // empty
		    if (has_value == 0 && has_comment == 0 && in_block == 0)
		      continue;

		    // incomplete: `invariant block signals' only...
		    // (comment is next line...)
		    if (state == PARSE_DATA_IN_INVARIANTS
			&& has_value == 1
			&& has_comment == 0)
		      {
			if (value.length() == 0)
			  mark_lineno = lineno;
			value += text;
			value += "\n";
			continue;
		      }
		  }

		if (value.length () == 0)
		  mark_lineno = lineno;
		value += text;
		value += "\n";

		var_list_const_iterator&
		  vi = (state == PARSE_DATA_IN_PARAMS
			? pi : (state == PARSE_DATA_IN_CONSTPARAMS
				? cpi : isi));

		var_list_const_iterator&
		  vend = (state == PARSE_DATA_IN_PARAMS
			  ? pend : (state == PARSE_DATA_IN_CONSTPARAMS
				    ? cpend : isend));

		// no more value
		if (vi == vend)
		  break;

		if (has_value == 1 && in_block == 0 && in_comment == 0)
		  {
		    if (has_sysdef == 0
			&& std::tr1::regex_search (value, smr, refdef))
		      {
			if (smr[1].matched)
			  {
			    system = smr[3];
			    block = smr[4];
			    valid = 1;
			    multi = false;
			  }
			else
			  {
			    multi = true;
			    msystem.clear ();

			    const char *vb = value.c_str ();
			    const char *ve = vb + value.length ();

			    while (vb < ve)
			      {
				if (!std::tr1::regex_search (vb, ve, cmr,
							     msysdef))
				  break;

				msystem.push_back (std::make_pair (cmr[2],
								   cmr[3]));
				vb = cmr[0].second;
			      }

			    valid = 1;
			  }
		      }

		    if (valid)
		      {
			if (multi == false)
			  {
			    if ((*vi)->system() != system
				|| (*vi)->block() != block)
			      {
				std::cout << "Warning: "
					  << (*vi)->system() << "/"
					  << (*vi)->block() << " not matched "
					  << system << "/" << block
					  << std::endl;
			      }
			  }
			else
			  {
			    multiple_system_T&
			      ms = (*vi)->multiple_system ();
			    multiple_system_iterator ib (ms.begin ());
			    multiple_system_iterator ie (ms.end ());

			    sysblk_list_T::iterator jb (msystem.begin ());
			    sysblk_list_T::iterator je (msystem.end ());

			    size_t vn = ms.size ();
			    size_t sn = msystem.size ();

			    if (vn != sn)
			      {
				std::cerr << "Error:"
					  << lineno
					  << ": different system count of "
				  "Variable/Data: "
					  << std::endl;
			      }

			    for (; ib != ie; ib++)
			      {
				std::string& vsys = ib->system ();
				std::string& vblk = ib->block ();
				int found = 0;

				for (sysblk_list_T::iterator sl (jb);
				     sl != je; sl++)
				  {
				    std::string& dsys = sl->first;
				    std::string& dblk = sl->second;

				    if (vsys == dsys && vblk == dblk)
				      {
					found = 1;
					break;
				      }
				  }

				if (found == 0)
				  {
				    std::cout << "Warning: "
					      << vsys << "/"
					      << vblk << " not found in data"
					      << std::endl;
				  }
			      }
			  }
		      }

		    // no last comma
		    if (set_init_value (*vi, value) == false)
		      {
			std::string::iterator p = value.end ();
			if (*--p == '\n')
			  value.insert (p, ',');
			else
			  value += "  ,\n"; // XXX
		      }

		    // init data for variable
		    (*vi)->code () = value;
		    (*vi)->codeline () = mark_lineno;

		    valid = 0;
		    has_sysdef = 0;

		    multiple_system_T tmplist;
		    multiple_system_T* runlist;

		    if ((*vi)->multiple_system ().size ())
		      runlist = &(*vi)->multiple_system ();
		    else
		      {
			const SystemBlockEnt s ((*vi)->system (),
						(*vi)->block (),
						(*vi)->blockinfo ());
			tmplist.push_back (s);
			runlist = &tmplist;
		      }

		    for (multiple_system_iterator x (runlist->begin ());
			 x != runlist->end (); x++)
		      {
			std::string& vsys = x->system ();
			std::string& vblk = x->block ();
			BlockInfo* binfo = bn.find_block (vsys, vblk);
			if (binfo == NULL)
			  {
			    const char* o;
			    if (state == PARSE_DATA_IN_PARAMS)
			      o = "param";
			    else if (state == PARSE_DATA_IN_CONSTPARAMS)
			      o = "const param";
			    else
			      o = "invariant signal block";

			    std::cerr << "Warning: unknown block "
				      << vsys << "/"
				      << vblk << " for "
				      << o
				      << ": "
				      << (*vi)->type() << ' '
				      << (*vi)->ptr() << (*vi)->name()
				      << (*vi)->array() << std::endl;
			  }
		      }

		    multi = false;
		    vi++;

		    has_comma = 0;
		    has_value = 0;
		    has_comment = 0;
		    value.clear();
		  }
	      }
	  }

	return 0;
      }
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}
      return -1;
    }

    int
    parse_data (Blocks& bn, const char* file)
    {
      std::ifstream ifs (file);
      if (!ifs)
	{
	  std::cerr << "Error: Couldn't open " << file
		    << ": " << strerror (errno) << std::endl;
	  return -1;
	}
      return parse_data (bn, ifs, file);
    }

    Blocks_PTR
    parse_all_code (const char* xml,
		    const char* header, const char* source, const char* data,
		    bool validation)
    {
      if (xml == NULL || header == NULL || source == NULL)
	return Blocks_PTR();

      blocks_T_PTR blks;

      try {
	if (validation)
	  blks.reset (blocks (xml).release ());
	else
	  blks.reset (blocks (xml,
			      xml_schema::flags::dont_validate).release ());
      }
      catch (const xml_schema::exception& e)
	{
	  std::cerr << e << std::endl;
	  return Blocks_PTR();
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what () << std::endl;
	  return Blocks_PTR();
	}
      catch (const boost::exception& e)
	{
	  std::cerr << "boost::exception Error in "
		    << __FUNCTION__ << std::endl;
	  return Blocks_PTR();
	}
      catch (...)
	{
	  std::cerr << "Error: XML " << xml << std::endl;
	  return Blocks_PTR();
	}

      Blocks_PTR bn (parse_header (header, blks));
      if (bn.get () == NULL)
	return Blocks_PTR();

      bn->set_filename (Blocks::XF_XML, xml);

      if (parse_code (*bn, source))
	return Blocks_PTR();

      if (data)
	{
	  if (parse_data (*bn, data))
	    return Blocks_PTR();
	}
      if (bn->has_dimensions ())
	{
	  bn->track_bus (Blocks::TRACK_BUS_FLOW);
	  bn->change_externals ();
	}
      else
	bn->trace_flow ();
      bn->xml_complete ();
      return bn;
    }

    void
    c_canonical_name (std::string& out, const std::string& in)
    {
      c_canonical_name (out, in.c_str());
    }

    void
    c_canonical_name (std::string& out, const char* in)
    {
      char buf[strlen (in) + 1];
      char *s = buf;
      for (const char* p = in; *p; p++)
	{
	  if (isalnum ((unsigned char)*p) || *p == '_')
	    *s++ = *p;
	  // skip other charactors
	}
      *s = 0;
      out = buf;
    }

    void
    xml_canonicalize (std::string& x)
    {
      char z[x.length() + 1];
      char *q = z;
      int f = 0;

      for (const char *p = x.c_str(); *p; p++)
	{
	  if (*p == '_' || isalnum ((unsigned char)*p))
	    *q++ = *p;
	  else if (*p == '-')
	    *q++ = '_';
	  else
	    f = 1;
	}
      *q = 0;
      if (f)
	x = z;
    }

    void
    set_parse_option (unsigned int option)
    {
      parse_options |= option;
    }

    void
    clear_parse_option (unsigned int option)
    {
      parse_options &= ~option;
    }

    FileCoding
    check_file_coding (void* addr, void*& skiped)
    {
      unsigned char* s = static_cast < unsigned char* > (addr);

      // UTF-8
      if (s[0] == 0xEF && s[1] == 0xBB && s[2] == 0xBF)
	{
	  skiped = s + 3;
	  return FileCoding::FC_U8;
	}

      // UTF-32 Big Endian
      if (s[0] == 0x00 && s[1] == 0x00 && s[2] == 0xFE && s[3] == 0xFF)
	{
	  skiped = s + 4;
	  return FileCoding::FC_U32B;
	}

      // UTF-16 Big Endian
      if (s[0] == 0xFE && s[1] == 0xFF)
	{
	  skiped = s + 2;
	  return FileCoding::FC_U16B;
	}

      // Little Endian...
      if (s[0] == 0xFF && s[1] == 0xFE)
	{
	  // UTF-32 Little Endian
	  if (s[2] == 0x00 && s[3] == 0x00)
	    {
	      skiped = s + 4;
	      return FileCoding::FC_U32L;
	    }

	  // other UTF-16 Little Endian
	  skiped = s + 2;
	  return FileCoding::FC_U16L;
	}

      // none or unknown
      skiped = addr;
      return FileCoding::FC_NONE;
    }
  }
}

//
// OS depended File Mapping function
//

#ifdef _WIN32			// test only MinGW
#include <windows.h>
#include <io.h>
#else
#include <sys/mman.h>
#endif

namespace Simulink
{
  namespace RTW
  {
    void*
    mapfile (int fd, size_t& size)
    {
      struct stat st;

      if (fstat (fd, &st) == -1)
	{
	  close (fd);
	  return NULL;
	}

      size = st.st_size;

#ifdef _WIN32			// test only MinGW
      if (size == 0)
	return NULL;

      HANDLE h = (HANDLE)_get_osfhandle (fd);
      if (h == INVALID_HANDLE_VALUE)
	return NULL;

      DWORD maxl = (DWORD)size;
#ifdef _WIN64			// not yet test
      DWORD maxh = (DWORD)(size >> 32);
#else
      DWORD maxh = 0;
#endif
      HANDLE m = CreateFileMapping (h, NULL, PAGE_READONLY, maxh, maxl, NULL);
      if (m == INVALID_HANDLE_VALUE)
	{
	  std::cerr << "CreateFileMapping: Error" << std::endl;
	  return NULL;
	}

      void* addr = MapViewOfFile (m, FILE_MAP_READ, 0, 0, size);
      if (addr == NULL)
	  std::cerr << "MapViewOfFile: Error" << std::endl;
#else
      void* addr = mmap (NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);

      if (addr == NULL)
	{
	  std::cerr << "mmap: " << std::strerror (errno) << std::endl;
	}
#endif

      close (fd);

      return addr;
    }

    void*
    mapfile (const char* f, size_t& size)
    {
      int fd;

      fd = open (f, O_RDONLY);

      if (fd == -1)
	{
	  std::cerr << "Error: Couldn't open " << f << ": "
		    << std::strerror (errno) << std::endl;
	  return NULL;
	}

      return mapfile (fd, size);
    }

    int
    unmapfile (void *addr, size_t size)
    {
#ifdef _WIN32
      return !UnmapViewOfFile(addr) ? 0 : -1;
#else
      return munmap (addr, size);
#endif
    }
  }
}
