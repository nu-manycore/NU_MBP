/// @file llvm-instcnt.cxx
/// @brief LLVM Pass for cycle count

#include <assert.h>
#include <limits.h>

#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <deque>

#include "llvm-BLXMLPerf.h"

#include <llvm/IR/ValueSymbolTable.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>

#include <llvm/Support/ToolOutputFile.h>
#include <llvm/Support/CommandLine.h>

// #define LLVM_DEBUG

#define MAX_EXITING_BLOCKS	8
#define MIN_LOOP_TAKEN		1 // use best/typical

#if LLVM_VERSION_MAJOR == 3
#if LLVM_VERSION_MINOR < 4 || 9 < LLVM_VERSION_MINOR
#error current llvm version not supported
#else
#if LLVM_VERSION_MINOR > 6
#define USE_LLVM_LOOPINFOWRAPPERPASS
#endif
#if LLVM_VERSION_MINOR > 7
#define USE_LLVM_SCALAREVOLUTIONWRAPPERPASS
#endif
#endif
#elif LLVM_VERSION_MAJOR >= 4
// build checked llvm-4.0.[01] and 5.0.0
#define USE_LLVM_LOOPINFOWRAPPERPASS
#define USE_LLVM_SCALAREVOLUTIONWRAPPERPASS
#endif

//
// options
//
static llvm::cl::opt < std::string >
SHIM_File ("shim",
	   llvm::cl::desc("input SHIM"),
	   llvm::cl::init("-no"));

static llvm::cl::opt < std::string >
SHIM_ComponentSet ("ComponentSet",
		   llvm::cl::desc("SHIM ComponentSet name"),
		   llvm::cl::init(""));

static llvm::cl::opt < std::string >
SHIM_MasterComponent ("MasterComponent",
		      llvm::cl::desc("SHIM MasterComponent name"),
		      llvm::cl::init(""));

static llvm::cl::opt < std::string >
SHIM_CommonInstructionSet ("CommonInstructionSet",
			   llvm::cl::desc("SHIM CommonInstruction name"),
			   llvm::cl::init(""));

static llvm::cl::opt < std::string >
SHIM_Schema ("shim-schema",
	     llvm::cl::desc("SHIM Schema file"),
	     llvm::cl::init(""));

static llvm::cl::opt < bool >
SHIM_NO_RET ("no-ret",
	     llvm::cl::desc("ignore RET instruction in SHIM"),
	     llvm::cl::init(false));

static llvm::cl::opt < bool >
SHIM_NO_BR ("no-br",
	    llvm::cl::desc("ignore BR instruction in SHIM"),
	    llvm::cl::init(false));

static llvm::cl::opt< std::string >
InputBLXML ("in-xml",
	    llvm::cl::desc("input BLXML file"),
	    llvm::cl::Required);

static llvm::cl::opt< std::string >
OutputBLXML ("out-xml",
	     llvm::cl::desc("output BLXML file"),
	     llvm::cl::init(""));

static llvm::cl::opt < bool >
BLXML_NO_VALIDATION("no-blxml-validation",
		    llvm::cl::desc("no validation BLXML Schema"),
		    llvm::cl::init(false));

static llvm::cl::opt< std::string >
OutputDOTBase ("out-dot",
	       llvm::cl::desc("basename of output GraphViz DOT file"),
	       llvm::cl::init(""));


namespace Simulink
{
  namespace LLVM
  {
    char BLXMLPerf::ID = 0;
    //FunctionInfoList BLXMLPerf::function_info_list;

    int BLXMLPerf::WrapBasicBlock::bb_seq = 0;

    int BLXMLPerf::dot_seq = 0;

    BLXMLPerf::LoopBlocks*
    BLXMLPerf::WrapBasicBlock::loop_head (void)
    {
      loop_list_iterator r (get_loops().begin ());

      if (r == get_loops().end ())
	return NULL;

      LoopBlocks* tl = *r;
      return tl->enter_block() == this ? tl : NULL;
    }

    bool
    BLXMLPerf::WrapBasicBlock::is_in_loop (const BLXMLPerf::LoopBlocks *lb)
      const
    {
      for (loop_list_const_iterator r (get_loops().begin ());
	   r != get_loops().end (); r++)
	{
	  const LoopBlocks* tl = *r;
	  if (tl == lb)
	    return true;
	}
      return false;
    }

    const BLXMLPerf::LoopBlocks*
    BLXMLPerf::WrapBasicBlock::loop_head (void) const
    {
      loop_list_const_reverse_iterator r (get_loops().rbegin ());

      if (r == get_loops().rend ())
	return NULL;

      const LoopBlocks* tl = *r;
      return tl->enter_block() == this ? tl : NULL;
    }

    bool
    BLXMLPerf::WrapBasicBlock::is_to_header (WrapBasicBlock* head) const
    {
      const wrap_block_list_T& wbl = headers ();
      for (wrap_block_list_const_iterator i (wbl.begin());
	   i != wbl.end(); i++)
	{
	  if (head == *i)
	    return true;
	}
      return false;
    }

    void
    BLXMLPerf::WrapBasicBlock::add_headers (WrapBasicBlock* head)
    {
      if (!is_to_header (head))
	headers ().push_back (head);
    }

    bool
    BLXMLPerf::LoopBlocks::find_child (const LoopBlocks* c) const
    {
      for (loop_list_const_iterator i (children().begin());
	   i != children().end(); i++)
	{
	  const LoopBlocks* x = *i;
	  if (c == x)
	    return true;
	  if (x->find_child (c))
	    return true;
	}
      return false;
    }

    bool
    BLXMLPerf::LoopBlocks::is_external (const WrapBasicBlock* b) const
    {
      for (wrap_block_list_const_iterator i (externals ().begin ());
	   i != externals ().end (); i++)
	{
	  const WrapBasicBlock* x = *i;
	  if (x == b)
	    return true;
	}
      return false;
    }

    bool
    BLXMLPerf::LoopBlocks::exit_path (const WrapBasicBlock* from,
				      const WrapBasicBlock* to) const
    {
      const loop_exit_list_T& l = exit_block_list();
      for (loop_exit_list_const_iterator i (l.begin()); i != l.end(); i++)
	{
	  const loop_exit_T& e = *i;
	  const WrapBasicBlock* f = e.first;
	  const wrap_block_list_T& wbl = e.second;

	  if (f != from)
	    continue;

	  for (wrap_block_list_const_iterator b (wbl.begin());
	       b != wbl.end(); b++)
	    {
	      const WrapBasicBlock* t = *b;
	      if (t == to)
		return true;
	    }
	}
      return false;
    }


    BLXMLPerf::WrapBasicBlock*
    BLXMLPerf::add_block (const llvm::BasicBlock *b)
    {
      BLXMLPerf::WrapBasicBlock* wbb;

      std::pair < block_map_iterator, bool >
	x = get_block_map().insert (std::make_pair (b, WrapBasicBlock (b)));
      wbb = &x.first->second;
      get_block_list().push_back (wbb);

      if (b->hasName())
	wbb->name() = b->getName().str();
      else
	{
	  std::ostringstream n;
	  n << "label%" << wbb->ident();
	  wbb->name() = n.str();
	}
      return wbb;
    }

