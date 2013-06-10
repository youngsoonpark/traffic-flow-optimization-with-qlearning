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

  // Update the roads leading into a sink
  for (auto sink_it = sinks.begin(); sink_it != sinks.end(); sink_it++) {
    // For each sink. Grab all the edges going to it.
    auto edges_to_sink = graph->get_edges_to(*sink_it);
    for (auto edge_it = edges_to_sink.begin(); edge_it != edges_to_sink.end(); edge_it++) {
      //std::cout << "Sinking Edge: " << edge_it->uid << std::endl;
      
      // If there is a car in the last position, remove it
      if (!edge_it->cars.empty()) {
        //std::cout << "front car position: " << edge_it->cars.back().position << std::endl;
        if (edge_it->cars.back().position == 0)
        {
          edge_it->cars.pop_back();
        }
      }
      
      // Go through the cars in the road backwards, moving them forward if possible
      int prev_car_pos = -1;
      for (auto car_it = edge_it->cars.rbegin(); car_it != edge_it->cars.rend(); car_it++)
      {
        if (car_it->position > static_cast<unsigned int>(prev_car_pos + 1)) // There is room to move forward
        {
          car_it->position--;
        }
        prev_car_pos = car_it->position;
      }
      graph->update_edge(*edge_it);
    }
  }



  // Update the intersections.
  for (auto inter_it = intersections.begin(); inter_it != intersections.end(); inter_it++) {
    //std::cout << "Updating Edge: " << inter_it->uid << std::endl;
    // Grab the roads going to the intersection.
    auto edges_to_intersection = graph->get_edges_to(*inter_it);
    
    // Grab the roads coming from the intersection.
    auto edges_from_intersection = graph->get_edges_from(*inter_it);
    
    // Map intersections coming in to intersections going out.
    
    // Iterate over all the edges coming too and attach them to edges going out.
    auto to_it = edges_from_intersection.begin();
    
    for (auto from_it = edges_to_intersection.begin(); from_it != edges_to_intersection.end(); from_it++) {
    
      // Cycle the from iterator, so if one is less, that is fine.
      if (to_it == edges_from_intersection.end()) to_it = edges_from_intersection.begin();

      // Only update in the valid direction.
      int road_id = (char)*from_it->uid.rbegin() - '0';
      if ( (road_id == 0 && state.getLights() == core::State::Lights::HORIZONTAL)
          || (road_id == 1 && state.getLights() == core::State::Lights::VERTICAL)) {
        //std::cout << "Intersection Open road id: " << road_id << std::endl;
        
        // Check that the destination road has room for another car
        if (to_it->cars.empty() || to_it->cars.front().position < to_it->capacity - 1)
        {
          // Check if there is a car waiting to go through the lights
          if (!from_it->cars.empty() && from_it->cars.back().position == 0)
          {
            // Remove the last car.
            core::Car car_to_move = from_it->cars.back();
            car_to_move.position = to_it->capacity - 1;
            from_it->cars.pop_back();
            // Push it to the head of the other lane.
            to_it->cars.push_front(car_to_move);
            // Update the edges
            graph->update_edge(*to_it);
            graph->update_edge(*from_it);
          }
        }
      }
      else
      {
        //std::cout << "Intersection Closed road id: " << road_id << std::endl;
      }
      // Increment the from iterator.
      to_it++;
    }
  }

  // Update the roads leading into an intersection
  for (auto inter_it = intersections.begin(); inter_it != intersections.end(); inter_it++) {
    // For each intersection. Grab all the edges going to it.
    auto edges_to_inter = graph->get_edges_to(*inter_it);
    for (auto edge_it = edges_to_inter.begin(); edge_it != edges_to_inter.end(); edge_it++) {
      //std::cout << "Updating Intersection For Edge: " << edge_it->uid << std::endl;
      
      // Go through the cars in the road backwards, moving them forward if possible
      int prev_car_pos = -1;
      for (auto car_it = edge_it->cars.rbegin(); car_it != edge_it->cars.rend(); car_it++)
      {
        if (car_it->position > static_cast<unsigned int>(prev_car_pos + 1)) // There is room to move forward
        {
          /*std::cout << "Moving car " << car_it->hash << " forward from position " <<
          car_it->position;*/
          car_it->position--;
          //std::cout << " to position " << car_it->position << std::endl;
        }
        prev_car_pos = car_it->position;
      }
      graph->update_edge(*edge_it);
    }
  }

  // Update the sources.
  for (auto source_it = sources.begin(); source_it != sources.end(); source_it++) {
    // Grab all the edges going from the source.
    auto edges_from_source = graph->get_edges_from(*source_it);
    for (auto edge_it = edges_from_source.begin(); edge_it != edges_from_source.end(); edge_it++) {
      //std::cout << "Updating Source For Edge: " << edge_it->uid << std::endl;
      
      // Check if it is possible to push a car
      if (edge_it->cars.empty() || edge_it->cars.front().position < edge_it->capacity - 1)
      {
        int car_placement_probability = rand() % 101;
        // Update the edge.
        if ((source_it == sources.begin() && car_placement_probability >= 70) ||
            (source_it != sources.begin() && car_placement_probability >= 20)) {
          //std::cout << "Not adding new car" << std::endl;
        } else {
          //std::cout << "Adding new car" << std::endl;
          edge_it->cars.push_front(core::Car(1, 0, edge_it->capacity - 1));
          graph->update_edge(*edge_it);
        }
      }
      else // It is impossible to push a car
      {
        //std::cout << "cars size == " << edge_it->cars.size();
        //std::cout << ", road capacity == " << edge_it->capacity;
        //std::cout << ", first car position == " << edge_it->cars.front().position;
        //std::cout << std::endl;
        
        //std::cout << "Traffic Jam At: " << edge_it->uid << std::endl;
      }
    }
  }
}

} // End of namespace sim.
} // End of namespace road.
