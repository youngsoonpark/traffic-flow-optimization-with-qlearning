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
    auto capacity = graph->get_edge_capacity(*edge_it);
    std::cout << "Edge " << edge_it->uid << " C: " << capacity << std::endl;
    while (edge_it->cars.size() < capacity) {
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
      std::cout << "Updating Sink For Edge: " << edge_it->uid << std::endl;
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
      std::cout << "Updating Source For Edge: " << edge_it->uid << std::endl;

      // Check for a traffic jam
      if (edge_it->cars.size() >= graph->get_edge_capacity(*edge_it)) {
        // Create a functor that returns whether the car exists.
        struct is_car {
          bool operator()(const core::Car& car) {
            return car.no_car;
          }
        };
        // Find the first is_car.
        auto empty_car_it = std::find_if(edge_it->cars.rbegin(), edge_it->cars.rend(), is_car());
        // The lane is totally full.
        if (empty_car_it == edge_it->cars.rend()) {
          std::cout << "Traffic Jam At: " << edge_it->uid << std::endl;
          continue;
        } else {
          // Just delete the empty spot.
          edge_it->cars.erase((empty_car_it + 1).base());
        }
      }

      // Update the edge.
      if (car_placement_probability >= 15) {
        std::cout << "Not adding new car" << std::endl;
        edge_it->cars.push_front(core::Car::empty_car());
      } else {
        std::cout << "Not adding new car" << std::endl;
        edge_it->cars.push_front(core::Car(1, 0, false));
      }
      graph->update_edge(*edge_it);
    }
  }

  // Update the intersections.
  for (auto inter_it = intersections.begin(); inter_it != intersections.end(); inter_it++) {
    std::cout << "Updating Edge: " << inter_it->uid << std::endl;
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

      // Only update in the valid direction.
      int road_id = (char)*from_it->uid.rbegin() - '0';
      std::cout << "Intersection Open road id: " << road_id << std::endl;;
      if ( (road_id == 2 && state.getLights() == core::State::Lights::HORIZONTAL)
          || (road_id == 3 && state.getLights() == core::State::Lights::VERTICAL)) {
        // If we have a valid place to pop from.
        if (!to_it->cars.empty() ) {
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
      // Increment the from iterator.
      from_it++;
    }
  }
}

} // End of namespace sim.
} // End of namespace road.