    BLXMLPerf::WrapBasicBlock*
    BLXMLPerf::get_block (const llvm::BasicBlock *b)
    {
      block_map_iterator i (get_block_map().find (b));
      if (i == get_block_map().end())
	return NULL;
      return &i->second;
    }

    const BLXMLPerf::WrapBasicBlock*
    BLXMLPerf::get_block (const llvm::BasicBlock *b) const
    {
      block_map_const_iterator i (get_block_map().find (b));
      if (i == get_block_map().end())
	return NULL;
      return &i->second;
    }

    BLXMLPerf::LoopBlocks*
    BLXMLPerf::add_loop (const llvm::Loop *l)
    {
      LoopBlocks* lb;

      std::pair < loop_map_iterator, bool >
	x = get_loop_map().insert (std::make_pair (l, LoopBlocks (l)));
      lb = &x.first->second;
      get_loop_list().push_back (lb);
      return lb;
    }

    BLXMLPerf::LoopBlocks*
    BLXMLPerf::find_loop (const llvm::Loop *l)
    {
      loop_map_iterator i (get_loop_map().find (l));
      if (i == get_loop_map().end())
	return NULL;
      return &i->second;
    }

    const BLXMLPerf::LoopBlocks*
    BLXMLPerf::find_loop (const llvm::Loop *l) const
    {
      loop_map_const_iterator i (get_loop_map().find (l));
      if (i == get_loop_map().end())
	return NULL;
      return &i->second;
    }


    InsnCycle
    BLXMLPerf::average_route_visit (Graph& g, Vertex u, Vertex t,
				    boost::default_color_type *color,
				    InsnCycle *cycles)
    {
      InsnCycle x = 0;

      if (u != t)
	{
	  boost::put (color, u, boost::gray_color);

	  int n = 0;

	  OutEdgeIterator ei, eend;
	  for (boost::tie (ei, eend) = boost::out_edges (u, g);
	       ei != eend; ei++)
	    {
	      Vertex v = boost::target (*ei, g);

	      int type = boost::get (boost::vertex_type, g, v);
	      if (type & VERTEX_TYPE_TERMINATOR)
		continue;	// dummy node

	      boost::default_color_type c = boost::get (color, v);

	      if (c == boost::gray_color)
		continue;	// backedge

	      if (c == boost::white_color)
		x += average_route_visit (g, v, t, color, cycles);
	      else
		x += boost::get (cycles, v); // visited
	      n++;
	    }

	  if (n > 1)
	    x /= n;
	}

      x += boost::get (boost::vertex_typical_cycle, g, u);

#ifdef LLVM_DEBUG
      std::string s (boost::get (boost::vertex_name, g, u));
      llvm::outs () << "| " << s << " [" << x << "]\n";
#endif
      boost::put (cycles, u, x);
      boost::put (color, u, boost::black_color);
      boost::put (boost::vertex_average_cycle, g, u, x);
      return x;
    }

    InsnCycle
    BLXMLPerf::average_route_search (Graph& g, Vertex u, Vertex t)
    {
      VertexIterator ui, ue;
      std::vector < boost::default_color_type >
	color (boost::num_vertices (g), boost::white_color);
      std::vector < InsnCycle >
	cycles (boost::num_vertices (g), 0);

      return average_route_visit (g, u, t, color.data(), cycles.data());
    }

    // route search
    InsnCycle
    BLXMLPerf::process_route (Graph& g, Vertex ent, Vertex ret, graph_mode m)
    {
      EdgeIterator ei, eend;

#ifdef LLVM_DEBUG
      {
	std::string mode;
	if (m == search_longest)
	  mode = "longest";
	else if (m == search_shortest)
	  mode = "shortest";
	else
	  mode = "typical";
	llvm::outs () << "--- Route setup (" << mode
		      << ", ent=" << ent << ", ret=" << ret << ") ---\n";
      }
#endif

      for (boost::tie (ei, eend) = boost::edges (g); ei != eend; ei++)
	{
	  InsnCycle d;
	  if (m == search_longest)
	    {
	      d = boost::get (boost::edge_worst_cycle, g, *ei);
	      d = -d;		// inverse
	    }
	  else if (m == search_shortest)
	    d = boost::get (boost::edge_best_cycle, g, *ei);
	  else
	    d = boost::get (boost::edge_typical_cycle, g, *ei);
	  boost::put (boost::edge_weight, g, *ei, d);
#ifdef LLVM_DEBUG
	  Vertex s = boost::source (*ei, g);
	  Vertex t = boost::target (*ei, g);
	  std::string ss = boost::get (boost::vertex_name, g, s);
	  std::string ts = boost::get (boost::vertex_name, g, t);
	  llvm::outs () << ss << "(" << s << ")->"
			<< ts << "(" << t << ") [" << d << "]\n";
#endif
	}

      VertexIterator vi, vend;
      for (boost::tie (vi, vend) = boost::vertices (g); vi != vend; vi++)
	{
	  InsnCycle d;
	  if (m == search_shortest)
	    d = boost::get (boost::vertex_best_cycle, g, *vi);
	  else if (m == search_longest)
	    d = boost::get (boost::vertex_worst_cycle, g, *vi);
	  else
	    d = boost::get (boost::vertex_typical_cycle, g, *vi);
	  boost::put (boost::vertex_current_cycle, g, *vi, d);
	}

      InsnCycle cycle = 0;

      int vertex_count = boost::num_vertices (g);

      if (m == search_shortest || m == search_longest)
	{
	  std::vector < Vertex > parents (vertex_count);
	  std::vector < InsnCycle > distance (vertex_count);
	  if (m == search_shortest)
	    {
	      boost::dijkstra_shortest_paths (g, ent,
					      boost::predecessor_map (&parents[0])
					      .distance_map(&distance[0]));
	    }
	  else
	    {
	      boost::property_map < Graph, boost::edge_weight_t >::type
		weight_pmap = boost::get (boost::edge_weight, g);

	      boost::bellman_ford_shortest_paths (g, vertex_count,
						  boost::weight_map(weight_pmap)
						  .distance_map(&distance[0])
						  .predecessor_map(&parents[0])
						  .root_vertex(ent));
	    }

	  if (parents[ret] == ret)
	    {
	      llvm::errs () << "!" << __LINE__
			    << ": no path to ret(" << ret <<")\n";
	      return -1;
	    }

	  //
	  // check path
	  //
	  std::deque < Vertex > route;
	  std::vector < int > visit (vertex_count);
	  Vertex u = ret;
	  for (Vertex v = ret; v != ent; v = parents[v])
	    {
	      if (visit[v] == 1)
		{
		  std::string&
		    un = boost::get (boost::vertex_name, g, u);
		  llvm::outs ().flush ();
		  llvm::errs () << "!" << __LINE__ << ": route looping, "
		    "couldn't reached to enter["
				<< u << ":" << un << "]\n";
		  int found = 0;
		  InEdgeIterator ei, eend;
		  for (boost::tie (ei, eend) = boost::in_edges (u, g);
		       ei != eend; ei++)
		    {
		      Vertex s = boost::source (*ei, g);
		      if (visit[s] == 0)
			{
			  std::string&
			    sn = boost::get (boost::vertex_name, g, s);
			  llvm::outs ().flush ();
			  llvm::errs () << "!" << __LINE__
					<< ": continine from["
					<< s << ":" << sn << "]\n";
			  v = s;
			  found = 1;
			  break;
			}
		    }
		  if (found == 0)
		    {
		      llvm::outs ().flush ();
		      llvm::errs () << "!" << __LINE__ << ": not continue\n";
		      break;
		    }
		}
	      route.push_front (v);
	      visit[v] = 1;
	      u = v;
	    }
	  visit[ent] = 1;
	  route.push_front (ent);

	  std::deque < Vertex >::iterator i (route.begin());
	  u = *i++;
	  for (; i != route.end(); i++)
	    {
	      Vertex v = *i;
	      std::pair < Edge, bool > x = boost::edge (u, v, g);
	      if (x.second)
		{
		  Edge e = x.first;
		  int t = boost::get (boost::edge_type, g, e);
		  if (m == search_shortest)
		    t |= EDGE_TYPE_BEST;
		  else if (m == search_longest)
		    t |= EDGE_TYPE_WORST;
		  boost::put (boost::edge_type, g, e, t);
		}
	      u = v;
	    }

	  cycle = distance[ret];

	  if (m == search_longest)
	    cycle = -cycle;

#ifdef LLVM_DEBUG
	  llvm::outs () << "Route:\n";
	  for (std::deque < Vertex >::const_iterator i (route.begin());
	       i != route.end(); i++)
	    {
	      const Vertex& v = *i;
	      std::string& n = get (boost::vertex_name, g, v);
	      llvm::outs () << "  " << n << " ["
			    << get (boost::vertex_current_cycle, g, v)
			    << "]\n";
	    }
	  llvm::outs () << "route cycle = " << cycle << "\n";
#endif

	  // add return block cycle
	  cycle += get (boost::vertex_current_cycle, g, ret);
	}
      else
	cycle = average_route_search (g, ent, ret);

#ifdef LLVM_DEBUG
      {
	std::string mode;
	if (m == search_longest)
	  mode = "longest";
	else if (m == search_shortest)
	  mode = "shortest";
	else
	  mode = "typical";
	llvm::outs () << "cycle = " << cycle << "[" << mode << "]\n";
      }
#endif

      return cycle;
    }


