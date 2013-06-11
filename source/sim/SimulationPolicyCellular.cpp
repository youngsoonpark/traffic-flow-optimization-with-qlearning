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
        const uint8_t g_probability = 45;
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
                return (car.position == pos && pos < roadLength);
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
            const int roadLength = state.getMaxCars();
            core::Graph* graph = state.getGraph();

            core::Graph::VertexContainer sources = graph->get_vertices(core::Vertex::Type::SOURCE);
            core::Graph::VertexContainer sinks = graph->get_vertices(core::Vertex::Type::SINK);
            core::Graph::VertexContainer intersections = graph->get_vertices(core::Vertex::Type::INTERSECTION);

            core::Graph::EdgeContainer roads;

            for (auto it = sinks.begin(); it != sinks.end(); ++it)
            {
                roads = graph->get_edges_to(*it);

               for (auto roadIt = roads.begin(); roadIt != roads.end(); ++roadIt)
               {
                   toPop = 0;

                    for (auto carRevIt = roadIt->cars.rbegin(); carRevIt != roadIt->cars.rend(); ++carRevIt)
                    {
                        // TO DO: Acceleration

                        // Deceleration phase
                        /*auto carIt =
                        while (true)
                        {
                            if (std::count_if(carIt, roadIt->cars.end(), carAtPos(carIt->position + carIt->speed, roadLength)) > 0)
                            {
                                --(carIt->speed);
                            }
                            else
                            {
                                break;
                            }
                        }*/

                        // TO DO: Random slowing

                        // Update position
                        carRevIt->position += carRevIt->speed;

                        // Check if off road and give permission to remove if it is
                        if (carRevIt->position >= roadLength)
                        {
                            ++toPop;
                        }
                    }
/*
                    while (!roadIt->cars.empty() && toPop != 0)
                    {
                        roadIt->cars.pop_back();
                        --toPop;
                    }

                    graph->update_edge(*roadIt);*/
                }
            }

            // Update the sources.
            for (auto source_it = sinks.begin(); source_it != sinks.end(); source_it++)
            {
                // Grab all the edges going from the source.
                auto edges_from_source = graph->get_edges_to(*source_it);
                for (auto edge_it = edges_from_source.begin(); edge_it != edges_from_source.end(); edge_it++)
                {
                    //std::cout << "Updating Source For Edge: " << edge_it->uid << std::endl;

                    // Check if it is possible to push a car
                    if (edge_it->cars.empty() || edge_it->cars.front().position < edge_it->capacity - 1)
                    {
                        int car_placement_probability = rand() % 101;
                        // Update the edge.
                        if ((source_it == sinks.begin() && car_placement_probability >= 70) || (source_it != sinks.begin() && car_placement_probability >= 20))
                        {
                            //std::cout << "Not adding new car" << std::endl;
                        }
                        else
                        {
                            //std::cout << "Adding new car" << std::endl;
                            edge_it->cars.push_front(core::Car(1, 0, edge_it->capacity - 1));
                            graph->update_edge(*edge_it);
                        }
                    }
                }
            }
        }

    } // End of namespace sim.
} // End of namespace road.
