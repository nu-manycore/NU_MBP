// -*- mode: c++; -*-
#ifndef SIMULINKXML_H
#define SIMULINKXML_H

#include <vector>
#include <fstream>
#include <string>
#include <memory>
#include <algorithm>

#include "SimulinkModel.hxx"

#if __cplusplus >= 201103L && !defined(XSD_CXX11)
#error "Not defined XSD_CXX11 in SimulinkModel.hxx, please Re-make it"
#elif __cplusplus < 201103L && defined (XSD_CXX11)
#error "defined XSD_CXX11 in SimulinkModel.hxx, please Re-make it"
#endif

namespace Simulink
{
  namespace XML
  {
#ifdef XSD_CXX11
    typedef std::unique_ptr< SimulinkModel::XSD::blocks_T > blocks_T_PTR;
#else
    typedef std::auto_ptr< SimulinkModel::XSD::blocks_T > blocks_T_PTR;
#endif

    // empty class for BlockInfo
    class empty_Base_BlockInfo
    {
    };

    // empty class for BlockConnect
    class empty_Base_BlockConnect
    {
    };

    // empty class for XML var_t wrapper
    class empty_Base_var_T
    {
    public:
      empty_Base_var_T(const SimulinkModel::XSD::var_T* v) {}
    };

    //
    // template class defines
    //

    // <T>: an any basic class of BlockInfo
    // <C>: an any basic class of BlockConnect
    // <V>: an any basic class of var_T_wrapper (in BlockConnect)
    // <BI>: delibered BlockInfo class

    // define BlockInfo template class
    template <class T = empty_Base_BlockInfo,
	      class C = empty_Base_BlockConnect,
	      class V = empty_Base_var_T > class Tmpl_BlockInfo;

    // Default type
    typedef Tmpl_BlockInfo<> BlockInfo;

    // define Reader template class
    template <class BI = BlockInfo,
	      class C = empty_Base_BlockConnect,
	      class V = empty_Base_var_T> class Tmpl_Reader;

    // defne BlockConnect template class
    template <class BI = BlockInfo,
	      class C = empty_Base_BlockConnect,
	      class V = empty_Base_var_T> class Tmpl_BlockConnect;

    // define block_connect_list_T template class
    template <class BI = BlockInfo,
	      class C = empty_Base_BlockConnect,
	      class V = empty_Base_var_T> class Tmpl_block_connect_list_T;

    // define an entry of Tmpl_var_list_T<V> (XML var_T wrapper)
    template <class V = empty_Base_var_T> class Tmpl_var_T_wrapper;

    // define vector of Tmpl_var_T_wrapper
    template <class V = empty_Base_var_T> class Tmpl_var_list_T;

    // define block name map
    template <class BI = BlockInfo> class Tmpl_block_map_T;

    //
    // template function
    //
    // find multirate subsystem
    template <class BI>
    BI*
    find_multirate_subsystem (const BI* binfo)
    {
      if (binfo == NULL)
	return NULL;

      BI* cur = binfo->upper_subsystem ();

      while (cur)
	{
	  if (cur->is_subsystem() &&
	      (cur->subsystem_type () == BI::TRIGGERED
	       || cur->subsystem_type () == BI::ENABLED))
	    return cur;
	  cur = cur->upper_subsystem ();
	}

      return NULL;
    }

    //
    // classes
    //

    //
    // BlockInfo(PTR) list
    //
    template <class BI = BlockInfo>
    class Tmpl_block_list_T
      : public std::vector < BI* >
    {
      typedef std::vector < BI* > this_vec;
      typedef typename this_vec::iterator this_iterator;
      typedef typename this_vec::const_iterator this_const_iterator;

    public:
      Tmpl_block_list_T () : l_any (0) {}
      Tmpl_block_list_T (size_t size, BI* i = NULL)
	: this_vec (size, i), l_any (0)
      {
      }

      const BI*
      operator+= (BI* b) { addinfo (b); return b; }

      bool
      checkinfo (const BI* b) const
      {
	this_const_iterator i = std::find (this_vec::begin (),
					     this_vec::end (), b);
	return i != this_vec::end ();
      }

      void
      addinfo (BI* b)
      {
	if (std::find (this_vec::begin (), this_vec::end (), b)
	    == this_vec::end ())
	  this_vec::push_back (b);
      }

      // flag for any
      unsigned long any (void) const { return l_any; }
      unsigned long& any (void) { return l_any; }
    private:
      unsigned long l_any;
    };

    // Default type
    typedef Tmpl_block_list_T<> block_list_T;
    typedef block_list_T::iterator block_list_iterator;
    typedef block_list_T::const_iterator block_list_const_iterator;

    // Default type
    typedef Tmpl_block_list_T< const BlockInfo > const_block_list_T;
    typedef const_block_list_T::iterator const_block_list_iterator;
    typedef const_block_list_T::const_iterator const_block_list_const_iterator;

    //
    // template of var_T* wrapper
    //
    template <class V>
    class Tmpl_var_T_wrapper : public V
    {
    public:
      Tmpl_var_T_wrapper(const SimulinkModel::XSD::var_T* var)
	: V (var), x_var(var) {}
      operator const SimulinkModel::XSD::var_T* () const { return x_var; }
      const SimulinkModel::XSD::var_T* var() const { return x_var; }
    private:
      const SimulinkModel::XSD::var_T* x_var;
    };

    //
    // template of XML Var list
    //
    template <class V>
    class Tmpl_var_list_T : public std::vector < Tmpl_var_T_wrapper< V > >
    {
    };

    // Default type
    typedef Tmpl_var_list_T<> var_list_T;

