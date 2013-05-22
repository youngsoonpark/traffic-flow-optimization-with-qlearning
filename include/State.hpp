#ifndef ROAD_CORE_STATE_H_
#define ROAD_CORE_STATE_H_

namespace road {
namespace core {

// Predefine a lane and an intersection.
class Road;
class Intersection;

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

	public:
		/**
		 * @description Default Constructor, sets running to true.
		 */	
		State();

		/**
		 * @description Default Destructor.
		 */
		~State();

		/**
		 * @description getter for m_running
		 * @return if the system is running or not.
		 */
		bool isRunning();

		/**
		 * @description settter for m_running
		 * @param new running value
		 */
		void setRunning(bool running);
};

} // End of namespace core.
} // End of namespace road.

#endif  // ROAD_CORE_STATE_H_
