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

        /**
         * @author Christopher Di Bella <chrisdb@cse.unsw.edu.au>
         * @description TO DO
         * @param cars - The collection of cars
         * @param carIt - The car being assessed
         * @param roadLength - the size of the road
         */
         void updateCarPosition(core::Edge::Container& cars, core::Edge::Container::reverse_iterator& carIt, const uint8_t& roadLength, const uint8_t& speedLimit)
         {
             bool p = probabilityCalculator(g_probability);
             while (true)
             {
                 // Let's create a way to find out if a car exists at position x
                 struct carAtPos
                 {
                     unsigned int pos;
                     const uint8_t roadSize;

                     carAtPos(unsigned int p, const uint8_t& r) : pos(p), roadSize(r) {}

                     bool operator()(const core::Car& car)
                     {
                         return (car.position <= pos && car.position < roadSize);
                     }
                 };

                 // Only update the car's position if it can occupy the space in question
                 auto it = (carIt + 1).base();
                 if (std::count_if(it, cars.end(), carAtPos(it->position + it->speed, roadLength)) == 0 && it->speed > 0)
                 {
                     // Move the car
                     carIt->position += carIt->speed;

                     if (carIt->speed < speedLimit && (carIt->speed == 0 || p == false))
                     {
                         ++carIt->speed;
                     }
                     else if (carIt->speed > 0 && p == true)
                     {
                         --carIt->speed;
                     }

                     return;
                 }
                 else if (it->speed > 0)
                 {
                     return;
                 }
                 else
                 {
                     // The car slows down by 1
                     --carIt->speed;
                 }
             }
         }

         /**
          * @author Christopher Di Bella <chrisdb@cse.unsw.edu.au>
          * @description TO DO
          * @param
          * @param
          * @param
          * @return The number of cars to pop
          */
          int8_t updateEdge(EdgeList::iterator& edgeIt, const uint8_t& roadLength)
          {
              int8_t toPop = 0;
              core::Edge::Container cars = edgeIt->cars;
              for (auto carIt = cars.rbegin(); carIt != cars.rend(); ++carIt)
              {
                  updateCarPosition(cars, carIt, roadLength, edgeIt->speed_limit);

                  // Get a counter for how many cars to remove (C++ will go nuts if we remove them now)
                  if (carIt->position >= roadLength)
                  {
                      ++toPop;
                  }
              }

              return toPop;
          }


        /**
          * Alright, here's where stuff gets interesting.
          * Unlike in Naive, Cellular will attempt to replicate a known, working traffic model.
          * This is called the Nagel–Schreckenberg model and serves as the underlying basis of the extended simulation.
          */
        void SimulationPolicyCellular::update(core::State& state)
        {
            bool carNotMoved;
            int8_t toPop, toPush;
            const int8_t roadLength  = state.getMaxCars();
            core::Graph* graph = state.getGraph();

            // Get our sources and our sinks
            VertexList sourceList       = graph->get_vertices(core::Vertex::Type::SOURCE);
            VertexList sinkList         = graph->get_vertices(core::Vertex::Type::SINK);
            VertexList intersectionList = graph->get_vertices(core::Vertex::Type::INTERSECTION);

            // Update the sinks
            for (auto it = sinkList.begin(); it != sinkList.end(); ++it)
            {
                toPop = 0;
                EdgeList outboundRoads = graph->get_edges_to(*it);

                // Update each outgoing road
                for (auto edgeIt = outboundRoads.begin(); edgeIt != outboundRoads.end(); ++edgeIt)
                {
                    std::cout << "Updating outbound traffic for " << edgeIt->uid << "..." << std::endl;

                    toPop = updateEdge(edgeIt, roadLength);

                    // Pop the cars now that we aren't dealing with iterators
                    while (toPop != 0)
                    {
                        outboundRoads.pop_front();
                        --toPop;
                    }
                    graph->update_edge(*edgeIt);
                }

                // TO DO: Update all inbound roads (when we have two-way traffic)
            }

            // Update the sources
            for (auto it = sourceList.begin(); it != sourceList.end(); ++it)
            {
               /* toPop = 0;
                toPush = 0;
                EdgeList inboundRoads = graph->get_edges_from(*it);

                // Update each incoming road
                for (auto edgeIt = inboundRoads.begin(); edgeIt != inboundRoads.end(); ++edgeIt)
                {
                    std::cout << "Updating inbound traffic for " << edgeIt->uid << "..." << std::endl;

                    toPop = updateEdge(edgeIt, roadLength);

                    if (toPop != 0 && state.getLights() == (char)*edgeIt->uid.rbegin() - '0')
                    {
                        // Get the sink edge we are after
                        EdgeList allSinkRoads = graph->get_edges_from(intersectionList.back());
                        while (!allSinkRoads.empty() && (char)*allSinkRoads.back().uid.rbegin() - '0' != state.getLights())
                        {
                            allSinkRoads.pop_back();
                        }

                        // START OF WELL SHIT CODE
                        auto carIt = allSinkRoads.back();
                        // END OF WELL SHIT CODE
                    }
                }*/

                toPop = 0;
                EdgeList outboundRoads = graph->get_edges_to(*it);

                // Update each outgoing road
                for (auto edgeIt = outboundRoads.begin(); edgeIt != outboundRoads.end(); ++edgeIt)
                {
                    std::cout << "Updating outbound traffic for " << edgeIt->uid << "..." << std::endl;

                    toPop = updateEdge(edgeIt, roadLength);

                    // Pop the cars now that we aren't dealing with iterators
                    while (toPop != 0)
                    {
                        outboundRoads.pop_front();
                        --toPop;
                    }
                    graph->update_edge(*edgeIt);
                }
            }

            for (auto source_it = sourceList.begin(); source_it != sourceList.end(); source_it++) {
    // Grab all the edges going from the source.
    auto edges_from_source = graph->get_edges_from(*source_it);
    for (auto edge_it = edges_from_source.begin(); edge_it != edges_from_source.end(); edge_it++) {
      //std::cout << "Updating Source For Edge: " << edge_it->uid << std::endl;

      // Check if it is possible to push a car
      if (edge_it->cars.empty() || edge_it->cars.front().position < edge_it->capacity - 1)
      {
        int car_placement_probability = rand() % 101;
        // Update the edge.
        if ((source_it == sourceList.begin() && car_placement_probability >= 70) ||
            (source_it != sourceList.begin() && car_placement_probability >= 20)) {
          //std::cout << "Not adding new car" << std::endl;
        } else {
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
