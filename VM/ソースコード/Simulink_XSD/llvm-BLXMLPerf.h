// -*- mode: c++; -*-

#ifndef llvm_BLXMLPerf_h
#define llvm_BLXMLPerf_h

///
/// @file  llvm-blxml_perf.h
/// @brief class define for llvm cycle count
///

#include <llvm/Pass.h>
#include <llvm/IR/Function.h>
#include <llvm/Analysis/LoopPass.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Analysis/ScalarEvolution.h>
#include <llvm/Analysis/ScalarEvolutionExpressions.h>

#include <iostream>

#include "shim_llvm.h"
#include "blxml_llvm.h"
#include "boost_llvm.h"

namespace Simulink
{
  namespace LLVM
  {
    /// @brief type of a callee list
    typedef std::vector < llvm::Function* > CalleeList;

    /// @brief information of c-function (name, cycle)
    class FunctionInfo
    {
    public:
      /// @brief default constructor
      FunctionInfo () {}

      /// @brief default destructor
      ~FunctionInfo () {}

      /// @brief constructor of infomation of function
      /// @param f pointer of Function
      /// @param n function name
      /// @param b best cycle
      /// @param w worst cycle
      FunctionInfo (llvm::Function* f, std::string n,
		    InsnCycle b = 0, InsnCycle t = 0, InsnCycle w = 0)
	: function_name (n),
	  best_cycle (b), typical_cycle (t), worst_cycle (w),
	  func_index (-1), llvm_function (f), called_function (false)
      {
      }

      /// @brief accessor for func_index
      /// @param none
      /// @return function
      int id (void) const { return func_index; }
      int& id (void) { return func_index; }

      /// @brief accessor for Function
      /// @param none
      /// @return function
      llvm::Function* function (void) const { return llvm_function; }
      llvm::Function*& function (void) { return llvm_function; }

      /// @brief accessor for function name
      /// @param none
      /// @return function name
      const std::string& name (void) const { return function_name; }
      std::string& name (void) { return function_name; }

      /// @brief accessor for the best cycle
      /// @param none
      /// @return the best cycle
      InsnCycle best (void) const { return best_cycle; }
      InsnCycle& best (void) { return best_cycle; }

      /// @brief accessor for the typical cycle
      /// @param none
      /// @return the typical cycle
      InsnCycle typical (void) const { return typical_cycle; }
      InsnCycle& typical (void) { return typical_cycle; }

      /// @brief accessor for the worst cycle
      /// @param none
      /// @return the worst cycle
      InsnCycle worst (void) const { return worst_cycle; }
      InsnCycle& worst (void) { return worst_cycle; }

      /// @brief accessor for callees
      /// @param none
      /// @return the callee list
      const CalleeList& callees (void) const { return callee_list; }
      CalleeList& callees (void) { return callee_list; }

      /// @brief add callee
      /// @param f calee function
      void add_callee (llvm::Function* f)
      {
	if (callee_list.end () == std::find (callee_list.begin (),
					     callee_list.end (), f))
	  callee_list.push_back (f);
      }

      /// @brief is this callee function
      /// @param none
      /// @return true if callee function
      bool is_callee (void) const { return called_function; }
      bool& is_callee (void) { return called_function; }

    private:
      std::string function_name; /// function name
      InsnCycle best_cycle;	   /// the best cycle
      InsnCycle typical_cycle;	   /// the typical cycle
      InsnCycle worst_cycle;	   /// the worst cycle
      int func_index;			/// index number
      llvm::Function* llvm_function; /// pointer of Function
      CalleeList callee_list;		/// callees
      bool called_function;
    };

    /// @brief list of information of each function
    typedef std::vector < FunctionInfo > FunctionInfoList;

    /// @brief iterator of information of each function
    typedef FunctionInfoList::iterator functioninfo_iterator;

    /// @brief constant iterator of information of each function
    typedef FunctionInfoList::const_iterator const_functioninfo_iterator;

    /// @brief map of id by Function
    typedef std::map < llvm::Function*, int > FunctionIdMap;

    /// @brief llvm pass class deriverd from FunctionPass
    class BLXMLPerf : public llvm::FunctionPass
    {
      class LoopBlocks;
      class WrapBasicBlock;

      typedef std::vector < LoopBlocks* > loop_list_T;
      typedef loop_list_T::iterator loop_list_iterator;
      typedef loop_list_T::const_iterator loop_list_const_iterator;
      typedef loop_list_T::reverse_iterator loop_list_reverse_iterator;
      typedef loop_list_T::const_reverse_iterator
      loop_list_const_reverse_iterator;

