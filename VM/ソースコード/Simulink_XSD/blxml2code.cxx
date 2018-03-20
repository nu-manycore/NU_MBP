#include <iostream>
#include <fstream>
#include <memory>
#include <deque>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "SimulinkModel.hxx"

#if __cplusplus >= 201103L && !defined(XSD_CXX11)
#error "Not defined XSD_CXX11 in SimulinkModel.hxx, please Re-make it"
#elif __cplusplus < 201103L && defined (XSD_CXX11)
#error "defined XSD_CXX11 in SimulinkModel.hxx, please Re-make it"
#endif

using namespace SimulinkModel;
using namespace SimulinkModel::XSD;

template< class V >
class name_map
  : public std::map< std::string, const V* >
{
public:
  typedef const V ValueType;
  typedef std::map< std::string, const V* > MapType;
  typedef std::pair< typename MapType::iterator, bool > ResType;

  static const V& value (typename MapType::const_iterator& i) { return **i; }
};

typedef name_map< block_T > block_map;
typedef block_map::iterator block_map_iterator;
typedef block_map::const_iterator block_map_const_iterator;

typedef std::deque < const block_T* > block_stack;
typedef block_stack::const_iterator block_stack_const_iterator;

class sig_list
  : public std::vector < const param_T* >
{
public:
  typedef const param_T ValueType;

  static const param_T& value (const_iterator& i) { return **i; }
};

typedef sig_list::iterator sig_list_iterator;
typedef sig_list::const_iterator sig_list_const_iterator;


template < class S >
class blxml_sequence_wrapper
{
public:
  blxml_sequence_wrapper (const S& seq) : bs_seq (seq) {}

  typedef typename S::value_type ValueType;
  typedef typename S::const_iterator const_iterator;

  const_iterator begin (void) const { return bs_seq.begin (); }
  const_iterator end (void) const { return bs_seq.end (); }

  static const ValueType& value (const_iterator& i) { return *i; }

private:
  const S& bs_seq;
};

class var_map
  : public std::map < const std::string, const void* >
{
public:
  typedef const var_T ValueType;
  typedef std::pair < iterator, bool > ResType;

  static const var_T& value (const_iterator& i)
  {
    return *reinterpret_cast < const var_T* > (i->second);
  }
};

typedef var_map::const_iterator var_map_const_iterator;
typedef var_map::iterator var_map_iterator;
typedef std::pair < var_map_iterator, bool > var_map_res;

class block_attr;
typedef std::vector < block_attr* > attr_list;

#if __cplusplus >= 201103L
typedef std::unique_ptr < blocks_T > blocks_T_PTR;
#else
typedef std::auto_ptr < blocks_T > blocks_T_PTR;
#endif

class var_list
  : public std::vector < const var_T* >
{
public:
  typedef std::vector < const var_T* > this_vec;
  typedef const var_T ValueType;
  static const var_T& value (const_iterator& i) { return **i; }

  void
  add_list (const var_T* v)
  {
    var_map_res r = vl_dup_check.insert (std::make_pair (v->name (), v));
    if (r.second)
      this_vec::push_back (v);
  }

private:
  var_map vl_dup_check;

  void push_back (const var_T* v); // not used
};

typedef std::map < std::string, var_list > storage_map_T;
typedef std::pair < storage_map_T::iterator, bool > storage_map_res;

class block_attr
{
public:
  block_attr (block_attr* u, const block_T* b, bool sb)
    : ba_id (ba_seq++), ba_block (b), ba_ignore (false), ba_subsys (sb),
      ba_upper (u)
  {
  }

  int id (void) const { return ba_id; }
  const block_T* block (void) const { return ba_block; }

  bool ignore (void) const { return ba_ignore; }

  void set_ignore (bool i) { ba_ignore = i; }

  block_attr* upper (void) { return ba_upper; }

  attr_list& lowers (void) { return ba_lowers; }

  const block_stack& merged (void) const { return ba_merged; }
  block_stack& merged (void) { return ba_merged; }

  const var_map& vin (void) const { return ba_vin; }
  var_map& vin (void) { return ba_vin; }

  const var_map& vout (void) const { return ba_vout; }
  var_map& vout (void) { return ba_vout; }

  operator bool() const { return ba_ignore; }

private:

  static int ba_seq;
  const int ba_id;
  const block_T* ba_block;
  bool ba_ignore;
  bool ba_subsys;
  block_attr* ba_upper;
  attr_list ba_lowers;
  block_stack ba_merged;
  var_map ba_vin;
  var_map ba_vout;
};

class attr_map
  : public std::map < const block_T*, block_attr >
{
public:
  const block_attr* get (const block_T* b) const
  {
    attr_map::const_iterator i = find (b);
    if (i == end ())
      return NULL;
    return &i->second;
  }

  block_attr* get (const block_T* b)
  {
    attr_map::iterator i = find (b);
    if (i == end ())
      return NULL;
    return &i->second;
  }

  int get_id (const block_T* b) const
  {
    const block_attr* ba = get (b);
    if (ba)
      return ba->id ();
    return -1;
  }

  block_attr& add (block_attr* u, const block_T* b, bool sb)
  {
    std::pair < attr_map::iterator, bool >
      x = insert (std::make_pair (b, block_attr (u, b, sb)));
    return x.first->second;
  }
};

typedef name_map< function_T > function_map;

class indent_writer
{
public:
  indent_writer (size_t ind) : iw_indent (ind) {}
  indent_writer (const indent_writer& iw) : iw_indent (iw.iw_indent) {}

  operator size_t () const { return iw_indent; }

private:
  size_t iw_indent;
};

class writer_attr
  : public indent_writer
{
public:
  writer_attr (const std::string& p = wa_null_prefix, size_t ind = 0,
	       bool pre_lf = false, bool post_lf = true,
	       const std::string* com = NULL, bool defmacro = false)
    : indent_writer (ind),
      wa_prefix (p), wa_pre_lf (pre_lf), wa_post_lf (post_lf),
      wa_comment (com), wa_defmacro (defmacro)
  {
  }

  writer_attr (const writer_attr& wa)
    : indent_writer (wa), wa_prefix (wa.wa_prefix),
      wa_pre_lf (wa.wa_pre_lf), wa_post_lf (wa.wa_post_lf),
      wa_comment (wa.wa_comment), wa_defmacro (wa.wa_defmacro)
  {
  }

  const std::string& prefix (void) const { return wa_prefix; }
  bool pre_lf (void) const { return wa_pre_lf; }
  bool post_lf (void) const { return wa_post_lf; }
  const std::string* comment (void) const { return wa_comment; }
  bool defmacro (void) const { return wa_defmacro; }

  void pre_lf (bool f) { wa_pre_lf = f; }
  void post_lf (bool f) { wa_post_lf = f; }
  void comment (const std::string* c) { wa_comment = c; }
  void defmacro (bool f) { wa_defmacro = f; }

private:
  writer_attr (const char*, ...); // DO NOT USE!

  static const std::string wa_null_prefix;

  const std::string& wa_prefix;
  bool wa_pre_lf;
  bool wa_post_lf;
  const std::string* wa_comment;
  bool wa_defmacro;
};

template< class O >
class base_writer
{
public:
  typedef O ObjectType;

  base_writer (const O& o, writer_attr& wa) : bw_wa (wa), bw_obj (o) {}

  operator writer_attr& () const { return bw_wa; }
  operator const O& () const { return bw_obj; }

private:
  writer_attr& bw_wa;
  const O& bw_obj;
};

template< class P >
struct var_defn_writer : public base_writer< P >
{
  var_defn_writer (const P& v, writer_attr& wa) : base_writer< P > (v, wa) {}
};

template< class P >
struct var_init_writer : public base_writer< P >
{
  var_init_writer (const P& v, writer_attr& wa) : base_writer< P > (v, wa) {}
};

template< class P >
struct inarg_defn_writer : public base_writer< P >
{
  inarg_defn_writer (const P& v, writer_attr& wa) : base_writer< P > (v, wa) {}
};

template< class P >
struct outarg_defn_writer : public base_writer< P >
{
  outarg_defn_writer (const P& v, writer_attr& wa) : base_writer< P > (v, wa) {}
};

template< class P >
struct arg_undef_writer : public base_writer< P >
{
  arg_undef_writer (const P& v, writer_attr& wa) : base_writer< P > (v, wa) {}
};

