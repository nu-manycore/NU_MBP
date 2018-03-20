// -*- mode: c++; coding: utf-8-unix; -*-
#ifndef SIMULINKRTW_H
#define SIMULINKRTW_H

#ifdef __cplusplus
#include <vector>
#include <fstream>
#include <string>
#include <memory>
#include <algorithm>
#include <list>
#include <deque>

#include "SimulinkModel.hxx"

#if __cplusplus >= 201103L && !defined(XSD_CXX11)
#error "Not defined XSD_CXX11 in SimulinkModel.hxx, please Re-make it"
#elif __cplusplus < 201103L && defined (XSD_CXX11)
#error "defined XSD_CXX11 in SimulinkModel.hxx, please Re-make it"
#endif

#include "SimulinkModel_ns.h"

/* parse options */
#define PARSE_OPTION_CUT_TRIGGERED_SUBSYSTEM	0x00000001
#define PARSE_OPTION_CUT_ATOMIC_SUBSYSTEM	0x00000002

namespace Simulink
{
  namespace RTW
  {
    template<class PTR, class ANY = int>
    class pointer_wrapper
    {
    public:
      pointer_wrapper (PTR* v) :  pw_ptr (v), pw_used (false) {}
      pointer_wrapper (const pointer_wrapper<PTR,ANY>& pw)
	: pw_ptr (pw.pw_ptr), pw_used (pw.pw_used), pw_any (pw.pw_any) {}

      bool is_used (void) const { return pw_used; }
      void used (bool u) { pw_used = u; }
      const ANY& any (void) const { return pw_any; }
      ANY& any (void) { return pw_any; }
      void any (ANY& a) { pw_any = a; }

      operator PTR* () const { return pw_ptr; }
      PTR& operator* () const { return *pw_ptr; }
      PTR* operator-> () const { return pw_ptr; }

    private:
      PTR* pw_ptr;		// real pointer
      bool pw_used;		// use flag
      ANY pw_any;		// with a any data
    };

    class code_frag_T
    {
    public:
      code_frag_T (int n = -1, unsigned int l = 0)
	: c_index (n), c_line (l) {}

      bool operator== (int n) const { return c_index == n; }

      int index (void) const { return c_index; }
      int& index (void) { return c_index; }

      unsigned int line (void) const { return c_line; }
      unsigned int& line (void) { return c_line; }

      const std::string& frag (void) const { return c_frag; }
      std::string& frag (void) { return c_frag; }

    private:
      int c_index;
      unsigned int c_line;
      std::string c_frag;
    };

    class CodeFrags : public std::list < code_frag_T >
    {
    public:
      CodeFrags () {}

      code_frag_T&
      create (int n = -1, unsigned int l = 0)
      {
	push_back (code_frag_T (n, l));
	return back ();
      }
    };

    class BlockInfo;
    class BlockNameEnt;

    class BlockNameEntPList
      : public std::vector < BlockNameEnt* >
    {
    };

    class BlockNameEnt
    {
    public:

      BlockNameEnt();

      BlockNameEnt(const char* name, const char* layer,
		   const char* symbol = NULL);

      ~BlockNameEnt();

      void set (const char* , const char* , const char* = NULL);

      const std::string&
      blockname (void) const
      {
	return bn;
      }

      const std::string&
      layer (void) const
      {
	return ln;
      }

      const std::string&
      symbol (void) const
      {
	return sn;
      }

      std::string&
      blockname (void)
      {
	return bn;
      }

      std::string&
      layer (void)
      {
	return ln;
      }

      std::string&
      symbol (void)
      {
	return sn;
      }

      bool reference (void) const { return ref; }
      void reference (bool f) { ref = f; }

      BlockNameEnt* set_top (void) const { return top; }
      void set_top (BlockNameEnt* bne) { top = bne; }

      BlockNameEnt* upper (void) const { return ubne; }
      void upper (BlockNameEnt* u) { ubne = u; }

      const BlockNameEntPList& lowers (void) const { return lbnel; }
      BlockNameEntPList& lowers (void) { return lbnel; }

      BlockInfo* blockinfo (void) const { return bi; }
      void blockinfo (BlockInfo* b) { bi = b; }

      const BlockNameEnt*
      find_top (void) const
      {
	if (bi)
	  return this;
	// assert (top);
	return top;
      }

      BlockNameEnt*
      find_top (void)
      {
	if (bi)
	  return this;
	// assert (top);
	return top;
      }

      void dump (void) const;

    protected:

    private:

      std::string bn;		// block name
      std::string ln;		// layer name
      std::string sn;		// symbol name

      BlockNameEnt* top;	// Reference Top
      bool ref;			// (Inherited)Reference block

      BlockNameEnt* ubne;	// upper BlockNameEnt
      BlockNameEntPList lbnel;	// lowers BlockNameEnt List

      BlockInfo* bi;		// BlockInfo if defined
    };

    typedef std::map< std::string, BlockNameEnt > blockname_map;
    typedef blockname_map::iterator blockname_map_iterator;
    typedef blockname_map::const_iterator blockname_map_const_iterator;

    class VariableEnt;
    class oldBusEnt;

    class var_entp : public pointer_wrapper < VariableEnt >
    {
    public:
      var_entp (VariableEnt* v) : pointer_wrapper < VariableEnt > (v) {}
    };

    // typedef std::vector< VariableEnt* > var_list;
    class var_list : public std::vector < var_entp >
    {
    public:
      void push_back (VariableEnt *ve); // don't use (for link error)
      var_entp&
      add_varent (VariableEnt *ve)
      {
	var_list::iterator i = std::find (begin (), end (), ve);
	if (i != end ())
	  return *i;
	std::vector < var_entp >::push_back (var_entp (ve));
	return back ();
      }
    };
    typedef var_list::iterator var_list_iterator;
    typedef var_list::const_iterator var_list_const_iterator;

    class blockinfo_list : public std::vector < BlockInfo* >
    {
    public:
      blockinfo_list () {}
      void addinfo (BlockInfo* b)
      {
	iterator i = std::find (begin (), end (), b);
	if (i == end ())
	  push_back (b);
      }

      bool isexist (const BlockInfo* b) const
      {
	return std::find (begin (), end (), b) != end ();
      }
    };

    typedef blockinfo_list::iterator blockinfo_list_iterator;
    typedef blockinfo_list::const_iterator blockinfo_list_const_iterator;

    class VarElement;

    // typedef std::vector< VarElement* > pvar_ptr_list;
    class pvar_ptr_list : public std::vector < VarElement* >
    {
    public:
      iterator vfind (iterator b, iterator e, const VarElement& ve);
      const_iterator vfind (const_iterator b, const_iterator e,
			    const VarElement& ve) const;

      iterator vfind (const VarElement& ve)
      {
	return vfind (begin (), end (), ve);
      }

      const_iterator vfind (const VarElement& ve) const
      {
	return vfind (begin (), end (), ve);
      }

      iterator pvfind (iterator b, iterator e, const VarElement* ve)
      {
	return std::find (b, e, ve);
      }

      const_iterator pvfind (const_iterator b, const_iterator e,
			     const VarElement* ve) const
      {
	return std::find (b, e, ve);
      }

      iterator pvfind (const VarElement* ve)
      {
	return pvfind (begin (), end (), ve);
      }

      const_iterator pvfind (const VarElement* ve) const
      {
	return pvfind (begin (), end (), ve);
      }

      void add_pvar (VarElement* pve)
      {
	if (pvfind (pve) == end ())
	  push_back (pve);
      }
    };
    typedef pvar_ptr_list::iterator pvar_ptr_list_iterator;
    typedef pvar_ptr_list::const_iterator pvar_ptr_list_const_iterator;

    class VarElement
    {
    public:
      VarElement();
      VarElement (const std::string& name, int beg, int end,
		  const std::string& mode, const std::string& block);
      VarElement (const std::string& name, int beg, int end,
		  const std::string& mode, const std::string& block,
		  const std::string& line, const std::string& port,
		  VariableEnt* varent = NULL);

      VarElement (const oldBusEnt& be, const std::string& mode);

      const std::string&
      name (void) const
      {
	return v_name;
      }

      std::string&
      name (void)
      {
	return v_name;
      }

      const std::string&
      mode (void) const
      {
	return v_mode;
      }

      std::string&
      mode (void)
      {
	return v_mode;
      }

      const std::string&
      block (void) const
      {
	return v_block;
      }

      std::string&
      block (void)
      {
	return v_block;
      }

      int&
      start (void)
      {
	return v_start;
      }

      int
      start (void) const
      {
	return v_start;
      }

      int&
      end (void)
      {
	return v_end;
      }

      int
      end (void) const
      {
	return v_end;
      }

      const var_list&
      varent (void) const
      {
	return v_varent;
      }

      var_list&
      varent (void)
      {
	return v_varent;
      }

      const std::string&
      line (void) const
      {
	return v_line;
      }

      std::string&
      line (void)
      {
	return v_line;
      }

      const std::string&
      port (void) const
      {
	return v_port;
      }

      std::string&
      port (void)
      {
	return v_port;
      }

      bool switched (void) const { return v_switched; }
      bool& switched (void) { return v_switched; }

      int index (void) const { return v_index; }
      int& index (void) { return v_index; }

      int parts (void) const { return v_parts; }
      int& parts (void) { return v_parts; }

      int nchunk (void) const { return v_nchunk; }
      int& nchunk (void) { return v_nchunk; }

      void switch_to_range (const VariableEnt* vent);

      void dump (int ind = 0);

      // for std::find
#if 0
      bool operator== (const VariableEnt* ve) const;
#endif
      bool operator== (const VarElement& v) const;

      // make var tag
      SimulinkModel::XSD::var_T make_var_tag (const VariableEnt*) const;

      const pvar_ptr_list& velist (void) const { return v_velist; }
      pvar_ptr_list& velist (void) { return v_velist; }

      bool
      is_same_name (const VarElement& v) const
      {
	return v.name () == name ();
      }

      bool
      is_same_port (const VarElement& v) const
      {
	return (v.line ().length () > 0
		&& v.line () == line ()
		&& v.port ().length () > 0
		&& v.port () == port ());
      }

