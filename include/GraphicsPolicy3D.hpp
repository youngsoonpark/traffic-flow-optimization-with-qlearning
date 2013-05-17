#ifndef ROAD_GRAPHICS_POLICY_3D_H_
#define ROAD_GRAPHICS_POLICY_3D_H_

namespace road {
namespace graphics{

class State; // Prototype of state class.

/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @description Takes a state and renders it to the screne in Haskell this
 * would be in the io monad, essentially this is just a side effect! It shouldn't
 * modify the state.
 */
class GraphicsPolicy3D {
	protected:
		void draw(const State& state) const;
};

} // End of namespace out.
} // End of namespace road.

#endif // ROAD_GRAPHICS_POLICY_3D_H_
