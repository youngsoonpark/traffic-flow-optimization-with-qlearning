#ifndef ROAD_SIM_INTERSECTION_NODE_H_
#define ROAD_SIM_INTERSECTION_NODE_H_

#include "Lane.hpp"

namespace road {
namespace sim {

/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @description An intersection, connects lanes, to other lanes in the traffic
 * graph. This is the actuator, for the learner.
 */
class IntersectionNode {
	public:
		InsersectionNode();
		~IntersectionNode();
};

} // End of namespace sim.
} // End of namespace road.

#endif // ROAD_SIM_INTERSECTION_NODE_H_