template< class P >
struct inarg_decl_writer : public base_writer< P >
{
  inarg_decl_writer (const P& v, writer_attr& wa) : base_writer< P > (v, wa) {}
};

template< class P >
struct outarg_decl_writer : public base_writer< P >
{
  outarg_decl_writer (const P& v, writer_attr& wa) : base_writer< P > (v, wa) {}
};

struct function_writer : public base_writer< function_T >
{
  typedef function_T ObjectType;

  function_writer (const function_T& f, writer_attr& wa)
    : base_writer< function_T > (f, wa)
  {
  }
};

struct code_writer : public base_writer< code_T >
{
  typedef code_T ObjectType;

  code_writer (const code_T& c, writer_attr& wa)
    : base_writer< code_T > (c, wa)
  {
  }
};

template < class W >
class writer_binder
{
public:
  typedef W WriterType;
  typedef typename W::ObjectType ObjectType;

  writer_binder (writer_attr& wa) : ow_wa (wa) {}

  operator writer_attr& () const { return ow_wa; }

private:
  writer_attr& ow_wa;
};

// this class useing for var_T, param_T, function_T, block_T, and blocks_T.
template< class T >
struct get_name_predictor
{
  typedef T ValueType;
  const std::string& operator() (const T& t) const { return t.name (); }
};

typedef get_name_predictor< var_T > var_name_predictor;
typedef get_name_predictor< param_T > param_name_predictor;
typedef get_name_predictor< function_T > function_name_predictor;

template < class T >
struct null_writer_hooks
{
  bool filter (const T& t) const { return false; }
  void pre_writer (const T&) const {}
  void post_write (const T&) const {}
  void clear (void) const {}
};

template< class NamePred >
struct map_name_predictor
{
  map_name_predictor () : mnp_pred (NamePred ()) {}
  const NamePred& mnp_pred;
};

template < class M, class NamePred >
struct dup_map_filter
  : virtual map_name_predictor< NamePred >,
    virtual null_writer_hooks< typename NamePred::ValueType >
{
  typedef typename NamePred::ValueType ValueType;

  dup_map_filter (M& m) : dmf_map (m) {}

  bool filter (const ValueType& t) const
  {
    return !dmf_map.insert (std::make_pair (this->mnp_pred (t), &t)).second;
  }

  void clear (void) const { dmf_map.clear (); }

  M& dmf_map;
};

template < class M, class NamePred >
struct ignore_map_filter
  : virtual map_name_predictor< NamePred >,
    virtual null_writer_hooks< typename NamePred::ValueType >
{
  typedef typename NamePred::ValueType Valuetype;

  ignore_map_filter (const M& m) : imf_map (m) {}

  bool filter (const Valuetype& t) const
  {
    return imf_map.find (this->mnp_pred (t)) != imf_map.end ();
  }

  const M& imf_map;
};

template< class IM, class DM, class NamePred >
struct ignore_and_dup_filter
  : ignore_map_filter< IM, NamePred >, dup_map_filter< DM, NamePred >
{
  typedef typename NamePred::ValueType Valuetype;
  typedef ignore_map_filter< IM, NamePred > ignore_filter;
  typedef dup_map_filter< DM, NamePred > dup_filter;

  ignore_and_dup_filter (const IM& im, DM& dm)
    : ignore_filter (im), dup_filter (dm)
  {
  }

  bool filter (const Valuetype& t) const
  {
    return ignore_filter::filter (t) || dup_filter::filter (t);
  }
};


template< class NamePred >
struct dup_check_filter
  : dup_map_filter< name_map< typename NamePred::ValueType >, NamePred >
{
  typedef name_map< typename NamePred::ValueType > MapType;

  dup_check_filter ()
    : dup_map_filter< MapType, NamePred > (fmh_map) {}

  MapType fmh_map;
};


template < class C, class B,
	   class H = null_writer_hooks < typename C::ValueType > >
class container_writer
{
public:
  typedef C ContainerType;

  container_writer (const C& c, const B& wbinder, H& hooks = cw_null_hooks)
    : cw_container (c), cw_wbinder (wbinder), cw_hooks (hooks)
  {
  }

  const C& container (void) const { return cw_container; }
  const B& wbinder (void) const { return cw_wbinder; }
  H& hooks (void) const { return cw_hooks; }

  static null_writer_hooks < typename C::ValueType > cw_null_hooks;

private:

  const C& cw_container;
  const B& cw_wbinder;
  H& cw_hooks;
};

template< class C, class B, class H >
null_writer_hooks< typename C::ValueType >
container_writer< C, B, H >::cw_null_hooks;

template < class Pred >
class selected_states
{
public:
  selected_states (const block_stack& stack,
		   const std::string& prefix,
		   const std::string& storage,
		   const attr_map& attrmap,
		   const Pred& pred)
    : ss_stack (stack), ss_prefix (prefix),
      ss_storage (storage), ss_attrmap (attrmap), ss_pred (pred)
  {
  }

  const block_stack& stack (void) const { return ss_stack; }
  const std::string& prefix (void) const { return ss_prefix; }
  const std::string& storage (void) const { return ss_storage; }
  const attr_map& attrmap (void) const { return ss_attrmap; }
  const Pred& pred (void) const { return ss_pred; }

private:
  const block_stack& ss_stack;
  const std::string& ss_prefix;
  const std::string& ss_storage;
  const attr_map& ss_attrmap;
  const Pred& ss_pred;
};


struct block_signal
{
  typedef block_T::signal_type ElementType;
  typedef block_T::signal_sequence SequenceType;

  const SequenceType&
  operator() (const block_T& b) const
  {
    return b.signal ();
  }
};

struct block_param
{
  typedef block_T::param_type ElementType;
  typedef block_T::param_sequence SequenceType;

  const SequenceType&
  operator() (const block_T& b) const
  {
    return b.param ();
  }
};

struct block_cparam
{
  typedef block_T::const_param_type ElementType;
  typedef block_T::const_param_sequence SequenceType;

  const SequenceType&
  operator() (const block_T& b) const
  {
    return b.const_param ();
  }
};

struct block_state
{
  typedef block_T::state_type ElementType;
  typedef block_T::state_sequence SequenceType;

  const SequenceType&
  operator() (const block_T& b) const
  {
    return b.state ();
  }
};

struct block_trigger
{
  typedef block_T::trigger_state_type ElementType;
  typedef block_T::trigger_state_sequence SequenceType;

  const SequenceType&
  operator() (const block_T& b) const
  {
    return b.trigger_state ();
  }
};

struct block_invariant
{
  typedef block_T::invariant_signal_type ElementType;
  typedef block_T::invariant_signal_sequence SequenceType;

  const SequenceType&
  operator() (const block_T& b) const
  {
    return b.trigger_state ();
  }
};


class BLXML
{
public:
  BLXML () {}
  BLXML (std::istream& is);
  BLXML (blocks_T_PTR& b) : blks (b.release ()) {}
  ~BLXML () {}

  int load (std::istream& is);
  int gencode (std::ostream& os) const;

  void check_merged_block (void);
  void merge_inout_vars (void);

  void dump (void) const;

#if __cplusplus >= 201103L
  operator bool () const { return this != NULL && blks; }
#else
  operator bool () const { return this != NULL && blks.get () != NULL; }
#endif

  static bool is_code_subsystem (const block_T& b);

private:

  void add_block_map (block_attr* upper, const blocks_T& bs);

  void gen_param_define (std::ostream& os, const std::string& prefix,
			 var_map& vin, var_map& vout, const var_map& vsig,
			 const block_T& b, bool& first) const;
  void gen_param_define (std::ostream& os, const std::string& prefix,
			 var_map& vin, var_map& vout, const var_map& vsig,
			 block_stack& stack, bool& first) const;
  void gen_param_define (std::ostream& os, const std::string& prefix,
			 var_map& vin, var_map& vout, const var_map& vsig,
			 const block_T& b, block_stack& stack) const;

  void gen_var_define (std::ostream& os, const std::string& prefix,
		       var_map& vmap, const var_map& vsig,
		       const block_T& b, bool& first) const;
  void gen_var_define (std::ostream& os, const std::string& prefix,
		       var_map& vmap, const var_map& vsig,
		       block_stack& stack, bool& first) const;
  void gen_var_define (std::ostream& os, const std::string& prefix,
		       var_map& vmap, const var_map& vsig,
		       const block_T& b, block_stack& stack) const;

