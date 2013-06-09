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
  // Debug message.
  std::cout << "SimulationPolicyNaive: Entering Simulaton" << std::endl;
  int chanceToGetCar = rand() % 101; // 101 so our range is [0, 100]
  core::Graph* g = state.getGraph(); // Retrieve the graph.

  core::Car tempCar(1, 1.0, false);    // This is a temporary location for our car
  std::string tempName;                // This is the source's uid that makes it different from all other sources
  
  auto verts_source = g->get_vertices(core::Vertex::Type::SOURCE);
  auto verts_sinks = g->get_vertices(core::Vertex::Type::SINK);
  auto intersection = g->get_vertices(core::Vertex::Type::INTERSECTION);
  // Edges from and to.
  core::Edge from, to;

  // move all cars up by 1
  for (auto it = verts_source.begin(); it != verts_source.end(); it++) {
    // Naive sources should only ever have one edge, so we can assume it is the back one
    from = g->get_edges_from(*it).back();
    // Get the only bit of source uid we can use to evaluate the sink
    tempName = it->uid.substr(sizeof("source-"), std::string::npos);
    // If there are no cars, just continue.
    if (from.cars.empty()) {
      continue;
    }
    // Get the car and then take it off the road
    tempCar = from.cars.front();
    from.cars.pop_front();

    // Decrement our counter each time we don't see a real car.
    if (!tempCar.no_car) from.actual_cars--;
    std::cout << "SimualtionPolicyNaive: Updating the road...." << std::endl;
    // Re-add the edge to the graph (this is only a copy of it!)
    g->update_edge(from);

    // Now find the corresponding sink
    for (auto jt = verts_sinks.begin(); jt != verts_sinks.end(); ++jt) {
      // We only care about the one that has the same uid as the source...
      if (jt->uid.substr(sizeof("sink-"), std::string::npos) == tempName) {
        // Again, Naive model, so we assume that there's only one edge leading to it
        to = g->get_edges_to(*jt).back();
        // Take the front car away and add the car we just took from the source
        to.cars.pop_front();
        to.cars.push_back(tempCar);
        if (tempCar.no_car == false) to.actual_cars++;
        // Re-add the edge to the graph (this is only a copy of it!)
        g->update_edge(to);
        // Remove this from the list since we don't need it anymore :)
        //verts_sinks.remove(jt); <== obviously this isn't working atm; either going to find a different solution or just ignore altogether :(
        break;
      }
    }
  }

  std::cout << "SimulationPolicyNaive: Adding cars to the " << verts_source.size()  << " road " << std::endl;
  for (auto it = verts_source.begin(); it != verts_source.end(); ++it) {
    // Retrieve the edge attached to the source.
    from = g->get_edges_from(*it).back();
    // Randmly add a car to the state.
    if (chanceToGetCar < 45) {
      std::cout << " -> SimulationPolicyNaive: Car Not Added To " << from.uid << std::endl;
      from.cars.push_back(core::Car(1, 1.0, false));
    } else {
      std::cout << "-> SimulationPolicyNaive: Car Added Added To " << from.uid << std::endl;
      from.cars.push_back(core::Car(1, 1.0, true));
    }
    g->update_edge(from);
  }
  std::cout << "SimulationPolicyNaive: Ending Simulation" << std::endl;

}

} // End of namespace sim.
} // End of namespace road.
