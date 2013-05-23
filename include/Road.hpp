#ifndef ROAD_CORE_ROAD_H_
#define ROAD_CORE_ROAD_H_

#include <vector>
#include <deque>

namespace road {
namespace core {

/**
 * POD struct holding data about the lane.
 * We utilise a deque in this situation because each tick, at most 1 element is
 * inserted to the head and 1 is deleted from the tail. We can preform these
 * operations in constant time when utilising a deque.
 */
struct Road {
	std::size_t max_capacity;   // Each lane can have a maximum capacity.
	std::size_t speed_limit;    // Each lane can have a maximum speed limit.
	std::size_t lanes;          // The number of lanes.
	std::vector< std::deque<int> > cells; // A vector, of lanes.
};

} // End of namspace core.
} // End of namespace road.

#endif  // ROAD_CORE_ROAD_H_