    /// @brief process loop
    void
    BLXMLPerf::process_loop (std::vector < LoopBlocks* >* children,
			     const llvm::Function& f,
			     llvm::Loop* l,
			     llvm::ScalarEvolution *se, unsigned int nest)
    {
      const llvm::BasicBlock* enter_bb = l->getHeader();
      WrapBasicBlock* wbent = get_block (enter_bb);

      typedef llvm::SmallVector < llvm::BasicBlock *,
				  MAX_EXITING_BLOCKS + 1 > BlockVec_T;
      BlockVec_T exiting_blocks;

      l->getExitingBlocks (exiting_blocks);

      if (exiting_blocks.size() == MAX_EXITING_BLOCKS + 1)
	{
	  llvm::errs () << "!" << __LINE__ << ": "
			<< f.getName().str() << ": " << wbent->name()
			<< ": too many exiting blocks (> 8)\n";
	  return;
	}

      // mark backedge to loop header [see LoopInfo.h:getNumBackEdges()]
      typedef llvm::GraphTraits <
			   llvm::Inverse < const llvm::BasicBlock* > >	InvBlockTraits;
      for (InvBlockTraits::ChildIteratorType I =
	     InvBlockTraits::child_begin(enter_bb),
	     E = InvBlockTraits::child_end(enter_bb); I != E; ++I)
	{
	  const llvm::BasicBlock* tail = *I;
	  if (l->contains(tail))
	    {
	      WrapBasicBlock* wbtail = get_block (tail);
	      wbtail->add_headers (wbent);
	    }
	}

      llvm::BasicBlock* exit_bb = NULL;
      WrapBasicBlock* wbexit __attribute__ ((unused)) = NULL;
      if (exiting_blocks.size() == 1)
	{
	  exit_bb = exiting_blocks[0];
	  wbexit = get_block (exit_bb);
	}

      std::vector < LoopBlocks* > chld;

#ifdef LLVM_DEBUG
      std::string mpf;
      for (unsigned int i = 0; i < nest; i++)
	mpf += "+";
      if (wbexit)
	{
	  llvm::outs() << mpf
		       << "Process Loop: " << wbent->name() << " -> "
		       << wbexit->name() << "\n";
	}
      else
	{
	  llvm::outs() << mpf
		       << "Process Loop: " << wbent->name() << " -> ...\n";
	}
      llvm::outs() << "  Blocks:";
      for (llvm::Loop::block_iterator bi (l->block_begin());
	   bi != l->block_end(); bi++)
	{
	  const WrapBasicBlock* wbb = get_block (*bi);
	  llvm::outs() << " " << wbb->name();
	}
      llvm::outs() << "\n";
#endif

      // internal loop first
      for (llvm::Loop::iterator i (l->begin()); i != l->end(); i++)
	{
	  llvm::Loop* ll = *i;
	  process_loop (&chld, f, ll, se, nest + 1);
	}

      uint64_t taken_count = ~(uint64_t)0;
      uint64_t max_count = ~(uint64_t)0;
      if (exiting_blocks.size () > 0)
	{
	  // fixed taken count
	  if (se->hasLoopInvariantBackedgeTakenCount (l))
	    {
	      unsigned x = se->getSmallConstantTripCount (l, NULL);
	      if (x)
		taken_count = x;
	    }

	  // max taken count
	  const llvm::SCEV* maxscev = se->getMaxBackedgeTakenCount (l);
	  if (!llvm::isa < llvm::SCEVCouldNotCompute > (maxscev))
	    {
	      const llvm::SCEVConstant*
		cv = llvm::cast < llvm::SCEVConstant >(maxscev);
	      llvm::ConstantInt *exit_value = cv->getValue();
	      if (exit_value->getValue().getActiveBits() <= 32)
		max_count = exit_value->getZExtValue () + 1;
	    }
	}

      // int max satulation
      if (max_count == (uint64_t)INT_MAX
	  || max_count == (uint64_t)UINT_MAX
	  || max_count == (uint64_t)LONG_MAX
	  || max_count == (uint64_t)ULONG_MAX)
	max_count = ~(uint64_t)0;

#ifdef LLVM_DEBUG
      if (wbexit)
	llvm::outs () << mpf << "Loop " << wbent->name()
		      << " - " << wbexit->name() << "\n";
      else
	llvm::outs () << mpf << "Loop " << wbent->name()
		      << " - [...]\n";
      llvm::outs () << mpf << "  taken_count=" << taken_count
		    << ", max_taken_count=" << max_count << "\n";
#endif

      // not handle this loop
      if (taken_count == ~(uint64_t)0 && max_count == ~(uint64_t)0)
	{
#ifdef LLVM_DEBUG
	  llvm::outs() << mpf << "* this loop not handled\n";
#endif
	  return;
	}

      loop_index++;

      LoopBlocks *lb = add_loop (l);

      lb->level () = nest;
      lb->ident () = loop_index;

      if (children)
	children->push_back (lb);

      for (loop_list_iterator i (chld.begin()); i != chld.end(); i++)
	{
	  LoopBlocks* c = *i;
	  lb->add_child (c);
	}

      if (taken_count != ~(uint64_t)0)
	{
	  lb->min_count() = taken_count;
	  lb->max_count() = taken_count;
	  lb->fix_taken() = true;
	}
      else if (max_count != ~(uint64_t)0)
	{
	  lb->min_count() = MIN_LOOP_TAKEN;
	  lb->max_count() = max_count;
	  lb->fix_taken() = false;
	}

      // add loop blocks
      for (llvm::Loop::block_iterator i (l->block_begin());
	   i != l->block_end (); i++)
	{
	  const llvm::BasicBlock* bb = *i;
	  WrapBasicBlock* wbb = get_block (bb);
	  lb->add_block (wbb);
	  if (!wbb->is_fix_loop_node())
	    wbb->loop_index() = loop_index;
	  wbb->add_loop (lb);
	}

      lb->enter_block() = wbent;

      // find next block
      for (BlockVec_T::iterator i (exiting_blocks.begin());
	   i != exiting_blocks.end(); i++)
	{
	  llvm::BasicBlock* bb = *i;
	  WrapBasicBlock *wbb = get_block (bb);

	  loop_exit_list_T& lel = lb->exit_block_list();
	  loop_exit_list_iterator lei;

	  lei = lel.insert (lel.end(),
			    std::make_pair (wbb, wrap_block_list_T()));

	  wrap_block_list_T& wbl = lei->second;

	  for (llvm::succ_iterator si (llvm::succ_begin (bb));
	       si != llvm::succ_end (bb); si++)
	    {
	      llvm::BasicBlock *pb = *si;

	      int found = 0;

	      WrapBasicBlock* wpb = get_block (pb);
	      for (loop_list_iterator li (wpb->get_loops ().begin ());
		   li != wpb->get_loops ().end(); li++)
		{
		  const LoopBlocks* xlb = *li;
		  if (xlb == lb)
		    found = 1;
		}

	      if (found == 0)
		{
		  wbl.push_back (wpb);
		  if (!lb->is_external (wpb))
		    lb->add_external (wpb);
		}
	    }
	}

      Vertex vin = 0;
      Vertex vout = 0;
      InsnCycle max_cycle;
      WrapBasicBlock* wbnext = NULL;
      VertexPairList vpl;
      TempGraph g = make_graph (f, nest + 1, lb, lb->list(),
				lb->enter_block(), wbnext,
				vin, vout, vpl, max_cycle);
      if (g.get() == NULL)
	return;

      lb->best() = process_route (*g, vin, vout, search_shortest);
      lb->typical() = process_route (*g, vin, vout, search_typical);
      lb->worst() = process_route (*g, vin, vout, search_longest);

      if (OutputDOTBase != "")
	{
	  correct_for_dot (*g, f, lb->enter_block(), vpl);
	  output_dot (*g, OutputDOTBase, dot_seq++);
	}

      // mark summed block
      for (wrap_block_list_iterator i (lb->list().begin());
	   i != lb->list().end (); i++)
	{
	  WrapBasicBlock* wbb = *i;
	  if (lb->fix_taken() && !wbb->is_fix_loop_node())
	    wbb->is_fix_loop_node() = true;

#ifdef LLVM_DEBUG
	  std::ostringstream flg;
	  if (wbb == wbent)
	    flg << "<";
	  if (wbb == wbexit)
	    flg << ">";
	  if (wbb->loop_index() != loop_index)
	    flg << "[" << wbb->loop_index() << "]";
	  llvm::outs () << mpf << "    [" << loop_index << "] "
			<< wbb->name()
			<< " " << flg.str() << "\n";
#endif
	}
    }