    private:
      std::string v_name;	// attribute line in output tag of XML
      int v_start;		// start of range
      int v_end;		// end of range
      std::string v_mode; // "extin", "extout", "input", "output", ...
      std::string v_block;
      std::string v_line;
      std::string v_port;
      var_list v_varent;
      bool v_switched;		// switched to range
      int v_index;		// an index of partitions
      int v_nchunk;		// number of chunks
      int v_parts;		// number of partitions

      pvar_ptr_list v_velist;	// output_var/input_var link list
    };

    class propagate_var_list : public std::list < VarElement >
    {
    public:
      void push_back (const VarElement& v); // for link error
      VarElement& add_var (const VarElement& v);
    };

    typedef propagate_var_list::iterator propagate_var_list_iterator;
    typedef propagate_var_list::const_iterator propagate_var_list_const_iterator;

    class SystemBlockEnt
    {
    public:
      SystemBlockEnt () : s_blockinfo (NULL) {}
      SystemBlockEnt (const std::string& sys, const std::string& blk,
		      BlockInfo* binfo = NULL)
	:  s_system (sys), s_block (blk), s_blockinfo (binfo)
      {
      }

      const std::string& system (void) const { return s_system; }
      std::string& system (void) { return s_system; }

      const std::string& block (void) const { return s_block; }
      std::string& block (void) { return s_block; }

      const BlockInfo* blockinfo (void) const { return s_blockinfo; }
      BlockInfo*& blockinfo (void) { return s_blockinfo; }

    private:
      std::string s_system;
      std::string s_block;
      BlockInfo* s_blockinfo;
    };

    typedef std::vector < SystemBlockEnt > multiple_system_T;
    typedef multiple_system_T::iterator multiple_system_iterator;
    typedef multiple_system_T::const_iterator multiple_system_const_iterator;

    class VariableEnt
    {
    public:
      enum var_mode
	{
	  V_AUTO,		// auto variables
	  V_VAR,		// variables
	  V_STATE,		// state variables
	  V_TRIGGER,		// trigger state varibales
	  V_INVARIANT,		// invariant block signals
	  V_PARAM,		// parameters
	  V_CONST,		// const parameters
	  V_SIGNAL,		// block signal
	  V_EXTIN,		// external inputs
	  V_EXTOUT		// external outputs
	};

      VariableEnt();

      VariableEnt(const std::string& t, const std::string& n,
		  const std::string& p, const std::string& a,
		  unsigned int sz, var_mode m = V_AUTO);

      VariableEnt(const std::string& t, const std::string& n,
		  const std::string& p, const std::string& a,
		  unsigned int sz,
		  const std::string& s, const std::string& b,
		  var_mode m = V_AUTO);

      ~VariableEnt();

      unsigned int
      id (void) const
      {
	return v_id;
      }

      const var_mode mode (void) const { return v_mode; }
      var_mode& mode (void) { return v_mode; }

      const std::string&
      type (void) const
      {
	return v_type;
      }

      const std::string&
      name (void) const
      {
	return v_name;
      }

      const std::string&
      ptr (void) const
      {
	return v_ptr;
      }

      const std::string&
      array (void) const
      {
	return v_array;
      }

      const std::string&
      system (void) const
      {
	return v_sys;
      }

      const std::string&
      block (void) const
      {
	return v_block;
      }

      bool
      is_ptr (void) const
      {
	return v_ptr[0] != 0;
      }

      bool
      is_array (void) const
      {
	return v_array[0] != 0;
      }

      std::string&
      type (void)
      {
	return v_type;
      }

      std::string&
      name (void)
      {
	return v_name;
      }

      std::string&
      ptr (void)
      {
	return v_ptr;
      }

      std::string&
      array (void)
      {
	return v_array;
      }

      std::string&
      system (void)
      {
	return v_sys;
      }

      std::string&
      block (void)
      {
	return v_block;
      }

      unsigned int
      array_size (void) const
      {
	return v_array_size;
      }

      unsigned int&
      array_size (void)
      {
	return v_array_size;
      }

      const BlockInfo*
      blockinfo (void) const
      {
	return v_binfo;
      }

      BlockInfo*&
      blockinfo (void)
      {
	return v_binfo;
      }

      const std::string&
      tag (void) const
      {
	return v_tag;
      }

      const std::string&
      obj (void) const
      {
	return v_obj;
      }

      std::string&
      tag (void)
      {
	return v_tag;
      }

      std::string&
      obj (void)
      {
	return v_obj;
      }

      const multiple_system_T&
      multiple_system (void) const
      {
	return v_multiple_system;
      }

      multiple_system_T&
      multiple_system (void)
      {
	return v_multiple_system;
      }

      const std::string&
      init (void) const
      {
	return v_init;
      }

      std::string&
      init (void)
      {
	return v_init;
      }

      const std::string&
      code (void) const
      {
	return v_code;
      }

      std::string&
      code (void)
      {
	return v_code;
      }

      int codeline (void) const
      {
	return v_codeline;
      }

      int& codeline (void)
      {
	return v_codeline;
      }

      void dump (void) const;

    private:

      void set(const std::string& t, const std::string& n,
	       const std::string& p, const std::string& a,
	       unsigned int z, var_mode m);

      void set(const std::string& t, const std::string& n,
	       const std::string& p, const std::string& a,
	       unsigned int z,
	       const std::string& s, const std::string& b,
	       var_mode m);

      static unsigned int seq;

      unsigned int v_id;

      // var / auto / state / param / const param
      enum var_mode v_mode;

      std::string v_type;
      std::string v_name;
      std::string v_ptr;
      std::string v_array;
      std::string v_sys;
      std::string v_block;

      unsigned int v_array_size;

      // State / Parameters / Constant Parameters
      std::string v_tag;
      std::string v_obj;

      BlockInfo* v_binfo;

      // Parameters / Constant Parameters?
      multiple_system_T v_multiple_system;

      // initial value
      std::string v_init;

      // initial code
      std::string v_code;
      int v_codeline;
    };

    typedef std::map< std::string, VariableEnt > c_variable_map;
    typedef c_variable_map::iterator c_variable_map_iterator;
    typedef c_variable_map::const_iterator c_variable_map_const_iterator;

    typedef std::map< std::string, const VariableEnt* > c_varentp_map;

    class BlockLink
    {
    public:
      enum link_type
	{
	  TYPE_NONE,
	  TYPE_PORT,
	  TYPE_EVENT,
	  TYPE_DATA,
	  TYPE_INIT
	};

      BlockLink ();

      BlockLink (BlockInfo *b, link_type t = TYPE_NONE);

      BlockLink (BlockInfo *b, std::string& l, link_type t = TYPE_NONE);

      ~BlockLink ();

      void
      type (link_type t)
      {
	l_type = t;
      }

      link_type
      type (void)
      {
	return l_type;
      }

      const std::string&
      type (void) const
      {
	return l_stype[l_type];
      }

      std::string&
      label (void)
      {
	return l_label;
      }

      const std::string&
      label (void) const
      {
	return l_label;
      }

      BlockInfo*&
      blockinfo (void)
      {
	return l_block;
      }

      const BlockInfo*
      blockinfo (void) const
      {
	return l_block;
      }

      // for algorithm (find)
      bool operator== (const BlockInfo* x) const;
      bool operator== (const BlockLink& x) const;

      const pvar_ptr_list&
      vars (void) const
      {
	return l_vars;
      }

      pvar_ptr_list&
      vars (void)
      {
	return l_vars;
      }

      const BlockLink* linkpair (void) const { return l_linkpair; }
      BlockLink*& linkpair (void) { return l_linkpair; }
      void linkpair (BlockLink* l) { l_linkpair = l; }

    private:
      static const std::string l_stype[5];

      link_type l_type;
      std::string l_label;
      BlockInfo* l_block;

      pvar_ptr_list l_vars;

      BlockLink* l_linkpair;
    };


    //typedef std::vector< BlockLink > block_link;
    typedef std::list< BlockLink > block_link;
    typedef block_link::iterator block_link_iterator;
    typedef block_link::const_iterator block_link_const_iterator;

    class BlockLinkPair
      : public std::pair < BlockLink*, BlockLink* >
    {
    public:
      BlockLinkPair () {}
      BlockLinkPair (BlockLink* forw, BlockLink* back, BlockInfo* orig)
      {
	first = forw;
	second = back;
	p_origin = orig;
      }
      BlockLink* forward (void) const { return first; }
      BlockLink*& forward (void) { return first; }
      BlockLink* backward (void) const { return second; }
      BlockLink*& backward (void) { return second; }

      BlockInfo* origin (void) const { return p_origin; }
      BlockInfo*& origin (void) { return p_origin; }

    private:
      BlockInfo* p_origin;
    };

    class BlockLinkPairList
      : public std::vector < BlockLinkPair >
    {
    public:
      BlockLinkPairList () {}
    };

    class IOPort;

    class NewSignal;
    class NewMux;

    class NewVecElem
    {
    public:
      NewVecElem () : ve_port (NULL), ve_index (-1) {}
      NewVecElem (IOPort* port, size_t index)
	: ve_port (port), ve_index ((int)index)
      {
      }

      IOPort* port (void) const { return ve_port; }
      IOPort*& port (void) { return ve_port; }

      int index (void) const { return ve_index; }
      int& index (void) { return ve_index; }

    private:
      IOPort* ve_port;
      int ve_index;		// vector(>=0), scalar(-1)
    };

    std::ostream& operator<< (std::ostream& os, const NewVecElem& e);

    class NewMuxElem
    {
    public:
      enum Type { None, Signal, Vecelem};

      NewMuxElem () : me_type (None), me_void (NULL)
      {
      }

      NewMuxElem (Type type, NewSignal* sig)
	: me_type (type), me_signal (sig)
      {
      }

      NewMuxElem (NewVecElem* ve)
	: me_type (Vecelem), me_ve (ve)
      {
      }

      NewMuxElem (const NewMuxElem& me)
	: me_type (me.me_type), me_void (NULL)
      {
	if (me.me_type == Vecelem)
	  me_ve = me.me_ve;
	else if (me.me_type == Signal)
	  me_signal = me.me_signal;
      }

      Type type (void) const { return me_type; }

      NewSignal* set_signal (NewSignal* sig)
      {
	me_type = Signal;
	me_signal = sig;
	return me_signal;
      }

