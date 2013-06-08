#ifndef ROAD_CORE_GRAPH_H_
#define ROAD_CORE_GRAPH_H_

#include <string>
#include <list>
#include <deque>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>

namespace road {
namespace core {

/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @description defines a vertex that can be of 3 types
 *  {INTERSECTION, SOURCE, SINK}
 */
struct Vertex {
  // Type of vertex.
  enum Type {
    INTERSECTION,
    SOURCE,
    SINK
  } type;
  std::string uid; // Custom uid, which is useful for serialization.
  int x; // Defines an x position.
  int y; // defines a y position.

  Vertex() {}
  Vertex(Type type, std::string uid, int x, int y) :
    type(type), uid(uid), x(x), y(y) {}
};


/**
 * @author Christopher Di Bella <chrisdb@cse.unsw.edu.au>
 * @description A car represents an object on the road.
 * The badboy factor represents how likely they are to speed
 * or run a red light.
 */
struct Car {
  std::size_t speed;
  double badboy_factor;
  bool no_car;

  // Constructs the car.
  Car(size_t speed, double badboy_factor, bool no_car) :
    speed(speed), badboy_factor(badboy_factor), no_car(no_car) {}
};

/**
 * @author Benjamin James Wright <bwright@cse.unsw.eu.au> 
 * @description Defines an edge in the graph, in our case this defines
 *  a road, which has a speed limit, a capacity and a deque of cars.
 */
struct Edge {
  std::string uid;         // What we use to access.          
  std::string src;   // Makes indexing easier.
  std::string dest;  // Makes indexing easier.

  std::size_t speed_limit; // Defines the speed limit for the edge.
  std::deque<Car> cars; // Cars on the road.
  std::size_t actual_cars;

  // Constructor.
  Edge() {}
  Edge(std::string uid, std::size_t speed_limit)
    : uid(uid), speed_limit(speed_limit) {}
};


/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @description Adaptor around the boost graph because it is needlessly
 *  complex for what we want to actually do with it. This simpler interface
 *  is designed to make everything a lot simpler to work with.
 */
class Graph {
  public:
  // Create a typedef of the type we are using to store our graph.
  typedef boost::labeled_graph< 
            boost::adjacency_list<boost::listS, boost::vecS,
            boost::bidirectionalS, Vertex, Edge>,
            std::string
          > graph_t;
  // Create a typedef for graph traits
  typedef typename boost::graph_traits<graph_t> graph_traits;
  typedef typename graph_traits::vertex_descriptor vertex_t;
  typedef typename graph_traits::edge_descriptor edge_t;
  typedef typename graph_traits::vertex_iterator vertex_iterator;
  typedef typename graph_traits::edge_iterator edge_iterator;

    /**
     * @description Constructs the default graph.
     */
    Graph() {}

    /**
     * @description returns a vertex, given its unique identifier.
     */
    Vertex get_vertex(std::string uid) {
      Vertex new_vertex;
      new_vertex.type = m_graph[uid].type;
      new_vertex.uid = m_graph[uid].uid;
      new_vertex.x = m_graph[uid].x;
      new_vertex.y = m_graph[uid].y;
      return new_vertex;
    }

    /**
     * @description add a vertex, to the graph.
     */
    void add_vertex(Vertex& to_add) {
      boost::add_vertex(to_add.uid, m_graph);
      // Add the properties.
      m_graph[to_add.uid].type = to_add.type;
      m_graph[to_add.uid].uid = to_add.uid;
      m_graph[to_add.uid].x = to_add.x;
      m_graph[to_add.uid].y = to_add.y;
    }

    /**
     * @description adds an edge with custom defined user data.
     */

    
    void add_edge(std::string src, std::string dest, Edge& edge) {
      edge_t new_edge;
      bool b;
      // Create the tie
      boost::tie(new_edge, b) = boost::add_edge_by_label(src, dest, m_graph);
      // Add the properties, to the graph
      m_graph.graph()[new_edge].speed_limit = edge.speed_limit;
      m_graph.graph()[new_edge].uid = edge.uid;
      m_graph.graph()[new_edge].src = src;
      m_graph.graph()[new_edge].dest = dest;
      m_graph.graph()[new_edge].cars = edge.cars;
      m_graph.graph()[new_edge].actual_cars = edge.actual_cars;
    }

    void add_edge(Vertex& src, Vertex& dest, Edge& edge) {
      // Use the above, just retrieve the uid.
      add_edge(src.uid, dest.uid, edge);
    }
    