    /// @brief make graph
    BLXMLPerf::TempGraph BLXMLPerf::make_graph (const llvm::Function& f,
						unsigned int level,
						LoopBlocks* lb,
						wrap_block_list_T& wbl,
						WrapBasicBlock* wbb_enter,
						WrapBasicBlock*& wbb_exit,
						Vertex& vent, Vertex& vexit,
						VertexPairList& deleted_edges,
						InsnCycle& max_cycle)
    {
      typedef std::map < const llvm::BasicBlock*, Vertex > bbmap_T;
      typedef bbmap_T::iterator bbmap_iterator;

      bbmap_T bbmap;

      TempGraph g (new Graph);	// `g' is auto_ptr or unique_ptr

      InsnCycle max_bb_cycle = 0;

      const llvm::BasicBlock* bbent = *wbb_enter;

#ifdef LLVM_DEBUG
      if (lb == NULL && wbb_exit == NULL)
	{
	  llvm::outs () << "make_graph: " << wbb_enter->name()
			<< " -> [RETURN]\n";
	}
      else if (wbb_exit == NULL)
	{
	  llvm::outs () << "make_graph:" << wbb_enter->name() << "\n";
	  for (loop_exit_list_iterator lei (lb->exit_block_list ().begin ());
	       lei != lb->exit_block_list ().end (); lei++)
	    {
	      loop_exit_T& le = *lei;
	      WrapBasicBlock* wbe = le.first;
	      for (wrap_block_list_iterator wbi (le.second.begin ());
		   wbi != le.second.end (); wbi++)
		{
		  WrapBasicBlock* wbn = *wbi;
		  llvm::outs () << " -> " << wbn->name() << "[@"
				<< wbe->name () << "]\n";
		}
	    }
	}
      else
	{
	  llvm::outs () << "make_graph: " << wbb_enter->name()
			<< " -> "
			<< wbb_exit->name() << "\n";
	}
#endif

      if (lb != NULL && wbb_exit == NULL)
	{
	  if (lb->exit_block_list().size() == 1)
	    wbb_exit = lb->exit_block_list()[0].second[0]; // single exit
	}

      // create graph node
      for (wrap_block_list_iterator i (wbl.begin()); i != wbl.end (); i++)
	{
	  WrapBasicBlock* wbb = *i;

	  InsnCycle best_cycle;
	  InsnCycle typical_cycle;
	  InsnCycle worst_cycle;

	  Vertex v = 0;
	  const llvm::BasicBlock* bb;

	  if (!wbb->is_fix_loop_node())
	    {
	      // normal node
	      bb = *wbb;

	      v = boost::add_vertex (*g);

	      best_cycle = wbb->best_cycle();
	      typical_cycle = wbb->typical_cycle();
	      worst_cycle = wbb->worst_cycle();

	      const LoopBlocks* ll = wbb->loop_head ();
	      if (ll == NULL)
		boost::put (boost::vertex_type, *g, v, VERTEX_TYPE_NORMAL);
	      else
		{
		  int t = VERTEX_TYPE_LOOP;
		  if (ll != NULL && ll->fix_taken())
		    t |= VERTEX_TYPE_FIXED;
		  boost::put (boost::vertex_type, *g, v, t);
		  int x = ll->min_count();
# if 0
		  if (x > 1)
		    {
		      // loop min-1
		      x--;
		      best_cycle += ll->best() * x;
		      typical_cycle += ll->typical() * x;
		    }
#else
		  // best: exit loop at last(in min) [incomplete]
		  if (x > 1)
		    best_cycle += ll->best() * (x - 1);

		  // typical: complete minimum count and exit next round
		  typical_cycle += ll->typical() * x;
#endif

		  // worst: exit loop at last(in max) [incomplete]
		  x = ll->max_count();
		  if (x > 1)
		    worst_cycle += ll->worst() * (x - 1);
		}

	      if (lb == NULL
		  && llvm::succ_begin (bb) == llvm::succ_end (bb))
		{
		  if (wbb_exit == NULL)
		    wbb_exit = wbb;

#ifdef LLVM_DEBUG
		  llvm::outs () << "  node " << wbb->name()
				<< "{RETURN} ["
				<< best_cycle << ","
				<< typical_cycle << ","
				<< worst_cycle << "]\n";
#endif
		}
#ifdef LLVM_DEBUG
	      else
		{
		  llvm::outs () << "  node " << wbb->name() << "["
				<< best_cycle << ","
				<< typical_cycle << ","
				<< worst_cycle << "]\n";
		}
#endif
	    }
	  else
	    {
	      bb = *wbb;
	      const LoopBlocks* ll = wbb->loop_head ();
	      if (ll == NULL)
		{
#ifdef LLVM_DEBUG
		  llvm::outs () << "  node "
				<< wbb->name()
				<< " is not loop head\n";
#endif
		  continue;
		}

#if 0
	      // accept child loop only
	      if (wbb->get_loops ().size () > level)
		{
#ifdef LLVM_DEBUG
		  llvm::outs () << "  node " << wbb->name()
				<< " ignore this loop ["
				<< wbb->get_loops().size()
				<< ">"
				<< level
				<< "]\n";
#endif
		  continue;	// not add this time
		}
#endif

	      v = boost::add_vertex (*g);
	      boost::put (boost::vertex_type, *g, v,
			  (VERTEX_TYPE_LOOP
			   |VERTEX_TYPE_FIXED
			   |VERTEX_TYPE_SET));

	      best_cycle = ll->best() * ll->min_count();
	      typical_cycle = ll->typical() * ll->min_count();
	      worst_cycle = ll->worst() * ll->max_count();
#ifdef LLVM_DEBUG
	      llvm::outs () << "  node " << wbb->name()
			    << "{LOOP["
			    << wbb->get_loops ().size()
			    << "<="
			    << level
			    << "]} ["
			    << best_cycle << ","
			    << typical_cycle << ","
			    << worst_cycle << "]\n";
#endif
	    }

	  if (max_bb_cycle < best_cycle)
	    max_bb_cycle = best_cycle;
	  if (max_bb_cycle < typical_cycle)
	    max_bb_cycle = typical_cycle;
	  if (max_bb_cycle < worst_cycle)
	    max_bb_cycle = worst_cycle;

	  boost::put (boost::vertex_name, *g, v, wbb->name());
	  boost::put (boost::vertex_best_cycle, *g, v, best_cycle);
	  boost::put (boost::vertex_typical_cycle, *g, v, typical_cycle);
	  boost::put (boost::vertex_worst_cycle, *g, v, worst_cycle);
	  boost::put (boost::vertex_basicblock, *g, v, bb);
	  bbmap.insert(std::make_pair(bb, v));
#ifdef LLVM_DEBUG
	  llvm::outs () << "  vertex[" << v << "] "
			<< wbb->name() << " created\n";
#endif
	}

      const llvm::BasicBlock* entbb = *wbb_enter;
      if (bbmap.find (entbb) == bbmap.end())
	{
	  llvm::outs ().flush ();
	  llvm::errs () << "!" << __LINE__ << ": "
			<< f.getName().str() << ":" << wbb_enter->name()
			<< " node not found\n";
	  llvm::errs ().flush ();
	  abort ();
	}

      // check exit block
      if (lb == NULL && wbb_exit != NULL)
	{
	  const llvm::BasicBlock* exitbb = *wbb_exit;
	  bbmap_iterator mi = bbmap.find (exitbb);
	  if (mi == bbmap.end ())
	    {
	      llvm::outs ().flush ();
	      llvm::errs () << "!" << __LINE__ << ": "
			    << f.getName().str() << ": "
			    << wbb_exit->name() << " node not found\n";
	      llvm::errs ().flush ();
	      abort ();
	    }
	}

      // create terminator node
      Vertex vt = 0;
      if (lb != NULL)
	{
	  vt = boost::add_vertex (*g);
	  boost::put (boost::vertex_name, *g, vt, "<terminator>");
	  boost::put (boost::vertex_best_cycle, *g, vt, 0);
	  boost::put (boost::vertex_typical_cycle, *g, vt, 0);
	  boost::put (boost::vertex_worst_cycle, *g, vt, 0);
	  boost::put (boost::vertex_type, *g, vt, VERTEX_TYPE_TERMINATOR);
	}

      // make edge
      for (wrap_block_list_iterator i (wbl.begin()); i != wbl.end (); i++)
	{
	  WrapBasicBlock* wbb = *i;
	  const llvm::BasicBlock* bb = *wbb;

	  Vertex v1 = 0;

	  if (!wbb->is_fix_loop_node())
	    {
	      bbmap_iterator mi = bbmap.find (bb);

	      if (mi == bbmap.end ())
		{
		  llvm::outs ().flush ();
		  llvm::errs () << "!" << __LINE__ << ": "
				<< f.getName().str() << ": "
				<< wbb->name() << " not found\n";
		  llvm::errs ().flush ();
		  abort ();
		}

	      v1 = mi->second;

	      InsnCycle db = boost::get (boost::vertex_best_cycle, *g, v1);
	      InsnCycle dt = boost::get (boost::vertex_typical_cycle,
					 *g, v1);
	      InsnCycle dw = boost::get (boost::vertex_worst_cycle,
					 *g, v1);

	      for (llvm::succ_const_iterator si (llvm::succ_begin(bb));
		   si != llvm::succ_end(bb); si++)
		{
		  const llvm::BasicBlock *pb = *si;

		  WrapBasicBlock* wpb = get_block (pb);

		  int del = 0;

		  if (wbb->is_to_header (wpb))
		    {
#ifdef LLVM_DEBUG
		      llvm::outs () << "backedge " << wbb->name()
				    << "->" << wpb->name() << "\n";
		      llvm::outs ().flush ();
#endif
		      if (lb)
			{
			  // connect to term-node (backedge)
			  Edge e = boost::add_edge (v1, vt, *g).first;
			  boost::put (boost::edge_best_cycle, *g, e, db);
			  boost::put (boost::edge_typical_cycle, *g, e, dt);
			  boost::put (boost::edge_worst_cycle, *g, e, dw);
			  boost::put (boost::edge_type, *g, e,
				      EDGE_TYPE_DUMMY);
			}
		      del = 1;
		      goto add_del_edge1;
		    }

		  if (lb != NULL && lb->is_external (wpb))
		    {
		      // remove loop exit
		      continue;
		    }

		  if (wpb->is_fix_loop_node () && wpb->loop_head () == NULL)
		    {
		      llvm::outs ().flush ();
		      llvm::errs () << "!" << __LINE__ << ": "
				    << f.getName().str () << ": "
				    << wbb->name() << " to "
				    << wpb->name() << " enter loop\n";
		      llvm::errs ().flush ();
		    }


#ifdef LLVM_DEBUG
		  llvm::outs () << wbb->name()
				<< " -> " << wpb->name()
				<< " [" << db << "," << dt << ","
				<< dw << "]\n";
#endif

		add_del_edge1:

		  bbmap_iterator mi = bbmap.find (pb);
		  if (mi == bbmap.end ())
		    {
		      llvm::outs ().flush ();
		      llvm::errs() << "!" << __LINE__ << ": "
				   << f.getName().str() << ": "
				   << wpb->name() << " not found\n";
		      llvm::errs ().flush ();
		    }
		  else if (del)
		    {
		      Vertex v2 = mi->second;
		      deleted_edges.push_back (std::make_pair (v1, v2));
		    }
		  else
		    {
		      Vertex v2 = mi->second;

		      if (v1 != v2)
			{
			  Edge e = boost::add_edge (v1, v2, *g).first;
			  boost::put (boost::edge_best_cycle, *g, e, db);
			  boost::put (boost::edge_typical_cycle, *g, e, dt);
			  boost::put (boost::edge_worst_cycle, *g, e, dw);
			  boost::put (boost::edge_type, *g, e,
				      EDGE_TYPE_NORMAL);
			}
		    }
		}
	    }
	  else
	    {
	      const LoopBlocks* ll = wbb->loop_head ();
	      if (ll == NULL)
		continue;

#if 0
	      // accept child loop only
	      if (wbb->get_loops ().size () > level)
		{
#ifdef LLVM_DEBUG
		  llvm::outs () << "  node " << wbb->name()
				<< " ignore this loop ["
				<< wbb->get_loops().size()
				<< ">"
				<< level
				<< "]\n";
#endif
		  continue;	// not add this time
		}
#endif

	      bb = *wbb;
	      bbmap_iterator mi = bbmap.find (bb);

	      if (mi == bbmap.end ())
		{
		  llvm::outs ().flush ();
		  llvm::errs() << "!" << __LINE__ << ": "
			       << f.getName().str() << ": "
			       << wbb->name() << " not found\n";
		  llvm::errs().flush ();
		  abort ();
		}

	      v1 = mi->second;

	      InsnCycle db = ll->best() * ll->min_count();
	      InsnCycle dt = ll->typical() * ll->min_count();
	      InsnCycle dw = ll->worst() * ll->max_count();

	      if (max_bb_cycle < db)
		max_bb_cycle = db;
	      if (max_bb_cycle < dt)
		max_bb_cycle = dt;
	      if (max_bb_cycle < dw)
		max_bb_cycle = dw;

	      for (wrap_block_list_const_iterator
		     wbi (ll->externals().begin ());
		   wbi != ll->externals().end (); wbi++)
		{
		  const WrapBasicBlock* nwbb = *wbi;
		  const llvm::BasicBlock* nbb = *nwbb;

		  nbb = *nwbb;

		  int del = 0;

		  if (nbb == bbent)
		    {
		      // connect to term-node (backedge)
		      Edge e = boost::add_edge (v1, vt, *g).first;
		      boost::put (boost::edge_best_cycle, *g, e, db);
		      boost::put (boost::edge_typical_cycle, *g, e, dt);
		      boost::put (boost::edge_worst_cycle, *g, e, dw);
		      boost::put (boost::edge_type, *g, e,
				  EDGE_TYPE_DUMMY);
		      del = 1;
		      goto add_del_edge2;
		    }

		  // exiting to other loop (enter in other loop)
		  if (nwbb->is_fix_loop_node()
		      && nwbb->loop_head () == NULL
#if 0
		      && nwbb->get_loops ().size () > level
#endif
		      && nwbb->is_in_loop (lb) == false)
		    {
		      llvm::outs().flush ();
		      llvm::errs () << "!" << __LINE__ << ": "
				    << f.getName().str() << ": "
				    << wbb->name() << " to "
				    << nwbb->name()
				    << " enter other loop\n"; // XXX
		      llvm::errs ().flush ();
		      continue;	// ignore
		    }

#ifdef LLVM_DEBUG
		  llvm::outs () << wbb->name()
				<< "{LOOP}"
				<< "  -> " << nwbb->name()
				<< "{EXIT} [" << db << "," << dt << ","
				<< dw << "]\n";
#endif

		add_del_edge2:

		  mi = bbmap.find (nbb);
		  if (mi == bbmap.end ())
		    {
		      llvm::outs().flush ();
		      llvm::errs() << "!" << __LINE__ << ": "
				   << f.getName().str() << ": "
				   << nwbb->name() << " not found\n";
		      llvm::errs ().flush ();
		      continue;	// ignore
		    }

		  Vertex v2 = mi->second;

		  if (del)
		    deleted_edges.push_back (std::make_pair (v1, v2));
		  else if (v1 != v2)
		    {
		      Edge e = boost::add_edge (v1, v2, *g).first;
		      boost::put (boost::edge_best_cycle, *g, e, db);
		      boost::put (boost::edge_typical_cycle, *g, e, dt);
		      boost::put (boost::edge_worst_cycle, *g, e, dw);
		      boost::put (boost::edge_type, *g, e,
				  EDGE_TYPE_NORMAL);
		    }
		}
	    }
	}

      vent = bbmap.find (bbent)->second;
      if (lb != NULL)
	vexit = vt;
      else if (wbb_exit == NULL)
	{
	  llvm::outs ().flush ();
	  llvm::errs() << "!" << __LINE__ << ": "
		       << f.getName().str() << ": "
		       << "end block not found\n";
	  llvm::errs ().flush ();
	  return TempGraph();	// return null pointer
	}
      else
	{
	  const llvm::BasicBlock* bbexit = *wbb_exit;
	  vexit = bbmap.find (bbexit)->second;
	}

      max_cycle = max_bb_cycle + 1;

      return g;
    }