      NewVecElem* set_vecelem (NewVecElem* ve)
      {
	me_type = Vecelem;
	me_ve = ve;
	return me_ve;
      }

      // 型判定不要で、ただコピーする目的で使用する
      NewMuxElem* set_muxelem (const NewMuxElem* me)
      {
	me_type = me->me_type;
	me_void = me->me_void;
	return this;
      }

      NewMuxElem* operator= (NewMuxElem* me) { return set_muxelem (me); }

      NewSignal* operator= (NewSignal* sig) { return set_signal (sig); }
      NewVecElem* operator= (NewVecElem* ve) { return set_vecelem (ve); }

      operator NewSignal* () const
      {
	if (me_type == Signal)
	  return me_signal;
	return NULL;
      }

      operator NewVecElem* () const
      {
	if (me_type == Vecelem)
	  return me_ve;
	return NULL;
      }

    private:
      Type me_type;
      union {
	NewSignal* me_signal;
	NewVecElem* me_ve;
	void* me_void;
      };
    };

    std::ostream& operator<< (std::ostream& os, const NewMuxElem& e);

    template <class E>
    class IndentWrapper
    {
    public:
      IndentWrapper(const E& e, int ind) : iw_e (e), iw_ind (ind) {}

      const E& get (void) const { return iw_e; }
      operator const E& () const { return iw_e; }
      size_t indent (void) const { return iw_ind; }
    private:
      const E& iw_e;
      size_t iw_ind;
    };

    template <class E>
    std::ostream& operator<< (std::ostream& os, const IndentWrapper<E>& iw)
    {
      os << std::string (' ', iw.indent () * 2) << iw.get ();
      return os;
    }

    class NewVarChunkElement
    {
    public:
      NewVarChunkElement() : vc_start (0), vc_end (0) {}

      NewVarChunkElement(int s, int e) : vc_start (s), vc_end (e) {}

      NewVarChunkElement(const NewVarChunkElement& e)
	: vc_start (e.vc_start), vc_end (e.vc_end)
      {
      }

      int start (void) const { return vc_start; }
      int& start (void) { return vc_start; }

      int end (void) const { return vc_end; }
      int& end (void) { return vc_end; }

      bool
      operator< (const NewVarChunkElement& c) const
      {
	return vc_start < c.vc_start;
      }

      bool
      overlapped_front (int end) const
      {
	return vc_start < end && end <= vc_end;
      }

      bool
      overlapped_front (const NewVarChunkElement& x) const
      {
	return overlapped_front (x.vc_end);
      }

      bool
      overlapped_back (int start) const
      {
	return vc_start <= start && start < vc_end;
      }

      bool
      overlapped_back (const NewVarChunkElement& x) const
      {
	return overlapped_back (x.vc_start);
      }

      bool
      joined_front (int end) const
      {
	return vc_start == end;
      }

      bool
      joined_front (const NewVarChunkElement& x) const
      {
	return joined_front (x.vc_end);
      }

      bool
      joined_back (int start) const
      {
	return vc_end == start;
      }

      bool
      joined_back (const NewVarChunkElement& x) const
      {
	return joined_back (x.vc_start);
      }

      bool
      do_merge (int start, int end)
      {
	if (joined_front (end))
	  vc_start = start;
	else if (joined_back (start))
	  vc_end = end;
	else if (overlapped_front (end))
	  {
	    if (!overlapped_back (start))
	      vc_start = start;
	  }
	else if (overlapped_back (start))
	  vc_end = end;
	else
	  return false;
	return true;
      }

      bool
      do_merge (const NewVarChunkElement& x)
      {
	return do_merge (x.vc_start, x.vc_end);
      }

    private:
      int vc_start;
      int vc_end;
    };

    typedef std::vector < NewVarChunkElement > NewVarChunks;

    class NewMux
      : public std::vector < NewMuxElem >
    {
    public:
      NewMux () : m_index (0), m_count (0) {}
      NewMux (size_t n) : std::vector < NewMuxElem > (n),
			  m_index (0), m_count (0)
      {
      }

      size_t mux_size (void) const { return size (); }

      size_t index (void) const { return m_index; }
      size_t& index (void) { return m_index; }

      size_t count (void) const { return m_count; }
      size_t& count (void) { return m_count; }

      void region (size_t index, size_t count)
      {
	m_index = index;
	m_count = count;
      }

      bool mux_to_chunks (NewVarChunks& chunks);

    private:
      size_t m_index;
      size_t m_count;
    };

    std::ostream& operator<< (std::ostream& os, const NewMux& m);

    class NewSignal
      : public std::vector < size_t > // array size
    {
    public:
      enum Type { None, Scalar, Vector };

      NewSignal () : s_type (None), s_port (NULL) {}

      Type get_type (void) const { return s_type; }
      void set_type (Type t) { s_type = t; }

      IOPort* port (void) const { return s_port; }
      IOPort*& port (void) { return s_port; }

      size_t dim_size (void) const { return size (); }

      bool dim_parse (const char*& p);

    private:
      Type s_type;
      IOPort* s_port;
    };

    std::ostream& operator<< (std::ostream& os, const NewSignal& s);

    class NewBus
      : public std::vector < NewSignal >
    {
    public:
      enum Type { None, Bus, Signal };

      NewBus () : b_type (None) {}
      NewBus (Type type, size_t size)
	: std::vector < NewSignal > (size), b_type (type)
      {
      }

      Type get_type (void) const { return b_type; }
      void set_type (Type t) { b_type = t; }

      bool dim_parse (const std::string& dim);

      const std::string& selected_signal (void) const { return b_ssignal; }
      void selected_signal (const std::string& s) { b_ssignal = s; }

      size_t bus_size (void) const { return size (); }

    private:
      Type b_type;
      std::string b_ssignal;
    };

    // Obsoleted
    class oldBusEnt
    {
    public:
      oldBusEnt (const IOPort* port,
		 int index = 0, int nchunk = 1, int total = 1)
	: bi_port (port), bi_index (index), bi_nchunk (nchunk),
	  bi_total (total) {}

      oldBusEnt ()
	: bi_port (NULL), bi_index (0), bi_nchunk(1), bi_total (1)
      {
      }

      const IOPort*& port (void) { return bi_port; }
      const IOPort* port (void) const { return bi_port; }

      int& index (void) { return bi_index; }
      int index (void) const { return bi_index; }

      int& nchunk (void) { return bi_nchunk; }
      int nchunk (void) const { return bi_nchunk; }

      int& total (void) { return bi_total; }
      int total (void) const { return bi_total; }

      const std::string& usename (void) const { return bi_usename; }
      std::string& usename (void) { return bi_usename; }

      bool operator< (const oldBusEnt& x) const;

      void reduction (void);

      void dump (void) const;

    private:
      const IOPort* bi_port;	// output port
      int bi_index;
      int bi_nchunk;
      int bi_total;
      std::string bi_usename;
    };

    // obsoleted
    class oldBusLine : public std::list < oldBusEnt >
    {
    public:
      oldBusLine () {}

      oldBusEnt& add (const oldBusEnt& ent)
      {
	push_back (ent);
	return back ();
      }

      oldBusEnt&
      add (const IOPort* port, int index = 0, int nchunk = 1, int total = 1)
      {
	push_back (oldBusEnt (port, index, nchunk, total));
	return back ();
      }

      void join_fragment (void); // no reorder, only join
      void defrag (oldBusLine& out) const; // result: reorder, de-fragment
      void reduction (void);
    };

    // obsoleted
    class oldBusPort : public std::vector < oldBusLine >
    {
    public:
      oldBusPort () {}

      size_t bus_count (void) const
      {
	size_t count = 0;
	for (const_iterator i (begin ()); i != end (); i++)
	  count += i->size ();
	return count;
      }

      void dump (int indent) const;
    };

    class Blocks;

#if __cplusplus >= 201103L
    typedef std::unique_ptr< Blocks > Blocks_PTR;
#else
    typedef std::auto_ptr< Blocks > Blocks_PTR;
#endif

    class function_code_T : public std::string
    {
    public:
      enum FuncKind { FK_NONE, FK_INIT, FK_TASK, FK_UPDATE };

      function_code_T (const std::string& x)
	: std::string (x), f_line (0), f_kind (FK_NONE) { f_file.clear (); }

      const std::string& file (void) const { return f_file; }
      std::string& file (void) { return f_file; }
      void line (unsigned int l) { f_line = l; }
      unsigned int line (void) const { return f_line; }
      unsigned int& line (void) { return f_line; }

      FuncKind kind (void) const { return f_kind; }
      FuncKind& kind (void) { return f_kind; }

      const std::vector < std::string >& call (void) const
      {
	return f_call;
      }

      std::vector < std::string >& call (void) { return f_call; }

      const blockinfo_list& blocks (void) const { return f_blocks; }
      blockinfo_list& blocks (void) { return f_blocks; }

      const std::string& kind_str (void) const
      {
	return FuncKindName[f_kind];
      }

    private:
      static const std::string FuncKindName[4];

      std::vector < std::string > f_call;
      std::string f_file;
      unsigned int f_line;
      FuncKind f_kind;
      blockinfo_list f_blocks;
    };

    class function_map_T
      : public std::map < std::string, function_code_T >
    {
    public:
      iterator
      add (const std::string& func, const std::string& text)
      {
	std::pair < iterator, bool >
	  x = insert (std::make_pair (func, function_code_T (text)));
	return x.first;
      }

      iterator
      add (const std::string& func, const char* start, const char* end)
      {
	return add (func, std::string (start, end - start));
      }

      iterator
      add (const std::string& func, const char* start, size_t size)
      {
	return add (func, std::string (start, size));
      }
    };

    typedef function_map_T::iterator function_map_iterator;
    typedef function_map_T::const_iterator function_map_const_iterator;

    class BlockInfo
    {
    public:
      enum block_type
	{
	  TYPE_NORMAL,		// XXX (currentry only this)
	  TYPE_ENABLED_SUBSYSTEM,
	  TYPE_TRIGGERED_SUBSYSTEM,
	  TYPE_ENABLED_TRIGGERED_SUBSYSTEM,
	  TYPE_ACTION_SUBSYSTEM,
	  TYPE_ITERATOR_SUBSYSTEM,
	  TYPE_ATOMIC_SUBSYSTEM,
	  TYPE_STATEFLOW_SUBSYSTEM,
	  TYPE_MUX,
	  TYPE_DEMUX,
	  TYPE_BUSCREATOR,
	  TYPE_BUSSELECTOR,
	  TYPE_EXTIOPORT,
	  TYPE_VIRTUALOUTPORT,
	  TYPE_OTHER
	};

