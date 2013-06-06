#ifndef ROAD_CORE_STATE_H_
#define ROAD_CORE_STATE_H_

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>

namespace road {
namespace core {

/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @description A node is, either a source, sink or intersection. They behave
 *  much the same for the most part. Their explict behaviour is interpreted
 *  based on their type by the simulator. This is just a simple data store.
 */
struct Vertex {
	// Defines the val
	enum Type { SOURCE, SINK, INTERSECTION } type; // Defines the type.
	size_t x;         // X position, used in the graphics.
	size_t y;         // Y Position, using in the graphics.
};

/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @description A road represents an edge in the property graph, it contains
 *  a speed limit, a vector of cars, a source and a destination. We keep the
 *  source and destination to make it easier.
 */
struct Edge {
	size_t speed_limit;    // Speed limit.
//	std::vector<Car> cars; // Cars on the road.
};

/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @descripton The state controls all the data about the 
 * current state of the simulation, where all the cars
 * are located at what speed and in which lane. In additon
 * to this everything about the current state of the intersections
 * and the learner.
 */
class State {
	public:
	// Declare the relevant types
  typedef boost::labeled_graph<
            boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS, Vertex, Edge>,
            std::string
          > Graph;
  typedef boost::graph_traits<Graph> graph_traits;

		/**
		 * @description Constructs the initial state, it doesn't load
		 *  anything this is left up to the SerializationPolicy that
		 *  takes a reference and modifies the internals, as required.
		 *  It does, however set the core state to running.
		 */
		State();

		/**
		 * @description cleans up the internals data structures
		 *   used to represent the states.
		 */
		~State();

		/**
		 * @return if the state is running.
		 */
		bool isRunning();

		/**
		 * @param what to set the running state to.
		 */
		void setRunning(bool running);

		/**
		 * @return returns the adjacency list, with everything we need.
		 */
		Graph* getGraph();
  private:
    bool m_running;      // Whether the game is running or not, we set this to true at the start.
    size_t m_tick_speed; // Determines the speed of a tick in the simulator.
    Graph m_graph;       // The map, the actual graph of the entire scene. This is critical.
};

} // End of namespace core.
} // End of namespace road.

#endif  // ROAD_CORE_STATE_H_
