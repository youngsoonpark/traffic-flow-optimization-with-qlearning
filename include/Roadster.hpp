#ifndef ROAD_ROADSTER_H_
#define ROAD_ROADSTER_H_

#include  <string>

// Import the core state.
#include "State.hpp"
// Include the valid policies.
#include "GraphicsPolicy3D.hpp"
#include "SerializationPolicyXML.hpp"
#include "LearnerPolicyRL.hpp"
#include "SimulationPolicyCellular.hpp"
#include "SimulationPolicyNaive.hpp"

namespace road {

/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @description This is the core class of the application, consisting of 3 policies
 * an input policy that reads in the configuration and map layout. An output policy
 * that decides how to display that output and a simulation policy that updates the
 * state. The roadster, then simply adapts these three policies.
 */
template <typename SerializationPolicy, typename GraphicsPolicy, typename SimulationPolicy, typename LearnerPolicy>
class Roadster: private SerializationPolicy, private GraphicsPolicy, private SimulationPolicy, private LearnerPolicy {
	using GraphicsPolicy::draw;        // Draw the simulation to output.
	using SimulationPolicy::update;    // Create an update tick, using the simulation policy.
	using LearnerPolicy::action;       // Gets fed back, the previous state, learns from that then decides an action.
	core::State m_state;               // Defines the actual state of the simulator.

	public:
		using SerializationPolicy::load;   // Parse the input configuration file.
		using SerializationPolicy::save;   // Parse the input configuration file.

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
			save(m_state); // Save the outgoing state.
		}

		/**
		 * Run calls, update then draw, update blocks for the ticks specified in state.
		 */
		void run(void) {
			while (m_state.isRunning()) {
				action(m_state); // Learner is given control to modify the state.
				update(m_state); // Update the current state.
				draw(m_state);   // Draw the updated state.
			}
		}
};

// Some useful typedefs, that build some standard approaches we want to utilise.
typedef Roadster<io::SerializationPolicyXML, graphics::GraphicsPolicy3D, sim::SimulationPolicyNaive, ml::LearnerPolicyRL> RoadsterNaive;
typedef Roadster<io::SerializationPolicyXML, graphics::GraphicsPolicy3D, sim::SimulationPolicyCellular, ml::LearnerPolicyRL> RoadsterCellular;

} // End of road namespace


#endif  // ROAD_ROADSTER_H_