      BlockInfo();

      BlockInfo(BlockInfo* u, SimulinkModel::XSD::block_T* b,
		block_type t = TYPE_NORMAL);

      ~BlockInfo();

      unsigned int id (void) const { return b_id; }

      block_type&
      type (void)
      {
	return b_type;
      }

      block_type
      type (void) const
      {
	return b_type;
      }

      SimulinkModel::XSD::block_T*&
      tag (void)
      {
	return b_tag;
      }

      const SimulinkModel::XSD::block_T*
      tag (void) const
      {
	return b_tag;
      }

      BlockInfo*&
      upper (void)
      {
	return b_upper;
      }

      const BlockInfo*
      upper (void) const
      {
	return b_upper;
      }

      const std::string&
      system (void) const
      {
	return b_system;
      }

      const std::string&
      block (void) const
      {
	return b_block;
      }

      bool
      defined (void) const
      {
	return b_defined;
      }

      bool
      frag (void) const
      {
	return b_frag;
      }

      const std::string&
      code (void) const
      {
	return b_code;
      }

      const CodeFrags&
      codefrags (void) const
      {
	return b_codefrags;
      }

      const std::string&
      update (void) const
      {
	return b_update;
      }

      const std::string&
      init_code (void) const
      {
	return b_init_code;
      }

      const std::string&
      init_state (void) const
      {
	return b_init_state;
      }

      std::string&
      system (void)
      {
	return b_system;
      }

      std::string&
      block (void)
      {
	return b_block;
      }

      bool&
      defined (void)
      {
	return b_defined;
      }

      bool&
      frag (void)
      {
	return b_frag;
      }

      std::string&
      code (void)
      {
	return b_code;
      }

      CodeFrags&
      codefrags (void)
      {
	return b_codefrags;
      }

      std::string&
      update (void)
      {
	return b_update;
      }

      std::string&
      init_code (void)
      {
	return b_init_code;
      }

      std::string&
      init_state (void)
      {
	return b_init_state;
      }

      const std::string&
      code_file (void) const
      {
	return b_code_file;
      }

      const std::string&
      update_file (void) const
      {
	return b_update_file;
      }

      const std::string&
      init_code_file (void) const
      {
	return b_init_code_file;
      }

      const std::string&
      init_state_file (void) const
      {
	return b_init_state_file;
      }

      std::string&
      code_file (void)
      {
	return b_code_file;
      }

      std::string&
      update_file (void)
      {
	return b_update_file;
      }

      std::string&
      init_code_file (void)
      {
	return b_init_code_file;
      }

      std::string&
      init_state_file (void)
      {
	return b_init_state_file;
      }

      unsigned int&
      code_line (void)
      {
	return b_code_line;
      }

      unsigned int&
      update_line (void)
      {
	return b_update_line;
      }

      unsigned int&
      init_code_line (void)
      {
	return b_init_code_line;
      }

      unsigned int&
      init_state_line (void)
      {
	return b_init_state_line;
      }

      unsigned int
      code_line (void) const
      {
	return b_code_line;
      }

      unsigned int
      update_line (void) const
      {
	return b_update_line;
      }

      unsigned int
      init_code_line (void) const
      {
	return b_init_code_line;
      }

      unsigned int
      init_state_line (void) const
      {
	return b_init_state_line;
      }

      const var_list&
      auto_var(void) const
      {
	return b_auto_vars;
      }

      var_list&
      auto_var(void)
      {
	return b_auto_vars;
      }

      const propagate_var_list&
      input_var(void) const
      {
	return b_input_vars;
      }

      propagate_var_list&
      input_var(void)
      {
	return b_input_vars;
      }

      const propagate_var_list&
      output_var(void) const
      {
	return b_output_vars;
      }

      propagate_var_list&
      output_var(void)
      {
	return b_output_vars;
      }

      const propagate_var_list&
      work_var(void) const
      {
	return b_work_vars;
      }

      propagate_var_list&
      work_var(void)
      {
	return b_work_vars;
      }

      const var_list&
      params (void) const
      {
	return b_params;
      }

      var_list&
      params (void)
      {
	return b_params;
      }

      const var_list&
      constant_params (void) const
      {
	return b_constant_params;
      }

      var_list&
      constant_params (void)
      {
	return b_constant_params;
      }

      const var_list&
      state_var(void) const
      {
	return b_state_vars;
      }

      var_list&
      state_var(void)
      {
	return b_state_vars;
      }

      const var_list&
      trigger_state_var(void) const
      {
	return b_trigger_state_vars;
      }

      var_list&
      trigger_state_var(void)
      {
	return b_trigger_state_vars;
      }

      const var_list&
      invariant_signal_var(void) const
      {
	return b_invariant_signal_vars;
      }

      var_list&
      invariant_signal_var(void)
      {
	return b_invariant_signal_vars;
      }

      const var_list&
      signal_var(void) const
      {
	return b_signal_vars;
      }

      var_list&
      signal_var(void)
      {
	return b_signal_vars;
      }

      const var_list&
      external_inputs (void) const
      {
	return b_external_inputs;
      }

      var_list&
      external_inputs (void)
      {
	return b_external_inputs;
      }

      const var_list&
      external_outputs (void) const
      {
	return b_external_outputs;
      }

      var_list&
      external_outputs (void)
      {
	return b_external_outputs;
      }

      void dump (void) const;

      bool
      has_code (void)
      {
	return b_code.length() != 0;
      }

      const block_link&
      forward (void) const
      {
	return b_forward;
      }

      const block_link&
      backward (void) const
      {
	return b_backward;
      }

      block_link&
      forward (void)
      {
	return b_forward;
      }

      block_link&
      backward (void)
      {
	return b_backward;
      }

      const SimulinkModel::XSD::ioport_T*
      enable_input (void) const
      {
	return b_enable_input;
      }

      const SimulinkModel::XSD::ioport_T*&
      enable_input (void)
      {
	return b_enable_input;
      }

      const BlockInfo* enable_port (void) const { return b_enable_port; }
      BlockInfo*& enable_port (void) { return b_enable_port; }

      const BlockInfo* enable_from (void) const { return b_enable_from; }
      BlockInfo*& enable_from (void) { return b_enable_from; }

      const VarElement& enable_var (void) const { return b_enable_var; }
      VarElement& enable_var (void) { return b_enable_var; }

      const SimulinkModel::XSD::ioport_T*
      trigger_input (void) const
      {
	return b_trigger_input;
      }

      const SimulinkModel::XSD::ioport_T*&
      trigger_input (void)
      {
	return b_trigger_input;
      }

      const BlockInfo* trigger_port (void) const { return b_trigger_port; }
      BlockInfo*& trigger_port (void) { return b_trigger_port; }

      const BlockInfo* trigger_from (void) const { return b_trigger_from; }
      BlockInfo*& trigger_from (void) { return b_trigger_from; }

      const VarElement& trigger_var (void) const { return b_trigger_var; }
      VarElement& trigger_var (void) { return b_trigger_var; }

      const BlockInfo* top_block (void) const { return b_top_block; }
      BlockInfo*& top_block (void) { return b_top_block; }

      const blockinfo_list& merged_to (void) const { return b_merged_to; }
      blockinfo_list& merged_to (void) { return b_merged_to; }

      const blockinfo_list&
      merge_blocks (void) const
      {
	return b_merge_blocks;
      }
      blockinfo_list& merge_blocks (void) { return b_merge_blocks; }

      const oldBusPort& busport (void) const { return b_busport; }
      oldBusPort& busport (void) { return b_busport; }

      bool bus_visited (void) const { return b_bus_visited; }
      bool& bus_visited (void) { return b_bus_visited; }

      bool bus_search_result (void) const { return b_bus_search_result; }
      bool& bus_search_result (void) { return b_bus_search_result; }

      const function_map_T& func_decl (void) const { return b_func_decl; }
      function_map_T& func_decl (void) { return b_func_decl; }

      const function_map_T& func_defn (void) const { return b_func_defn; }
      function_map_T& func_defn (void) { return b_func_defn; }

      void make_link (BlockInfo* orig,
		      BlockInfo* f,
		      BlockLinkPairList& pairlist,
		      BlockLink::link_type t = BlockLink::TYPE_NONE);

      void make_link (BlockInfo* f,
		      BlockLink::link_type t = BlockLink::TYPE_NONE);

#if 0
      BlockLink& add_forward (BlockInfo* f,
			      BlockLink::link_type t = BlockLink::TYPE_NONE);
      BlockLink& add_backward (BlockInfo* b,
			       BlockLink::link_type t = BlockLink::TYPE_NONE);
#endif

      void complete (const Blocks *);

      void add_storage_if_existing (const Blocks* b,
				    SimulinkModel::XSD::var_T& vt,
				    const VariableEnt& ve) const;

      unsigned int&
      any (void)
      {
	return b_any;
      }

      void
      code_appearence (unsigned int line,
		       const SimulinkModel::XSD::file_T::name_type* file)
      {
	if (b_code_line == 0)
	  b_code_line = line;
	if (b_code_file.length () == 0)
	  b_code_file = *file;
      }

      void
      update_appearence (unsigned int line,
			 const SimulinkModel::XSD::file_T::name_type* file)
      {
	if (b_update_line == 0)
	  b_update_line = line;
	if (b_update_file.length () == 0)
	  b_update_file = *file;
      }

      void
      init_code_appearence (unsigned int line,
			    const SimulinkModel::XSD::file_T::name_type* file)
      {
	if (b_init_code_line == 0)
	  b_init_code_line = line;
	if (b_init_code_file.length () == 0)
	  b_init_code_file = *file;
      }

      void
      init_state_appearence (unsigned int line,
			     const SimulinkModel::XSD::file_T::name_type* file)
      {
	if (b_init_state_line == 0)
	  b_init_state_line = line;
	if (b_init_state_file.length () == 0)
	  b_init_state_file = *file;
      }