  void gen_param_undefine (std::ostream& os, const std::string& prefix,
			   const block_T& b, bool& first) const;
  void gen_param_undefine (std::ostream& os, const std::string& prefix,
			   block_stack& stack, bool& first) const;
  void gen_param_undefine (std::ostream& os, const std::string& prefix,
			   const block_T& b, block_stack& stack) const;

  int find_out_signal (sig_list&, const block_T& b) const;

  template < class Pred >
  inline selected_states < Pred >
  get_selected_states (const block_stack& stack,
		       const std::string& prefix,
		       const std::string& storage,
		       const Pred& pred) const;

  void gen_input (std::ostream& os, int& n,
		  var_map& vin, const var_map& vsig, const block_T& b) const;
  void gen_input (std::ostream& os, int& n,
		  var_map& vin, const var_map& vsig, block_stack& stack) const;
  void gen_input (std::ostream& os, int& n,
		  var_map& vin, const var_map& vsig,
		  const block_T& b, block_stack& stack) const;

  void gen_output (std::ostream& os, int& n,
		   var_map& vout, const var_map& vsig, const block_T& b) const;
  void gen_output (std::ostream& os, int& n,
		   var_map& vout, const var_map& vsig,
		   block_stack& stack) const;
  void gen_output (std::ostream& os, int& n,
		   var_map& vout, const var_map& vsig,
		   const block_T& b, block_stack& stack) const;

  int gencode (std::ostream& os, const block_T& b, const std::string& type,
	       const sig_list& siglist, const var_map& vsig,
	       const code_T* autovars, block_stack& stack) const;
  int gencode (std::ostream& os, const blocks_T& bs,
	       const code_T* autovars, block_stack& stack) const;

  void define_states (std::ostream& os, const block_T& b,
		      sig_list& siglist,
		      const std::string& prefix, block_stack& stack) const;
  void undefine_states (std::ostream& os, const block_T& b,
			sig_list& siglist,
			const std::string& prefix, block_stack& stack) const;

  void ignore_mark_block (block_attr* cur);
  void ignore_mark_block (void);
  void check_merged_block (block_attr* base, block_attr* cur);

  void merge_inout_vars (block_attr* base, const block_T* b);

  template< class Pred >
  void get_storage_and_size (const block_T& b, const block_attr* ba,
			     int& cnt, const std::string*& storage,
			     const Pred& pred) const;

  void pickup_storage_variable (const block_T& b, const block_attr* ba,
				storage_map_T& storage_map) const;

  void get_storage (block_stack& stack,
		    int& sig_cnt, int& param_cnt, int& cparam_cnt,
		    int& state_cnt, int& trigger_cnt, int& invariant_cnt,
		    const std::string*& sig_storage,
		    const std::string*& param_storage,
		    const std::string*& cparam_storage,
		    const std::string*& state_storage,
		    const std::string*& trigger_storage,
		    const std::string*& invariant_storage,
		    storage_map_T& storage_map) const;

  int push_dsm (const block_T& b, block_stack& stack) const;

  void load_function_map (const blocks_T& bs);

  blocks_T_PTR blks;

  block_map blockmap;
  block_map ignore_map;

  block_map datastore_map;

  attr_list attrlist;
  attr_map attrmap;

  function_map func_map;
};

static int verbose = 0;		// TODO

static int gen_function_params = 1;

static int no_validate = 0;

static std::vector < const char * > header_list;

int block_attr::ba_seq = 0;

static const std::string empty_prefix ("");
static const std::string x_prefix ("x");

const std::string writer_attr::wa_null_prefix ("");

BLXML::BLXML (std::istream& is)
{
  load (is);
}

void
BLXML::add_block_map (block_attr* upper, const blocks_T& bs)
{
  for (blocks_T::block_const_iterator i (bs.block ().begin ());
       i != bs.block ().end (); i++)
    {
      const block_T& b = *i;
      blockmap.insert (std::make_pair (b.name(), &b));

      if (b.blocktype () == "DataStoreMemory")
	{
	  const block_T::DataStoreName_optional& o_dsn = b.DataStoreName ();
	  if (o_dsn)
	    datastore_map.insert (std::make_pair (*o_dsn, &b));
	}

      block_attr& ba = attrmap.add (upper, &b, b.blocktype () == "SubSystem");
      attrlist.push_back (&ba);

      if (upper)
	upper->lowers ().push_back (&ba);

      for (block_T::blocks_const_iterator j (b.blocks ().begin ());
	   j != b.blocks ().end (); j++)
	add_block_map (&ba, *j);
    }
}

int
BLXML::load (std::istream& is)
{
  blocks_T_PTR b;

  try
    {
      if (no_validate)
	b.reset (blocks (is, xml_schema::flags::dont_validate).release ());
      else
	b.reset(blocks (is).release ());
      add_block_map (NULL, *b);
      load_function_map (*b);
    }
  catch (const xml_schema::error& e)
    {
      std::cerr << "Error: " << e << std::endl;
      return -1;
    }
  catch (const xercesc::XMLException& e)
    {
      std::cerr << "Error: unexpected Xerces-C++ exception" << std::endl;
      return -1;
    }
  catch (const std::exception& e)
    {
      std::cerr << "Error: " << e.what() << std::endl;
      return -1;
    }
  catch (...)
    {
      std::cerr << "Error" << std::endl;
      return -1;
    }

  blks.reset (b.release ());
  return 0;
}

std::string
c_canonicalname (const std::string& in)
{
  std::string out;
  char c[in.length() + 1];
  char* d = c;
  for (const char* s = in.c_str(); *s; s++)
    {
      unsigned char u = *s;
      if (isalnum (u) || u == '_')
	*d++ = u;
      else if (u == '-')	// replace `-' to `_', and ignore others.
	*d++ = '_';
    }
  *d = 0;
  return std::string (c);
}


int
BLXML::find_out_signal (sig_list& list, const block_T& b) const
{
  const block_attr* ba = attrmap.get (&b);
  if (ba->merged().size ())
    {
      for (block_stack::const_iterator i (ba->merged().begin ());
	   i != ba->merged ().end (); i++)
	find_out_signal (list, **i);
    }

  for (block_T::backward_const_iterator bci (b.backward ().begin ());
       bci != b.backward ().end (); bci++)
    {
      block_map_const_iterator bi = blockmap.find (bci->block());
      const block_T* fb = bi->second;

      for (block_T::signal_const_iterator si (fb->signal ().begin ());
	   si != fb->signal ().end (); si++)
	{
	  const param_T& p = *si;
	  for (block_T::var_const_iterator vci (b.var ().begin ());
	       vci != b.var ().end (); vci++)
	    {
	      const var_T& v = *vci;

	      if (v.name () == p.name ())
		{
		  sig_list_const_iterator x = std::find (list.begin (),
							 list.end (),
							 &p);
		  if (x == list.end ())
		    list.push_back (&p);
		  break;
		}
	    }
	}
    }
  return list.size ();
}


template < class C, class WB >
container_writer < C, WB >
get_container_writer (const C& container, const WB& wbinder)
{
  return container_writer < C, WB > (container, wbinder);
}

template < class C, class WB, class H >
container_writer < C, WB, H >
get_container_writer (const C& container, const WB& wbinder, H& hooks)
{
  return container_writer < C, WB, H > (container, wbinder, hooks);
}

template < class Pred >
inline selected_states < Pred >
BLXML::get_selected_states (const block_stack& stack,
			    const std::string& prefix,
			    const std::string& storage,
			    const Pred& pred) const
{
  return selected_states < Pred > (stack, prefix, storage, attrmap, pred);
}


template < class S >
blxml_sequence_wrapper< S >
get_blxml_sequence_wrapper (const S& s)
{
  return blxml_sequence_wrapper< S > (s);
}


std::ostream&
operator<< (std::ostream& os, const indent_writer& iw)
{
  size_t ind  = iw;
  size_t t = ind / 4;
  size_t s = ind % 4;

  while (t--)
    os << '\t';
  while (s--)
    os << "  ";
  return os;
}

