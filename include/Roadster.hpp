#ifndef ROAD_ROADSTER_H_
#define ROAD_ROADSTER_H_

#include  <string>

// Import the core state.
#include "State.hpp"
// Include the valid policies.
#include "GraphicsPolicy3D.hpp"
#include "InputPolicyText.hpp"
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
template <typename InputPolicy, typename GraphicsPolicy, typename SimulationPolicy, typename LearnerPolicy>
class Roadster: private InputPolicy, private GraphicsPolicy, private SimulationPolicy, private LearnerPolicy {
	using InputPolicy::parse;       // Parse the input configuration file.
	using GraphicsPolicy::draw;     // Draw the simulation to output.
	using SimulationPolicy::update; // Create an update tick, using the simulation policy.
	using LearnerPolicy::action;    // Gets fed back, the previous state, learns from that then decides an action.
	core::State m_state;             // Defines the actual state of the simulator.

	public:
		/**
		 * Constructor requires a filepath to generate a state.
		 */
		Roadster(const std::string& filepath) : m_state() {
			parse(m_state);  // Parse the configuration file, return a state.
		}

		/**
		 * Default destructor.
		 */
		~Roadster(void) {}

		/**
		 * Run calls, update then draw, update blocks for the ticks specified in state.
		 */
		void run(void) {
			while (true) {
				action(m_state); // Learner is given control to modify the state.
				update(m_state); // Update the current state.
				draw(m_state);   // Draw the updated state.
			}
		}
};

// Some useful typedefs, that build some standard approaches we want to utilise.
typedef Roadster<io::InputPolicyText, graphics::GraphicsPolicy3D, sim::SimulationPolicyNaive, ml::LearnerPolicyRL> RoadsterNaive;
typedef Roadster<io::InputPolicyText, graphics::GraphicsPolicy3D, sim::SimulationPolicyCellular, ml::LearnerPolicyRL> RoadsterCellular;

} // End of road namespace


#endif  // ROAD_ROADSTER_H_
