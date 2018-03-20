#include "llvm-BLXMLPerf.h"

#include <boost/graph/graphviz.hpp>
#include <boost/graph/topological_sort.hpp>

#include <string.h>
#include <errno.h>

namespace Simulink
{
  namespace LLVM
  {
    template <class G>
    class label_writer {
    public:
      label_writer(const G& g) : graph(g) {}
      template <class V>
      void operator()(std::ostream& out, const V& v) const {
	const std::string& n = boost::get(boost::vertex_name, graph, v);
	InsnCycle b = boost::get(boost::vertex_best_cycle, graph, v);
	InsnCycle t = boost::get(boost::vertex_typical_cycle, graph, v);
	InsnCycle w = boost::get(boost::vertex_worst_cycle, graph, v);
	InsnCycle a = boost::get(boost::vertex_average_cycle, graph, v);

	int vt = boost::get (boost::vertex_type, graph, v);

	std::ostringstream l;

	l << n << "\\n" << b << "\\n" << t << "\\n" << w << "\\n[" << a << "]";

	out << "[label=";

#if BOOST_VERSION >= 104400
	out << boost::escape_dot_string (l.str());
#else
	out << "\"" << l.str() << "\"";
#endif
	if (vt & VERTEX_TYPE_LOOP)
	  {
	    if (vt & VERTEX_TYPE_FIXED)
	      out << ",color=\"red\"";
	    else
	      out << ",color=\"blue\"";
	    if (vt & VERTEX_TYPE_SET)
	      out << ",style=filled,fillcolor=\"plum\"";
	  }
	else if (vt & VERTEX_TYPE_TERMINATOR)
	  out << ",shape=\"box\"";
	out << "]";
      }

    private:
      const G& graph;
    };

    template <class G>
    class edge_writer {
    public:
      edge_writer(const G& g) : graph(g) {}
      template <class E>
      void operator()(std::ostream& out, const E& e) const {
	InsnCycle b = boost::get(boost::edge_best_cycle, graph, e);
	InsnCycle t = boost::get(boost::edge_typical_cycle, graph, e);
	InsnCycle w = boost::get(boost::edge_worst_cycle, graph, e);
	int et = boost::get (boost::edge_type, graph, e);

	std::ostringstream l;

	l << b << "\\n" << t << "\\n" << w;

	out << "[label=";

#if BOOST_VERSION >= 104400
	out << boost::escape_dot_string (l.str());
#else
	out << "\"" << l.str() << "\"";
#endif

	if (et & EDGE_TYPE_BACKEDGE)
	  out << ",style=dashed,color=red";
	else if (et & EDGE_TYPE_DUMMY)
	  out << ",style=dashed";
	else if (et & EDGE_TYPE_BEST)
	  out << ",color=green";
	else if (et & EDGE_TYPE_WORST)
	  out << ",color=blue";

	out << "]";
      }

    private:
      const G& graph;
    };

    template <class G>
    class graph_writer {
    public:
      graph_writer(const G& g) : graph (g) {}
      void operator()(std::ostream& out) const {
	const std::string&
	  n = boost::get_property(graph, boost::graph_name);

	out << "graph [rankdir=LR";
	if (n.length())
	  {
	    out << ",label=";
#if BOOST_VERSION >= 104400
	    out << boost::escape_dot_string (n);
#else
	    out << "\"" << n << "\"";
#endif
	  }
	out << "];\n";
      }
    private:
      const G& graph;
    };

    template <class G> inline label_writer<G>
    my_make_label_writer(G& g) { return label_writer<G>(g); }

    template <class G> inline edge_writer<G>
    my_make_edge_writer(G& g) { return edge_writer<G>(g); }


    template <class G> inline graph_writer<G>
    my_make_graph_writer(G& g) { return graph_writer<G>(g); }

    void
    BLXMLPerf::correct_for_dot (Graph& g, const llvm::Function& f,
				const WrapBasicBlock* h,
				const VertexPairList& d)
    {
      std::string fn (f.getName().str() + "()");

      if (h)
	fn += ":" + h->name();

      boost::get_property (g, boost::graph_name) = fn;

      for (VertexPairList::const_iterator i (d.begin()); i != d.end(); i++)
	{
	  Vertex u = i->first;
	  Vertex v = i->second;

	  InsnCycle b = boost::get(boost::vertex_best_cycle, g, u);
	  InsnCycle t = boost::get(boost::vertex_typical_cycle, g, u);
	  InsnCycle w = boost::get(boost::vertex_worst_cycle, g, u);

	  Edge e = boost::add_edge (u, v, g).first;
	  boost::put (boost::edge_best_cycle, g, e, b);
	  boost::put (boost::edge_typical_cycle, g, e, t);
	  boost::put (boost::edge_worst_cycle, g, e, w);
	  boost::put (boost::edge_type, g, e, EDGE_TYPE_BACKEDGE);
	}
    }

    void
    BLXMLPerf::output_dot (Graph& g,
			   const std::string& base, int seq)
    {
      std::ostringstream oss;
      if (seq == -1)
	oss << base << ".dot";
      else
	oss << base << "-" << seq << ".dot";
#ifdef LLVM_DEBUG
      llvm::outs() << "-- output dot file" <<  oss.str() << "\n";
#endif
      std::ofstream out (oss.str().c_str());
      if (!out)
	{
	  std::cerr << oss.str() << ": " << strerror (errno) << std::endl;
	  return;
	}

      boost::write_graphviz(out, g,
			    my_make_label_writer(g),
			    my_make_edge_writer(g),
			    my_make_graph_writer(g));
    }

    void
    BLXMLPerf::tsort_callflow (size_t vsize,
			       TSortEdges& edges, TSortResult& result)
    {
      boost::adjacency_list < boost::vecS,
			      boost::vecS,
			      boost::directedS >
	g (edges.begin (), edges.end (), vsize);

      boost::topological_sort (g, std::back_inserter (result));
    }
  }
}