      bool
      is_cond_subsystem (void) const
      {
	if (type () == TYPE_ENABLED_SUBSYSTEM
	    || type () == TYPE_TRIGGERED_SUBSYSTEM
	    || type () == TYPE_ENABLED_TRIGGERED_SUBSYSTEM)
	  return true;
	return false;
      }

      bool is_same_merged_head (const BlockInfo* info) const;

    private:

      void add_var_tag (const Blocks* b,
			const block_link& blink,
			const propagate_var_list& pvl,
			c_varentp_map& vmap, bool merge);

      static unsigned int b_seq;

      unsigned int b_id;

      block_type b_type;
      BlockInfo* b_upper;
      SimulinkModel::XSD::block_T* b_tag;
      std::string b_system;
      std::string b_block;
      bool b_defined;

      bool b_frag;

      CodeFrags b_codefrags;

      std::string b_code;
      std::string b_update;
      std::string b_init_code;
      std::string b_init_state;

      std::string b_code_file;
      std::string b_update_file;
      std::string b_init_code_file;
      std::string b_init_state_file;

      unsigned int b_code_line;
      unsigned int b_update_line;
      unsigned int b_init_code_line;
      unsigned int b_init_state_line;

      var_list b_auto_vars;	// XXX

      propagate_var_list b_input_vars;
      propagate_var_list b_output_vars;

      propagate_var_list b_work_vars; // work

      var_list b_params;
      var_list b_constant_params;
      var_list b_state_vars;
      var_list b_trigger_state_vars;
      var_list b_invariant_signal_vars;
      var_list b_signal_vars;

      var_list b_external_inputs;
      var_list b_external_outputs;

      // internal data-flow chain
      block_link b_forward;
      block_link b_backward;

      // any
      unsigned int b_any;

      // Triggered SubSystem
      //   for EnabbePort
      const SimulinkModel::XSD::ioport_T* b_trigger_input;

      //   for SubSystem
      BlockInfo* b_trigger_port; // blocktype="TriggerPort"
      BlockInfo* b_trigger_from; // input from
      VarElement b_trigger_var;	 // variable for trigger

      // Enabled SubSystem
      //   for EnablePort
      const SimulinkModel::XSD::ioport_T* b_enable_input;

      //   for SubSystem
      BlockInfo* b_enable_port;	// blocktype="EnablePort"
      BlockInfo* b_enable_from;	// input from
      VarElement b_enable_var;	// variable for enable

      // For/While Iterator Subsystem
      BlockInfo* b_top_block;	// toplevel subsystem

      // this block merged
      blockinfo_list b_merged_to;

      // this block contain merge block
      blockinfo_list b_merge_blocks;

      // Mux/Demux or Bus Creator/Selector
      oldBusPort b_busport;
      bool b_bus_visited;
      bool b_bus_search_result;

      // function
      function_map_T b_func_decl;
      function_map_T b_func_defn;
    };

    typedef std::multimap< std::string, IOPort > ioport_map_T;
    typedef ioport_map_T::iterator ioport_map_iterator;
    typedef ioport_map_T::const_iterator ioport_map_const_iterator;
    typedef std::pair< ioport_map_iterator, ioport_map_iterator > ioport_map_iterator_pair;
    typedef std::pair< ioport_map_const_iterator, ioport_map_const_iterator >
    ioport_map_const_iterator_pair;

    class ioport_deq_T : public std::deque< IOPort* >
    {
    public:
      ioport_deq_T() {}

      void dump (void);
    };

    class IOPort
    {
    public:
      enum type { NONE, IN, OUT };

      IOPort ();

      IOPort (type type,
	      const SimulinkModel::XSD::block_T* blk,
	      const SimulinkModel::XSD::ioport_T* io, int index,
	      BlockInfo* bi);

      ~IOPort ();

      type& iotype (void) { return io_type; }
      type iotype (void) const { return io_type; }

      const SimulinkModel::XSD::ioport_T*&
      ioport_tag (void)
      {
	return io_tag;
      }

      const SimulinkModel::XSD::ioport_T*
      ioport_tag (void) const
      {
	return io_tag;
      }

      int&
      ioport_index (void)
      {
	return io_index;
      }

      int
      ioport_index (void) const
      {
	return io_index;
      }

      const SimulinkModel::XSD::block_T*&
      block_tag (void)
      {
	return blk_tag;
      }

      const SimulinkModel::XSD::block_T*
      block_tag (void) const
      {
	return blk_tag;
      }

      BlockInfo*&
      blockinfo (void)
      {
	return blk_info;
      }

      BlockInfo*
      blockinfo (void) const
      {
	return blk_info;
      }

      bool
      match (const std::string& block, const std::string& port) const
      {
	return blk_tag->name() == block && io_tag->port() == port;
      }

      const NewBus& businfo (void) const { return bus_info; }
      NewBus& businfo (void) { return bus_info; };

      IOPort* outport (void) const { return src_outport; }
      IOPort*& outport (void) { return src_outport; }

      const ioport_deq_T& inports (void) const { return dst_inport; }
      ioport_deq_T& inports (void) { return dst_inport; }

      const NewBus& in_businfo (void) const
      {
	assert (src_outport);
	return src_outport->bus_info;
      }

      NewBus& in_businfo (void)
      {
	assert (src_outport);
	return src_outport->bus_info;
      }

    private:
      type io_type;
      const SimulinkModel::XSD::block_T* blk_tag;
      const SimulinkModel::XSD::ioport_T* io_tag;
      int io_index;
      BlockInfo *blk_info;

      NewBus bus_info;

      IOPort *src_outport;	// source(<output>), <input> only
      ioport_deq_T dst_inport;	// distination(<input>)
    };

    class Goto;
    class From;

    typedef std::map< std::string, Goto > goto_map_T;
    typedef goto_map_T::iterator goto_map_iterator;
    typedef goto_map_T::const_iterator goto_map_const_iterator;

    typedef std::map< std::string, From > from_map_T;
    typedef from_map_T::iterator from_map_iterator;
    typedef from_map_T::const_iterator from_map_const_iterator;

    typedef std::vector < Goto* > goto_list_T;
    typedef goto_list_T::iterator goto_list_iterator;
    typedef goto_list_T::const_iterator goto_list_const_iterator;

    typedef std::vector < From* > from_list_T;
    typedef from_list_T::iterator from_list_iterator;
    typedef from_list_T::const_iterator from_list_const_iterator;

    class Goto
    {
    public:
      Goto (const SimulinkModel::XSD::block_T* blk, BlockInfo* bi)
	: g_tag (blk), g_info (bi)
      {
      }

      ~Goto () {}

      const SimulinkModel::XSD::block_T*&
      goto_tag (void)
      {
	return g_tag;
      }

      const SimulinkModel::XSD::block_T*
      goto_tag (void) const
      {
	return g_tag;
      }

      BlockInfo*& goto_blockinfo (void) { return g_info; }
      const BlockInfo* goto_blockinfo (void) const { return g_info; }

      from_list_T& from_list (void) { return g_from_list; }
      const from_list_T& from_list (void) const { return g_from_list; }

      void add_from (From* f) { g_from_list.push_back (f); }

    private:
      const SimulinkModel::XSD::block_T* g_tag;
      BlockInfo* g_info;

      from_list_T g_from_list;
    };


    class From
    {
    public:
      From (const SimulinkModel::XSD::block_T* blk, BlockInfo* bi)
	: f_tag (blk), f_info (bi), f_goto (NULL)
      {
      }

      ~From() {}

      const SimulinkModel::XSD::block_T*&
      from_tag (void)
      {
	return f_tag;
      }

      const SimulinkModel::XSD::block_T*
      from_tag (void) const
      {
	return f_tag;
      }

      BlockInfo*& from_blockinfo (void) { return f_info; }
      const BlockInfo* from_blockinfo (void) const { return f_info; }

      Goto*& from_goto (void) { return f_goto; }
      const Goto* from_goto (void) const { return f_goto; }

    private:
      const SimulinkModel::XSD::block_T* f_tag;
      BlockInfo* f_info;

      Goto* f_goto;
    };


    typedef std::map< std::string, BlockInfo> block_map_T;
    typedef block_map_T::iterator block_map_iterator;
    typedef block_map_T::const_iterator block_map_const_iterator;

    // xxx for system
    class SysData
    {
    public:
      enum sd_type {SD_BSIG, SD_BSTATE, SD_PARAM}; // TODO constant params?

      SysData () { id = index++; }
      SysData (sd_type st,
	       const std::string& sys, const std::string& blk,
	       BlockInfo* binfo,
	       const std::string& vt, bool hs, const std::string& tg,
	       const std::string& c)
	: s_stype (st), s_system (sys), s_block (blk), s_blockinfo (binfo),
	  s_vtype (vt), s_has_struct (hs), s_code (c)
      {
	id = index++;
      }

      sd_type stype (void) const { return s_stype; }
      sd_type& stype (void) { return s_stype; }

      const std::string& system (void) const { return s_system; }
      std::string& system (void) { return s_system; }

      const std::string& block (void) const { return s_block; }
      std::string& block (void) { return s_block; }

      BlockInfo* blockinfo (void) const { return s_blockinfo; }
      BlockInfo*& blockinfo (void) { return s_blockinfo; }

      const std::string& vtype (void) const { return s_vtype; }
      std::string& vtype (void) { return s_vtype; }

      const std::string& code (void) const { return s_code; }
      std::string& code (void) { return s_code; }

      bool has_struct (void) const { return s_has_struct; }
      bool& has_struct (void) { return s_has_struct; }

    private:
      static int index;

      int id;
      sd_type s_stype;

      std::string s_system;
      std::string s_block;
      BlockInfo* s_blockinfo;

      std::string s_vtype;	// typedef or struct tag
      bool s_has_struct;

      std::string s_code;
    };

    typedef std::multimap < std::string, SysData* > sysdata_ptrmap_T;
    typedef sysdata_ptrmap_T::iterator sysdata_ptrmap_iterator;
    typedef sysdata_ptrmap_T::const_iterator sysdata_ptrmap_const_iterator;
    typedef std::pair < sysdata_ptrmap_iterator,
			sysdata_ptrmap_iterator
			> sysdata_ptrmap_iterator_pair;
    typedef std::pair < sysdata_ptrmap_const_iterator,
			sysdata_ptrmap_const_iterator
			> sysdata_ptrmap_const_iterator_pair;