    //
    // template of Block Connect Pair
    //
    template <class BI, class C, class V>
    class Tmpl_BlockConnect : public C
    {
      typedef Tmpl_var_list_T<V> this_var_list_T;
      typedef Tmpl_BlockConnect<BI,C,V> this_BlockConnect;
    public:
      Tmpl_BlockConnect ()
	: bc_tail (NULL), bc_head (NULL), bc_pair (NULL) {}
      Tmpl_BlockConnect (BI* tail, BI* head,
			 const SimulinkModel::XSD::link_T* link = NULL)
	: bc_tail (tail), bc_head (head), bc_link (link), bc_pair (NULL)
      {
      }

      const this_var_list_T& var_list (void) const { return bc_var_list; }
      this_var_list_T& var_list (void) { return bc_var_list; }

      BI* head (void) const { return bc_head; }
      BI*& head (void) { return bc_head; }
      BI* tail (void) const { return bc_tail; }
      BI*& tail (void) { return bc_tail; }
      const SimulinkModel::XSD::link_T* link (void) const { return bc_link; }
      const SimulinkModel::XSD::link_T*& link (void) { return bc_link; }

      bool operator== (const this_BlockConnect& bc) const
      {
	return tail () == bc.tail () && head () == bc.head ();
      }

      const this_BlockConnect* pair (void) const { return bc_pair; }
      this_BlockConnect*& pair (void) { return bc_pair; }

    private:

      this_var_list_T bc_var_list;

      BI* bc_tail;
      BI* bc_head;

      const SimulinkModel::XSD::link_T* bc_link;
      this_BlockConnect* bc_pair;
    };

    // Default Type
    typedef Tmpl_BlockConnect<> BlockConnect;

    //
    // template of BlockConnect list
    //
    template <class BI, class C, class V>
    class Tmpl_block_connect_list_T
      : public std::vector < Tmpl_BlockConnect<BI,C,V> >
    {
    };

    // Default Type
    typedef Tmpl_block_connect_list_T<> block_connect_list_T;

    //
    // BlockInfo
    //

    // Base BlockInfo
    class base_BlockInfo
    {
    public:
      typedef enum { NONE,
		     ATOMIC,
		     ENABLED,
		     TRIGGERED,
		     ENABLED_TRIGGERED,
		     ACTION,
		     ITERATOR,
		     STATEFLOW } subsystem_type_T;

      base_BlockInfo(SimulinkModel::XSD::block_T* block,
		     SimulinkModel::XSD::block_T* upper)
	: b_block(block), b_upper(upper),
	  b_is_subsystem (false),
	  b_has_update (false), b_is_sfunction (false),
	  b_subsystem_type (NONE)
      {
	SimulinkModel::XSD::block_T::id_optional& oid = block->id ();
	if (oid)
	  {
	    b_id = *oid;
	    if (seq < b_id)
	      seq = b_id;
	  }
	else
	  b_id = 0;
      }

      unsigned int
      id (void) const
      {
	return b_id;
      }

      SimulinkModel::XSD::block_T*
      block (void) const
      {
	return b_block;
      }

      SimulinkModel::XSD::block_T*&
      block (void)
      {
	return b_block;
      }

      SimulinkModel::XSD::block_T*
      upper (void) const
      {
	return b_upper;
      }

      SimulinkModel::XSD::block_T*&
      upper (void)
      {
	return b_upper;
      }

      bool is_subsystem (void) const { return b_is_subsystem; }
      bool& is_subsystem (void) { return b_is_subsystem; }

      bool has_update (void) const { return b_has_update; }
      bool& has_update (void) { return b_has_update; }

      bool is_sfunction (void) const { return b_is_sfunction; }
      bool& is_sfunction (void) { return b_is_sfunction; }

      subsystem_type_T
      subsystem_type (void) const
      {
	return b_subsystem_type;
      }

      subsystem_type_T& subsystem_type (void) { return b_subsystem_type; }

      bool operator== (const std::string& name) const
      {
	return block ()->name () == name;
      }

      static unsigned int max_id (void) { return seq; }

    protected:
      void set_newid (void) { b_id = ++seq; }

    private:
      unsigned int b_id;

      SimulinkModel::XSD::block_T* b_block;
      SimulinkModel::XSD::block_T* b_upper;

      bool b_is_subsystem;
      bool b_has_update;
      bool b_is_sfunction;

      subsystem_type_T b_subsystem_type;

      static unsigned int seq;
    };

    // template of BlockInfo
    template <class T, class C, class V>
    class Tmpl_BlockInfo : public base_BlockInfo, public T
    {
      typedef Tmpl_var_list_T<V> this_var_list_T;
      typedef Tmpl_BlockInfo<T,C,V> this_BlockInfo;
      typedef Tmpl_block_list_T<this_BlockInfo> this_block_list_T;

      typedef Tmpl_BlockConnect<this_BlockInfo,C,V> this_BlockConnect;

      typedef Tmpl_block_connect_list_T<this_BlockInfo,C,V>
      this_block_connect_list_T;

      typedef typename this_block_list_T::const_iterator
      this_block_list_const_iterator;

    public:
      Tmpl_BlockInfo(SimulinkModel::XSD::block_T* block,
		     SimulinkModel::XSD::block_T* upper)
	: base_BlockInfo(block, upper),
	  b_upper_subsystem (NULL),
	  b_top (NULL), b_top_subsystem (NULL),
	  b_trigger_port (NULL), b_enable_port (NULL), b_action_port (NULL)
      {
      }

      this_BlockInfo*
      upper_subsystem (void) const { return b_upper_subsystem; }

      this_BlockInfo*&
      upper_subsystem (void) { return b_upper_subsystem; }