template < class P >
std::ostream&
operator<< (std::ostream& os, const var_defn_writer< P >& vdw)
{
  const P& v = vdw;
  writer_attr& wa = vdw;
  const std::string& prefix = wa.prefix ();
  const typename P::size_optional& s = v.size ();
  const indent_writer& iw = wa;
  const std::string* c = wa.comment ();

  if (wa.pre_lf ())
    {
      os << '\n';
      wa.pre_lf (false);
    }

  if (wa.defmacro ())
    {
      os << "#define " << v.name () << ' ';
      if (prefix.length ())
	os << prefix << "__";
      os << v.name () << '\n';
    }

  if (iw)
    os << iw;

  os << v.type () << ' ';

  if (prefix.length ())
    os << prefix << "__";
  os << v.name ();
  if (s)
    os << '[' << *s << ']';
  os << ';';

  if (c)
    os << " /* " << *c << " */";

  if (wa.post_lf ())
    os << '\n';
  return os;
}

template < class P >
std::ostream&
operator<< (std::ostream& os, const var_init_writer< P >& viw)
{
  const P& p = viw;
  writer_attr& wa = viw;
  const typename P::initcode_optional& ic = p.initcode ();
  if (ic)
    {
      if (wa.pre_lf ())
	{
	  os << '\n';
	  wa.pre_lf (false);
	}
      os << *ic;
      if (wa.post_lf ())
	os << '\n';
    }
  return os;
}

template < class P >
std::ostream&
operator<< (std::ostream& os, const inarg_defn_writer< P >& iadw)
{
  writer_attr& wa = iadw;
  const indent_writer& iw = wa;
  const std::string& prefix = wa.prefix ();
  const std::string* com = wa.comment ();
  const P& p = iadw;

  if (wa.pre_lf ())
    {
      os << '\n';
      wa.pre_lf (false);
    }
  os << iw << "#define " << p.name () << ' ';
  if (prefix.length ())
    os << prefix << "__";
  os << p.name ();
  if (com)
    os << " /* " << *com << " */";
  if (wa.post_lf ())
    os << '\n';
  return os;
}

template < class P >
std::ostream&
operator<< (std::ostream& os, const outarg_defn_writer< P >& oadw)
{
  writer_attr& wa = oadw;
  const indent_writer& iw = wa;
  const std::string& prefix = wa.prefix ();
  const std::string* com = wa.comment ();
  const P& p = oadw;
  const typename P::size_optional& s = p.size ();

  if (wa.pre_lf ())
    {
      os << '\n';
      wa.pre_lf (false);
    }
  os << iw << "#define " << p.name () << ' ';
  if (s)
    {
      if (prefix.length ())
	os << prefix << "__";
      os << p.name ();
    }
  else
    {
      os << "(*";
      if (prefix.length ())
	os << prefix << "__";
      os << p.name () << ")";
    }
  if (com)
    os << " /* " << *com << " */";
  if (wa.post_lf ())
    os << '\n';
  return os;
}

template < class P >
std::ostream&
operator<< (std::ostream& os, const arg_undef_writer< P >& auw)
{
  writer_attr& wa = auw;
  const indent_writer& iw = wa;
  const P& p = auw;

  if (wa.pre_lf ())
    {
      os << '\n';
      wa.pre_lf (false);
    }
  os << iw << "#undef " << p.name ();
  if (wa.post_lf ())
    os << '\n';
  return os;
}

template < class P >
std::ostream&
operator<< (std::ostream& os, const inarg_decl_writer< P >& idw)
{
  const P& p = idw;
  writer_attr& wa = idw;
  const std::string& prefix = wa.prefix ();
  const typename P::size_optional &s = p.size();

  os << p.type() << ' ';
  if (prefix.length ())
    os << prefix << "__";
  os << p.name();
  if (s)
    os << "[" << *s << "]";
  return os;
}

template < class P >
std::ostream&
operator<< (std::ostream& os, const outarg_decl_writer< P >& idw)
{
  const P& p = idw;
  writer_attr& wa = idw;
  const std::string& prefix = wa.prefix ();
  const typename P::size_optional &s = p.size();

  os << p.type() << ' ';
  if (!s)
    os << '*';
  if (prefix.length ())
    os << prefix << "__";
  os << p.name();
  if (s)
    os << "[" << *s << "]";
  return os;
}

std::ostream&
operator<< (std::ostream& os, const function_writer& fw)
{
  const function_T& f = fw;
  const function_T::code_optional& fcode = f.code ();
  if (fcode)
    os << *fcode;
  return os;
}

std::ostream&
operator<< (std::ostream& os, const code_writer& cw)
{
  const code_T& c = cw;
  writer_attr& wa = cw;

  if (wa.pre_lf ())
    {
      os << '\n';
      wa.pre_lf (false);
    }

  os << c;

  if (wa.post_lf ())
    os << '\n';

  return os;
}

template < class C, class B, class H >
std::ostream&
operator<< (std::ostream& os, const container_writer< C, B, H >& vcw)
{
  const C& container = vcw.container ();
  const B& wb = vcw.wbinder ();
  H& hooks = vcw.hooks ();
  for (typename C::const_iterator vi (container.begin ());
       vi != container.end (); vi++)
    {
      const typename C::ValueType& v = C::value (vi);
      if (hooks.filter (v))
	continue;

      hooks.pre_writer (v);
      os << typename B::WriterType (v, wb);
      hooks.post_write (v);
    }
  return os;
}

template < class B, class H, class Pred >
static void
output_storage_member (std::ostream& os, const block_stack& stack,
		       const attr_map& attrmap,
		       const B& wrbinder, H& wrhook, const Pred& pred)
{
  for (block_stack_const_iterator bi (stack.begin ());
       bi != stack.end (); bi++)
    {
      const block_T& b = **bi;
      os << get_container_writer (get_blxml_sequence_wrapper (pred (b)),
				  wrbinder, wrhook);

      const block_attr* ba = attrmap.get (&b);
      for (block_stack::const_iterator i (ba->merged ().begin ());
	   i != ba->merged ().end (); i++)
	os << get_container_writer (get_blxml_sequence_wrapper (pred (**i)),
				    wrbinder, wrhook);
    }
}


template < class Pred >
std::ostream&
operator<< (std::ostream& os, const selected_states< Pred >& ss)
{
  typedef typename Pred::ElementType ElementType;
  typedef get_name_predictor< ElementType > filter_pred;
  struct init_code_check : dup_check_filter < filter_pred >
  {
    init_code_check (bool f = true) : has_initcode (f) {}

    void
    pre_writer (const ElementType& p)
    {
      const typename ElementType::initcode_optional& ic = p.initcode ();
      if (!ic)
	has_initcode = false;
    }

    bool has_initcode;
  };

  const std::string& storage = ss.storage ();
  const std::string& prefix = ss.prefix ();
  const Pred& pred = ss.pred ();
  const block_stack& stack = ss.stack ();
  const attr_map& attrmap = ss.attrmap ();

  init_code_check icc;
  writer_attr wa (empty_prefix, 1, false, true, NULL, false);
  writer_binder< var_defn_writer < ElementType > > wb (wa);

  os << '\n'
     << "#define " << storage << " ";
  if (prefix.length ())
    os << prefix << "__";
  os << storage << "\n"
     << "struct {\n";

  output_storage_member (os, stack, attrmap, wb, icc, pred);

  os << "} ";
  if (prefix.length ())
    os << prefix << "__";
  os << storage;

  if (icc.has_initcode)
    {
      dup_check_filter< filter_pred >& wh = icc;
      writer_binder< var_init_writer < ElementType > > init_wb (wa);
      wa.post_lf (false);
      wh.clear ();

      os << " = {\n";

      output_storage_member (os, stack, attrmap, init_wb, wh, pred);

      os << '}';
    }

  os << ";\n";

  return os;
}


template< class S >
static int
get_storage_and_size_local (const S& seq, const std::string*& storage)
{
  int size = seq.size ();
  if (size)
    {
      if (storage == NULL)
	storage = &(*seq.begin()).storage ();
    }
  return size;
}


template< class Pred >
void
BLXML::get_storage_and_size (const block_T& b, const block_attr* ba,
			     int& cnt, const std::string*& storage,
			     const Pred& pred) const
{
  cnt += get_storage_and_size_local (pred (b), storage);
  for (block_stack::const_iterator i (ba->merged ().begin ());
       i != ba->merged ().end (); i++)
    cnt += get_storage_and_size_local (pred (**i), storage);
}


