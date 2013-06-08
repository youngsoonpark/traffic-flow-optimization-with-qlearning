#include "SimulationPolicyNaive.hpp"
#include "State.hpp"
#include <cstdlib>

namespace road {
    namespace sim {

        void SimulationPolicyNaive::update(core::State& state) {/*
            int chanceToGetCar = rand() % 101; // 101 so our range is [0, 100]
            int road = rand() % 2; // Value meaning is dependent on State::Lights
            core::State::Graph* g = state.getGraph();

            core::Car tempCar;
            std::string tempName;
            size_t location;
            // move all cars up by 1
            for (core::State::graph_traits::vertex_iterator vit = g->begin(); vit != g->end(); ++g)
            {
                location = vit->getTrait().find("Source");
                if (location != std::string::npos)
                {
                    tempName = vit->getTrait().substr(0, location)
                    tempCar = vit->somehowyouaccesstheedge.cars.front();
                    vit->somehowyouaccesstheedge.cars.pop_front();

                    if (tempName.find("Horisontal") != std::string::npos)
                    {
                        // somehow find the sink edge that joins intersection and sink
                        // sink.push_back(tempCar)
                    }
                    else
                    {
                        // Do the same as above, but for vertical instead...
                    }
                }
            }

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
