#include "sim/SimulationPolicyCellular.hpp"
#include "core/State.hpp"
#include "core/Graph.hpp"
#include <algorithm>
#include <list>
/**
 * Style not to be changed until completed.
 * Style not to be changed until completed.
 * Style not to be changed until completed.
 * Style not to be changed until completed.
 * Style not to be changed until completed.
 * Style not to be changed until completed.
 * Style not to be changed until completed.
 * Style not to be changed until completed.
 * Style not to be changed until completed.
 * Style not to be changed until completed.
 * Style not to be changed until completed.
 * Style not to be changed until completed.
 * Style not to be changed until completed.
 * Style not to be changed until completed.
 * Style not to be changed until completed.
 * Style not to be changed until completed.
 * Style not to be changed until completed.
 */
namespace road
{
    typedef std::list<core::Vertex> VertexList;
    typedef std::list<core::Edge> EdgeList;

    namespace sim
    {
        const uint8_t g_probability = 50;
        /**
         * @author Christopher Di Bella <chrisdb@cse.unsw.edu.au>
         * @description Determines if something with a probability p will happen
         * @param p - The probability of action a occurring
         * @param q - The chance of the event occurring. Leave as -1 if you don't
                      have a random chance to begin with.
         * @return Whether or not the action shall happen
         */
        bool probabilityCalculator(int p, int chance = -1)
        {
            if (chance < 0)
            {
                chance = rand() % 101;
            }

            return (chance < p);
        }

        struct carAtPos
        {
            unsigned int pos;
            unsigned int roadLength;

            carAtPos(int p, int r) : pos(p), roadLength(r) {}

            bool operator()(const core::Car& car)
            {
                return (car.position == pos && pos > 0);
            }
        };

        struct isEdge
        {
            std::string id;

            isEdge(std::string id) : id(id) {}

            bool operator()(const core::Edge& e)
            {
                return (((char)*e.uid.rbegin() - '0') == ((char)*id.rbegin() - '0'));
            }
        };

         /**
          * Alright, here's where stuff gets interesting.
          * Unlike in Naive, Cellular will attempt to replicate a known, working traffic model.
          * This is called the Nagel–Schreckenberg model and serves as the underlying basis of the extended simulation.
          */
        void SimulationPolicyCellular::update(core::State& state)
        {
            int toPop;
            int roadIdentity;
            const int roadLength = state.getMaxCars();
            core::Graph* graph = state.getGraph();

            core::Graph::VertexContainer sources = graph->get_vertices(core::Vertex::Type::SOURCE);
            core::Graph::VertexContainer sinks = graph->get_vertices(core::Vertex::Type::SINK);
            core::Graph::VertexContainer intersections = graph->get_vertices(core::Vertex::Type::INTERSECTION);

            core::Graph::EdgeContainer roads;

            // Update the sinks
            for (auto it = sinks.begin(); it != sinks.end(); ++it)
            {
                roads = graph->get_edges_to(*it);

               for (auto roadIt = roads.begin(); roadIt != roads.end(); ++roadIt)
               {
                   toPop = 0;

                    for (auto carRevIt = roadIt->cars.rbegin(); carRevIt != roadIt->cars.rend(); ++carRevIt)
                    {
                        // Acceleration
                        if (carRevIt->speed < roadIt->speed_limit)
                        {
                            ++(carRevIt->speed);
                        }

                        // Deceleration phase
                        auto carIt = (carRevIt + 1).base();
                        while (true)
                        {
                            if (std::count_if(carIt, roadIt->cars.end(), carAtPos(carIt->position - carIt->speed, roadLength)) > 0)
                            {
                                --(carIt->speed);
                            }
                            else
                            {
                                break;
                            }
                        }

                        // Random slowing
                        if (carRevIt->speed > 1 && probabilityCalculator(g_probability))
                        {
                            --(carIt->speed);
                        }

                        // Update position
                        carRevIt->position -= carRevIt->speed;

                        // Check if off road and give permission to remove if it is
                        if (carRevIt->position < 0)
                        {
                            ++toPop;
                        }
                    }

                    while (!roadIt->cars.empty() && toPop != 0)
                    {
                        roadIt->cars.pop_back();
                        --toPop;
                    }

                    graph->update_edge(*roadIt);
                }
            }

            // Update the sources.
            for (auto it = sources.begin(); it != sources.end(); ++it)
            {
                roads = graph->get_edges_from(*it);

                for (auto roadIt = roads.begin(); roadIt != roads.end(); ++roadIt)
                {
                    toPop = 0;
                    roadIdentity = (char)*(roadIt->uid.rbegin()) - '0';

                    // Get the corresponding sink road
                    core::Edge correspondingSink;
                    bool found = false;
                    /*for (auto sinkIt = sinks.begin(); sinkIt != sinks.end() && found == false; ++sinkIt)
                    {
                        core::Graph::EdgeContainer destinations = graph->get_edges_to(*sinkIt);

                        correspondingSink = *std::find_if(destinations.begin(), destinations.end(), isEdge(roadIt->uid));
                    }*/

                    for (auto carRevIt = roadIt->cars.rbegin(); carRevIt != roadIt->cars.rend(); ++carRevIt)
                    {
                        // Acceleration
                        if (carRevIt->speed < roadIt->speed_limit)
                        {
                            ++(carRevIt->speed);
                        }

                        // Deceleration
                        auto carIt = (carRevIt + 1).base();
                        while (true)
                        {
                            if (std::count_if(carIt, roadIt->cars.end(), carAtPos(carIt->position - carIt->speed, roadLength)) > 0)
                                //(carIt->position - carIt->speed < 0 && std::count_if(correspondingSink.cars.begin(), correspondingSink.cars.end(), carAtPos(carIt->position * -1, roadLength)) > 0))
                            {
                                --(carIt->speed);
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (roadIdentity != state.getLights() && carIt->position - carIt->speed > 1 && carIt->speed > 0)
                        {
                            --carIt->speed;
                        }

                        // Random slowing
                        if (carRevIt->speed > 1 && probabilityCalculator(g_probability))
                        {
                            --(carIt->speed);
                        }

                        // Update position
                        carRevIt->position -= carRevIt->speed;

                        // Check if off road and give permission to remove if it is
                        if (carRevIt->position < 0)
                        {
                            ++toPop;
                        }
                    }

                    /*while (!roadIt->cars.empty() && toPop != 0)
                    {
                        correspondingSink.cars.push_front(roadIt->cars.back());
                        roadIt->cars.pop_back();
                        --toPop;
                    }*/

                    // Check if it is possible to push a car
                    if (roadIt->cars.empty() || roadIt->cars.front().position < roadIt->capacity - 1)
                    {
                        int car_placement_probability = rand() % 101;
                        // Update the edge.
                        if ((it == sources.begin() && car_placement_probability >= 70) || (it != sources.begin() && car_placement_probability >= 20))
                        {
                            //std::cout << "Not adding new car" << std::endl;
                        }
                        else
                        {
                            //std::cout << "Adding new car" << std::endl;
                            roadIt->cars.push_front(core::Car(5, 0, roadIt->capacity - 1));
                        }
                    }

                    graph->update_edge(*roadIt);
                    graph->update_edge(correspondingSink);
                }
            }
        }

    } // End of namespace sim.
} // End of namespace road.
