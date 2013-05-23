#include "State.hpp"
#include "Road.hpp"
#include "Intersection.hpp"

namespace road {
namespace core {

State::State() : m_running(true) {
}

State::~State() {
}

bool State::isRunning() {
	return m_running;
}

void State::setRunning(bool running) {
	m_running = running;
}

} // End of namespace core.
} // End of namespace road.

