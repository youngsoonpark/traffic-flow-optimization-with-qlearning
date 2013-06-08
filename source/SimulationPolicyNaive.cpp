#include "Graph.hpp"
#include "SimulationPolicyNaive.hpp"
#include "State.hpp"
#include <cstdlib>
#include <list>

#include <iostream>

namespace road {
    namespace sim {

    typedef std::list<core::Vertex>::iterator vertex_iterator;

        void SimulationPolicyNaive::update(core::State& state) {

            int chanceToGetCar = rand() % 101; // 101 so our range is [0, 100]
            int rd = rand() % 2;             // Value meaning is dependent on State::Lights
            core::Graph* g = state.getGraph();

            core::Car tempCar(1, 1.0, false);    // This is a temporary location for our car
            std::string tempName; // This is the source's uid that makes it different from all other sources
            std::list<core::Vertex> verticiesSource = g->get_vertices(core::Vertex::Type::SOURCE);
            std::list<core::Vertex> verticiesSink = g->get_vertices(core::Vertex::Type::SINK);
            std::list<core::Vertex> intersection = g->get_vertices(core::Vertex::Type::INTERSECTION);

            core::Edge from, to;

            // move all cars up by 1
            for (vertex_iterator it = verticiesSource.begin(); it != verticiesSource.end(); ++it)
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
std::cout << "Updating the road...." << std::endl;
                // Re-add the edge to the graph (this is only a copy of it!)
                g->add_edge(*it, intersection.back(), from);

                // Now find the corresponding sink
                for (vertex_iterator jt = verticiesSink.begin(); jt != verticiesSink.end(); ++jt)
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

            // There is a 45% chance that a car will enter a lane
                /*
                 * FOR EACH Vertex v in g
                 *     IF v->trait == goal THEN
                 *         Edge e = v->from
                 *         IF e->actual_cars <= state.maxCars THEN
                 *             e->push_back(car)
                 *         END IF
                 *
                 *         BREAK
                 *     END IF
                 * NEXT v
                 */
                for (vertex_iterator it = verticiesSource.begin(); it != verticiesSource.end(); ++it)
                {
                std::cout << "Attempting to add a car...." << std::endl;
                    from = g->get_edges_from(*it).back();

                        if (from.actual_cars <= state.getMaxCars())
                        {
                            if (chanceToGetCar < 45 && atoi(it->uid.substr(sizeof("source-"), std::string::npos).c_str()) == rd)
                            {
                                from.cars.push_back(core::Car(1, 1.0, false));
                            }
                            else
                            {
                                from.cars.push_back(core::Car(1, 1.0, true));
                            }
                        }
                }
        }
    } // End of namespace sim.
} // End of namespace road.