    typedef std::vector < SysData > sysdata_list_T;
    typedef sysdata_list_T::iterator sysdata_list_iterator;
    typedef sysdata_list_T::const_iterator sysdata_list_const_iterator;

    class inc_file_T : public std::string
    {
    public:
      inc_file_T (const std::string& file, char mode)
	: std::string (file), f_mode (mode)
      {
      }

      inc_file_T () : f_mode (0) {}

      std::string incfile (void) const
      {
	char e = f_mode;
	if (e == '<')
	  e = '>';
	return std::string (f_mode + *this + e);
      }

    private:
      char f_mode;
    };

    typedef std::vector < inc_file_T > include_files_T;
    typedef std::map < std::string, include_files_T > include_map_T;

    typedef std::pair < std::string, unsigned int > file_code_T;

#ifdef XSD_CXX11
    class blocks_T_PTR
      : public std::unique_ptr< SimulinkModel::XSD::blocks_T >
    {
      typedef SimulinkModel::XSD::blocks_T this_block;
      typedef std::unique_ptr< this_block > this_ptr;
    public:
      blocks_T_PTR () {}
      blocks_T_PTR (this_block *ptr) : this_ptr (ptr) {}
      blocks_T_PTR (blocks_T_PTR& b) : this_ptr (b.release ()) {}
    };
#else
    typedef std::auto_ptr< SimulinkModel::XSD::blocks_T > blocks_T_PTR;
#endif

    class Blocks
    {
    public:

      Blocks()
	: b_rtm_extern_has_struct (false), b_rtm_extern_has_const (false),
	  b_rtm_object_has_const (false),
	  b_rtm_line (0), b_rtm_init_line (0),
	  b_common_line (0), b_common_init_line (0),
	  b_has_dimensions (false)
      {
      }

      Blocks(blocks_T_PTR& blks)
	: b_xml (blks),
	  b_rtm_extern_has_struct (false), b_rtm_extern_has_const (false),
	  b_rtm_object_has_const (false),
	  b_rtm_line (0), b_rtm_init_line (0),
	  b_common_line (0), b_common_init_line (0),
	  b_has_dimensions (false)
      {
	process_all ();
      }

      ~Blocks()
      {
      }

      // blockname store
      const blockname_map&
      blockname_entry(void) const
      {
	return b_blockname;
      }

      blockname_map&
      blockname_entry(void)
      {
	return b_blockname;
      }

      BlockNameEnt& add_blockname_entry (const std::string& s,
					 const BlockNameEnt& b);

      // search functions
      // auto variables
      const c_variable_map&
      auto_variable_entry(void) const
      {
	return b_autovars;
      }

      c_variable_map&
      auto_variable_entry(void)
      {
	return b_autovars;
      }

      c_variable_map_const_iterator
      find_auto_var (const std::string& name) const;

      c_variable_map_iterator find_auto_var (const std::string& name);

      VariableEnt& add_auto_variable_entry (const std::string& s,
					    const VariableEnt& v);

      // ext variables
      const c_variable_map&
      ext_variable_entry(void) const
      {
	return b_extvars;
      }

      c_variable_map&
      ext_variable_entry(void)
      {
	return b_extvars;
      }

      c_variable_map_const_iterator
      find_ext_var (const std::string& name) const;

      c_variable_map_iterator find_ext_var (const std::string& name);

      VariableEnt& add_ext_variable_entry (const std::string& s,
					   const VariableEnt& v);

      // ext states
      const c_variable_map&
      ext_state_entry(void) const
      {
	return b_extstates;
      }

      c_variable_map&
      ext_state_entry(void)
      {
	return b_extstates;
      }

      c_variable_map_const_iterator
      find_ext_state (const std::string& name) const;

      c_variable_map_iterator find_ext_state (const std::string& name);

      VariableEnt& add_ext_state_entry (const std::string& s,
					const VariableEnt& v);

      // states
      const std::string&
      state_type (void) const
      {
	return b_state_type;
      }

      std::string&
      state_type (void)
      {
	return b_state_type;
      }

      const std::string&
      state_name (void) const
      {
	return b_state_name;
      }

      std::string&
      state_name (void)
      {
	return b_state_name;
      }

      const c_variable_map&
      state_entry (void) const
      {
	return b_state_vars;
      }

      c_variable_map&
      state_entry (void)
      {
	return b_state_vars;
      }

      c_variable_map_const_iterator
      find_state_var (const std::string& name) const;

      c_variable_map_iterator find_state_var (const std::string& name);

      const var_list&
      state_order (void) const
      {
	return b_state_orders;
      }

      var_list&
      state_order (void)
      {
	return b_state_orders;
      }


      VariableEnt& add_state_entry (const std::string &s,
				    const VariableEnt& v);

      // trigger state
      const std::string&
      trigger_state_type (void) const
      {
	return b_trigger_state_type;
      }

      std::string&
      trigger_state_type (void)
      {
	return b_trigger_state_type;
      }

      const std::string&
      trigger_state_name (void) const
      {
	return b_trigger_state_name;
      }

      std::string&
      trigger_state_name (void)
      {
	return b_trigger_state_name;
      }

      const c_variable_map&
      trigger_state_entry (void) const
      {
	return b_trigger_state_vars;
      }

      c_variable_map&
      trigger_state_entry (void)
      {
	return b_trigger_state_vars;
      }

      c_variable_map_const_iterator
      find_trigger_state_var (const std::string& name) const;

      c_variable_map_iterator
      find_trigger_state_var (const std::string& name);

      const var_list&
      trigger_state_order (void) const
      {
	return b_trigger_state_orders;
      }

      var_list&
      trigger_state_order (void)
      {
	return b_trigger_state_orders;
      }

      VariableEnt& add_trigger_state_entry (const std::string &s,
					    const VariableEnt& v);

      // invariant block signal
      const std::string&
      invariant_signal_type (void) const
      {
	return b_invariant_signal_type;
      }

      std::string&
      invariant_signal_type (void)
      {
	return b_invariant_signal_type;
      }

      const std::string&
      invariant_signal_name (void) const
      {
	return b_invariant_signal_name;
      }

      std::string&
      invariant_signal_name (void)
      {
	return b_invariant_signal_name;
      }

      const c_variable_map&
      invariant_signal_entry (void) const
      {
	return b_invariant_signal_vars;
      }

      c_variable_map&
      invariant_signal_entry (void)
      {
	return b_invariant_signal_vars;
      }

      c_variable_map_const_iterator
      find_invariant_signal_var (const std::string& name) const;

      c_variable_map_iterator
      find_invariant_signal_var (const std::string& name);

      const var_list&
      invariant_signal_order (void) const
      {
	return b_invariant_signal_orders;
      }

      var_list&
      invariant_signal_order (void)
      {
	return b_invariant_signal_orders;
      }


      VariableEnt& add_invariant_signal_entry (const std::string &s,
					       const VariableEnt& v);

      // parameters
      const std::string&
      param_type (void) const
      {
	return b_param_type;
      }

      std::string&
      param_type (void)
      {
	return b_param_type;
      }

      const std::string&
      param_name (void) const
      {
	return b_param_name;
      }

      std::string&
      param_name (void)
      {
	return b_param_name;
      }

      const c_variable_map&
      param_entry (void) const
      {
	return b_parameters;
      }

      c_variable_map&
      param_entry (void)
      {
	return b_parameters;
      }

      c_variable_map_const_iterator
      find_param (const std::string& name) const;

      c_variable_map_iterator find_param (const std::string& name);

      const var_list&
      param_order (void) const
      {
	return b_param_orders;
      }

      var_list&
      param_order (void)
      {
	return b_param_orders;
      }

      VariableEnt& add_param_entry (const std::string& s,
				    const VariableEnt& v);

      // Constant parameters
      const std::string&
      constant_param_type (void) const
      {
	return b_constant_param_type;
      }

      std::string&
      constant_param_type (void)
      {
	return b_constant_param_type;
      }

      void
      constant_param_type (const std::string& t)
      {
	b_constant_param_type = t;
      }

      const std::string&
      constant_param_name (void) const
      {
	return b_constant_param_name;
      }

      std::string&
      constant_param_name (void)
      {
	return b_constant_param_name;
      }

      void
      constant_param_name (const std::string& n)
      {
	b_constant_param_name = n;
      }

      const c_variable_map&
      constant_param_entry (void) const
      {
	return b_constant_parameters;
      }

      c_variable_map&
      constant_param_entry (void)
      {
	return b_constant_parameters;
      }

      c_variable_map_const_iterator
      find_constant_param (const std::string& name) const;

      c_variable_map_iterator find_constant_param (const std::string& name);

      const var_list&
      constant_param_order (void) const
      {
	return b_constant_param_orders;
      }

      var_list&
      constant_param_order (void)
      {
	return b_constant_param_orders;
      }

      VariableEnt& add_constant_param_entry (const std::string s,
					     const VariableEnt& v);

      // External Inputs
      const std::string&
      external_input_type (void) const
      {
	return b_external_input_type;
      }

      std::string&
      external_input_type (void)
      {
	return b_external_input_type;
      }

      void
      external_input_type (const std::string& t)
      {
	b_external_input_type = t;
      }

      const std::string&
      external_input_name (void) const
      {
	return b_external_input_name;
      }

      std::string&
      external_input_name (void)
      {
	return b_external_input_name;
      }

      void
      external_input_name (const std::string& n)
      {
	b_external_input_name = n;
      }

      const c_variable_map&
      external_input_entry (void) const
      {
	return b_external_inputs;
      }

      c_variable_map&
      external_input_entry (void)
      {
	return b_external_inputs;
      }

      c_variable_map_const_iterator
      find_external_input (const std::string& name) const;

      c_variable_map_iterator find_external_input (const std::string& name);

      const var_list&
      external_input_order (void) const
      {
	return b_external_input_orders;
      }

      var_list&
      external_input_order (void)
      {
	return b_external_input_orders;
      }

      VariableEnt& add_external_input_entry (const std::string &s,
					     const VariableEnt& v);

      // External Outputs
      const std::string&
      external_output_type (void) const
      {
	return b_external_output_type;
      }

      std::string&
      external_output_type (void)
      {
	return b_external_output_type;
      }

      void
      external_output_type (const std::string& t)
      {
	b_external_output_type = t;
      }