    //
    // process functions
    //
    bool
    BLXMLPerf::runOnFunction (llvm::Function& f)
    {
      if (f.size () == 0)
	return false;

#ifdef LLVM_DEBUG
      llvm::outs () << "Function Name : " << f.getName().str() << "\n";
#endif

      int id = functioninfo ().size ();
      functioninfo ().push_back (FunctionInfo (&f, f.getName ().str ()));
      FunctionInfo& fi = functioninfo ().back ();
      fi.id () = id;
      add_idmap (&f, id);

      for (llvm::Function::iterator bi (f.begin()); bi != f.end(); bi++)
	{
	  llvm::BasicBlock &bb = *bi;

	  for (llvm::BasicBlock::iterator ii (bb.begin());
	       ii != bb.end(); ii++)
	    {
	      llvm::Instruction &inst = *ii;

	      llvm::CallInst* CI;
	      CI = llvm::dyn_cast < llvm::CallInst > (&inst);
	      if (CI)
		{
		  llvm::Function* cf = CI->getCalledFunction ();
		  if (cf != NULL)
		    fi.add_callee (cf);
		}
	    }
	}

      return false;
    }

    void
    BLXMLPerf::myRunOnFunction (FunctionInfo& fi)
    {
      get_block_map ().clear ();
      get_block_list ().clear ();
      get_loop_map ().clear ();
      get_loop_list ().clear ();

      const SHIM::XML::Latency dummy_latency (1.0, 1.5, 2);

      llvm::Function& f = *fi.function ();

#ifdef LLVM_DEBUG
      llvm::outs () << "Function Name : " << f.getName().str() << "\n";
#endif

      // create wrap basicblock list
      WrapBasicBlock::seq() = 0;
      for (llvm::Function::iterator bi (f.begin());
	   bi != f.end(); bi++)
	{
	  llvm::BasicBlock &bb = *bi;

	  add_block (&bb);
	}

      //
      // calcualate cycle for all BasicBlocks
      //
      for (wrap_block_list_iterator wbi (get_block_list().begin());
	   wbi != get_block_list().end(); wbi++)
	{
	  WrapBasicBlock* wbb = *wbi;

	  const llvm::BasicBlock *bb = *wbb;


#ifdef LLVM_DEBUG
	  llvm::outs () << "# " << wbb->name() << "\n";
#endif

#ifdef LLVM_DEBUG
	  int insn_num = 0;
#endif
	  InsnCycle best_bb_cycle = 0;
	  InsnCycle typical_bb_cycle = 0;
	  InsnCycle worst_bb_cycle = 0;
	  for (llvm::BasicBlock::const_iterator ii (bb->begin());
	       ii != bb->end(); ii++)
	    {
	      const llvm::Instruction &inst = *ii;

	      const llvm::ReturnInst* RI;
	      RI = llvm::dyn_cast < llvm::ReturnInst > (&inst);
	      if (RI && SHIM_NO_RET == true)
		continue;

	      const llvm::BranchInst* BR;
	      BR = llvm::dyn_cast < llvm::BranchInst > (&inst);
	      if (BR && SHIM_NO_BR == true)
		continue;

	      const SHIM::XML::Latency* l = NULL;
	      if (shim)
		l = shim->latency (inst.getOpcodeName());
	      if (l == NULL)
		{
		  llvm::outs().flush ();
		  llvm::errs () << "!" << __LINE__ << ": "
				<< f.getName().str() << ": "
				<< inst.getOpcodeName() << " not found\n";
		  llvm::errs ().flush ();
		  l = &dummy_latency;
		}
	      InsnCycle best_insn_cycle = l->best ();
	      InsnCycle worst_insn_cycle = l->worst ();
	      InsnCycle typical_insn_cycle = l->typical ();

	      const llvm::CallInst* CL;
	      CL = llvm::dyn_cast < llvm::CallInst > (&inst);
	      if (CL)
		{
		  llvm::Function* cf = CL->getCalledFunction ();
		  if (cf)
		    {
		      int cfid = get_id (cf);
		      if (cfid != -1)
			{
			  FunctionInfo& cfi = functioninfo ()[cfid];
			  best_insn_cycle += cfi.best ();
			  typical_insn_cycle += cfi.typical ();
			  worst_insn_cycle += cfi.worst ();
#ifdef LLVM_DEBUG
			  llvm::outs () << "  ++ call "
					<< cfi.name ()
					<< '['
					<< cfi.best () << ','
					<< cfi.typical () << ','
					<< cfi.worst () << "]\n";
#endif
			}
		    }
		}

#ifdef LLVM_DEBUG
	      llvm::outs () << "  " << insn_num++
			    << ":" << inst.getOpcodeName() << "["
			    << best_insn_cycle << ","
			    << typical_insn_cycle << ","
			    << worst_insn_cycle << "]\n";
#endif
	      best_bb_cycle += best_insn_cycle;
	      typical_bb_cycle += typical_insn_cycle;
	      worst_bb_cycle += worst_insn_cycle;
	    }

	  wbb->best_cycle () = best_bb_cycle;
	  wbb->typical_cycle () = typical_bb_cycle;
	  wbb->worst_cycle () = worst_bb_cycle;

#ifdef LLVM_DEBUG
	  if (llvm::succ_begin (bb) != llvm::succ_end (bb))
	    {
	      llvm::outs () << "  " << "succ list:\n";
	      for (llvm::succ_const_iterator si (llvm::succ_begin (bb));
		   si != llvm::succ_end (bb); si++)
		{
		  const llvm::BasicBlock *sb = *si;

		  WrapBasicBlock *wsb = get_block (sb);

		  llvm::outs () << "    " << wsb->name() << "\n";
		}
	    }
#endif
	}

      //
      // create loop infomations
      //
#ifdef USE_LLVM_LOOPINFOWRAPPERPASS
      llvm::LoopInfoWrapperPass&
	liwp = getAnalysis < llvm::LoopInfoWrapperPass > ();
      llvm::LoopInfo &li = liwp.getLoopInfo();
#else // !USE_LLVM_LOOPINFOWRAPPERPASS
      llvm::LoopInfo &li = getAnalysis< llvm::LoopInfo > ();
#endif // !USE_LLVM_LOOPINFOWRAPPERPASS
#ifdef USE_LLVM_SCALAREVOLUTIONWRAPPERPASS
      llvm::ScalarEvolutionWrapperPass&
	sewp = getAnalysis< llvm::ScalarEvolutionWrapperPass > ();
      llvm::ScalarEvolution& se = sewp.getSE ();
#else
      llvm::ScalarEvolution &se = getAnalysis< llvm::ScalarEvolution > ();
#endif

#ifdef LLVM_DEBUG
      if (li.empty ())
	llvm::outs () << "no loop\n";
      else
	se.print(llvm::outs(), f.getParent());
#endif

      loop_index = 0;
      for (llvm::LoopInfo::iterator i (li.begin ()); i != li.end (); i++)
	{
	  llvm::Loop* l = *i;

	  process_loop (NULL, f, l, &se, 0);
	}

      //
      // create graph
      //

      const llvm::BasicBlock& entry = f.getEntryBlock();
      WrapBasicBlock* wbent = get_block (&entry);
      WrapBasicBlock* wbret = NULL;
      Vertex vent = 0;
      Vertex vret = 0;
      VertexPairList vpl;
      InsnCycle max_bb_cycle = 0;
      TempGraph g = make_graph (f, 1, NULL, get_block_list(),
				wbent, wbret, vent, vret, vpl,
				max_bb_cycle);
      if (g.get() == NULL)
	return;

      //
      // graph search
      //

      InsnCycle best_cycle = 0;
      InsnCycle typical_cycle = 0;
      InsnCycle worst_cycle = 0;
      int vertex_count = boost::num_vertices (*g);
      if (vertex_count == 0)
	;
      else if (vertex_count > 1)
	{
	  best_cycle = process_route (*g, vent, vret, search_shortest);
	  worst_cycle = process_route (*g, vent, vret, search_longest);
	  typical_cycle = process_route (*g, vent, vret, search_typical);
	}
      else
	{
	  best_cycle = boost::get (boost::vertex_best_cycle, *g, vent);
	  worst_cycle = boost::get (boost::vertex_worst_cycle, *g, vent);
	  typical_cycle = boost::get (boost::vertex_typical_cycle,
				      *g, vent);
	}

      if (OutputDOTBase != "")
	{
	  correct_for_dot (*g, f, NULL, vpl);
	  output_dot (*g, OutputDOTBase, dot_seq++);
	}

#ifdef LLVM_DEBUG
      llvm::outs () << "best cycle = " << best_cycle << "\n";
      llvm::outs () << "worst cycle = " << worst_cycle << "\n";
      llvm::outs () << "typical cycle = " << typical_cycle << "\n";
#endif

      fi.best () = best_cycle;
      fi.worst () = worst_cycle;
      fi.typical () = typical_cycle;
    }