      const this_block_list_T&
      lower_block (void) const { return b_lower_block; }

      this_block_list_T&
      lower_block (void) { return b_lower_block; }

      const this_block_connect_list_T&
      forward (void) const { return b_forward; }

      this_block_connect_list_T&
      forward (void) { return b_forward; }

      const this_block_connect_list_T&
      backward (void) const { return b_backward; }

      this_block_connect_list_T&
      backward (void) { return b_backward; }

      this_BlockInfo*
      top_subsystem (void) const { return b_top_subsystem; }

      this_BlockInfo*&
      top_subsystem (void) { return b_top_subsystem; }

      SimulinkModel::XSD::block_T* top (void) const { return b_top; }
      SimulinkModel::XSD::block_T*& top (void) { return b_top; }

      const this_block_list_T&
      contain_block (void) const { return b_contain_block; }

      this_block_list_T&
      contain_block (void) { return b_contain_block; }

      bool is_contained (const this_BlockInfo* b) const;
      bool is_contained (const SimulinkModel::XSD::block_T* b) const;

      const this_block_list_T&
      merged_to (void) const { return b_merged_to; }
      this_block_list_T&
      merged_to (void) { return b_merged_to; }

      const this_block_list_T&
      merge_blocks (void) const { return b_merge_blocks; }
      this_block_list_T&
      merge_blocks (void) { return b_merge_blocks; }

      const this_block_list_T&
      bound_to (void) const { return b_bound_to; }
      this_block_list_T&
      bound_to (void) { return b_bound_to; }

      const this_block_list_T&
      bind_blocks (void) const { return b_bind_blocks; }

      this_block_list_T& bind_blocks (void) { return b_bind_blocks; }

      this_BlockInfo*
      trigger_port (void) const { return b_trigger_port; }

      this_BlockInfo*&
      trigger_port (void) { return b_trigger_port; }

      this_BlockInfo*
      enable_port (void) const { return b_enable_port; }

      this_BlockInfo*&
      enable_port (void) { return b_enable_port; }

      this_BlockInfo*
      action_port (void) const { return b_action_port; }

      this_BlockInfo*&
      action_port (void) { return b_action_port; }

      bool is_atomic_subsystem (void) const;

      bool is_noblock_subsystem (void) const
      {
	return contain_block ().size () == 0;
      }

      void dump (void) const;

      friend class Tmpl_Reader<this_BlockInfo,C,V>;

    private:
      this_BlockInfo* b_upper_subsystem;	/* SubSystem */
      this_block_list_T b_lower_block;	/* lower block in SubSystem */

      this_block_connect_list_T b_forward;
      this_block_connect_list_T b_backward;

      // Toplevel Subsystem (e.g. ForIterator)
      SimulinkModel::XSD::block_T* b_top;
      this_BlockInfo* b_top_subsystem;

      this_block_list_T b_contain_block;

      // merge
      this_block_list_T b_merged_to;
      this_block_list_T b_merge_blocks;

      // bind
      this_block_list_T b_bound_to;
      this_block_list_T b_bind_blocks;

      // trigger port
      this_BlockInfo* b_trigger_port;
      // enable port
      this_BlockInfo* b_enable_port;
      // action port
      this_BlockInfo* b_action_port;
    };

    template <class T, class C, class V>
    bool
    Tmpl_BlockInfo<T,C,V>::is_contained (const this_BlockInfo* b) const
    {
      for (this_block_list_const_iterator bl (contain_block ().begin ());
	   bl != contain_block ().end (); bl++)
	{
	  if (b == *bl)
	    return true;
	}
      return false;
    }

    template <class T, class C, class V>
    bool
    Tmpl_BlockInfo<T,C,V>::is_contained (const SimulinkModel::XSD::block_T* b)
      const
    {
      for (this_block_list_const_iterator bl (contain_block ().begin ());
	   bl != contain_block ().end (); bl++)
	{
	  if (b == (*bl)->block ())
	    return true;
	}
      return false;
    }

    template <class T, class C, class V>
    bool
    Tmpl_BlockInfo<T,C,V>::is_atomic_subsystem (void) const
    {
      if (subsystem_type () == ATOMIC
	  || (subsystem_type () == ITERATOR
	      && contain_block ().size () > 0))
	return true;
      const SimulinkModel::XSD::block_T::TreatAsAtomicUnit_optional&
	ao = block ()->TreatAsAtomicUnit ();
      if (ao && *ao == "on")
	return true;
      return false;
    }

    template <class T, class C, class V>
    void
    Tmpl_BlockInfo<T,C,V>::dump (void) const
    {
      std::cout << "id=" << id() << " Block=" << block()->name() << "\n";
      std::cout << "  BlockType=" << block ()->blocktype ();
      if (is_subsystem ())
	{
	  if (subsystem_type () == ATOMIC)
	    std::cout << "[atomic]";
	  else if (subsystem_type () == ENABLED)
	    std::cout << "[enabled]";
	  else if(subsystem_type () == TRIGGERED)
	    std::cout << "[triggered]";
	  else if (subsystem_type () == ENABLED_TRIGGERED)
	    std::cout << "[enabled_triggered]";
	  else if (subsystem_type () == ACTION)
	    std::cout << "[action]";
	  else if (subsystem_type () == ITERATOR)
	    std::cout << "[iterator]";
	}
      std::cout << "\n";
      if (upper())
	std::cout << "  Upper=" << upper()->name() << "\n";
      if (top())
	std::cout << "  Top=" << top()->name () << "\n";
      for (block_list_const_iterator bl (contain_block ().begin ());
	   bl != contain_block ().end (); bl++)
	{
	  const this_BlockInfo* cb = *bl;
	  std::cout << "  contain: " << cb->block ()->name () << "\n";
	}
      std::cout << std::endl;
    }

