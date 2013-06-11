#ifndef ROAD_ML_LEARNER_POLICY_DUMB_H_
#define ROAD_ML_LEARNER_POLICY_DUMB_H_

#include "core/State.hpp"
#include "core/Graph.hpp"

namespace road
{

// State prototype
namespace core
{
class State;
}

namespace ml
{

/**
 * @author Daniel Playfair Cal <danielpc@cse.unsw.edu.au>
 * @description A dumb light switching policy which changes the light setting
 * at a fixed interval
 */

class LearnerPolicyDumb
{
public:
  static const unsigned int PHASE_TIME = 10; // The length of each phase
  static const unsigned int MAX_APPROACHING_LANES = 2;
  static const unsigned int MAX_CAR_DISTANCE = 8;

protected:

  /**
   * @description Takes the state, learns from its mistakes, then changes itself.
   * @param state the current state of the simulator.
   */
  void action(core::State& state);
  
private:

  /**
   * @description Given a state, return a vector of integers indicating how
   * far the nearest car is from the intersection in each approaching lane.
   * @return The vector
   */
  std::vector<uint8_t> approachingCars(core::State& state, unsigned int max_car_distance);
  
  /**
   * @description Given a state, return a vector of integers representing how
   * many cars there are waiting at each lane approaching thee intersection
   */
  std::vector<uint8_t> queueLengths(core::State& state, unsigned int max_car_distance);
  
  /**
   * @description print out the current queue length as well as the average queue
   * length sinc the learner has been running to the console
   */
  void printPerformance();
  
  /**
   * @description Update the records of queue lengths
   */
  void updatePerformance(core::State& state);
  
  /**
   * A vector storing the cumulative queue length (i.e. for both roads) at each
   * step
   */
  std::vector<unsigned int> queue_records;
  
  /**
   * The number of cycles completed
   */
  unsigned int cycles_completed = 0;
};


} // End of namespace ml.
} // End of namespace road.

#endif // ROAD_ML_LEARNER_POLICY_RL_H_
