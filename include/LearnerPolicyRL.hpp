#ifndef ROAD_ML_LEARNER_POLICY_RL_H_
#define ROAD_ML_LEARNER_POLICY_RL_H_

namespace road {
namespace ml {

class State; // Prototype of state.

/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @description A reinforcement learning policy.
 */
class LearnerPolicyRL {
protected:
	/**
	 * @description Takes the state, learns from its mistakes, then changes itself.
	 * @param state the current state of the simulator.
	 */
	void action(State & const state);
};

} // End of namespace ml.
} // End of namespace road.

#endif // ROAD_ML_LEARNER_POLICY_RL_H_
