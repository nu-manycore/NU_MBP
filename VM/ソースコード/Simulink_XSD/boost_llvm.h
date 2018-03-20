// -*- mode: c++; -*-
#ifndef boost_llvm_h
#define boost_llvm_h

#include <boost/config.hpp>
#ifdef BOOST_NO_EXCEPTIONS
#include <boost/assert.hpp>
namespace boost
{
  template <class E>
  void
  throw_exception (const E& e)
  {
    std::cerr << e.what () << std::endl;
    BOOST_ASSERT(false);
  }
}
#endif

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>
#include <boost/assign/list_of.hpp>

#define VERTEX_TYPE_NORMAL	0
#define VERTEX_TYPE_LOOP	1
#define VERTEX_TYPE_FIXED	2
#define VERTEX_TYPE_SET		4
#define VERTEX_TYPE_TERMINATOR	8

#define EDGE_TYPE_NORMAL	0
#define EDGE_TYPE_DUMMY		1
#define EDGE_TYPE_BACKEDGE	2
#define EDGE_TYPE_BEST		4
#define EDGE_TYPE_WORST		8

namespace boost
{
  /// @brief vertex property for instruction cycle in llvm::BasicBlock
  enum vertex_current_cycle_t { vertex_current_cycle };
  enum vertex_best_cycle_t { vertex_best_cycle };
  enum vertex_typical_cycle_t { vertex_typical_cycle };
  enum vertex_worst_cycle_t { vertex_worst_cycle };

  /// @brief vertex property for pointer to llvm::BasicBlock
  enum vertex_basicblock_t { vertex_basicblock };

  /// @brief average cycle
  enum vertex_average_cycle_t { vertex_average_cycle };

  /// @brief type (0=norm, 1=loop-head, 2=term)
  enum vertex_type_t { vertex_type };

  /// @brief edge property for instruction cycle in llvm::BasicBlock
  enum edge_best_cycle_t { edge_best_cycle };
  enum edge_typical_cycle_t { edge_typical_cycle };
  enum edge_worst_cycle_t { edge_worst_cycle };

  /// @brief edge property for type (0=none, 0x01=best, 0x02=worst, 0x04=back)
  enum edge_type_t { edge_type };

  BOOST_INSTALL_PROPERTY (vertex, current_cycle);
  BOOST_INSTALL_PROPERTY (vertex, best_cycle);
  BOOST_INSTALL_PROPERTY (vertex, typical_cycle);
  BOOST_INSTALL_PROPERTY (vertex, worst_cycle);

  BOOST_INSTALL_PROPERTY (vertex, basicblock);

  BOOST_INSTALL_PROPERTY (vertex, average_cycle);

  BOOST_INSTALL_PROPERTY (vertex, type);

  BOOST_INSTALL_PROPERTY (edge, best_cycle);
  BOOST_INSTALL_PROPERTY (edge, typical_cycle);
  BOOST_INSTALL_PROPERTY (edge, worst_cycle);

  BOOST_INSTALL_PROPERTY (edge, type);
}

namespace Simulink
{
  namespace LLVM
  {
    /// @brief type of cycle
    typedef double InsnCycle;

    /// @brief edge property
    typedef boost::property <
      boost::edge_weight_t,
      InsnCycle,
      boost::property <
	boost::edge_best_cycle_t,
	InsnCycle,
	boost::property <
	  boost::edge_typical_cycle_t,
	  InsnCycle,
	  boost::property <
	    boost::edge_worst_cycle_t,
	    InsnCycle,
	    boost::property <
	      boost::edge_type_t,
	      int > > > > > edge_property;

    /// @brief vertex property
    ///
    /// vertex has instruction cycle each llvm::BasicBlock and pointer
    /// to llvm::BasicBlock
    typedef boost::property <
      boost::vertex_name_t,
      std::string,
      boost::property <
	boost::vertex_current_cycle_t,
	InsnCycle,
	boost::property <
	  boost::vertex_basicblock_t,
	  const llvm::BasicBlock*,
	  boost::property <
	    boost::vertex_best_cycle_t,
	    InsnCycle,
	    boost::property <
	      boost::vertex_typical_cycle_t,
	      InsnCycle,
	      boost::property <
		boost::vertex_worst_cycle_t,
		InsnCycle,
		boost::property <
		  boost::vertex_average_cycle_t,
		  InsnCycle,
		  boost::property <
		    boost::vertex_type_t,
		    int
		    > > > > > > > > vertex_property;

    typedef boost::property <
      boost::graph_name_t, std::string
      > graph_property;

    /// @brief Boost Graph (directed)
    typedef boost::adjacency_list <
      boost::listS,
      boost::vecS,
      boost::bidirectionalS,
      vertex_property,
      edge_property,
      graph_property > Graph;

    /// @brief Boost Graph vertex
    typedef boost::graph_traits < Graph >::vertex_descriptor Vertex;

    /// @brief Boost Graph edge
    typedef boost::graph_traits < Graph >::edge_descriptor Edge;

    typedef boost::graph_traits < Graph >::edge_iterator EdgeIterator;
    typedef boost::graph_traits < Graph >::vertex_iterator VertexIterator;
    typedef boost::graph_traits < Graph >::out_edge_iterator OutEdgeIterator;
    typedef boost::graph_traits < Graph >::in_edge_iterator InEdgeIterator;

    typedef std::pair < Vertex, Vertex > VertexPair;
    typedef std::vector < VertexPair > VertexPairList;
  }
}

#endif