    //
    // map of BlockInfo
    //
    template <class BI>
    class Tmpl_block_map_T
      : public std::map < std::string, BI >
    {
    };

    // Default Type
    typedef Tmpl_block_map_T<> block_map_T;
    typedef block_map_T::iterator block_map_iterator;
    typedef block_map_T::const_iterator block_map_const_iterator;

    //
    // IOPort
    //
    class IOPort
    {
    public:
      IOPort ();
      IOPort (SimulinkModel::XSD::block_T* block,
	      SimulinkModel::XSD::ioport_T* ioport);

      SimulinkModel::XSD::block_T*
      block_tag (void) const
      {
	return iop_block_tag;
      }

      SimulinkModel::XSD::block_T*&
      block_tag (void)
      {
	return iop_block_tag;
      }

      SimulinkModel::XSD::ioport_T*
      ioport_tag (void) const
      {
	return iop_ioport_tag;
      }

      SimulinkModel::XSD::ioport_T*&
      ioport_tag (void)
      {
	return iop_ioport_tag;
      }

      const std::string&
      blockname (void) const
      {
	return iop_block_tag->name();
      }

      std::string&
      blockname (void)
      {
	return iop_block_tag->name();
      }

      const std::string&
      port (void) const
      {
	return iop_ioport_tag->port();
      }

      std::string&
      port (void)
      {
	return iop_ioport_tag->port();
      }

      const std::string&
      line (void) const
      {
	return iop_ioport_tag->line();
      }

      std::string&
      line (void)
      {
	return iop_ioport_tag->line();
      }

      bool
      match (const std::string& b, const std::string& p) const
      {
	return blockname() == b && port () == p;
      }

      void dump (void) const;

    private:
      SimulinkModel::XSD::block_T* iop_block_tag;
      SimulinkModel::XSD::ioport_T* iop_ioport_tag;
    };

    typedef std::multimap< std::string, IOPort > ioport_map_T;
    typedef ioport_map_T::iterator ioport_map_iterator;
    typedef ioport_map_T::const_iterator ioport_map_const_iterator;
    typedef std::pair< ioport_map_iterator,
		       ioport_map_iterator > ioport_map_iterator_pair;
    typedef std::pair< ioport_map_const_iterator,
		       ioport_map_const_iterator >
    ioport_map_const_iterator_pair;

    //
    // Reader
    //

    // basic class of Reader
    class base_Reader
    {
    public:
      const ioport_map_T&
      input_map (void) const
      {
	return r_input_map;
      }

      ioport_map_T&
      input_map (void)
      {
	return r_input_map;
      }

      const ioport_map_T&
      output_map (void) const
      {
	return r_output_map;
      }

      ioport_map_T&
      output_map (void)
      {
	return r_output_map;
      }

      const IOPort* find_input_port (const std::string& block,
				     const std::string& port) const;
      IOPort* find_input_port (const std::string& block,
			       const std::string& port);
      const IOPort* find_output_port (const std::string& block,
				      const std::string& port) const;
      IOPort* find_output_port (const std::string& block,
				const std::string& port);

    protected:
      void process_port (SimulinkModel::XSD::blocks_T& blocks);

      IOPort& add_input_map (SimulinkModel::XSD::block_T* block,
			     SimulinkModel::XSD::ioport_T* ioport);

      IOPort& add_output_map (SimulinkModel::XSD::block_T* block,
			      SimulinkModel::XSD::ioport_T* ioport);

    private:
      ioport_map_T r_input_map;
      ioport_map_T r_output_map;
    };

    // template of Reader
    template <class BI, class C, class V>
    class Tmpl_Reader : public base_Reader
    {
      typedef Tmpl_block_map_T<BI> Reader_block_map_T;
      typedef Tmpl_block_list_T<BI> Reader_block_list_T;
      typedef Tmpl_BlockConnect<BI,C,V> Reader_BlockConnect;
      typedef Tmpl_block_connect_list_T<BI,C,V> Reader_block_connect_list_T;
      typedef Tmpl_var_T_wrapper<V> Reader_var_T_wrapper;

      typedef typename Reader_block_map_T::iterator Reader_block_map_iterator;
      typedef typename Reader_block_map_T::const_iterator
      Reader_block_map_const_iterator;

      typedef typename Reader_block_list_T::iterator Reader_block_list_iterator;
      typedef typename Reader_block_list_T::const_iterator
      Reader_block_list_const_iterator;

      typedef typename Reader_block_connect_list_T::iterator
      Reader_block_connect_list_iterator;
      typedef typename Reader_block_connect_list_T::const_iterator
      Reader_block_connect_list_const_iterator;

    public:
      Tmpl_Reader();
      Tmpl_Reader (const char* xml, bool validation);
      Tmpl_Reader (const char* xml,
		   const char* ns = NULL, const char* sc = NULL,
		   bool validation = true);

      SimulinkModel::XSD::blocks_T&
      xml_blocks (void) const
      {
	return *r_blocks;
      }

      const Reader_block_map_T&
      block_map (void) const
      {
	return r_block_map;
      }

      Reader_block_map_T&
      block_map (void)
      {
	return r_block_map;
      }

      const Reader_block_list_T&
      block_list (void) const
      {
	return r_block_list;
      }

      Reader_block_list_T&
      block_list (void)
      {
	return r_block_list;
      }

      const BI* find_block (const std::string& block) const;
      BI* find_block (const std::string& block);