      typedef std::vector < WrapBasicBlock* > wrap_block_list_T;
      typedef wrap_block_list_T::iterator wrap_block_list_iterator;
      typedef wrap_block_list_T::const_iterator wrap_block_list_const_iterator;

      typedef std::pair < WrapBasicBlock*, wrap_block_list_T > loop_exit_T;
      typedef std::vector < loop_exit_T > loop_exit_list_T;
      typedef loop_exit_list_T::iterator loop_exit_list_iterator;
      typedef loop_exit_list_T::const_iterator loop_exit_list_const_iterator;

      class WrapBasicBlock
      {
      public:
	WrapBasicBlock ()
	  : bb (NULL), bb_is_fix_loop_node (false), bb_loop_index(0),
	    bb_best_cycle (0), bb_typical_cycle (0), bb_worst_cycle (0),
	    bb_flags (0)
	{
	  bb_ident = bb_seq++;
	}

	WrapBasicBlock (const llvm::BasicBlock* b)
	  : bb (b), bb_is_fix_loop_node (false), bb_loop_index (0),
	    bb_best_cycle (0), bb_typical_cycle (0), bb_worst_cycle (0),
	    bb_flags (0)
	{
	  bb_ident = bb_seq++;
	}

	~WrapBasicBlock () {}

	bool& is_fix_loop_node(void) { return bb_is_fix_loop_node; }
	const bool& is_fix_loop_node(void) const { return bb_is_fix_loop_node; }

	operator const llvm::BasicBlock* () const { return bb; }

	std::string& name (void) { return bb_name; }
	const std::string& name (void) const { return bb_name; }

	int& ident(void) { return bb_ident; }
	const int& ident (void) const { return bb_ident; }

	bool operator== (const llvm::BasicBlock* b) const { return b == bb; }

	int& loop_index(void) { return bb_loop_index; }
	const int& loop_index(void) const { return bb_loop_index; }

	loop_list_T& get_loops (void) { return bb_in_loops; }
	const loop_list_T& get_loops (void) const { return bb_in_loops; }

	InsnCycle& best_cycle (void) { return bb_best_cycle; }
	const InsnCycle& best_cycle (void) const { return bb_best_cycle; }

	InsnCycle& typical_cycle (void) { return bb_typical_cycle; }
	const InsnCycle& typical_cycle (void) const { return bb_typical_cycle; }

	InsnCycle& worst_cycle (void) { return bb_worst_cycle; }
	const InsnCycle& worst_cycle (void) const { return bb_worst_cycle; }

	void add_loop (LoopBlocks* l)
	{
	  bb_in_loops.push_back (l);
	}

	LoopBlocks* loop_head (void);
	const LoopBlocks* loop_head (void) const;

	bool is_in_loop (const LoopBlocks *lb) const;

	const wrap_block_list_T& headers (void) const { return bb_headers; }
	wrap_block_list_T& headers (void) { return bb_headers; }
	bool is_to_header (WrapBasicBlock* head) const;
	void add_headers (WrapBasicBlock* head);

	int& flags (void) { return bb_flags; }
	const int& flags (void) const { return bb_flags; }

	static int& seq (void) { return bb_seq; }

      private:
	static int bb_seq;

	int bb_ident;

	const llvm::BasicBlock* bb;

	std::string bb_name;

	bool bb_is_fix_loop_node;

	int bb_loop_index;

	InsnCycle bb_best_cycle;
	InsnCycle bb_typical_cycle;
	InsnCycle bb_worst_cycle;

	loop_list_T bb_in_loops;

	wrap_block_list_T bb_headers;

	int bb_flags;
      };

      class LoopBlocks
      {
      public:
	LoopBlocks ()
	  : lb_level (0), lb_ident (0), lb_loop (NULL),
	    lb_enter_block (NULL), lb_best(0), lb_worst(0), lb_typical (0),
	    lb_min_count (1), lb_max_count (1), lb_fix_taken (false) {}
	LoopBlocks (const llvm::Loop *l)
	  : lb_level (0), lb_ident (0), lb_loop (l),
	    lb_enter_block (NULL), lb_best(0), lb_worst(0), lb_typical (0),
	    lb_min_count (1), lb_max_count (1), lb_fix_taken (false) {}

	~LoopBlocks () {}

	int& level (void) { return lb_level; }
	const int& level (void) const { return lb_level; }

	int& ident (void) { return lb_ident; }
	const int& ident (void) const { return lb_ident; }

	const llvm::Loop* loop (void) const { return lb_loop; }
	const llvm::Loop*& loop (void) { return lb_loop; }

