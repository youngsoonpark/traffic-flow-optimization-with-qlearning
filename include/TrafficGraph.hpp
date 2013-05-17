#ifndef ROAD_SIM_TRAFFIC_GRAPH_H_
#define ROAD_SIM_TRAFFIC_GRAPH_H_

#include "LaneNode.hpp"
#include "IntersectionNode.hpp"

namespace road {
namespace sim {

/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @description This is the core component of the traffic simulatior, it connects
 * the lanes to the intersections, from this representation we can build the view,
 * modify the entire traffic simulation etc. The state is essentially at its core this.
 */
class TrafficGraph {
	public:
		TrafficGraph();
		~TrafficGraph();
};

} // End of namespace sim.
} // End of namespace road.

#endif // ROAD_SIM_TRAFFIC_GRAPH_H_
