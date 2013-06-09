#include "Graph.hpp"

namespace road
{
namespace core
{

Graph::Graph() {}

Vertex Graph::get_vertex(std::string uid) {
  Vertex new_vertex;
  new_vertex.type = m_graph[uid].type;
  new_vertex.uid = m_graph[uid].uid;
  new_vertex.x = m_graph[uid].x;
  new_vertex.y = m_graph[uid].y;
  return new_vertex;
}

void Graph::add_vertex(Vertex& to_add) {
  boost::add_vertex(to_add.uid, m_graph);
  // Add the properties.
  m_graph[to_add.uid].type = to_add.type;
  m_graph[to_add.uid].uid = to_add.uid;
  m_graph[to_add.uid].x = to_add.x;
  m_graph[to_add.uid].y = to_add.y;
}

void Graph::add_edge(std::string src, std::string dest, Edge& edge) {
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

void Graph::add_edge(Vertex& src, Vertex& dest, Edge& edge) {
  // Use the above, just retrieve the uid.
  add_edge(src.uid, dest.uid, edge);
}

std::list<Vertex> Graph::get_vertices() {
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

std::list<Vertex> Graph::get_vertices(typename Vertex::Type type) {
  // Define a nested predicate functor.
  struct is_type {
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

std::list<Edge> Graph::get_edges() {
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

std::list<Edge> Graph::get_edges(Vertex& vertex) {
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

std::list<Edge> Graph::get_edges_from(Vertex& vertex) {
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

std::list<Edge> Graph::get_edges_to(Vertex vertex) {
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

void Graph::update_edge(const Edge& edge) {
  std::pair<edge_iterator, edge_iterator> it;
  // Iterate over the edges.
  for (it = boost::edges(m_graph); it.first != it.second; it.first++) {
    if (m_graph.graph()[*it.first].uid == edge.uid) {
      m_graph.graph()[*it.first].speed_limit = edge.speed_limit;
      m_graph.graph()[*it.first].cars.assign(edge.cars.begin(), edge.cars.end());
      m_graph.graph()[*it.first].actual_cars = edge.actual_cars;
      break;
    }
  }
}

} // End of namespace core.
} // End of namespace graph.

