#include "Graph.hpp"
#include "SimulationPolicyNaive.hpp"
#include "State.hpp"
#include <cstdlib>
#include <list>
#include <iostream>

namespace road
{
namespace sim
{


void SimulationPolicyNaive::update(core::State& state)
{
  // Retrieve the graph.
  auto graph = state.getGraph();
  
  // Grab the sources, sinks and intersections.
  auto sources = graph->get_vertices(core::Vertex::Type::SOURCE);
  auto sinks = graph->get_vertices(core::Vertex::Type::SINK);
  auto intersections = graph->get_vertices(core::Vertex::Type::INTERSECTION);

  // Calculate the probablity of a car being placed.
  int car_placement_probability = rand() % 101;

  // Check all edges are full. If not, push till they reach their full capacity.
  auto edges = graph->get_edges();
  for (auto edge_it = edges.begin(); edge_it != edges.end(); edge_it++) {
    while (edge_it->cars.size() < graph->get_edge_capacity(*edge_it)) {
      edge_it->cars.push_back(core::Car::empty_car());
    }
    // Update the graph.
    graph->update_edge(*edge_it);
  } 
  
  // Update the sinks.
  for (auto sink_it = sinks.begin(); sink_it != sinks.end(); sink_it++) {
    // For each sink. Grab all the edges going to it.
    auto edges_to_sink = graph->get_edges_to(*sink_it);
    for (auto edge_it = edges_to_sink.begin(); edge_it != edges_to_sink.end(); edge_it++) {
      // For each edge going to the sink, remove the last element, if it is not empty.
      if (!edge_it->cars.empty()) { 
        edge_it->cars.pop_back();
        graph->update_edge(*edge_it);
     }
    }
  }
  
  // Update the sources.
  for (auto source_it = sources.begin(); source_it != sources.end(); source_it++) {
    // Grab all the edges going from the source.
    auto edges_from_source = graph->get_edges_from(*source_it);
    for (auto edge_it = edges_from_source.begin(); edge_it != edges_from_source.end(); edge_it++) {
      if (car_placement_probability > 45) {
        edge_it->cars.push_front(core::Car::empty_car());
      } else {
        edge_it->cars.push_front(core::Car(1, 0, true));
      }
      graph->update_edge(*edge_it);
    }
  }

  // Update the intersections.
  for (auto inter_it = intersections.begin(); inter_it != intersections.end(); inter_it++) {
    // Grab the roads going to the intersection.
    auto edges_to_intersection = graph->get_edges_to(*inter_it);
    // Grab the roads coming from the intersection.
    auto edges_from_intersection = graph->get_edges_from(*inter_it);
    // Map intersections coming in to intersections going out.
    // Iterate over all the edges coming too and attach them to edges going out.
    auto from_it = edges_from_intersection.begin();
    for (auto to_it = edges_to_intersection.begin(); to_it != edges_to_intersection.end(); to_it++) {
      // Cycle the from iterator, so if one is less, that is fine.
      if (from_it == edges_from_intersection.end()) from_it = edges_from_intersection.begin();

      // If we have a valid place to pop from.
      if (!to_it->cars.empty()) {
        // Remove the last car.
        core::Car car_to_move = to_it->cars.back();
        to_it->cars.pop_back();
        // Push it to the head of the other lane.
        from_it->cars.push_front(car_to_move);
        // Update the edges
        graph->update_edge(*to_it);
        graph->update_edge(*from_it);
      }
    }
  }
}

} // End of namespace sim.
} // End of namespace road.