static void
pickup_storage_variable_local (const block_T& b, storage_map_T& storage_map)
{
  // pick up input/output variables in storage
  for (block_T::var_const_iterator vi (b.var().begin());
       vi != b.var().end(); vi++)
    {
      const var_T& v = *vi;
      const var_T::storage_optional& vso = v.storage();
      if (vso)
	{
	  storage_map_res
	    res = storage_map.insert (std::make_pair(*vso, var_list()));
	  var_list& vl = res.first->second;
	  vl.add_list (&v);
	}
    }
}

void
BLXML::pickup_storage_variable (const block_T& b, const block_attr* ba,
				storage_map_T& storage_map) const
{
  pickup_storage_variable_local (b, storage_map);
  for (block_stack::const_iterator i (ba->merged ().begin ());
       i != ba->merged ().end (); i++)
    pickup_storage_variable_local (**i, storage_map);
}

void
BLXML::get_storage (block_stack& stack,
		    int& sig_cnt, int& param_cnt, int& cparam_cnt,
		    int& state_cnt, int& trigger_cnt, int& invariant_cnt,
		    const std::string*& sig_storage,
		    const std::string*& param_storage,
		    const std::string*& cparam_storage,
		    const std::string*& state_storage,
		    const std::string*& trigger_storage,
		    const std::string*& invariant_storage,
		    storage_map_T& storage_map) const
{
  sig_cnt = 0;
  param_cnt = 0;
  cparam_cnt = 0;
  state_cnt = 0;
  trigger_cnt = 0;
  invariant_cnt = 0;

  sig_storage = NULL;
  param_storage = NULL;
  cparam_storage = NULL;
  state_storage = NULL;
  trigger_storage = NULL;
  invariant_storage = NULL;

  for (block_stack_const_iterator bi (stack.begin());
       bi != stack.end(); bi++)
    {
      const block_T& sb = **bi;
      const block_attr* ba = attrmap.get (&sb);
      get_storage_and_size (sb, ba, sig_cnt, sig_storage,
			    block_signal ());
      get_storage_and_size (sb, ba, param_cnt, param_storage,
			    block_param ());
      get_storage_and_size (sb, ba, cparam_cnt, cparam_storage,
			    block_cparam ());
      get_storage_and_size (sb, ba, state_cnt, state_storage,
			    block_state ());
      get_storage_and_size (sb, ba, trigger_cnt, trigger_storage,
			    block_trigger ());
      get_storage_and_size (sb, ba, invariant_cnt, invariant_storage,
			    block_invariant ());
      pickup_storage_variable (sb, ba, storage_map);
    }
}


int
BLXML::push_dsm (const block_T& b, block_stack& stack) const
{
  int n = 0;
  const block_attr* ba = attrmap.get (&b);

  if (ba->merged().size ())
    {
      for (block_stack::const_iterator i (ba->merged().begin ());
	   i != ba->merged ().end (); i++)
	n += push_dsm (**i, stack);
    }

  const block_T::DataStoreName_optional& o_dsn = b.DataStoreName ();

  if (!o_dsn || b.blocktype () == "DataStoreMemory")
    return n;

  block_map_const_iterator i = datastore_map.find (*o_dsn);
  if (i == datastore_map.end ())
    return n;

  const block_T* dsm = i->second;
  if (dsm == &b)
    return n;

  if (std::find (stack.begin (), stack.end (), dsm) != stack.end ())
    return n;

  stack.push_front (dsm);
  return n + 1;
}

void
BLXML::define_states (std::ostream& os, const block_T& b,
		      sig_list& siglist,
		      const std::string& prefix, block_stack& stack) const
{
  std::string name (prefix + "__" + c_canonicalname (b.name ()));
  int sig_cnt = 0;
  int param_cnt = 0;
  int cparam_cnt = 0;
  int state_cnt = 0;
  int trigger_cnt = 0;
  int invariant_cnt = 0;

  const std::string* sig_storage = NULL;
  const std::string* param_storage = NULL;
  const std::string* cparam_storage = NULL;
  const std::string* state_storage = NULL;
  const std::string* trigger_storage = NULL;
  const std::string* invariant_storage = NULL;

  int dsm_pushed = push_dsm (b, stack);

  stack.push_front (&b);

  storage_map_T storage_map;

  get_storage (stack,
	       sig_cnt, param_cnt, cparam_cnt,
	       state_cnt, trigger_cnt, invariant_cnt,
	       sig_storage, param_storage, cparam_storage,
	       state_storage, trigger_storage, invariant_storage,
	       storage_map);

  if (siglist.size())
    {
      sig_cnt += siglist.size();

      if (sig_storage == NULL)
	{
	  const param_T* p = siglist.front();
	  sig_storage = &p->storage ();
	}
    }

  // signal & param & state macros
  if (sig_cnt)
    os << get_selected_states (stack, name, *sig_storage, block_signal ());

  if (param_cnt)
    os << get_selected_states (stack, name, *param_storage, block_param ());

  if (cparam_cnt)
    os << get_selected_states (stack, name, *cparam_storage, block_cparam ());

  if (state_cnt)
    os << get_selected_states (stack, name, *state_storage, block_state ());

  if (trigger_cnt)
    os << get_selected_states (stack, name, *trigger_storage,
			       block_trigger ());

  if (invariant_cnt)
    os << get_selected_states (stack, name, *invariant_storage,
			       block_invariant ());


  // output variables in extin/extout/signal storage
  dup_check_filter< var_name_predictor > wh;
  writer_attr wa (empty_prefix, 1, false, true, NULL, false);
  writer_binder< var_defn_writer < var_T > > wb (wa);
  for (storage_map_T::iterator i (storage_map.begin ());
       i != storage_map.end (); i++)
    {
      const std::string& st = i->first;
      if (sig_cnt && *sig_storage == st)
	continue;		// already output

      os << '\n'
	 << "#define " << st << " " << name << "__" << st << "\n"
	 << "struct {\n";

      const var_list& vl = i->second;
      os << get_container_writer (vl, wb, wh);

      os << "} " << name << "__" << st << ";\n";
    }

  stack.pop_front();

  while (dsm_pushed--)
    stack.pop_front ();
}


void
BLXML::undefine_states (std::ostream& os, const block_T& b,
			sig_list& siglist,
			const std::string& prefix, block_stack& stack) const
{
  std::string name (prefix + "__" + c_canonicalname (b.name ()));

  int sig_cnt = 0;
  int param_cnt = 0;
  int cparam_cnt = 0;
  int state_cnt = 0;
  int trigger_cnt = 0;
  int invariant_cnt = 0;

  const std::string* sig_storage = NULL;
  const std::string* param_storage = NULL;
  const std::string* cparam_storage = NULL;
  const std::string* state_storage = NULL;
  const std::string* trigger_storage = NULL;
  const std::string* invariant_storage = NULL;

  int dsm_pushed = push_dsm (b, stack);

  stack.push_front (&b);

  storage_map_T storage_map;

  get_storage (stack,
	       sig_cnt, param_cnt, cparam_cnt,
	       state_cnt, trigger_cnt, invariant_cnt,
	       sig_storage, param_storage, cparam_storage,
	       state_storage, trigger_storage, invariant_storage,
	       storage_map);

  if (siglist.size())
    {
      sig_cnt += siglist.size ();
      if (sig_storage == NULL)
	sig_storage = &siglist.front()->storage ();
    }

  os << '\n';

  if (sig_cnt)
    os << "#undef " << *sig_storage << "\n";
  if (param_cnt)
    os << "#undef " << *param_storage << "\n";
  if (cparam_cnt)
    os << "#undef " << *cparam_storage << "\n";
  if (state_cnt)
    os << "#undef " << *state_storage << "\n";
  if (trigger_cnt)
    os << "#undef " << *trigger_storage << "\n";
  if (invariant_cnt)
    os << "#undef " << *invariant_storage << "\n";

  if (storage_map.size ())
    {
      for (storage_map_T::iterator i (storage_map.begin ());
	   i != storage_map.end (); i++)
	{
	  const std::string& st = i->first;
	  if (sig_cnt && *sig_storage == st)
	    continue;
	  os << "#undef " << st << "\n";
	}
    }

  stack.pop_front ();

  while (dsm_pushed--)
    stack.pop_front ();
}


