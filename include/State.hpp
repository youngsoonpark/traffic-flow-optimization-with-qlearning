#ifndef ROAD_CORE_STATE_H_
#define ROAD_CORE_STATE_H_

#include "Lane.hpp"
#include "Intersection.hpp"

namespace road {
namespace core {

class State {
	bool m_running;
	public:
		State() : m_running(true) {}
		~State() {}
		bool isRunning() { return m_running; }
		void setRunning(bool running) { m_running = running; }
};

} // End of namespace core.
} // End of namespace road.

#endif  // ROAD_CORE_STATE_H_
