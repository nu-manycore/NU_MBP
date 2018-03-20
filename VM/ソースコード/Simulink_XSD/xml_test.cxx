#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <sstream>
#include <deque>
#include <stack>

#include <errno.h>
#include <string.h>

#include <boost/graph/graphviz.hpp>

#include "SimulinkXML.h"

using namespace std;
using namespace SimulinkModel;
using namespace SimulinkModel::XSD;
using namespace Simulink;
using namespace Simulink::XML;

#include <unistd.h>

typedef std::map< std::string, std::string > DotAttributes;

typedef boost::adjacency_list <
  boost::listS, boost::vecS, boost::bidirectionalS,
  // vertex properties
  boost::property <
    boost::vertex_index_t, int,
    boost::property <
      boost::vertex_attribute_t, DotAttributes
      > >,
  // edge properties
  boost::property <
    boost::edge_index_t, int,
    boost::property <
      boost::edge_attribute_t, DotAttributes
    > >,
  // graph properties
  boost::property <
    boost::graph_name_t, std::string,
    boost::property <
      boost::graph_graph_attribute_t, DotAttributes,
      boost::property <
	boost::graph_vertex_attribute_t, DotAttributes,
	boost::property <
	  boost::graph_edge_attribute_t, DotAttributes
	  > > > > > Graph;

typedef boost::subgraph < Graph > SubGraph;

typedef boost::graph_traits < Graph >::edge_descriptor Edge;
typedef boost::graph_traits < Graph >::vertex_descriptor Vertex;

typedef std::pair < const BlockInfo*, bool > vbinfo_T;
typedef std::deque < vbinfo_T > vblist_T;

typedef std::vector < int > vertex_vector_T;
typedef std::vector < bool > vertex_valid_vector_T;
typedef std::vector < SubGraph* > subgraph_graph_vector_T;

static vblist_T nodes;

static vertex_vector_T Vertex_vector;
static vertex_valid_vector_T Vertex_valid_vector;

static subgraph_graph_vector_T SubGraph_graph_vector;

static bool need_subgraph = false;

static bool no_var_label = false;
static bool no_block_name = false;

static bool
is_merged_lower (const BlockInfo* subsys)
{
  // no input/output
  if (subsys->forward ().size () == 0 && subsys->backward ().size () == 0)
    return false;

  // true when lower blocks are merged by this subsystem.
  for (block_list_const_iterator i (subsys->lower_block ().begin ());
       i != subsys->lower_block ().end (); i++)
    {
      block_list_const_iterator e = subsys->merge_blocks ().end ();
      block_list_const_iterator
	x = std::find (subsys->merge_blocks ().begin (), e, *i);
      if (x == e)
	return false;
    }

  return true;
}

void
check_node (Reader& xrd, const blocks_T& bs, int& n)
{
  for (blocks_T::block_const_iterator i (bs.block ().begin ());
       i != bs.block ().end (); i++)
    {
      const block_T& b = *i;
      const BlockInfo* binfo = xrd.find_block (b.name());

      if (b.blocktype() == "SubSystem" && !is_merged_lower (binfo))
	{
	  nodes.push_back (std::make_pair (binfo, true));

	  int old_n = n;
	  for (block_T::blocks_const_iterator j (b.blocks ().begin ());
	       j != b.blocks ().end (); j++)
	    check_node (xrd, *j, n);

	  if (old_n == n)
	    {
	      // discard this subgraph when no subgraph node.
	      nodes.pop_back ();
	    }
	  else
	    {
	      // need subgragh, and add end mark
	      need_subgraph = true;
	      nodes.push_back (std::make_pair ((BlockInfo*)NULL, false));
	    }
	}
      else if (binfo->forward ().size () || binfo->backward ().size ())
	{
	  // graph node
	  nodes.push_back (std::make_pair (binfo, false));
	  n++;
	}
    }

#if 0
  const BlockInfo* cur = NULL;
  std::stack < const BlockInfo* > binfo_stack;
  for (vblist_T::const_iterator i (nodes.begin ()); i != nodes.end (); i++)
    {
      const vbinfo_T& v = *i;

      if (v.second)
	{
	  std::cout << "SubGraph: ";
	  if (cur == NULL)
	    std::cout << "Root";
	  else
	    std::cout << cur->block ()-> name ();
	  binfo_stack.push (cur);
	  cur = v.first;
	  std::cout << " -> " << cur->block ()->name () << std::endl;

	}
      else if (v.first == NULL)
	{
	  std::cout << "SubGraph: ";
	  const BlockInfo* prev = binfo_stack.top ();
	  binfo_stack.pop ();
	  if (prev == NULL)
	    std::cout << "Root";
	  else
	    std::cout << prev->block ()->name ();
	  std::cout << " <- " << cur->block ()->name ();
	  std::cout << std::endl;
	  cur = prev;
	}
      else
	std::cout << "Node: " << v.first->block ()-> name () << std::endl;
    }
  std::cout << "Size: " << n << std::endl;
#endif
}

