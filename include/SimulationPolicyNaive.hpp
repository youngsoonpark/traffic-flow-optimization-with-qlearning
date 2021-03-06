#ifndef ROAD_SIM_SIMULATION_POLICY_NAIVE_H_
#define ROAD_SIM_SIMULATION_POLICY_NAIVE_H_

namespace road
{

namespace core
{
struct Car;
class State;
}

namespace sim
{

/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>,
 *         Christopher Di Bella <chrisdb@cse.unsw.edu.au>
 * @description Naive simulation policy, just randomly distributes vehicles in the lanes
 * of speed 1. It is stupid, but it is the requirement for the basic assignment. So we provide
 * it here.
 */
class SimulationPolicyNaive
{
protected:
  void update(core::State& state);
};

} // End of namespace sim.
} // End of namespace road.

#endif // ROAD_SIM_SIMULATION_POLICY_NAIVE_H_
