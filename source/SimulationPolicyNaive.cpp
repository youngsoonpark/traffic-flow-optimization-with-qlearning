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
  core::Graph* graph = state.getGraph(); // Retrieve the graph.

  core::Car tmp_car(1, 1.0, false);    // This is a temporary location for our car
  std::string tmp_uid;                // This is the source's uid that makes source_it different from all other sources
  
  auto verts_source = graph->get_vertices(core::Vertex::Type::SOURCE);
  auto verts_sinks = graph->get_vertices(core::Vertex::Type::SINK);
  auto intersection = graph->get_vertices(core::Vertex::Type::INTERSECTION);

  // Edges from and to.
  core::Edge from, to;

  // move all cars up by 1
  for (auto source_it = verts_source.begin(); source_it != verts_source.end(); source_it++) {
    // Naive sources should only ever have one edge, so we can assume source_it is the back one
    from = graph->get_edges_from(*source_it).back();
    // Get the only bsource_it of source uid we can use to evaluate the sink
    tmp_uid = source_it->uid.substr(sizeof("source-"), std::string::npos);
    // If there are no cars, just continue.
    if (from.cars.empty()) {
      continue;
    }
    // Get the car and then take source_it off the road
    tmp_car = from.cars.front();
    from.cars.pop_front();

    // Decrement our counter each time we don't see a real car.
    if (!tmp_car.no_car) from.actual_cars--;
    std::cout << "SimualtionPolicyNaive: Updating the road...." << std::endl;
    // Re-add the edge to the graph (this is only a copy of source_it!)
    graph->update_edge(from);

    // Now find the corresponding sink
    for (auto sink_source_it = verts_sinks.begin(); sink_source_it != verts_sinks.end(); ++sink_source_it) {
      // We only care about the one that has the same uid as the source...
      if (sink_source_it->uid.substr(sizeof("sink-"), std::string::npos) == tmp_uid) {
        // Again, Naive model, so we assume that there's only one edge leading to source_it
        to = graph->get_edges_to(*sink_source_it).back();
        // Take the front car away and add the car we just took from the source
        to.cars.pop_front();
        to.cars.push_back(tmp_car);
        if (tmp_car.no_car == false) to.actual_cars++;
        // Re-add the edge to the graph (this is only a copy of source_it!)
        graph->update_edge(to);
        // Remove this from the list since we don't need source_it anymore :)
        //verts_sinks.remove(sink_source_it); <== obviously this isn't working atm; esource_ither going to find a different solution or just ignore altogether :(
        break;
      }
    }
  }

  std::cout << "SimulationPolicyNaive: Adding cars to the " << verts_source.size()  << " road " << std::endl;
  for (auto source_it = verts_source.begin(); source_it != verts_source.end(); ++source_it) {
    // Retrieve the edge attached to the source.
    from = graph->get_edges_from(*source_it).back();
    // Randmly add a car to the state.
    if (chanceToGetCar < 45) {
      std::cout << " -> SimulationPolicyNaive: Car Not Added To " << from.uid << std::endl;
      from.cars.push_back(core::Car(1, 1.0, false));
    } else {
      std::cout << "-> SimulationPolicyNaive: Car Added Added To " << from.uid << std::endl;
      from.cars.push_back(core::Car(1, 1.0, true));
    }
    graph->update_edge(from);
  }
  std::cout << "SimulationPolicyNaive: Ending Simulation" << std::endl;

}

} // End of namespace sim.
} // End of namespace road.
