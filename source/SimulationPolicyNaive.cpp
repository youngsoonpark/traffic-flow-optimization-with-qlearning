#include "Graph.hpp"
#include "SimulationPolicyNaive.hpp"
#include "State.hpp"
#include <cstdlib>
#include <list>

namespace road {
    namespace sim {

        void SimulationPolicyNaive::update(core::State& state) {
            int chanceToGetCar = rand() % 101; // 101 so our range is [0, 100]
            int road = rand() % 2;             // Value meaning is dependent on State::Lights
            core::Graph* g = state.getGraph();

            core::Car tempCar(1, 1.0, false);    // This is a temporary location for our car
            std::string tempName; // This is the source's uid that makes it different from all other sources
            std::list<core::Vertex> verticiesSource = g->get_vertices(core::Vertex::Type::SOURCE);
            std::list<core::Vertex> verticiesSink = g->get_vertices(core::Vertex::Type::SINK);
            std::list<core::Vertex> intersection = g->get_vertices(core::Vertex::Type::INTERSECTION);
            core::Edge from, to;

            // move all cars up by 1
            for (std::list<core::Vertex>::iterator it = verticiesSource.begin(); it != verticiesSource.end(); ++it)
            {
                // Naive sources should only ever have one edge, so we can assume it is the back one
                from = g->get_edges_from(*it).back();

                // Get the only bit of source uid we can use to evaluate the sink
                tempName = it->uid.substr(sizeof("source-"), std::string::npos);

                // Get the car and then take it off the road
                tempCar = from.cars.front();
                from.cars.pop_front();

                if (tempCar.no_car == false)
                {
                    --from.actual_cars;
                }

                // Re-add the edge to the graph (this is only a copy of it!)
                g->add_edge(*it, intersection.back(), from);

                // Now find the corresponding sink
                for (std::list<core::Vertex>::iterator jt = verticiesSink.begin(); jt != verticiesSink.end(); ++jt)
                {
                    // We only care about the one that has the same uid as the source...
                    if (jt->uid.substr(sizeof("sink-"), std::string::npos) == tempName)
                    {
                        // Again, Naive model, so we assume that there's only one edge leading to it
                        to = g->get_edges_to(*jt).back();

                        // Take the front car away and add the car we just took from the source
                        to.cars.pop_front();
                        to.cars.push_back(tempCar);

                        if (tempCar.no_car == false)
                        {
                            ++to.actual_cars;
                        }

                        // Re-add the edge to the graph (this is only a copy of it!)
                        g->add_edge(intersection.back(), *jt, to);

                        // Remove this from the list since we don't need it anymore :)
                        //verticiesSink.remove(jt); <== obviously this isn't working atm; either going to find a different solution or just ignore altogether :(
                        break;
                    }
                }
            }

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************
 *******************************************************************************
 *******************************************************************************
 *******************************************************************************
 *******************************************************************************
 *******************************************************************************



 Don't go any further.... shitty code that needs to be updated!




            // There is a 45% chance that a car will enter a lane
            if (chanceToGetCar >= 45)
            {
                std::string edgeTrait = (road == State::Lights::Horisontal) ? "HorisontalSource" : "VerticalSource";

                for (core::State::graph_traits::vertex_iterator vit = g->begin(); vit != g->end(); ++g)
                {
                    if (vit->getTrait() == edgeTrait && vit->somehowyouaccesstheedge.cars.size() <= state.maxCars())
                    {
                        vit->somehowyouaccesstheedge.cars.push_back(Car()vit->somehowyouaccesstheedge.speedlimit, 1.0));
                    }
                }
            }*/
        }
    } // End of namespace sim.
} // End of namespace road.
