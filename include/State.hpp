#ifndef ROAD_CORE_STATE_H_
#define ROAD_CORE_STATE_H_

#include <cstdint>
#include <deque>
#include <utility>
#include <vector>
#include "Graph.hpp"

namespace road
{
namespace core
{
/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>,
 *         Christopher Di Bella <chrisdb@cse.unsw.edu.au>
 * @descripton The state controls all the data about the
 * current state of the simulation, where all the cars
 * are located at what speed and in which lane. In additon
 * to this everything about the current state of the intersections
 * and the learner.
 */
class State
{
public:
  /**
   * @description Which lights are allowed to be green.
   * Turning lanes are read as "X turn on to Y road".
   * e.g. RightTurnHorisontal == "Right turn on to Horisontal road"
   *
   * At present, lights only work for a T-intersection (i.e. Hor, Vert).
   */
  enum Lights {
    HORIZONTAL,
    VERTICAL
  };

  /**
   * @description Constructs the initial state, it doesn't load
   *  anything this is left up to the SerializationPolicy that
   *  takes a reference and modifies the internals, as required.
   *  It does, however set the core state to running.
   */
  State();

  /**
   * @description cleans up the internals data structures
   *   used to represent the states.
   */
  ~State();

  /**
   * @return if the state is running.
   */
  bool isRunning();

  /**
   * @param what to set the running state to.
   */
  void setRunning(bool running);

  /**
   * @return returns the adjacency list, with everything we need.
   */
  Graph* getGraph();

  /**
   * @return returns the state of the traffic lights
   */
  Lights getLights() const;

  /**
   * @param What the lights are set to
   */
  void setLights(const Lights lights);
  
  /**
   * @description Get the number of cycles the lights have been in their current
   * position
   */
  uint8_t getDelay();

  /**
   * @return returns the maximum number of allowed cars
   */
  int8_t getMaxCars() const;
private:
  bool m_running;      // Whether the game is running or not, we set this to true at the start.
  Lights m_lights;     // This determines what state the lights are in.
  size_t m_tick_speed; // Determines the speed of a tick in the simulator.
  Graph m_graph;       // The map, the actual graph of the entire scene. This is critical.
  uint8_t m_delay;       // The number of cycles the lights have been in their current position.
  const int8_t m_maxCars; // The maximum number of cars that a road is allowed to hold at any given time.
};

} // End of namespace core.
} // End of namespace road.

#endif  // ROAD_CORE_STATE_H_
