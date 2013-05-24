#ifndef ROAD_CORE_STATE_H_
#define ROAD_CORE_STATE_H_

#include <boost/graph/adjacency_list.hpp>

namespace road {
namespace core {

struct Car {
	int speed;  // Speed of the car.
};

enum NodeType {
	SOURCE,
	SINK,
	INTERSECTION
};

struct Node {
	NodeType type;    // Source, Sink or Intersection.
	size_t x;         // X position.
	size_t y;         // Y Position.
};

struct Road {
	size_t speed_limit;    // Speed limit.
	std::vector<Car> cars; // Cars on the road.
};

/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @descripton The state controls all the data about the 
 * current state of the simulation, where all the cars
 * are located at what speed and in which lane. In additon
 * to this everything about the current state of the intersections
 * and the learner.
 *
 */
class State {
	bool m_running; // Whether the game is running or not, we set this to true at the start.
	typedef boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS, Node, Road> Map;
	Map m_map;
	public:
		State();
		~State();
		bool isRunning();
		void setRunning(bool running);
		Map& getMap();
};

} // End of namespace core.
} // End of namespace road.

#endif  // ROAD_CORE_STATE_H_