void
BLXML::gen_param_define (std::ostream& os, const std::string& prefix,
			 var_map& vin, var_map& vout, const var_map& vsig,
			 const block_T& b, bool& first) const
{
  typedef writer_binder< inarg_defn_writer < var_T > > inarg_writer_binder;
  typedef writer_binder< outarg_defn_writer < var_T > > outarg_writer_binder;
  typedef ignore_and_dup_filter< var_map, var_map,
				 var_name_predictor > writer_hooks;

  const block_attr* ba = attrmap.get (&b);

  writer_attr wa (prefix, 0, first, true, NULL, false);
  writer_hooks iwh (vsig, vin);
  writer_hooks owh (vsig, vout);

  // variables macros
  os << get_container_writer (ba->vin (), inarg_writer_binder (wa), iwh)
     << get_container_writer (ba->vout (), outarg_writer_binder (wa), owh);

  first = wa.pre_lf ();
}


void
BLXML::gen_param_define (std::ostream& os, const std::string& prefix,
			 var_map& vin, var_map& vout, const var_map& vsig,
			 block_stack& stack, bool& first) const
{
  for (block_stack_const_iterator bi (stack.begin ());
       bi != stack.end (); bi++)
    {
      const block_T& sb = **bi;
      gen_param_define (os, prefix, vin, vout, vsig, sb, first);
    }
}

void
BLXML::gen_param_define (std::ostream& os, const std::string& prefix,
			 var_map& vin, var_map& vout, const var_map& vsig,
			 const block_T& b, block_stack& stack) const
{
  bool first = true;
  gen_param_define (os, prefix, vin, vout, vsig, stack, first);
  gen_param_define (os, prefix, vin, vout, vsig, b, first);
}


void
BLXML::gen_var_define (std::ostream& os, const std::string& prefix,
		       var_map& vmap, const var_map& vsig,
		       const block_T& b, bool& first) const
{
  typedef writer_binder< var_defn_writer < var_T > > writer_binder;
  typedef ignore_and_dup_filter< var_map, var_map,
				 var_name_predictor > writer_hooks;

  const block_attr* ba = attrmap.get (&b);

  writer_attr wa (prefix, 0, first, true, NULL, true);
  writer_hooks wh (vsig, vmap);

  // variables macros
  os << get_container_writer (ba->vin (), writer_binder (wa), wh)
     << get_container_writer (ba->vout (), writer_binder (wa), wh);

  first = wa.pre_lf ();
}


void
BLXML::gen_var_define (std::ostream& os, const std::string& prefix,
		       var_map& vmap, const var_map& vsig,
		       block_stack& stack, bool& first) const
{
  for (block_stack_const_iterator bi (stack.begin ());
       bi != stack.end (); bi++)
    {
      const block_T& sb = **bi;
      gen_var_define (os, prefix, vmap, vsig, sb, first);
    }
}

void
BLXML::gen_var_define (std::ostream& os, const std::string& prefix,
		       var_map& vmap, const var_map& vsig,
		       const block_T& b, block_stack& stack) const
{
  bool first = true;
  gen_var_define (os, prefix, vmap, vsig, stack, first);
  gen_var_define (os, prefix, vmap, vsig, b, first);
}


void
BLXML::gen_param_undefine (std::ostream& os, const std::string& prefix,
			   const block_T& b, bool& first) const
{
  typedef writer_binder< arg_undef_writer < var_T > > undef_binder;

  const block_attr* ba = attrmap.get (&b);

  // variables macros
  writer_attr wa (prefix, 0, first, true, NULL, false);
  dup_check_filter< var_name_predictor > iwh;
  dup_check_filter< var_name_predictor > owh;
  os << get_container_writer (ba->vin (), undef_binder (wa), iwh)
     << get_container_writer (ba->vout (), undef_binder (wa), owh);
  first = wa.pre_lf ();
}


void
BLXML::gen_param_undefine (std::ostream& os,  const std::string& prefix,
			   block_stack& stack, bool& first) const
{
  for (block_stack_const_iterator bi (stack.begin ());
       bi != stack.end (); bi++)
    {
      const block_T& sb = **bi;
      gen_param_undefine (os, prefix, sb, first);
    }
}


void
BLXML::gen_param_undefine (std::ostream& os, const std::string& prefix,
			   const block_T& b, block_stack& stack) const
{
  bool first = true;
  gen_param_undefine (os, prefix, stack, first);
  gen_param_undefine (os, prefix, b, first);
}


void
BLXML::gen_input (std::ostream& os, int& n,
		  var_map& vin, const var_map& vsig, const block_T& b) const
{
  typedef writer_binder < inarg_decl_writer < var_T > > inarg_binder;

  typedef ignore_map_filter< var_map, var_name_predictor > ign_filter;
  typedef dup_map_filter< var_map, var_name_predictor > dup_filter;
  struct writer_hooks : ign_filter, dup_filter
  {
    writer_hooks (var_map& vmap, const var_map& sig,
		  std::ostream& os, int& num)
      : ign_filter (sig), dup_filter (vmap), wh_os (os), wh_num (num)
    {
    }

    bool
    filter (const var_T& v) const
    {
      return (ign_filter::filter (v)
	      || ((v.mode () == "extin" || v.mode () == "input")
		  && v.storage())
	      || dup_filter::filter (v));
    }

    void pre_writer (const var_T&) const { if (wh_num++) wh_os << ", "; }

    std::ostream& wh_os;
    int& wh_num;
  };

  const block_attr* ba = attrmap.get (&b);

  writer_attr wa (x_prefix, 0, false, false, NULL, false);
  writer_hooks wh (vin, vsig, os, n);

  // input variables
  os << get_container_writer (ba->vin (), inarg_binder (wa), wh);
}

void
BLXML::gen_input (std::ostream& os, int& n,
		  var_map& vin, const var_map& vsig, block_stack& stack) const
{
  for (block_stack_const_iterator bi (stack.begin ());
       bi != stack.end (); bi++)
    {
      const block_T& sb = **bi;
      gen_input (os, n, vin, vsig, sb);
    }
}


void
BLXML::gen_input (std::ostream& os, int& n,
		  var_map& vin, const var_map& vsig,
		  const block_T& b, block_stack& stack) const
{
  gen_input (os, n, vin, vsig, stack);
  gen_input (os, n, vin, vsig, b);
}


void
BLXML::gen_output (std::ostream& os, int& n,
		   var_map& vout, const var_map& vsig, const block_T& b) const
{
  typedef ignore_map_filter< var_map, var_name_predictor > ign_filter;
  typedef dup_map_filter< var_map, var_name_predictor > dup_filter;
  typedef writer_binder < outarg_decl_writer < var_T > > outarg_binder;

  struct writer_hooks : ign_filter, dup_filter
  {
    writer_hooks (var_map& vmap, const var_map& sig,
		  std::ostream& os, int& num)
      : ign_filter (sig), dup_filter (vmap), wh_os (os), wh_num (num)
    {
    }

    bool
    filter (const var_T& v) const
    {
      return (ign_filter::filter (v)
	      || ((v.mode () == "extout" || v.mode () == "output")
		  && v.storage())
	      || dup_filter::filter (v));
    }

    void pre_writer (const var_T&) const { if (wh_num++) wh_os << ", "; }

    std::ostream& wh_os;
    int& wh_num;
  };

  const block_attr* ba = attrmap.get (&b);

  writer_attr wa (x_prefix, 0, false, false, NULL, false);
  writer_hooks wh (vout, vsig, os, n);

  os << get_container_writer (ba->vout (), outarg_binder (wa), wh);
}


void
BLXML::gen_output (std::ostream& os, int& n, var_map& vout,
		   const var_map& vsig, block_stack& stack) const
{
  for (block_stack_const_iterator bi (stack.begin ());
       bi != stack.end (); bi++)
    {
      const block_T& sb = **bi;
      gen_output (os, n, vout, vsig, sb);
    }
}


void
BLXML::gen_output (std::ostream& os, int& n,
		   var_map& vout, const var_map& vsig,
		   const block_T& b, block_stack& stack) const
{
  gen_output (os, n, vout, vsig, stack);
  gen_output (os, n, vout, vsig, b);
}


