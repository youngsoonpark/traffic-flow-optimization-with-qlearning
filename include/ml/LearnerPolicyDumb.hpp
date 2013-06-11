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

protected:

  /**
   * @description Takes the state, learns from its mistakes, then changes itself.
   * @param state the current state of the simulator.
   */
  void action(core::State& state);
};


} // End of namespace ml.
} // End of namespace road.

#endif // ROAD_ML_LEARNER_POLICY_RL_H_
