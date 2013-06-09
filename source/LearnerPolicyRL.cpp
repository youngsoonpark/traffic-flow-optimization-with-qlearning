#include "LearnerPolicyRL.hpp"

#include <assert.h>
#include <float.h>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <iomanip>

namespace road
{
namespace ml
{

LearnerPolicyRL::LearnerPolicyRL(): learning_rate(DEFAULT_LEARNING_RATE),
  discount_factor(DEFAULT_DISCOUNT_FACTOR),
  exploration_rate(DEFAULT_EXPLORATION_RATE)
{
  for (int state = 0; state < NUM_STATES; state++) {
    //optimal_actions[state] = -1;
    for (int action = 0; action < NUM_LIGHT_SETTINGS; action++) {
      reward_map[state][action] = 0;
    }
  }
}

LearnerPolicyRL::~LearnerPolicyRL()
{
}

void LearnerPolicyRL::setLearningRate(double new_learning_rate)
{
  assert(new_learning_rate >= 0 && new_learning_rate <= 1);
  learning_rate = new_learning_rate;
}

void LearnerPolicyRL::setExplorationRate(double new_exploration_rate)
{
  assert(new_exploration_rate >= 0 && new_exploration_rate <= 1);
  exploration_rate = new_exploration_rate;
}

void LearnerPolicyRL::setDiscountFactor(double new_discount_factor)
{
  assert(new_discount_factor >= 0 && new_discount_factor < 1);
  discount_factor = new_discount_factor;
}

void LearnerPolicyRL::action(core::State& state)
{
  // Check that there is one intersection on the map  
  core::Graph *graph = state.getGraph();
  std::list<core::Vertex> intersection_list;
  intersection_list = graph->get_vertices(core::Vertex::INTERSECTION);
  if (intersection_list.size() != 1) {
    /*std::cout << "Intersections found: " << intersection_list.size() <<
        ". Learning is impossible." << std::endl;*/
    return;
  }
  
  // Determine what action to perform
  int state_index = stateIndex(state);
  int new_action = -1;
  if (rand() % 100 <= exploration_rate * 100) {
    std::cout << "Choosing random action" << std::endl;
    new_action = rand() % NUM_LIGHT_SETTINGS;
  } else {
    // Find the optimal action to perform
    new_action = optimalAction(state_index);
  }
  std::cout << "Chose action " << new_action << std::endl;

  // Perform the action
  state.setLights(static_cast<core::State::Lights>(new_action));

  // Learn from action:

  // Find the reward of the new state
  double observed_reward = reward(state);
  observed_reward += discount_factor * optimalReward(stateIndex(state));

  // Change some of the recorded reward
  reward_map[state_index][new_action] *= 1 - learning_rate;
  reward_map[state_index][new_action] += observed_reward * learning_rate;
}

int LearnerPolicyRL::stateIndex(core::State& state)
{
  int state_index = 0;
  std::vector<uint8_t> lanes = approachingCars(state);
  assert(lanes.size() == (unsigned int) NUM_APPROACHING_LANES);
  
  for (int lane = 0; lane < NUM_APPROACHING_LANES; lane++) {
    assert(0 <= lanes[lane] && lanes[lane] < MAX_CAR_DISTANCE + 2);
    state_index *= MAX_CAR_DISTANCE + 2;
    state_index += static_cast<int>(lanes[lane]);
  }
  
  int light_state = static_cast<int>(state.getLights());
  assert(0 <= light_state && light_state < 2);
  state_index *= NUM_LIGHT_SETTINGS;
  state_index += light_state;
  
  int delay = static_cast<int>(state.getDelay());
  delay = delay > MAX_DELAY ? MAX_DELAY : delay;
  assert(0 <= delay && delay <= MAX_DELAY);
  state_index *= MAX_DELAY + 1;
  state_index += delay;
  
  if (state_index >= NUM_STATES) {
    state_index = 0;
    std::cout << "Error in LearnerPolicyRL::stateIndex(core::State& state): " <<
                "state_index == " << state_index << " >= NUM_STATES == " <<
                NUM_STATES << std::endl;
    //assert(false);
  }
  return state_index;
}

double LearnerPolicyRL::reward(core::State& state)
{
  std::vector<uint8_t> approaching_cars = approachingCars(state);
  if (approaching_cars[1 - state.getLights()] == 0) {
    // There is a car waiting in this lane
    return -1;
  }
  return 0;
}

int LearnerPolicyRL::optimalAction(int state_index)
{
  // Find the optimal reward attainable
  double optimal_reward = -DBL_MAX;
  for (int action = 0; action < NUM_LIGHT_SETTINGS; action++) {
    std::cout << "Action " << action << " has reward " <<
          reward_map[state_index][action] << ". ";
    if (reward_map[state_index][action] > optimal_reward) {
      optimal_reward = reward_map[state_index][action];
    }
  }
  std::cout << std::endl;
  
  // Make a list of optimal actions
  std::list<int> optimal_actions;
  for (int action = 0; action < NUM_LIGHT_SETTINGS; action++) {
    if (reward_map[state_index][action] == optimal_reward) {
      optimal_actions.push_back(action);
    }
  }
  
  if (optimal_actions.size() == 1)
  {
    return optimal_actions.front(); // Only optimal actions
  }
  else if (rand() % NUM_LIGHT_SETTINGS == 1)
  { // If both actions have equal reward, choose one randomly
    return optimal_actions.front();
  }
  else
  {
    return optimal_actions.back();
  }
}

double LearnerPolicyRL::optimalReward(int state_index)
{
  // Find the optimal action to perform
  double optimal_reward = -DBL_MAX;
  for (int action = 0; action < NUM_LIGHT_SETTINGS; action++) {
    if (reward_map[state_index][action] > optimal_reward) {
      optimal_reward = reward_map[state_index][action];
    }
  }
  return optimal_reward;
}

std::vector<uint8_t> LearnerPolicyRL::approachingCars(core::State& state)
{
  core::Graph *graph = state.getGraph();
  
  // Find the vertex containing THE ONLY intersection
  std::list<core::Vertex> intersection_list;
  intersection_list = graph->get_vertices(core::Vertex::INTERSECTION);
  
  assert(intersection_list.size() == 1); // Can't handle more than one
  
  core::Vertex intersection = intersection_list.front();
  
  // Get the edges that go to the intersection (approachine lanes)
  std::list<core::Edge> lanes = graph->get_edges_to(intersection);
  assert(lanes.size() == static_cast<uint8_t>(NUM_APPROACHING_LANES));
  
  std::vector<uint8_t> result;
  
  // for each one, iterate over them in reverse to find the closest car and
  // add it to the result
  for (std::list<core::Edge>::iterator it = lanes.begin(); it != lanes.end(); it++) {
    core::Edge::Container cars = it->cars;
    int car_distance = MAX_CAR_DISTANCE + 1;
    int current_position = 0;
    for (core::Edge::Container::reverse_iterator it = cars.rbegin();
          it != cars.rend() && current_position <= MAX_CAR_DISTANCE; it++)
    {
      if(!it->no_car)
      {
        car_distance = current_position;
        break;
      }
      current_position++;
    }
    result.push_back(car_distance);
  }
  return result;
}

} // End of namespace ml.
} // End of namespace road.
