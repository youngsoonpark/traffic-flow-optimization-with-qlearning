#ifndef ROAD_CORE_GRAPH_H_
#define ROAD_CORE_GRAPH_H_

#include <cstdlib>
#include <string>
#include <list>
#include <deque>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>

namespace road
{
namespace core
{

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

// Used for internal referenes
static unsigned int car_count;

/**
 * @author Christopher Di Bella <chrisdb@cse.unsw.edu.au>
 * @description A car represents an object on the road.
 * The badboy factor represents how likely they are to speed
 * or run a red light.
 */
struct Car {
  std::size_t speed;
  double badboy_factor;
  unsigned int position; // position in the road, 0 is the next car to be popped
  unsigned int hash;

  // Constructs the car.
  Car(size_t speed, double badboy_factor, unsigned int position) :
    speed(speed), badboy_factor(badboy_factor), position(position) {
      hash = car_count++; // Reference counter / hash
  }

};

/**
 * @author Benjamin James Wright <bwright@cse.unsw.eu.au>
 * @description Defines an edge in the graph, in our case this defines
 *  a road, which has a speed limit, a capacity and a deque of cars.
 */
struct Edge {
  typedef std::deque<Car> Container;
  std::string uid;   // What we use to access.
  std::string src;   // Makes indexing easier.
  std::string dest;  // Makes indexing easier.

  std::size_t speed_limit; // Defines the speed limit for the edge.
  std::size_t capacity;    // Defines the maximum number of cars.
  Container cars;          // Cars on the road.
  std::int8_t actual_cars; // Defines the number of actual cars.

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
class Graph
{
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

  // Defines an edge and vertex container.
  typedef std::list<Edge> EdgeContainer;
  typedef std::list<Vertex> VertexContainer;

  /**
   * @description Constructs the default graph.
   */
  Graph();

  /**
   * @description returns a vertex, given its unique identifier.
   */
  Vertex get_vertex(std::string uid);

  /**
   * @description add a vertex, to the graph.
   */
  void add_vertex(Vertex& to_add);

  /**
   * @description adds an edge with custom defined user data.
   */
  void add_edge(std::string src, std::string dest, Edge& edge);
  void add_edge(Vertex& src, Vertex& dest, Edge& edge);

  /**
   * @description retrieves all the verticies
   */
  VertexContainer get_vertices();

  /**
   * @description retrieves all the verticies of a given type.
   **/
  VertexContainer get_vertices(typename Vertex::Type type);

  /**
   * @description retrieves all the edges in the graph.
   */
  EdgeContainer get_edges();

  /**
   * @description get all the edges attached to a vertex, in both ways. This is pretty
   *  inefficient but for what we are simulating it ismore that sufficient.
   */
  EdgeContainer get_edges(Vertex& vertex);

  /**
   * @description retrieves all the edges that come from the vertex.
   */
  EdgeContainer get_edges_from(Vertex& vertex);

  /**
   * @description retrievs all the edges that come to the vertex.
   */
  EdgeContainer get_edges_to(Vertex vertex);

  /**
   * @description provides an interface for updating an edge if it has been changed
   */
  void update_edge(const Edge& edge);

  /**
   * @description retrieves the capacity of an edge.
   */
  std::size_t get_edge_capacity(const Edge& edge);

private:
  graph_t m_graph; // Graph member function.
};

} // End of namespace core.
} // End of namespace graph.

#endif // ROAD_CORE_GRAPH_H_