      const IOPort* find_inport_input (const BI* port) const;
      IOPort* find_inport_input (const BI* port);

      const IOPort* find_outport_output (const BI* port) const;
      IOPort* find_outport_output (const BI* port);

      int load_xml (const char* xml,
		    const char* ns = NULL, const char* sc = NULL,
		    bool validation = true);
      int load_xml (const char* xml, bool validation = true);

      void dump (void) const;

      operator bool () const
      {
	return r_blocks.get() != NULL;
      }

      enum search_dir { SEARCH_FORWARD, SEARCH_BACKWARD };

      int find_loop (int index, int *route,
		     search_dir dir = SEARCH_FORWARD,
		     bool break_multirate = false) const;

      int find_loop (int index, search_dir dir = SEARCH_FORWARD,
		     bool break_multirate = false) const;

    protected:

      int depth_search_for_loop (int s_index, int c_index,
				 int* color, int* route,
				 search_dir dir,
				 bool break_multirate) const;

      BI& add_block_map (SimulinkModel::XSD::block_T* upper,
			 SimulinkModel::XSD::block_T& block);

      void process_blocks (BI* top,
			   SimulinkModel::XSD::block_T* upper,
			   BI* subsystem,
			   SimulinkModel::XSD::blocks_T& blocks);
      void process_blocks_complete ();
      void process_graph (void);
      void process_all (void);

      void process_top_subsystem (BI* top,
				  BI* binfo);

    private:

      blocks_T_PTR r_blocks;

      Reader_block_map_T r_block_map;

      Reader_block_list_T r_block_list;
    };

    template <class BI, class C, class V>
    Tmpl_Reader<BI,C,V>::Tmpl_Reader ()
    {
    }

    template <class BI, class C, class V>
    Tmpl_Reader<BI,C,V>::Tmpl_Reader (const char* xml, bool validation)
    {
      load_xml (xml, NULL, NULL, validation);
    }

    template <class BI, class C, class V>
    Tmpl_Reader<BI,C,V>::Tmpl_Reader (const char* xml,
				      const char* ns,
				      const char* sc,
				      bool validation)
    {
      load_xml (xml, ns, sc, validation);
    }

    template <class BI, class C, class V>
    BI&
    Tmpl_Reader<BI,C,V>::add_block_map (SimulinkModel::XSD::block_T* upper,
					SimulinkModel::XSD::block_T& block)
    {
      std::pair< Reader_block_map_iterator,
		   bool > x
	= block_map().insert (std::make_pair (block.name(),
						BI (&block, upper)));
      return x.first->second;
    }

    template <class BI, class C, class V>
    const BI*
    Tmpl_Reader<BI,C,V>::find_block (const std::string& block) const
    {
      Reader_block_map_const_iterator i (block_map().find (block));
      if (i == block_map().end())
	return NULL;
      return &i->second;
    }

    template <class BI, class C, class V>
    BI*
    Tmpl_Reader<BI,C,V>::find_block (const std::string& block)
    {
      Reader_block_map_iterator i (block_map().find (block));
      if (i == block_map().end())
	return NULL;
      return &i->second;
    }

    template <class BI, class C, class V>
    const IOPort*
    Tmpl_Reader<BI,C,V>::find_inport_input (const BI* port) const
    {
      const BI* subsys = port->upper_subsystem ();
      if (subsys == NULL)
	return NULL;
      return find_input_port (subsys->block ()->name (),
			      port->block ()->name ());
    }

    template <class BI, class C, class V>
    IOPort*
    Tmpl_Reader<BI,C,V>::find_inport_input (const BI* port)
    {
      const BI* subsys = port->upper_subsystem ();
      if (subsys == NULL)
	return NULL;
      return find_input_port (subsys->block ()->name (),
			      port->block ()->name ());
    }

    template <class BI, class C, class V>
    const IOPort*
    Tmpl_Reader<BI,C,V>::find_outport_output (const BI* port) const
    {
      const BI* subsys = port->upper_subsystem ();
      if (subsys == NULL)
	return NULL;
      return find_output_port (subsys->block ()->name (),
			       port->block ()->name ());
    }

    template <class BI, class C, class V>
    IOPort*
    Tmpl_Reader<BI,C,V>::find_outport_output (const BI* port)
    {
      const BI* subsys = port->upper_subsystem ();
      if (subsys == NULL)
	return NULL;
      return find_output_port (subsys->block ()->name (),
			       port->block ()->name ());
    }