    bool
    BLXMLPerf::doInitialization (llvm::Module& m)
    {
      if (SHIM_File != "-no")
	{
	  SHIM::XML::File*
	    f = new SHIM::XML::File (SHIM_File.c_str(),
				     SHIM_ComponentSet,
				     SHIM_MasterComponent,
				     SHIM_CommonInstructionSet,
				     SHIM_Schema.c_str());

	  if (f == NULL || !*f)
	    {
	      if (f)
		delete f;
	      return false;
	    }

	  shim = f;
	}

      SimulinkModel::BLXML*
	b = new SimulinkModel::BLXML (InputBLXML.c_str(),
				      BLXML_NO_VALIDATION);
      if (b == NULL || !*b)
	{
	  if (b)
	    delete b;
	  llvm::errs () << "** BLXML input error\n";
	  return false;
	}

      blxml = b;

      return true;
    }

    bool
    BLXMLPerf::doFinalization (llvm::Module& m)
    {
      if (blxml == NULL)
	return false;

      TSortEdges call_edges;
      for (functioninfo_iterator i (functioninfo ().begin ());
	   i != functioninfo ().end (); i++)
	{
	  FunctionInfo& fi = *i;
	  CalleeList& cl = fi.callees ();
	  for (CalleeList::iterator i (cl.begin ()); i != cl.end (); i++)
	    {
	      llvm::Function* cf = *i;
	      int fid = get_id (cf);
	      if (fid == -1)
		continue;
	      FunctionInfo& cfi = functioninfo ()[fid];
	      cfi.is_callee () = true;
	      call_edges.push_back (std::make_pair (fi.id (), fid));
	    }
	}

      TSortResult result;
      tsort_callflow (functioninfo ().size (), call_edges, result);

#ifdef LLVM_DEBUG
      for (TSortEdges::iterator i (call_edges.begin ());
	   i != call_edges.end (); i++)
	{
	  if (i != call_edges.begin ())
	    llvm::outs () << ',';
	  llvm::outs() << '(' << i->first << ',' << i->second << ')';
	}
      llvm::outs () << '\n';

      for (TSortResult::iterator i (result.begin ()); i != result.end (); i++)
	{
	  if (i != result.begin ())
	    llvm::outs () << ',';
	  llvm::outs() << *i;
	}
      llvm::outs () << '\n';
#endif

      for (TSortResult::const_iterator i (result.begin ());
	   i != result.end (); i++)
	{
	  int id = *i;
	  FunctionInfo& f = functioninfo ()[id];
	  myRunOnFunction (f);
	}

      for (const_functioninfo_iterator i (functioninfo ().begin ());
	   i != functioninfo ().end (); i++)
	{
	  const FunctionInfo& f = *i;

	  std::string fname (f.name());
	  if (fname.compare (0, 6, "task__") == 0)
	    {
	      std::string bname (fname, 6);
	      blxml->set_performance (bname, "task",
				      f.best(), f.typical(), f.worst());
#ifdef LLVM_DEBUG
	      llvm::outs () << bname << "[task] "
			    << f.best() << ","
			    << f.typical() << ","
			    << f.worst() << "\n";
#endif
	    }
	  else if (fname.compare (0, 8, "update__") == 0)
	    {
	      std::string bname (fname, 8);
	      blxml->set_performance (bname, "update",
				      f.best(), f.typical(), f.worst());
#ifdef LLVM_DEBUG
	      llvm::outs () << bname << "[update] "
			    << f.best() << ","
			    << f.typical() << ","
			    << f.worst() << "\n";
#endif
	    }
	  else if (fname.compare (0, 6, "init__") == 0)
	    {
	      std::string bname (fname, 6);
	      blxml->set_performance (bname, "init",
				      f.best(), f.typical(), f.worst());
#ifdef LLVM_DEBUG
	      llvm::outs () << bname << "[init] "
			    << f.best() << ","
			    << f.typical() << ","
			    << f.worst() << "\n";
#endif
	    }
	  else if (!f.is_callee ())
	    {
	      llvm::outs().flush ();
	      llvm::errs () << "!" << __LINE__ << ": Warning: "
			    << fname << ": "
			    << ": not BLXML/Callee function\n";
	      llvm::errs ().flush ();
	    }
	}

      if (SHIM_Schema != "")
	blxml->set_filename (SimulinkModel::BLXML::FT_SHIM_SCHEMA, SHIM_Schema);
      blxml->set_filename (SimulinkModel::BLXML::FT_XML, InputBLXML);

      if (shim)
	{
	  blxml->set_filename (SimulinkModel::BLXML::FT_SHIM, SHIM_File);
	  blxml->set_shim_param (SimulinkModel::BLXML::ET_COMPONENTSET,
				 shim->ComponentSet());
	  blxml->set_shim_param (SimulinkModel::BLXML::ET_MASTERCOMPONENT,
				 shim->MasterComponent());
	  blxml->set_shim_param (SimulinkModel::BLXML::ET_COMMONINSTRUCTIONSET,
				 shim->CommonInstructionSet());
	}

      if (OutputBLXML == "")
	blxml->output (std::cout);
      else
	blxml->output (OutputBLXML.c_str());

      return true;
    }

    void
    BLXMLPerf::getAnalysisUsage (llvm::AnalysisUsage& au) const
    {
      au.setPreservesAll ();	// not change pass

#ifdef USE_LLVM_LOOPINFOWRAPPERPASS
      au.addRequiredTransitive < llvm::LoopInfoWrapperPass > ();
#else
      au.addRequiredTransitive < llvm::LoopInfo > ();
#endif
#ifdef USE_LLVM_SCALAREVOLUTIONWRAPPERPASS
      au.addRequiredTransitive < llvm::ScalarEvolutionWrapperPass > ();
#else
      au.addRequiredTransitive < llvm::ScalarEvolution > ();
#endif
    }
  }
}

static llvm::RegisterPass<Simulink::LLVM::BLXMLPerf>
BLXMLPerfPass("blxml-perf", "BLXMLPerf test", false, false);
