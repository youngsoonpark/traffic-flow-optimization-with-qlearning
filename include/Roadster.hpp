#ifndef ROAD_ROADSTER_H_
#define ROAD_ROADSTER_H_

#include  <string>

// Import the core state.
#include "State.hpp"
// Include the valid policies.
#include "GraphicsPolicy3D.hpp"
#include "LearnerPolicyRL.hpp"
#include "SimulationPolicyCellular.hpp"
#include "SimulationPolicyNaive.hpp"

#include <irrlicht.h>

using namespace irr;
using namespace irr::core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

namespace road
{

/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @description This is the core class of the application, consisting of 3 policies
 * an input policy that reads in the configuration and map layout. An output policy
 * that decides how to display that output and a simulation policy that updates the
 * state. The roadster, then simply adapts these three policies.
 */
template <typename GraphicsPolicy, typename SimulationPolicy, typename LearnerPolicy>
class Roadster : private GraphicsPolicy, private SimulationPolicy, private LearnerPolicy
{
  using GraphicsPolicy::draw;        // Draw the simulation to output.
  using SimulationPolicy::update;    // Create an update tick, using the simulation policy.
  using LearnerPolicy::action;       // Gets fed back, the previous state, learns from that then decides an action.
  core::State m_state;               // Defines the actual state of the simulator.

public:
  /**
   * Constructor requires a filepath to generate a state.
   */
  Roadster() : GraphicsPolicy(m_state), m_state() {
  }

  /**
   * Default destructor, it makes sure that everything gets saved on exit and stuff gets
   * freed.
   */
  ~Roadster(void) {
  }

  /**
   * Run calls, update then draw, update blocks for the ticks specified in state.
   */
  void run(void) {
    // Create a null device to get time.
    IrrlichtDevice* null_device = createDevice(EDT_NULL);
    u32 before;
    u32 now;

    while (m_state.isRunning()) {
      // Calculate  before hand.
      before = null_device->getTimer()->getTime();
      //action(m_state); // Learner is given control to modify the state.
      update(m_state); // Update the current state.
      draw(m_state, true);   // Draw the updated state.
      // Get a new calculation 
      now = null_device->getTimer()->getTime() - before;
      // Sleep for a bit.
      if (now < 0.1) {
        // Don't bother updating, just busy wait on the drawing.
        draw(m_state, false); // Draw the updated state.
      } 
    }
  }
};

// Some useful typedefs, that build some standard approaches we want to utilise.
typedef Roadster<graphics::GraphicsPolicy3D, sim::SimulationPolicyNaive, ml::LearnerPolicyRL> RoadsterNaive;
typedef Roadster<graphics::GraphicsPolicy3D, sim::SimulationPolicyCellular, ml::LearnerPolicyRL> RoadsterCellular;

} // End of road namespace


#endif  // ROAD_ROADSTER_H_