    template <class BI, class C, class V>
    void
    Tmpl_Reader<BI,C,V>::process_blocks (BI* top,
					 SimulinkModel::XSD::block_T* upper,
					 BI* subsys,
					 SimulinkModel::XSD::blocks_T& blocks)
    {
      BI* t;

      for (SimulinkModel::XSD::blocks_T::block_iterator
	     bs (blocks.block().begin());
	   bs != blocks.block().end(); bs++)
	{
	  SimulinkModel::XSD::block_T& b = *bs;

	  if (find_block (b.name ()))
	    {
	      std::cout << "Warning: Duplicate block in XML: "
			  << b.blocktype () << ":" << b.name ()
			  << std::endl;
	      continue;
	    }

	  BI& binfo = add_block_map (upper, b);

	  binfo.upper_subsystem () = subsys;
	  if (subsys)
	    subsys->lower_block ().push_back (&binfo);

	  if (b.blocktype () == "SubSystem")
	    {
	      binfo.is_subsystem () = true;
	      SimulinkModel::XSD::block_T::type_optional& bt = b.type ();
	      if (bt)
		{
		  if (*bt == "atomic")
		    binfo.subsystem_type () = BI::ATOMIC;
		  else if (*bt == "enabled")
		    binfo.subsystem_type () = BI::ENABLED;
		  else if (*bt == "triggered")
		    binfo.subsystem_type () = BI::TRIGGERED;
		  else if (*bt == "enabled_triggered")
		    binfo.subsystem_type () = BI::ENABLED_TRIGGERED;
		  else if (*bt == "action")
		    binfo.subsystem_type () = BI::ACTION;
		  else if (*bt == "iterator")
		    binfo.subsystem_type () = BI::ITERATOR;
		  else if (*bt == "stateflow")
		    binfo.subsystem_type () = BI::STATEFLOW;
		}

	      SimulinkModel::XSD::block_T::stateflow_optional&
		so = b.stateflow ();
	      if (so)
		binfo.subsystem_type () = BI::STATEFLOW;
	    }
	  else if (b.blocktype () == "S-Function")
	    binfo.is_sfunction () = true;
	  else if (b.blocktype () == "TriggerPort" && subsys != NULL)
	    subsys->trigger_port () = &binfo;
	  else if (b.blocktype () == "EnablePort" && subsys != NULL)
	    subsys->enable_port () = &binfo;
	  else if (b.blocktype () == "ActionPort" && subsys != NULL)
	    subsys->action_port () = &binfo;

	  for (SimulinkModel::XSD::block_T::code_iterator
		 ci (b.code ().begin ());
	       ci != b.code ().end (); ci++)
	    {
	      if (ci->type () == "update")
		{
		  binfo.has_update () = true;
		  break;
		}
	    }

	  if (top)
	    {
	      binfo.top_subsystem () = top;
	      binfo.top () = top->block ();
	      top->contain_block ().push_back (&binfo);
	    }

	  t = top;
	  if (t == NULL
	      && binfo.subsystem_type () == base_BlockInfo::ITERATOR)
	    t = &binfo;

	  for (SimulinkModel::XSD::block_T::blocks_iterator
		 i (b.blocks().begin ());
	       i != b.blocks ().end (); i++)
	    process_blocks (t, &b, &binfo, *i);
	}
    }

    template <class BI, class C, class V>
    void
    Tmpl_Reader<BI,C,V>::process_blocks_complete (void)
    {
      for (Reader_block_map_iterator i (block_map ().begin ());
	   i != block_map ().end (); i++)
	{
	  BI& b = i->second;
	  SimulinkModel::XSD::block_T* bt = b.block ();

	  if (b.id () == 0)
	    b.set_newid ();

	  for (SimulinkModel::XSD::block_T::merged_to_iterator
		 mi (bt->merged_to().begin ());
	       mi != bt->merged_to ().end (); mi++)
	    {
	      SimulinkModel::XSD::block_name_T& m = *mi;
	      BI* mb = find_block (m.block ());

	      if (mb)
		b.merged_to ().push_back (mb);
	    }

	  for (SimulinkModel::XSD::block_T::merge_block_iterator
		 mi (bt->merge_block().begin ());
	       mi != bt->merge_block ().end (); mi++)
	    {
	      SimulinkModel::XSD::block_name_T& m = *mi;
	      BI* mb = find_block (m.block ());

	      if (mb)
		b.merge_blocks ().push_back (mb);
	    }

	  for (SimulinkModel::XSD::block_T::bound_to_iterator
		 bi (bt->bound_to().begin ());
	       bi != bt->bound_to ().end (); bi++)
	    {
	      SimulinkModel::XSD::block_name_T& m = *bi;
	      BI* bb = find_block (m.block ());

	      if (bb)
		b.bound_to ().push_back (bb);
	    }

	  for (SimulinkModel::XSD::block_T::bind_iterator
		 bi (bt->bind().begin ());
	       bi != bt->bind ().end (); bi++)
	    {
	      SimulinkModel::XSD::block_name_T& m = *bi;
	      BI* bb = find_block (m.block ());

	      if (bb)
		b.bind_blocks ().push_back (bb);
	    }
	}

      // correct `top_subsystem'
      for (Reader_block_map_iterator i (block_map ().begin ());
	   i != block_map ().end (); i++)
	{
	  BI& b = i->second;

	  if (!b.is_subsystem ()
	      || b.top_subsystem () != NULL
	      || !b.contain_block ().empty ()
	      || !b.merged_to ().empty ()
	      || b.lower_block ().empty ()
	      || b.merge_blocks ().empty ())
	    continue;

	  bool all_ok = true;
	  Reader_block_list_const_iterator mb (b.merge_blocks ().begin ());
	  Reader_block_list_const_iterator me (b.merge_blocks ().end ());

	  for (Reader_block_list_const_iterator
		 bi (b.lower_block ().begin ());
	       bi != b.lower_block ().end (); bi++)
	    {
	      if (std::find (mb, me, *bi) == me)
		{
		  all_ok = false;
		  break;
		}
	    }

	  if (all_ok)
	    {
	      for (Reader_block_list_iterator bi (b.lower_block ().begin ());
		   bi != b.lower_block ().end (); bi++)
		process_top_subsystem (&b, *bi);
	    }
	}
    }