    /**
     * @description retrieves all the verticies
     */
    std::list<Vertex> get_vertices() {
      std::list<Vertex> vertices;
      std::pair<vertex_iterator, vertex_iterator> it;

      for (it = boost::vertices(m_graph); it.first != it.second; it.first++) {
        // Retrieve the vertex
        Vertex to_add;
        // Add the properties to the vertex.
        to_add.type = m_graph.graph()[*it.first].type;
        to_add.x = m_graph.graph()[*it.first].x;
        to_add.y = m_graph.graph()[*it.first].y;
        to_add.uid = m_graph.graph()[*it.first].uid;
        // Push the copy of the vertex.
        vertices.push_back(to_add);
      }
      return vertices;
    }
    
    /**
     * @description retrieves all the verticies of a given type.
     **/
    std::list<Vertex> get_vertices(typename Vertex::Type type) {
      // Define a nested predicate functor.
      struct is_type{
        typename Vertex::Type type;
        is_type(typename Vertex::Type type) : type(type) {}
        bool operator()(const Vertex& vertex) {
          return vertex.type != type;
        }
      };
      // Retrieve the list of verticies.
      std::list<Vertex> vertices = get_vertices();
      // Remove the ones we don't want.
      vertices.remove_if(is_type(type));
      // Return the list.
      return vertices;
    }
    
    /**
     * @description retrieves all the edges in the graph.
     */
    std::list<Edge> get_edges() {
      std::list<Edge> edges;
      std::pair<edge_iterator, edge_iterator> it;
      // Iterate over the edges.
      for (it = boost::edges(m_graph); it.first != it.second; it.first++) {
        // Set the edge we want to add.
        Edge to_add;
        // Retrieve the source and destination.
        vertex_t source = boost::source(*it.first, m_graph);
        vertex_t destination = boost::target(*it.first, m_graph);
        // Add properties to the edge.
        // Add internal representation to the edge. 
        to_add.uid = m_graph.graph()[*it.first].uid;
        to_add.speed_limit = m_graph.graph()[*it.first].speed_limit;
        to_add.cars = m_graph.graph()[*it.first].cars;
        to_add.actual_cars = m_graph.graph()[*it.first].actual_cars;
        to_add.src = m_graph.graph()[source].uid;
        to_add.dest = m_graph.graph()[destination].uid;
        // Push the edge back.
        edges.push_back(to_add);
      }
      // Returns the edges.
      return edges;
    }

    /**
     * @description get all the edges attached to a vertex, in both ways. This is pretty
     *  inefficient but for what we are simulating it ismore that sufficient.
     */
    std::list<Edge> get_edges(Vertex& vertex) {
      // Create the predicate functor, todo make efficent.
      struct has_vertex {
        Vertex& vertex;
        has_vertex(Vertex& vertex) : vertex(vertex) {}
        bool operator()(const Edge& edge) {
          return edge.src != vertex.uid && edge.dest != vertex.uid;
        }
      };
      // Retrieve the edges.
      std::list<Edge> edges = get_edges();
      // Remove the components of the edge we don't.
      edges.remove_if(has_vertex(vertex));
      // Rteturn the edge.
      return edges;
    }

    /**
     * @description retrieves all the edges that come from the vertex.
     */ 
    std::list<Edge> get_edges_from(Vertex& vertex) {
      // Create the predicate functor, todo make efficient.
      struct has_vertex { 
        Vertex& vertex;
        has_vertex(Vertex& vertex) : vertex(vertex) {}
        bool operator()(const Edge& edge) {
          return edge.src != vertex.uid;
        }
      };
      // Retrieve the edges.
      std::list<Edge> edges = get_edges();
      // Remove the components of the edge we don't.
      edges.remove_if(has_vertex(vertex));
      // Rteturn the edge.
      return edges;
    }

    /**
     * @description retrievs all the edges that come to the vertex.
     */
    std::list<Edge> get_edges_to(Vertex vertex) {
      // Create the predicate functor, todo make efficient.
      struct has_vertex {
        Vertex& vertex;
        has_vertex(Vertex& vertex) : vertex(vertex) {}
        bool operator()(const Edge& edge) {
          return edge.dest != vertex.uid;
        }
      };
      // Retrieve the edges.
      std::list<Edge> edges = get_edges();
      // Remove the components of the edge we don't.
      edges.remove_if(has_vertex(vertex));
      // Rteturn the edge.
      return edges;
    }

  private:
    graph_t m_graph; // Graph member function.
};

} // End of namespace core.
} // End of namespace graph.

#endif // ROAD_CORE_GRAPH_H_