void
create_subgraphs (SubGraph& top)
{
  SubGraph* g = &top;
  const BlockInfo* g_binfo = NULL; // Root
  std::stack < std::pair < SubGraph*, const BlockInfo* > > graph_stack;

  for (vblist_T::const_iterator i (nodes.begin ()); i != nodes.end (); i++)
    {
      const BlockInfo* binfo = i->first;
      bool is_subgraph = i->second;

      if (is_subgraph)
	{
	  // start subgraph
	  assert (binfo);

	  const block_T& b = *binfo->block ();
	  std::string subgraph_name ("cluster" + b.name ());
	  SubGraph& ssysgraph = g->create_subgraph ();
	  boost::get_property (ssysgraph, boost::graph_name) = subgraph_name;
	  DotAttributes&
	    da = boost::get_property (ssysgraph, boost::graph_graph_attribute);
	  da["label"] = b.name ();
	  da["style"] = "dashed";
	  da["pencolor"] = "forestgreen";
	  da["fontcolor"] = "forestgreen";

	  graph_stack.push (std::make_pair (g, g_binfo));
	  g = &ssysgraph;
	  g_binfo = binfo;
	}
      else if (binfo == NULL)
	{
	  // end subgraph, back to previous
	  assert (graph_stack.size ());
	  assert (g_binfo);

	  std::pair < SubGraph*, const BlockInfo* >& s = graph_stack.top ();

	  g = s.first;
	  g_binfo = s.second;
	  graph_stack.pop ();
	}
      else
	{
	  // set subgraph
	  SubGraph_graph_vector[binfo->id ()] = g;
	}
    }
}

static inline SubGraph*
get_subgraph (SubGraph& g, long id)
{
  return SubGraph_graph_vector[id];
}

static inline Graph*
get_subgraph (Graph& g, long id)
{
  return &g;
}

static void
make_node_attr (const BlockInfo* binfo, DotAttributes& da)
{
  const block_T& b = *binfo->block ();

  ostringstream oss;

  oss << 't' << binfo->id ();
  if (!no_block_name)
    oss << "\\n" << b.name ();

  std::string name = oss.str ();

  da["label"] = name;

  if (b.blocktype() == "SubSystem")
    {
      da["style"] = "filled";
      da["fillcolor"] = "plum";
    }
  else if (b.blocktype () == "Inport" || b.blocktype () == "Outport")
    da["shape"] = "box";
}


static void
make_edge_attr (const BlockConnect* bc, DotAttributes& da)
{
  ostringstream oss;
  int n = 0;

  for (var_list_T::const_iterator vi (bc->var_list ().begin ());
       vi != bc->var_list ().end (); vi++)
    {
      const var_T* var = *vi;

      if (n++)
	oss << ',';
      oss << var->name ();

      const var_T::start_optional& st = var->start();
      const var_T::end_optional& ed = var->end();
      const var_T::size_optional& sz = var->size();

      if (st && ed)
	oss << '[' << *st << '-' << *ed << ']';
      else if (st)
	oss << '[' << *st << ']';
      else if (sz)
	oss << "[sz=" << *sz << ']';
    }

  da["label"] = oss.str ();
  da["fontcolor"] = "purple";

  const std::string& tbtype = bc->tail ()->block ()->blocktype ();
  const std::string& hbtype = bc->head ()->block ()->blocktype ();
  if (tbtype == "Inport" || hbtype == "Outport")
    da["color"] = "green";
}

template < class G >
void
create_node (G& graph)
{
  int n = 0;
  for (vblist_T::const_iterator i (nodes.begin ()); i != nodes.end (); i++)
    {
      const BlockInfo* binfo = i->first;
      bool is_subgraph = i->second;

      if (binfo == NULL || is_subgraph)
	continue;

      G* sg = get_subgraph (graph, binfo->id ());

      DotAttributes da;
      make_node_attr (binfo, da);

      boost::add_vertex (n, *sg);
      boost::put (boost::vertex_attribute, graph, n, da);
      Vertex_vector[binfo->id ()] = n++;
      Vertex_valid_vector[binfo->id ()] = true;
    }
}

template < class G>
Edge
create_edge (G& graph, const BlockConnect* bc, int u, int v,
	     DotAttributes& da)
{
  std::pair < Edge, bool > ret = boost::add_edge (u, v, graph);

  Edge& e = ret.first;

  assert (ret.second);

  if (bc == NULL || no_var_label)
    return e;

  make_edge_attr (bc, da);

  boost::put (boost::edge_attribute, graph, e, da);

  return e;
}