    template <class BI, class C, class V>
    void
    Tmpl_Reader<BI,C,V>::process_graph (void)
    {
      for (Reader_block_map_iterator i (block_map ().begin ());
	   i != block_map ().end (); i++)
	{
	  BI& binfo = i->second;

	  const SimulinkModel::XSD::block_T* b = binfo.block ();

	  for (SimulinkModel::XSD::block_T::forward_const_iterator
		 fi (b->forward ().begin ());
	       fi != b->forward ().end (); fi++)
	    {
	      BI* head = find_block (fi->block());

	      Reader_BlockConnect bc (&binfo, head, &*fi);

	      Reader_block_connect_list_iterator
		beg = binfo.forward ().begin ();
	      Reader_block_connect_list_iterator
		end = binfo.forward ().end ();

	      Reader_block_connect_list_iterator
		bci = std::find (beg, end, bc);

	      Reader_BlockConnect* xbc;

	      if (bci == end)
		{
		  binfo.forward ().push_back (bc);
		  xbc = &binfo.forward ().back ();
		}
	      else
		xbc = &*bci;

	      for (SimulinkModel::XSD::link_T::var_const_iterator
		     vi (fi->var ().begin ());
		   vi != fi->var ().end (); vi++)
		xbc->var_list().push_back (Reader_var_T_wrapper(&*vi));
	    }

	  for (SimulinkModel::XSD::block_T::backward_const_iterator
		 bi (b->backward ().begin ());
	       bi != b->backward ().end (); bi++)
	    {
	      BI* tail = find_block (bi->block());

	      Reader_BlockConnect bc (tail, &binfo, &*bi);

	      Reader_block_connect_list_iterator
		beg = binfo.backward ().begin ();
	      Reader_block_connect_list_iterator
		end = binfo.backward ().end ();

	      Reader_block_connect_list_iterator
		bci = std::find (beg, end, bc);

	      Reader_BlockConnect* xbc;

	      if (bci == end)
		{
		  binfo.backward ().push_back (bc);
		  xbc = &binfo.backward ().back ();
		}
	      else
		xbc = &*bci;

	      for (SimulinkModel::XSD::link_T::var_const_iterator
		     vi (bi->var ().begin ());
		   vi != bi->var ().end (); vi++)
		xbc->var_list().push_back (Reader_var_T_wrapper(&*vi));
	    }
	}

      // pairing
      for (Reader_block_map_iterator i (block_map ().begin ());
	   i != block_map ().end (); i++)
	{
	  BI& bi_tail = i->second;

	  for (Reader_block_connect_list_iterator
		 j (bi_tail.forward ().begin ());
	       j != bi_tail.forward ().end (); j++)
	    {
	      Reader_BlockConnect& fbc = *j;

	      BI* bi_head = fbc.head ();
	      for (Reader_block_connect_list_iterator
		     k (bi_head->backward ().begin ());
		   k != bi_head->backward ().end (); k++)
		{
		  Reader_BlockConnect& bbc = *k;
		  if (bbc.tail () == &bi_tail)
		    {
		      bbc.pair () = &fbc;
		      fbc.pair () = &bbc;
		      break;
		    }
		}
	    }
	}
    }

    template <class BI, class C, class V>
    void
    Tmpl_Reader<BI,C,V>::process_all (void)
    {
      process_blocks (NULL, NULL, NULL, *r_blocks);
      process_blocks_complete ();
      process_port (*r_blocks);
      process_graph ();

      size_t size = base_BlockInfo::max_id () + 1; // id is 1 origin

      Reader_block_list_T& bl = block_list ();
      bl.resize (size + 1);

      for (Reader_block_map_iterator i (block_map ().begin ());
	   i != block_map ().end (); i++)
	{
	  BI& b = i->second;
	  if (b.id() <= 0 || size < b.id())
	    std::cerr << "Error: BlockInfo id=" << b.id() << std::endl;
	  else
	    bl[b.id ()] = &b;
	}
    }

    template <class BI, class C, class V>
    int
    Tmpl_Reader<BI,C,V>::load_xml (const char* xml,
				   const char* ns,
				   const char* sc,
				   bool validation)
    {
      try
	{
	  if (validation)
	    {
	      if (ns != NULL && sc != NULL)
		{
		  xml_schema::properties props;
		  props.schema_location (ns, sc);
		  r_blocks.reset (SimulinkModel::XSD::blocks (xml, 0, props)
				  .release ());
		}
	      else
		r_blocks.reset (SimulinkModel::XSD::blocks (xml).release ());
	    }
	  else
	    r_blocks.reset (SimulinkModel::XSD::blocks (xml,
							xml_schema::flags::dont_validate)
			    .release ());

	  process_all ();
	}
      catch (const xml_schema::exception& e)
	{
	  std::cerr << "Error: " << e << std::endl;
	  return -1;
	}
      catch (...)
	{
	  std::cerr << "Error: " << xml << std::endl;
	  return -1;
	}
      return 0;
    }

    template <class BI, class C, class V>
    int
    Tmpl_Reader<BI,C,V>::load_xml (const char* xml, bool validation)
    {
      return load_xml(xml, NULL, NULL, validation);
    }

    template <class BI, class C, class V>
    void
    Tmpl_Reader<BI,C,V>::dump (void) const
    {
      if (block_map().size())
	{
	  std::cout << "-- block map --" << std::endl;

	  for (Reader_block_map_const_iterator i (block_map().begin());
	       i != block_map().end(); i++)
	    i->second.dump();
	}
      if (input_map().size())
	{
	  std::cout << "--input map --" << std::endl;

	  for (ioport_map_const_iterator i (input_map().begin());
	       i != input_map().end(); i++)
	    i->second.dump();
	}
      if (output_map().size())
	{
	  std::cout << "--output map --" << std::endl;

	  for (ioport_map_const_iterator i (output_map().begin());
	       i != output_map().end(); i++)
	    i->second.dump();
	}
    }