	loop_list_T& children (void) { return lb_children; }
	const loop_list_T& children (void) const { return lb_children; }

	wrap_block_list_T&
	list (void) { return lb_list; }
	const wrap_block_list_T&
	list (void) const { return lb_list; }

	InsnCycle& best (void) { return lb_best; }
	const InsnCycle& best (void) const { return lb_best; }

	InsnCycle& worst (void) { return lb_worst; }
	const InsnCycle& worst (void) const { return lb_worst; }

	InsnCycle& typical (void) { return lb_typical; }
	const InsnCycle& typical (void) const { return lb_typical; }

	int& min_count (void) { return lb_min_count; }
	const int& min_count (void) const { return lb_min_count; }

	int& max_count (void) { return lb_max_count; }
	const int& max_count (void) const { return lb_max_count; }

	bool& fix_taken (void) { return lb_fix_taken; }
	const bool& fix_taken (void) const { return lb_fix_taken; }

	WrapBasicBlock*& enter_block (void) { return lb_enter_block; }
	const WrapBasicBlock*
	enter_block (void) const
	{
	  return lb_enter_block;
	}

	loop_exit_list_T& exit_block_list (void) { return lb_exit_block_list; }
	const loop_exit_list_T&
	exit_block_list (void) const
	{
	  return lb_exit_block_list;
	}

	void add_child (LoopBlocks *c) { lb_children.push_back (c); }

	bool find_child (const LoopBlocks *c) const;

	wrap_block_list_T& externals(void) { return lb_externals; }
	const wrap_block_list_T& externals(void) const { return lb_externals; }

	bool is_external (const WrapBasicBlock* b) const;

	void add_external (WrapBasicBlock* b) { lb_externals.push_back (b); }

	bool exit_path (const WrapBasicBlock* from,
			const WrapBasicBlock* to) const;

	void add_block (WrapBasicBlock* b) { lb_list.push_back (b); }

      private:

	int lb_level;		// loop level in function

	int lb_ident;		// loop ident

	const llvm::Loop* lb_loop; // contained loops

	loop_list_T lb_children;

	wrap_block_list_T lb_list;

	WrapBasicBlock* lb_enter_block;
	loop_exit_list_T lb_exit_block_list;
	wrap_block_list_T lb_externals;

	InsnCycle lb_best;
	InsnCycle lb_worst;
	InsnCycle lb_typical;

	int lb_min_count;
	int lb_max_count;
	bool lb_fix_taken;
      };

      /// @brief basic block map
      typedef std::map < const llvm::BasicBlock*,
			 WrapBasicBlock > block_map_T;
      typedef block_map_T::iterator block_map_iterator;
      typedef block_map_T::const_iterator block_map_const_iterator;

      typedef std::map < const llvm::Loop*, LoopBlocks > loop_map_T;
      typedef loop_map_T::iterator loop_map_iterator;
      typedef loop_map_T::const_iterator loop_map_const_iterator;

#if __cplusplus >= 201103L
      typedef std::unique_ptr < Graph > TempGraph;
#else
      typedef std::auto_ptr < Graph > TempGraph;
#endif

      typedef std::pair < int, int > TSortEdge;
      typedef std::vector < TSortEdge > TSortEdges;
      typedef std::vector < int > TSortResult;

      enum graph_mode { search_shortest, search_longest, search_typical };

      /// @brief make graph
      TempGraph make_graph (const llvm::Function& f,
			    unsigned int level, LoopBlocks* lb,
			    wrap_block_list_T& wbl,
			    WrapBasicBlock* wbb_enter,
			    WrapBasicBlock*& wbb_exit,
			    Vertex& vent, Vertex& vexit,
			    VertexPairList& delete_edges,
			    InsnCycle& max_cycle);

      /// @brief process loop
      void process_loop (std::vector < LoopBlocks* >* c,
			 const llvm::Function& f,
			 llvm::Loop* l,
			 llvm::ScalarEvolution* se, unsigned int ind);

      /// @brief accessor for blockmap
      block_map_T& get_block_map (void) { return basic_block_map; }
      const block_map_T& get_block_map (void) const { return basic_block_map; }

      wrap_block_list_T&
      get_block_list (void)
      {
	return wrap_basic_block_list;
      }
      const wrap_block_list_T&
      get_block_list (void) const
      {
	return wrap_basic_block_list;
      }

      WrapBasicBlock* add_block (const llvm::BasicBlock *b);

