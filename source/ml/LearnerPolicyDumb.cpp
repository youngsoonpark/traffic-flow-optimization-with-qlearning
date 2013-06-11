#include "ml/LearnerPolicyDumb.hpp"

namespace road
{
namespace ml
{

void LearnerPolicyDumb::action(core::State& state)
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
  
  if (state.getDelay() >= PHASE_TIME)
  {
    state.setLights(static_cast<core::State::Lights>(1 - state.getLights())); // Change the lights
  }
  else
  {
    state.setLights(state.getLights());   // Update the delay
  }
  
  updatePerformance(state);
  if (queue_records.size() % 25 == 0)
  {
    printPerformance();
    queue_records.clear();
  }
}

void LearnerPolicyDumb::printPerformance()
{
  if (queue_records.size() == 0) return;
  unsigned int average_length = 0;
  for (unsigned int cycle = 0; cycle < queue_records.size(); cycle++)
  {
    average_length += queue_records[cycle];
  }
  average_length /= queue_records.size();
  
  std::cout << "Learner::Performance metrics: Total cycles: " <<
        cycles_completed << ". Average queue length for last " <<
        queue_records.size() << " cycles: " <<
        average_length << "." << std::endl;
}

void LearnerPolicyDumb::updatePerformance(core::State& state)
{
  // Update the records
  cycles_completed++;
  if (cycles_completed > 60)  // Wait for the iitial cars to reach the intersection and queue up
  {
    std::vector<uint8_t> queue_lengths = queueLengths(state, 50);
    unsigned int total_queue_length = 0;
    for (unsigned int lane = 0; lane < queue_lengths.size(); lane++)
    {
      total_queue_length += queue_lengths[lane];
    }
    queue_records.push_back(total_queue_length);
  }
}

std::vector<uint8_t> LearnerPolicyDumb::approachingCars(core::State& state,
                                        unsigned int maximum_distance)
{
  core::Graph *graph = state.getGraph();
  
  // Find the vertex containing THE ONLY intersection
  std::list<core::Vertex> intersection_list;
  intersection_list = graph->get_vertices(core::Vertex::INTERSECTION);
  
  core::Vertex intersection = intersection_list.front();
  
  // Get the edges that go to the intersection (approachine lanes)
  std::list<core::Edge> lanes = graph->get_edges_to(intersection);
  assert(lanes.size() <= static_cast<uint8_t>(MAX_APPROACHING_LANES));
  
  std::vector<uint8_t> result;
  
  // for each one, iterate over them in reverse to find the closest car and
  // add it to the result
  for (std::list<core::Edge>::iterator it = lanes.begin(); it != lanes.end(); it++) {
    core::Edge::Container cars = it->cars;
    if (cars.empty())
    {
      result.push_back(maximum_distance + 1);
    }
    else if (cars.back().position <= maximum_distance)
    {
      result.push_back(cars.back().position);
    }
    else
    {
      result.push_back(MAX_CAR_DISTANCE + 1);
    }
    /*std::cout << "Closest car on road " << result.size() - 1 << ": " <<
      static_cast<int>(result[result.size() - 1]) << std::endl;*/
  }
  return result;
}

std::vector<uint8_t> LearnerPolicyDumb::queueLengths(core::State& state,
                                          unsigned int maximum_distance)
{
  core::Graph *graph = state.getGraph();
  
  // Find the vertex containing THE ONLY intersection
  std::list<core::Vertex> intersection_list;
  intersection_list = graph->get_vertices(core::Vertex::INTERSECTION);
  
  assert(intersection_list.size() == 1); // Can't handle more than one
  
  core::Vertex intersection = intersection_list.front();
  
  // Get the edges that go to the intersection (approachine lanes)
  std::list<core::Edge> lanes = graph->get_edges_to(intersection);
  assert(lanes.size() <= static_cast<uint8_t>(MAX_APPROACHING_LANES));
  
  std::vector<uint8_t> result;
  
  // for each one, iterate over them in reverse to find the closest car and
  // add it to the result
  for (std::list<core::Edge>::iterator it = lanes.begin(); it != lanes.end(); it++) {
    core::Edge::Container cars = it->cars;
    unsigned int queue_length = 0;
    int prev_pos = -1;
    for (core::Edge::Container::reverse_iterator it = cars.rbegin();
          it != cars.rend() && queue_length <= maximum_distance; it++)
    {
      if(it->position == static_cast<unsigned int>(prev_pos + 1))
      {
        queue_length++;
        prev_pos++;
      }
      else
      {
        break;
      }
    }
    result.push_back(queue_length);
    /*std::cout << "Queue length on road " << result.size() - 1 << ": " <<
      static_cast<int>(result[result.size() - 1]) << std::endl;*/
  }
  return result;
}

} // End of namespace ml.
} // End of namespace road.
