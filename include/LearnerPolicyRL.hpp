#ifndef ROAD_ML_LEARNER_POLICY_RL_H_
#define ROAD_ML_LEARNER_POLICY_RL_H_

#include "State.hpp"
#include "Graph.hpp"

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
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @description A reinforcement learning policy.
 */
class LearnerPolicyRL
{
public:
  static constexpr double DEFAULT_LEARNING_RATE = 0.1f;
  static constexpr double DEFAULT_EXPLORATION_RATE = 0.1f;
  static constexpr double DEFAULT_DISCOUNT_FACTOR = 0.9f;

  static const int NUM_LIGHT_SETTINGS = 2;
  static const int NUM_APPROACHING_LANES = 2;
  static const int MAX_CAR_DISTANCE = 8;
  static const int MAX_DELAY = 3;
  static const int NUM_STATES = (MAX_DELAY + 1) *
                                (MAX_CAR_DISTANCE + 2) *
                                (MAX_CAR_DISTANCE + 2) * // two lanes
                                NUM_LIGHT_SETTINGS;

protected:
  /**
   * @description Construct a new learner policy wiht the default parameters
   */
  LearnerPolicyRL();

  /**
   * @description Destroy the learner policy
   */
  ~LearnerPolicyRL();

  /**
   * @description Set the learning rate to the specified value
   * @param new_learning_rate The new learning rate. 0 <= rate <= 1.
   */
  void setLearningRate(double new_learning_rate);

  /**
   * @description Set the exploration rate to the specified value
   * @param new_exploration_rate The new exploration rate. 0 <= rate <= 1.
   */
  void setExplorationRate(double new_exploration_rate);

  /**
   * @description Set the discount factor to the specified value
   * @param new_learning_rate The new learning rate. 0 <= factor < 1.
   */
  void setDiscountFactor(double new_discount_factor);

  /**
   * @description Takes the state, learns from its mistakes, then changes itself.
   * @param state the current state of the simulator.
   */
  void action(core::State& state);

private:
  /**
   * @description Get a integer value >= 0 and < NUM_STATES which represents
   * each possible observed state
   */
  int stateIndex(core::State& state);

  /**
   * @description Get the reward earned in the specified state
   */
  double reward(core::State& state);

  /**
   * @description Get the optimal action to perform in the specified state
   * @param stateIndex the index of the state
   */
  int optimalAction(int state_index);

  /**
   * @description Get the maximum reward that can be earned from the specified
   * state, including discounted future reward.
   */
  double optimalReward(int state_index);
  
  /**
   * @description Given a state, return a vector of integers indicating how
   * far the nearest car is from the intersection in each approaching lane.
   * @return The vector
   */
  std::vector<uint8_t> approachingCars(core::State& state);
  
  /**
   * @description Given a state, return a vector of integers representing how
   * many cars there are waiting at each lane approaching thee intersection
   */
  std::vector<uint8_t> queueLengths(core::State& state);

  /**
   * The learning rate is the extent to which old information overwrites new
   * information. A larger value will result in faster learning but more
   * instability.
   */
  double learning_rate;

  /**
   * The discount factor controls the discount applied to future reward over
   * immediate reward. A value of 0 will make the learner completely short
   * sighted, and values close to one will consider long term rewards to be
   * almost as valueble as immediate rewards.
   */
  double discount_factor;

  /**
   * The exploration rate controls the probability at each step that the
   * learner will chosoe a random action instead of the optimal action (as it
   * sees it). A excessively low value may result in the learner taking too
   * long to find the optimal policy because it does not explore some policies,
   * while a excessively high value will result in the learner wasting time
   * exploring policies with little chance of being optimal.
   */
  double exploration_rate;

  /**
   * the map of States and Actions to rewards
   */
  double reward_map[NUM_STATES][NUM_LIGHT_SETTINGS];

  /**
   * The map of states to optimal actions
   */
  //int optimal_actions[NUM_STATES];
};

} // End of namespace ml.
} // End of namespace road.

#endif // ROAD_ML_LEARNER_POLICY_RL_H_