      const std::string&
      external_output_name (void) const
      {
	return b_external_output_name;
      }

      std::string&
      external_output_name (void)
      {
	return b_external_output_name;
      }

      void
      external_output_name (const std::string& n)
      {
	b_external_output_name = n;
      }

      const c_variable_map&
      external_output_entry (void) const
      {
	return b_external_outputs;
      }

      c_variable_map&
      external_output_entry (void)
      {
	return b_external_outputs;
      }

      c_variable_map_const_iterator
      find_external_output (const std::string& name) const;

      c_variable_map_iterator find_external_output (const std::string& name);

      const var_list&
      external_output_order (void) const
      {
	return b_external_output_orders;
      }

      var_list&
      external_output_order (void)
      {
	return b_external_output_orders;
      }

      VariableEnt& add_external_output_entry (const std::string &s,
					      const VariableEnt& v);

      // signals
      const std::string&
      signal_type (void) const
      {
	return b_signal_type;
      }

      std::string&
      signal_type (void)
      {
	return b_signal_type;
      }

      const std::string&
      signal_name (void) const
      {
	return b_signal_name;
      }

      std::string&
      signal_name (void)
      {
	return b_signal_name;
      }

      const c_variable_map&
      signal_entry (void) const
      {
	return b_signal_vars;
      }

      c_variable_map&
      signal_entry (void)
      {
	return b_signal_vars;
      }

      c_variable_map_const_iterator
      find_signal_var (const std::string& name) const;

      c_variable_map_iterator find_signal_var (const std::string& name);

      const var_list&
      signal_order (void) const
      {
	return b_signal_orders;
      }

      var_list&
      signal_order (void)
      {
	return b_signal_orders;
      }


      VariableEnt& add_signal_entry (const std::string &s,
				     const VariableEnt& v);


      // XML
      void xml (blocks_T_PTR& x);

      SimulinkModel::XSD::blocks_T*
      xml (void) const
      {
	return b_xml.get();
      }

      block_map_T&
      block_map (void)
      {
	return b_block_map;
      }

      const block_map_T&
      block_map (void) const
      {
	return b_block_map;
      }

      BlockInfo& add_block_map (const std::string& s, const BlockInfo& b);

      const ioport_map_T&
      in_port_map (void) const
      {
	return b_in_port_map;
      }

      ioport_map_T&
      in_port_map (void)
      {
	return b_in_port_map;
      }

      IOPort& add_in_port_map (const std::string& s, const IOPort& p);

      const ioport_map_T&
      out_port_map (void) const
      {
	return b_out_port_map;
      }

      ioport_map_T&
      out_port_map (void)
      {
	return b_out_port_map;
      }

      IOPort& add_out_port_map (const std::string& s, const IOPort& p);

      const goto_map_T& goto_map (void) const { return b_goto_map; }
      goto_map_T& goto_map (void) { return b_goto_map; }

      Goto& add_goto_map (const std::string& s, const Goto& g);

      const from_map_T& from_map (void) const { return b_from_map; }
      from_map_T& from_map (void) { return b_from_map; }

      From& add_from_map (const std::string& s, const From& g);

      const std::string&
      block_io_loop_type (void) const
      {
	return b_bio_loop_type;
      }

      std::string&
      block_io_loop_type (void)
      {
	return b_bio_loop_type;
      }

      const std::string&
      block_io_loop_var (void) const
      {
	return b_bio_loop_var;
      }

      std::string&
      block_io_loop_var (void)
      {
	return b_bio_loop_var;
      }

      const std::string*
      xml_systemsymbol (const std::string& s) const;

      std::string
      xml_blockname (const std::string& s, const std::string& b) const;

      const BlockNameEnt* find_blockname_entry (const std::string& s) const;
      BlockNameEnt* find_blockname_entry (const std::string& s);

      const BlockNameEnt* find_reference_block (const std::string& s) const;
      BlockNameEnt* find_reference_block (const std::string& s);

      BlockInfo* find_block (const std::string& s, const std::string& b);
      const BlockInfo* find_block (const std::string& s,
				   const std::string& b) const;

      BlockInfo* find_block (const std::string& n);
      const BlockInfo* find_block (const std::string& n) const;

      BlockInfo* previous_block (const SimulinkModel::XSD::ioport_T& in);

      IOPort* find_in_port (const std::string& b, const std::string& p);
      const IOPort* find_in_port (const std::string& b,
				  const std::string& p) const;

      IOPort* find_out_port (const std::string& b, const std::string& p);
      const IOPort* find_out_port (const std::string& b,
				   const std::string& p) const;

      Goto* find_goto (const std::string& b);
      const Goto* find_goto (const std::string& b) const;

      From* find_from (const std::string& b);
      const From* find_from (const std::string& b) const;

      void trace_flow_forward (BlockInfo& head, BlockInfo& tail,
			       pvar_ptr_list& pvpl);

      void trace_flow_backward (BlockInfo& head, BlockInfo& tail,
				pvar_ptr_list& pvpl);

      void trace_flow (void);

      void xml_complete (void);

      void out_xml (std::ostream &os);
      void out_xml (const char* file);
      void out_xml (void);

      // for access to parse_complete()
      friend int parse_code (Blocks&, void*, size_t);

      operator bool() const
      {
	return this != NULL;
      }

      enum file_type { XF_C_SOURCE, XF_C_HEADER, XF_C_DATA, XF_XML };

      void set_filename (file_type type, const char* file);

      const SimulinkModel::XSD::file_T::name_type*
      get_filename (file_type type) const;

      const std::string& rtm_macro (void) const { return b_rtm_macro; }
      std::string& rtm_macro (void) { return b_rtm_macro; }

      const std::string& rtm_struct (void) const { return b_rtm_struct; }
      std::string& rtm_struct (void) { return b_rtm_struct; }

      const std::string& rtm_extern_type (void) const
      {
	return b_rtm_extern_type;
      }
      std::string& rtm_extern_type (void) { return b_rtm_extern_type; }

      const std::string& rtm_extern_name (void) const
      {
	return b_rtm_extern_name;
      }
      std::string& rtm_extern_name (void) { return b_rtm_extern_name; }

      bool rtm_extern_has_struct (void) const
      {
	return b_rtm_extern_has_struct;
      }
      bool& rtm_extern_has_struct (void) { return b_rtm_extern_has_struct; }

      bool rtm_extern_has_const (void) const
      {
	return b_rtm_extern_has_const;
      }
      bool& rtm_extern_has_const (void) { return b_rtm_extern_has_const; }

      const std::string& rtm_object_type (void) const
      {
	return b_rtm_object_type;
      }
      std::string& rtm_object_type (void) { return b_rtm_object_type; }

      const std::string& rtm_object_name (void) const
      {
	return b_rtm_object_name;
      }
      std::string& rtm_object_name (void) { return b_rtm_object_name; }

      const std::string& rtm_object_pointer (void) const
      {
	return b_rtm_object_pointer;
      }
      std::string& rtm_object_pointer (void)
      {
	return b_rtm_object_pointer;
      }

      bool rtm_object_has_const (void) const { return b_rtm_object_has_const; }
      bool& rtm_object_has_const (void) { return b_rtm_object_has_const; }

      const std::string& rtm_code (void) const { return b_rtm_code; }
      std::string& rtm_code (void) { return b_rtm_code; }

      unsigned int rtm_line (void) const { return b_rtm_line; }
      unsigned int& rtm_line (void) { return b_rtm_line; }

      const std::string& rtm_init_code (void) const
      {
	return b_rtm_init_code;
      }
      std::string& rtm_init_code (void) { return b_rtm_init_code; }

      unsigned int rtm_init_line (void) const { return b_rtm_init_line; }
      unsigned int& rtm_init_line (void) { return b_rtm_init_line; }

      const std::string& common_code (void) const { return b_common_code; }
      std::string& common_code (void) { return b_common_code; }
      unsigned int common_line (void) const { return b_common_line; }
      unsigned int& common_line (void) { return b_common_line; }

      const std::string& common_init_code (void) const
      {
	return b_common_init_code;
      }
      std::string& common_init_code (void) { return b_common_init_code; }
      unsigned int common_init_line (void) const { return b_common_init_line; }
      unsigned int& common_init_line (void) { return b_common_init_line; }

      const function_map_T& function_decl (void) const
      {
	return b_function_decl_map;
      }
      function_map_T& function_decl (void) { return b_function_decl_map; }

      const function_map_T& function_defn (void) const
      {
	return b_function_defn_map;
      }
      function_map_T& function_defn (void) { return b_function_defn_map; }

      const std::string& step_func_auto_vars (void) const
      {
	return b_step_func_auto_vars;
      }
      std::string&
      step_func_auto_vars (void)
      {
	return b_step_func_auto_vars;
      }

      const std::vector < file_code_T >&
      code_macros (void) const
      {
	return b_code_macros;
      }
      std::vector < file_code_T >&
      code_macros (void)
      {
	return b_code_macros;
      }

      const sysdata_ptrmap_T& sysdata_ptrmap (void) const
      {
	return b_sysdata_ptrmap;
      }
      sysdata_ptrmap_T& sysdata_ptrmap (void) { return b_sysdata_ptrmap; }

      void
      add_sysdata_ptrmap (const std::string& name, SysData* sd)
      {
	sysdata_ptrmap ().insert (std::make_pair (name, sd));
      }

      const sysdata_list_T& sysdata_list (void) const
      {
	return b_sysdata_list;
      }
      sysdata_list_T& sysdata_list (void) { return b_sysdata_list; }

      void
      add_sysdata_list (const SysData& sd)
      {
	sysdata_list ().push_back (sd);
      }

      void change_externals (void);

      void check_call_function (void);

      const std::string& source_filename (void) const
      {
	return b_source_filename;
      }
      std::string& source_filename (void) { return b_source_filename; }

      const std::string header_filename (void) const
      {
	return b_header_filename;
      }
      std::string& header_filename (void) { return b_header_filename; }

      const include_map_T& include_map (void) const { return b_include_map; }
      include_map_T& include_map (void) { return b_include_map; }

      void add_include (const std::string& source,
			const std::string& include, char mode);

      const include_files_T* include_files (const std::string& source) const;
      include_files_T* include_files (const std::string& source);

      std::string storage_qualifier_variable (const VariableEnt& ve) const;