template < class G >
void
make_graph (G& root, const std::string& graph_name, bool lr, Reader& xrd)
{
  boost::get_property (root, boost::graph_name) = graph_name;

  if (lr)
    boost::get_property (root,
			 boost::graph_graph_attribute)["rankdir"] = "LR";

  create_node (root);

  for (vblist_T::const_iterator i (nodes.begin ()); i != nodes.end (); i++)
    {
      const BlockInfo* binfo = i->first;
      bool is_subgraph = i->second;
      if (binfo == NULL || is_subgraph)
	continue;

      if (!Vertex_valid_vector[binfo->id ()])
	continue;

      const block_T* b = binfo->block ();

      int u = Vertex_vector[binfo->id ()];

      if (binfo->forward ().size ())
	{
	  for (block_connect_list_T::const_iterator
		 ci (binfo->forward ().begin ());
	       ci != binfo->forward ().end (); ci++)
	    {
	      const BlockConnect& bc = *ci;
	      const BlockInfo* head = bc.head ();

	      assert (Vertex_valid_vector[head->id ()]);

	      int v = Vertex_vector[head->id ()];

	      DotAttributes da;
	      create_edge (root, &bc, u, v, da);
	    }
	}
      else if (b->blocktype () == "SubSystem")
	{
	  for (block_list_const_iterator
		 j (binfo->lower_block ().begin ());
	       j != binfo->lower_block ().end (); j++)
	    {
	      const BlockInfo* sbinfo = *j;

	      if (!Vertex_valid_vector[sbinfo->id ()])
		continue;

	      int v = Vertex_vector[sbinfo->id ()];

	      DotAttributes da;
	      da["color"] = "red";
	      da["style"] = "dashed";

	      create_edge (root, (BlockConnect*)NULL, u, v, da);
	    }
	}
    }
}

int
main(int argc, char * const *argv)
{
  int c;
  int lr = 0;
  int do_dump = 0;
  bool xml_validation = true;
  const char* output = NULL;

  while ((c = getopt (argc, argv, "ldnLNV")) != -1)
    {
      switch (c)
	{
	case 'l':
	  lr = 1;
	  break;

	case 'd':
	  do_dump = 1;
	  break;

	case 'n':
	  xml_validation = false;
	  break;

	case 'L':
	  no_var_label = true;
	  no_block_name = true;
	  break;

	case 'N':
	  no_block_name = true;
	  break;

	case 'V':
	  no_var_label = true;
	  break;

	case '?':
	usage:
	  std::cerr << "usage: "
		    << argv[0]
		    << " [-n] [-l] [-N] [-V] [-L] input.xml [output.dot]"
		    << std::endl;
	  return 1;
	}
    }

  argc -= optind - 1;

  if (argc != 2 && argc != 3)
    goto usage;

  argv += optind - 1;

  if (argc == 3 && argv[2][0] != 0)
    output = argv[2];

  Reader xrd (argv[1], xml_validation);

  if (!xrd)
    return 1;

  if (do_dump)
    xrd.dump ();

  int n = 0;
  check_node (xrd, xrd.xml_blocks(), n);

  std::string graph_name ("testout");

  Vertex_vector.resize (xrd.block_list ().size ());
  Vertex_valid_vector.resize (xrd.block_list ().size (), false);

  std::ofstream fout;
  std::ostream* outp = &std::cout;

  if (output)
    {
      fout.open (output);
      if (!fout)
	{
	  std::cerr << output << ": " << strerror (errno) << std::endl;
	  return -1;
	}
      outp = &fout;
    }

  std::ostream& out = *outp;

  if (need_subgraph)
    {
      SubGraph graph (n);

      SubGraph& top = graph.create_subgraph ();
      std::string subgraph_name ("cluster_Root");
      boost::get_property (top, boost::graph_name) = subgraph_name;
      boost::get_property (top,
			   boost::graph_graph_attribute)["label"] = "Root";
      SubGraph_graph_vector.resize (xrd.block_list ().size (),
				    (SubGraph*)NULL);
      create_subgraphs (top);
      make_graph (graph, graph_name, lr, xrd);
      boost::write_graphviz (out, graph);
    }
  else
    {
      Graph graph;

      make_graph (graph, graph_name, lr, xrd);
      boost::write_graphviz (out, graph,
			     boost::make_vertex_attributes_writer (graph),
			     boost::make_edge_attributes_writer (graph),
			     boost::make_graph_attributes_writer (graph));
    }

  return 0;
}