      WrapBasicBlock* get_block (const llvm::BasicBlock *b);
      const WrapBasicBlock* get_block (const llvm::BasicBlock *b) const;

      /// @brief accessor for loop map
      loop_map_T& get_loop_map (void) { return loop_map; }
      const loop_map_T& get_loop_map (void) const { return loop_map; }

      /// @brief accessor for loop list
      loop_list_T& get_loop_list (void) { return loop_list; }
      const loop_list_T& get_loop_list (void) const { return loop_list; }

      /// @brief accessor for loops
      LoopBlocks* add_loop (const llvm::Loop *l);
      LoopBlocks* find_loop (const llvm::Loop *l);
      const LoopBlocks* find_loop (const llvm::Loop *l) const;

      /// @brief path search function (shortest, longest)
      /// @param g Boost Graph
      /// @param ent entry of graph (entry block)
      /// @param ret end of graph (return block)
      /// @param base inverted bias
      InsnCycle process_route (Graph& g, Vertex ent, Vertex ret, graph_mode m);

      /// @brief average path trace function (typical)
      /// @param g Boost Graph
      /// @param u entry of graph (entry block)
      /// @param t end of graph (termination block)
      InsnCycle average_route_search (Graph& g, Vertex u, Vertex t);

      /// @brief recursive trace average path
      /// @param g Boost Graph
      /// @param u visiting of graph (visit block)
      /// @param t end of graph (termination block)
      /// @param color array of colormap(white,gray,black)
      /// @param cycles array of average cycle
      InsnCycle average_route_visit (Graph& g, Vertex u, Vertex t,
				     boost::default_color_type *color,
				     InsnCycle *cycles);

      /// @brief output GraphViz DOT file
      /// @param g Boost Graph
      /// @param base basename of dot file
      /// @param seq sequence of filename
      void output_dot (Graph& g,
		       const std::string& base, int seq = -1);

      /// @brief correct graph for GraphViz
      /// @param g Boost Graph
      /// @param f llvm Function
      /// @param h loop head
      /// @param d deleted edges
      void correct_for_dot (Graph& g, const llvm::Function& f,
			    const WrapBasicBlock* h,
			    const VertexPairList& d);

      /// @brief tsort call flow
      /// @param vsize vertex size
      /// @param edges edge list
      /// @param result sort result
      void tsort_callflow (size_t vsize,
			   TSortEdges& edges, TSortResult& result);

      /// @brief accessor for informations of each function
      FunctionInfoList& functioninfo (void) { return function_info_list; }
      const FunctionInfoList& functioninfo (void) const
      {
	return function_info_list;
      }

      void add_idmap (llvm::Function* f, int id)
      {
	function_id_map.insert (std::make_pair (f, id));
      }

      int get_id (llvm::Function* f) const
      {
	FunctionIdMap::const_iterator i = function_id_map.find (f);
	if (i == function_id_map.end ())
	  return -1;
	return i->second;
      }

    public:
      /// @brief default constructor
      BLXMLPerf()
	: llvm::FunctionPass (ID),
	  shim ((SHIM::XML::File*)NULL),
	  blxml ((SimulinkModel::BLXML*)NULL)
      {}

      /// @brief default destructor
      ~BLXMLPerf()
      {
	if (shim != NULL)
	  delete shim;
	if (blxml != NULL)
	  delete blxml;
      }

      /// @brief dummy parameter for LLVM Pass
      static char ID;

      /// @brief Pass initialization
      /// @param m Module
      virtual bool doInitialization (llvm::Module& m);

      /// @brief Pass finalization
      /// @param m Module
      virtual bool doFinalization (llvm::Module& m);

      /// @brief process function
      /// @param f function
      virtual bool runOnFunction (llvm::Function& f);

      /// @brief setup analysis usage
      /// @param au AnalysisUsage
      virtual void getAnalysisUsage (llvm::AnalysisUsage& au) const;

    private:
      /// @brief My runOnFunction
      /// @param fi FunctionInfo
      void myRunOnFunction (FunctionInfo& fi);

      /// @brief list of information of each function
      FunctionInfoList function_info_list;

      /// @brief basic block map
      block_map_T basic_block_map;

      wrap_block_list_T wrap_basic_block_list;

      /// @brief loops (associative array, key is `llvm::Loop')
      loop_map_T loop_map;

      /// @brief loop list (most internal loop the first)
      loop_list_T loop_list;

      int loop_index;

      /// @brief Function map by id
      FunctionIdMap function_id_map;

      static int dot_seq;

      SHIM::XML::File* shim;

      SimulinkModel::BLXML* blxml;
    };
  }
}

#endif