    template <class BI, class C, class V>
    int
    Tmpl_Reader<BI,C,V>::depth_search_for_loop (int s_index, int c_index,
						int* color, int* route,
						search_dir dir,
						bool break_multirate) const
    {
      const Reader_block_list_T& bl = block_list ();
      const BI* c_binfo = bl[c_index];

      color[c_index] = -1;	// Gray

      Reader_block_connect_list_const_iterator beg;
      Reader_block_connect_list_const_iterator end;

      if (dir == SEARCH_FORWARD)
	{
	  beg = c_binfo->forward ().begin ();
	  end = c_binfo->forward ().end ();
	}
      else
	{
	  beg = c_binfo->backward ().begin ();
	  end = c_binfo->backward ().end ();
	}

      // search backward (search to Root)
      for (Reader_block_connect_list_const_iterator i (beg); i != end; i++)
	{
	  const BI* n_binfo;

	  if (dir == SEARCH_FORWARD)
	    n_binfo = i->head (); // head of edge (forward)
	  else
	    n_binfo = i->tail (); // tail of edge (backward)

	  int n_index = n_binfo->id ();

	  if (s_index == n_index)
	    {
	      route[c_index] = n_index;
	      return 1;
	    }

	  if (break_multirate)
	    {
	      const BI*
		c_mss = find_multirate_subsystem (c_binfo);
	      const BI*
		n_mss = find_multirate_subsystem (n_binfo);

	      if (c_mss != n_mss)
		continue;	// break multirate
	    }

	  if (color[n_index] == 0) // White
	    {
	      int x = depth_search_for_loop (s_index, n_index, color, route,
					     dir, break_multirate);
	      if (x)
		{
		  route[c_index] = n_index;
		  return x;
		}
	    }

	  color[c_index] = 1;	// Black
	}
      return 0;
    }

    template <class BI, class C, class V>
    int
    Tmpl_Reader<BI,C,V>::find_loop (int index, int *route,
				    search_dir dir,
				    bool break_multirate) const
    {
      const Reader_block_list_T& bl = block_list ();
      std::vector < int > color (bl.size(), 0);
      int x = depth_search_for_loop (index, index,
				     color.data (), route,
				     dir, break_multirate);
#if 0
      if (x)
	{
	  int i = index;
	  const BI* cur = bl[i];
	  std::cout << "----------------\n";
	  std::cout << "Found loop: " << i
		      << "(" << cur->block()->name() << ")"
		      << std::endl;
	  do
	    {
	      int j = route[i];
	      if (j == -1)
		j = index;

	      const BI* nxt = bl[j];
	      std::cout << "  " << i << "(" << cur->block()->name() << ")"
			  << (dir == SEARCH_FORWARD ? " -> " : " <- ")
			  << j << "(" << nxt->block()->name() << ")"
			  << std::endl;
	      i = j;
	      cur = nxt;
	    } while (i != index);
	}
#endif
      return x;
    }

    template <class BI, class C, class V>
    int
    Tmpl_Reader<BI,C,V>::find_loop (int index, search_dir dir,
				    bool break_multirate) const
    {
      std::vector < int > route (block_list().size(), -1);
      return find_loop (index, route.data (), dir, break_multirate);
    }

    template <class BI, class C, class V>
    void
    Tmpl_Reader<BI,C,V>::process_top_subsystem (BI* top, BI* binfo)
    {
      binfo->top_subsystem () = top;
      binfo->top () = top->block ();
      top->contain_block ().push_back (binfo);
      for (Reader_block_list_iterator bi (binfo->lower_block ().begin ());
	   bi != binfo->lower_block ().end (); bi++)
	process_top_subsystem (top, *bi);
    }

    // Default Type
    typedef Tmpl_Reader<> Reader;

    //
    // other class
    //

    class c_mark
    {
    public:
      c_mark (const char* b, const char* e,
	      const char* tb, const char* te,
	      char t, unsigned int i, const std::string& n)
	: x_beg (b), x_end (e), x_text_beg (tb), x_text_end (te),
	  x_type (t), x_num (i), x_name (n)
      {}

      void
      set (const char* b, const char* e,
	   const char* tb, const char* te,
	   char t, unsigned int i, const std::string& n)
      {
	x_beg = b;
	x_end = e;
	x_text_beg = tb;
	x_text_end = te;
	x_type = t;
	x_num = i;
	x_name = n;
      }

      const char* beg (void) const { return x_beg; }
      const char*& beg (void) { return x_beg; }
      const char* end (void) const { return x_end; }
      const char*& end (void) { return x_end; }
      const char* text_beg (void) const { return x_text_beg; }
      const char*& text_beg (void) { return x_text_beg; }
      const char* text_end (void) const { return x_text_end; }
      const char*& text_end (void) { return x_text_end; }
      char type (void) const { return x_type; }
      char& type (void) { return x_type; }
      unsigned int num (void) const { return x_num; }
      unsigned int& num (void) { return x_num; }
      const std::string& name (void) const { return x_name; }
      std::string& name (void) { return x_name; }

      operator std::string () const
      {
	return std::string (x_text_beg, x_text_end - x_text_beg);
      }

    private:
      const char* x_beg;
      const char* x_end;
      const char* x_text_beg;
      const char* x_text_end;
      char x_type;
      unsigned int x_num;
      std::string x_name;
    };

    typedef std::vector < c_mark > marklist_T;

    class codelist
    {
    public:

      codelist() {}
      ~codelist() {}

      void process (const char* beg, const char* end);

      codelist (const char* beg, const char* end) { process (beg, end); }
      codelist (const std::string& str)
      {
	const char* beg = str.c_str();
	const char* end = beg + str.length ();
	process (beg, end);
      }

      const marklist_T& mark_list (void) const { return list; }
      marklist_T& mark_list (void) { return list; }

    private:
      marklist_T list;
    };
  }
}

#endif	/* SIMULINKXML_H */
