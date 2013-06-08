#include "SimulationPolicyNaive.hpp"
#include <cstdlib>

namespace road {
    namespace sim {

        void SimulationPolicyNaive::update(core::State& state) {
            int chanceToGetCar = rand() % 100;
            int road = rand() % 2;
            State::Graph* g = state.getGraph();

            // move all cars up by 1
            // TO DO: just committing for comments for now

            // There is a 45% chance that a car will enter the lane
            if (chanceToGetCar >= 45)
            {
                std::string edgeTrait = (road == Horisontal) ? "HorisontalSource" : "VerticalSource";

                for (State::graph_traits::vertex_iterator vit = g->begin(); vit != g->end(); ++g)
                {
                    if (vit->getTrait() == edgeTrait && vit->somehowyouaccesstheedge.cars.size() <= state.maxCars())
                    {
                        vit->somehowyouaccesstheedge.cars.push(Car()vit->somehowyouaccesstheedge.speedlimit, 1.0));
                    }
                }
            }
        }
    } // End of namespace sim.
} // End of namespace road.