int
BLXML::gencode (std::ostream& os, const block_T& b, const std::string& type,
		const sig_list& siglist, const var_map& vsig,
		const code_T* autovars, block_stack& stack) const
{
  const block_attr* ba = attrmap.get (&b);
  if (ba->ignore ())
    {
      os << "\n/* ignore: --- " << b.name () << " --- */\n";
      return 0;
    }

  size_t npush = 0;

  for (block_stack::const_iterator i (ba->merged().begin ());
       i != ba->merged().end (); i++)
    {
      block_stack::const_iterator x = std::find (stack.begin (),
						 stack.end (), *i);
      if (x == stack.end ())
	{
	  stack.push_front (*i);
	  npush++;
	}
    }

  std::string name (type + "__" + c_canonicalname (b.name ()));
  int n = 0;

  bool has_block_function = (b.function ().size () != 0);

  if (!has_block_function && gen_function_params)
    {
      // variables macros
      var_map vin;
      var_map vout;
      gen_param_define (os, "x", vin, vout, vsig, b, stack);
    }

  os << '\n'
     << "void " << name << "(";

  bool nothing = true;
  if (!has_block_function && gen_function_params)
    {
      // input variables
      var_map vin;
      gen_input (os, n, vin, vsig, b, stack);

      // output variables;
      var_map vout;
      gen_output (os, n, vout, vsig, b, stack);

      if (vin.size () || vout.size ())
	nothing = false;
    }

  if (nothing)
    os << "void";

  os << ")\n" << "{\n";

  if (autovars)
    {
      os << *autovars;
      os << '\n';
    }

  struct code_hooks : null_writer_hooks < code_T >
  {
    code_hooks (const std::string& type) : ch_type (type) {}
    bool filter (const code_T& c) const
    {
      return c.type () != ch_type;
    }
    const std::string& ch_type;
  };

  writer_attr wa (empty_prefix, 0, false, false, NULL, false);
  code_hooks wh (type);
  os << get_container_writer (get_blxml_sequence_wrapper (b.code ()),
			      writer_binder< code_writer > (wa), wh);

  os << "}\n";
  if (!has_block_function && gen_function_params)
    gen_param_undefine (os, "x", b, stack);

  while (npush-- > 0)
    stack.pop_front ();

  return 0;
}

int
BLXML::gencode (std::ostream& os, const blocks_T& bs,
		const code_T* autovars, block_stack& stack) const
{
  for (blocks_T::block_const_iterator bi (bs.block().begin());
       bi != bs.block().end(); bi++)
    {
      const block_T& b = *bi;
      const block_attr* ba = attrmap.get (&b);
      if (ba->ignore ())
	{
	  os << "\n/* ignore: --- " << b.name () << " --- */\n";
	  continue;
	}

      const std::string& blocktype = b.blocktype();

      bool done_task = false;
      bool done_update = false;
      bool done_init = false;

      bool no_code_block = false;

      if ((blocktype == "SubSystem" && !is_code_subsystem (b))
	  || blocktype == "Inport"
	  || blocktype == "Outport"
	  || blocktype == "EnablePort"
	  || blocktype == "TriggerPort")
	no_code_block = true;

      sig_list siglist;

      find_out_signal (siglist, b);

      var_map vsig;
      for (sig_list_const_iterator si (siglist.begin ());
	   si != siglist.end (); si++)
	{
	  const param_T* s = *si;
	  vsig.insert (std::make_pair (s->name(), s));
	}

      for (block_T::signal_const_iterator si (b.signal().begin());
	   si != b.signal().end(); si++)
	{
	  const param_T& s = *si;
	  vsig.insert (std::make_pair (s.name(), &s));
	}

      bool has_block_function = (b.function ().size () != 0);

      std::string prefix ("block__" + c_canonicalname (b.name ()));

      if (!no_code_block && has_block_function)
	{
	  var_map vmap;
	  gen_var_define (os, prefix, vmap, vsig, b, stack);
	  undefine_states (os, b, siglist, "block", stack);
	  define_states (os, b, siglist, "block", stack);

	  for (block_T::function_const_iterator fi (b.function ().begin ());
	       fi != b.function ().end (); fi++)
	    {
	      function_map::const_iterator i = func_map.find (fi->name ());
	      if (i == func_map.end ())
		continue;
	      const function_T::code_optional& fcode = i->second->code ();
	      if (fcode)
		os << '\n' << *fcode;
	    }
	}
      else if (gen_function_params)
	{
	  undefine_states (os, b, siglist, "block", stack);
	  define_states (os, b, siglist, "block", stack);
	}

      for (block_T::code_const_iterator ci (b.code().begin());
	   ci != b.code().end(); ci++)
	{
	  const code_T& c = *ci;
	  const std::string& type = c.type ();

	  if (type == "task" && done_task == false)
	    {
	      if (no_code_block)
		goto no_code_block_error;
	      else
		{
		  int
		    x = gencode (os, b, type, siglist, vsig, autovars, stack);
		  if (x)
		    return x;
		}
	      done_task = true;
	    }
	  else if (type == "update" && done_update == false)
	    {
	      if (no_code_block)
		goto no_code_block_error;
	      else
		{
		  int
		    x = gencode (os, b, type, siglist, vsig, autovars, stack);
		  if (x)
		    return x;
		}
	      done_update = true;
	    }
	  else if (type == "init" && done_init == false)
	    {
	      if (no_code_block)
		{
		no_code_block_error:
		  os << "\n/* !!! " << blocktype << " " << b.name()
		     << " has some code [" << type << "] */\n";
		}
	      else
		{
		  int
		    x = gencode (os, b, type, siglist, vsig, autovars, stack);
		  if (x)
		    return x;
		}
	      done_init = true;
	    }
	}

      if (has_block_function)
	gen_param_undefine (os, prefix, b, stack);

      undefine_states (os, b, siglist, "block", stack);

      stack.push_back (&b);

      for (block_T::blocks_const_iterator bsi (b.blocks().begin());
	   bsi != b.blocks().end(); bsi++)
	{
	  const blocks_T& sbs = *bsi;
	  int x = gencode (os, sbs, autovars, stack);
	  if (x)
	    return x;
	}

      stack.pop_back ();
    }

  return 0;
}

static bool
inc_cmp (const std::pair < int, const include_T* >& a,
	 const std::pair < int, const include_T* >& b)
{
  return a.first < b.first;
}

