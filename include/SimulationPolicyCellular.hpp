#ifndef ROAD_SIM_SIMULATION_POLICY_CELLULAR_H_
#define ROAD_SIM_SIMULATION_POLICY_CELLULAR_H_

namespace road {

namespace core { class State; }

namespace sim {


/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @description Utilises the Nagel-Schaoesuaosuth cellular automata
 * approach.
 */
class SimulationPolicyCellular {
	protected:
		void update(core::State& state);
};

} // End of namespace sim.
} // End of namespace road.

#endif // ROAD_SIM_SIMULATION_POLICY_CELLULAR_H_
