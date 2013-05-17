

namespace road {

namespace core {class State;} // Prototype of the state.

namespace io {


/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 */
struct InputPolicyText {
protected:
	/**
	 * @param state the state of the simulation
	 */
	void parse(core::State& state);
};

} // End of namespace io.
} // End of namespace road.