      bool has_variable (const BlockInfo& binfo, const VariableEnt* ve) const;
      bool has_variable (const BlockInfo& binfo, const BlockInfo* top,
			 const VariableEnt* ve) const;

      // track_bus
      bool has_dimensions (void) const { return b_has_dimensions; }
      bool& has_dimensions (void) { return b_has_dimensions; }

      enum track_mode { TRACK_BUS_PV, TRACK_BUS_FLOW };
      void track_bus (track_mode mode);

      const std::string& header_preamble (void) const
      {
	return b_header_preamble;
      }

      std::string& header_preamble (void) { return b_header_preamble; }

    protected:

      void parse_complete (void);

      int
      find_input_var (propagate_var_list& pv,
		      const SimulinkModel::XSD::block_T& blk,
		      const SimulinkModel::XSD::ioport_T& in,
		      std::string& usename);

      int
      find_subblock_input_var (propagate_var_list& pv,
			       const std::string& port,
			       const SimulinkModel::XSD::blocks_T& bs,
			       std::string& usename);

      int
      find_inport_var (propagate_var_list& pv,
		       const BlockInfo* up, const std::string& port,
		       std::string& usename);

      void process_blocks (BlockInfo* upper,
			   SimulinkModel::XSD::blocks_T& blks);

      void change_outport_var (const SimulinkModel::XSD::block_T& b);


      // for found Demnux
      BlockInfo*
      check_demux_output (propagate_var_list& pv,
			  const SimulinkModel::XSD::block_T& b,
			  const SimulinkModel::XSD::ioport_T& out,
			  const SimulinkModel::XSD::ioport_T& start_out,
			  std::string& usename);
      BlockInfo*
      check_subblock_demux_output (propagate_var_list& pv,
				   const std::string& port,
				   const SimulinkModel::XSD::blocks_T& bs,
				   const SimulinkModel::XSD::ioport_T&
				   start_out,
				   std::string& usename);

      BlockInfo*
      check_outport_demux_output (propagate_var_list& pv,
				  const BlockInfo* up,
				  const std::string& port,
				  const SimulinkModel::XSD::ioport_T&
				  start_out,
				  std::string& usename);

#define BLOCKINFO_LINEOUT_TO_TRIGGERPORT	0x01
#define BLOCKINFO_LINEOUT_TO_ENABLEPORT		0x02
#define BLOCKINFO_LINEOUT_TO_ACTIONPORT		0x04
#define BLOCKINFO_LINEOUT_TO_OTHER		0x08

      // for found TriggerPort and EnablePort
      int
      check_ports_output (BlockInfo& start_binfo,
			  const SimulinkModel::XSD::ioport_T& start_out,
			  std::string& usename,
			  const std::string& blockname,
			  const SimulinkModel::XSD::ioport_T& out);

      int
      check_subblock_ports_output (BlockInfo& start_binfo,
				   const SimulinkModel::XSD::ioport_T&
				   start_out,
				   std::string& usename,
				   const SimulinkModel::XSD::ioport_T& in,
				   const SimulinkModel::XSD::blocks_T& bs);

      int
      check_outport_ports_output (BlockInfo& start_binfo,
				  const SimulinkModel::XSD::ioport_T&
				  start_out,
				  std::string& usename,
				  const BlockInfo* up,
				  const std::string& port);

#ifdef ADD_INPUT_TO_ALL_SUBBLOCK
      void subblock_add_input (BlockInfo& subsys,
			       const SimulinkModel::XSD::block_T& b);
      void subblock_add_flow (BlockInfo* head, BlockInfo* sbinfo);
#endif	// ADD_INPUT_TO_ALL_SUBBLOCK

      void process_variables (BlockInfo& binfo,
			      const SimulinkModel::XSD::block_T* b,
			      const SimulinkModel::XSD::ioport_T* start_out = NULL,
			      const SimulinkModel::XSD::block_T* ob = NULL);

      void process_variables (void);

      void process_port (void);

      void process_goto_from (void);

      // track_bus
      static const std::string& line_varname (const std::string& usename,
					      const IOPort* start_port)
      {
	if (usename.length ())
	  return usename;
	return start_port->ioport_tag ()->line ();
      }

      static bool match_line_name (const ioport_deq_T& pass_ports,
				   const std::string& name)
      {
	for (ioport_deq_T::const_reverse_iterator ri (pass_ports.rbegin ());
	     ri != pass_ports.rend (); ri++)
	  {
	    // exclude input
	    if ((*ri)->iotype () == IOPort::IN)
	      continue;

	    const std::string& l = (*ri)->ioport_tag ()->line ();
	    if (l == name)
	      return true;
	  }
	return false;
      }

      size_t mux_out_index (IOPort* out_port, IOPort* in_port);

      bool track_bus_inport (IOPort* start_port,
			     int chunk_start, int chunk_size,
			     ioport_deq_T& pass_ports,
			     IOPort* in_port, NewMux* mux,
			     std::string& usename,
			     track_mode mode);

      bool track_bus_output (IOPort* start_port,
			     int chunk_start, int chunk_size,
			     ioport_deq_T& pass_ports,
			     const SimulinkModel::XSD::block_T& block,
			     NewMux* mux, std::string& usename,
			     track_mode mode);

      bool track_bus_outport (IOPort* start_port,
			      int chunk_start, int chunk_size,
			      ioport_deq_T& pass_ports,
			      IOPort* out_port, NewMux* mux,
			      std::string& usename,
			      track_mode mode);

      void track_bus (BlockInfo& start, track_mode mode);

      void process_all (void);

      void process_blocks (void);

      void process_one_subsystem (void);
      void process_one_subsystem (BlockInfo* top,
				  SimulinkModel::XSD::blocks_T& blks);

      int search_bus_input (BlockInfo& start);
      int search_bus_input (BlockInfo& start, int index,
			    const SimulinkModel::XSD::ioport_T& in,
			    std::string& usename);

      void process_bus (void);

      void subblock_input_from (BlockInfo* head, BlockInfo* sbinfo,
				const std::string& port);

      void input_from (BlockInfo* head,
		       const SimulinkModel::XSD::ioport_T& in);

      void subblock_output_to (BlockInfo* head, BlockInfo* sbinfo,
			       const std::string& port);

      void output_to (BlockInfo* head,
		      const SimulinkModel::XSD::ioport_T& out);

    private:

      blocks_T_PTR b_xml;

      blockname_map b_blockname;
      c_variable_map b_autovars;
      c_variable_map b_extvars;
      c_variable_map b_extstates;

      // state
      std::string b_state_type;
      std::string b_state_name;
      c_variable_map b_state_vars;
      var_list b_state_orders;

      // trigger state
      std::string b_trigger_state_type;
      std::string b_trigger_state_name;
      c_variable_map b_trigger_state_vars;
      var_list b_trigger_state_orders;

      // invariant block signal
      std::string b_invariant_signal_type;
      std::string b_invariant_signal_name;
      c_variable_map b_invariant_signal_vars;
      var_list b_invariant_signal_orders;

      // parameters
      std::string b_param_type;
      std::string b_param_name;
      c_variable_map b_parameters;
      var_list b_param_orders;

      // constant parameters
      std::string b_constant_param_type;
      std::string b_constant_param_name;
      c_variable_map b_constant_parameters;
      var_list b_constant_param_orders;

      // initializing
      std::string b_bio_loop_type;
      std::string b_bio_loop_var;

      // signals
      std::string b_signal_type;
      std::string b_signal_name;
      c_variable_map b_signal_vars;
      var_list b_signal_orders;

      // external inputs
      std::string b_external_input_type;
      std::string b_external_input_name;
      c_variable_map b_external_inputs;
      var_list b_external_input_orders;

      // external outputs
      std::string b_external_output_type;
      std::string b_external_output_name;
      c_variable_map b_external_outputs;
      var_list b_external_output_orders;

      // Real-Time Model
      std::string b_rtm_macro;  // in Header
      std::string b_rtm_struct; // in Header
      std::string b_rtm_extern_type;
      std::string b_rtm_extern_name;
      bool b_rtm_extern_has_struct;
      bool b_rtm_extern_has_const;

      std::string b_rtm_object_type; // in C
      std::string b_rtm_object_name;
      std::string b_rtm_object_pointer;
      bool b_rtm_object_has_const;

      std::string b_rtm_code;	// in C
      unsigned int b_rtm_line;	// in C

      std::string b_rtm_init_code;
      unsigned int b_rtm_init_line;

      std::string b_common_code; // in C
      unsigned int b_common_line;

      std::string b_common_init_code; // in C
      unsigned int b_common_init_line;	// in C

      function_map_T b_function_decl_map; // in C/H
      function_map_T b_function_defn_map;  // in C

      std::string b_step_func_auto_vars; // in C

      std::vector< file_code_T > b_code_macros;

      block_map_T b_block_map;
      ioport_map_T b_in_port_map;
      ioport_map_T b_out_port_map;

      sysdata_ptrmap_T b_sysdata_ptrmap;
      sysdata_list_T b_sysdata_list;

      std::string b_source_filename;
      std::string b_header_filename;

      include_map_T b_include_map;

      goto_map_T b_goto_map;
      from_map_T b_from_map;

      bool b_has_dimensions;

      std::string b_header_preamble;
    };

    Blocks_PTR parse_header (void* addr, size_t size, blocks_T_PTR& xml);
    Blocks_PTR parse_header (const char* f, blocks_T_PTR& xml);
    Blocks_PTR parse_header (const char* f);

    int parse_header (Blocks& bn, void* addr, size_t size);

    int parse_code (Blocks& bn, void* addr, size_t size);
    int parse_code (Blocks& bn, int fd);
    int parse_code (Blocks& bn, const char* file);

    int parse_data (Blocks& bn, std::istream& is);
    int parse_data (Blocks& bn, const char* file);

    Blocks_PTR parse_all_code (const char* xml, const char* header,
			       const char* source, const char* data,
			       bool validation);

    void c_canonical_name (std::string& out, const std::string& in);
    void c_canonical_name (std::string& out, const char* in);

    void xml_canonicalize (std::string& x);

    void set_parse_option (unsigned option);
    void clear_parse_option (unsigned option);
  }
}
#endif	// __cplusplus
#endif	// SIMULINKRTW_H