int
BLXML::gencode (std::ostream& os) const
{
  typedef writer_binder< function_writer > function_binder;

  struct function_decl_hooks : null_writer_hooks < function_T >
  {
    bool filter (const function_T& f) const { return f.type () != "decl"; }
  };

  struct function_defn_hooks : null_writer_hooks < function_T >
  {
    bool filter (const function_T& f) const
    {
      return f.type () != "defn" || f.block ().size () != 0;
    }
  };

  const blocks_T& bs = *blks;
  const code_T* autovars = NULL;
  int mark_index = -1;

  std::vector < std::pair < int, const include_T* > > before_inclist;
  std::vector < std::pair < int, const include_T* > > after_inclist;

  for (blocks_T::includes_const_iterator ii (bs.includes().begin());
       ii != bs.includes().end (); ii++)
    {
      include_T::mark_optional mark = ii->mark ();
      if (mark)
	{
	  mark_index = ii->index ();
	  break;
	}
    }

  for (blocks_T::includes_const_iterator ii (bs.includes().begin());
       ii != bs.includes().end (); ii++)
    {
      if (mark_index != -1 && ii->index () >= mark_index)
	{
	  if (ii->index () > mark_index)
	    after_inclist.push_back (std::make_pair (ii->index(), &*ii));
	}
      else
	before_inclist.push_back (std::make_pair (ii->index(), &*ii));
    }

  std::sort (before_inclist.begin (), before_inclist.end (), inc_cmp);
  std::sort (after_inclist.begin (), after_inclist.end (), inc_cmp);

  for (std::vector < std::pair < int, const include_T* > >::const_iterator
	 i (before_inclist.begin ()); i != before_inclist.end (); i++)
    {
      os << "#include " << i->second->file () << std::endl;
    }

  bool out_preamble = false;
  for (blocks_T::code_const_iterator ci (bs.code().begin());
       ci != bs.code().end(); ci++)
    {
      const code_T& c = *ci;
      if (c.type () == "preamble")
	{
	  os << '\n' << c << '\n';
	  out_preamble = true;
	}
    }

  for (blocks_T::code_const_iterator ci (bs.code().begin());
       ci != bs.code().end(); ci++)
    {
      const code_T& c = *ci;
      if (c.type () == "c_macro")
	os << c << "\n";
    }

  for (blocks_T::code_const_iterator ci (bs.code().begin());
       ci != bs.code().end(); ci++)
    {
      const code_T& c = *ci;
      if (c.type () == "rt-macro")
	os << c << "\n";
    }

  for (blocks_T::code_const_iterator ci (bs.code().begin());
       ci != bs.code().end(); ci++)
    {
      const code_T& c = *ci;
      if (c.type () == "rt-type")
	os << c << "\n";
      if (c.type () == "autovars")
	autovars = &c;
    }

  for (blocks_T::deftype_const_iterator di (bs.deftype().begin());
       di != bs.deftype().end(); di++)
    {
      const object_T& o = *di;

      const object_T::kind_optional& ko = o.kind ();
      const object_T::size_optional& sz = o.size();

      if (ko && (*ko == "param" || *ko == "bsignal" || *ko == "bstate"))
	os << o;
      else
	{
	  os << "typedef " << o.type() << " " << o.name();
	  if (sz)
	    os << "[" << *sz << "]";
	  os << ";\n";
	}
    }

  for (blocks_T::object_const_iterator oi (bs.object().begin());
       oi != bs.object().end(); oi++)
    {
      const object_T& o = *oi;

      const object_T::value_optional& v = o.value();

      os << o.type() << " " << o.name();
      if (v)
	os << " = " << *v;
      os << ";\n";
    }

  if (out_preamble)
    os << "\n#endif\n\n";

  for (std::vector < std::pair < int, const include_T* > >::const_iterator
	 i (after_inclist.begin ()); i != after_inclist.end (); i++)
    os << "#include " << i->second->file () << std::endl;

  for (std::vector < const char *>::const_iterator i (header_list.begin ());
       i != header_list.end (); i++)
    os << "#include \"" << *i << "\"\n";

  writer_attr wa (empty_prefix, 0, true, false, NULL, false);
  function_decl_hooks decl_hooks;
  function_defn_hooks defn_hooks;

  // output sub function declarations
  os << get_container_writer (get_blxml_sequence_wrapper (bs.function ()),
			      function_binder (wa), decl_hooks);

  // output sub function definitions
  wa.pre_lf (true);
  os << get_container_writer (get_blxml_sequence_wrapper (bs.function ()),
			      function_binder (wa), defn_hooks);

  block_stack stack;

  return gencode (os, bs, autovars, stack);
}


void
BLXML::ignore_mark_block (block_attr* cur)
{
  const block_T* b = cur->block ();

  if (cur->ignore ())
    return;

  cur->set_ignore (true);

  if (b->blocktype () == "SubSystem")
    {
      for (attr_list::iterator x (cur->lowers ().begin ());
	   x != cur->lowers ().end (); x++)
	ignore_mark_block (*x);
    }
}


void
BLXML::ignore_mark_block (void)
{
  for (attr_list::iterator i (attrlist.begin ()); i != attrlist.end (); i++)
    {
      block_attr* ba = *i;
      if (ba->ignore ())
	continue;		// already ignored

      const block_T* b = ba->block ();
      for (block_T::merge_block_const_iterator mi (b->merge_block ().begin ());
	   mi != b->merge_block ().end (); mi++)
	{
	  block_map_iterator bi = blockmap.find (mi->block ());
	  block_attr* mba = attrmap.get (bi->second);

	  ignore_mark_block (mba);
	}
    }
}


void
BLXML::check_merged_block (block_attr* base, block_attr* cur)
{
  const block_T* b = cur->block ();

  base->merged ().push_back (b);

  if (b->blocktype () == "SubSystem")
    {
      for (attr_list::iterator x (cur->lowers ().begin ());
	   x != cur->lowers ().end (); x++)
	check_merged_block (base, *x);
    }
}


void
BLXML::check_merged_block (void)
{
  ignore_mark_block ();

  for (attr_list::iterator i (attrlist.begin ());
       i != attrlist.end (); i++)
    {
      block_attr* ba = *i;
      if (ba->ignore ())
	continue;		// already ignored

      const block_T* b = ba->block ();
      for (block_T::merge_block_const_iterator mi (b->merge_block ().begin ());
	   mi != b->merge_block ().end (); mi++)
	{
	  block_map_iterator bi = blockmap.find (mi->block ());
	  block_attr* mba = attrmap.get (bi->second);

	  check_merged_block (ba, mba);
	}
    }
}


void
BLXML::merge_inout_vars (block_attr* base, const block_T* b)
{
  for (block_T::var_const_iterator vi (b->var ().begin ());
       vi != b->var ().end (); vi++)
    {
      const var_T& v = *vi;
      var_map_iterator xi = base->vin ().find (v.name ());
      if (xi != base->vin ().end ())
	continue;		// already existing in vin
      if (v.storage ())
	continue;		// ignore variable in storage
      xi = base->vout ().find (v.name ());
      if (xi != base->vout ().end ())
	{
	  // remove from vout, add to vin
	  base->vout ().erase (xi);
	  base->vin ().insert (std::make_pair (v.name (), &v));
	  continue;
	}

      if (v.mode () == "extout" || v.mode () == "output")
	base->vout ().insert (std::make_pair (v.name (), &v));
      else
	base->vin ().insert (std::make_pair (v.name (), &v));
    }
}


void
BLXML::merge_inout_vars (void)
{
  for (attr_list::iterator i (attrlist.begin ());
       i != attrlist.end (); i++)
    {
      block_attr* ba = *i;
      if (ba->ignore ())
	continue;		// already ignored

      merge_inout_vars (ba, ba->block ());
      for (block_stack::iterator i (ba->merged ().begin ());
	   i != ba->merged ().end (); i++)
	merge_inout_vars (ba, *i);
    }
}


void
BLXML::dump (void) const
{
  // TODO
}


bool
BLXML::is_code_subsystem (const block_T& b)
{
  // Atomic SubSystem
  const block_T::TreatAsAtomicUnit_optional& at = b.TreatAsAtomicUnit ();
  if (at && *at == "on")
    return true;

  const block_T::type_optional& bt = b.type ();
  if (bt && (*bt == "iterator"
	     || *bt == "stateflow"
	     || *bt == "atomic"
	     || *bt == "action"
	     || ((*bt == "triggered"
		  || *bt == "enabled"
		  || *bt == "enabled_triggered")
		 && b.merge_block ().size () > 0)))
    return true;
  return false;
}


void
BLXML::load_function_map (const blocks_T& bs)
{
  for (blocks_T::function_const_iterator fi (bs.function ().begin ());
       fi != bs.function ().end (); fi++)
    {
      const function_T& func = *fi;
      if (func.type () == "defn")
	func_map[func.name ()] = &func;
    }
}


int
main(int argc, char * const *argv)
{
  int c;
  const char *output = NULL;
  const char *input = NULL;
  int x;

  while ((c = getopt (argc, argv, "nvo:NH:")) != -1)
    {
      switch (c)
	{
	case 'v':
	  verbose = 1;
	  break;
	case 'o':
	  output = optarg;
	  break;
	case 'N':
	  gen_function_params = 0;
	  break;
	case 'H':
	  header_list.push_back (optarg);
	  break;
	case 'n':
	  no_validate = 1;
	  break;
	case '?':
	  std::cerr << "usage: " << argv[0]
		    << " [-n] [-v] [-o output] [-N] [-H user-header] [BLXML]"
		    << std::endl;
	  return 1;
	}
    }

  argv += optind;

  if (*argv)
    input = *argv;

  BLXML blxml;

  if (input)
    {
      std::ifstream ifs (input);
      if (!ifs)
	{
	  std::cerr << input << ": " << strerror (errno) << std::endl;
	  return 1;
	}
      x = blxml.load (ifs);
    }
  else
    x = blxml.load (std::cin);

  if (x)
    return 1;

  blxml.check_merged_block ();
  blxml.merge_inout_vars ();

  if (output)
    {
      std::ofstream ofs (output);
      if (!ofs)
	{
	  std::cerr << input << ": " << strerror (errno) << std::endl;
	  return 1;
	}
      x = blxml.gencode (ofs);
    }
  else
    {
      x = blxml.gencode (std::cout);
    }

  if (x)
    return 1;
  return 0;
}
