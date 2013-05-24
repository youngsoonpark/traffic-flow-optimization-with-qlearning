#include "State.hpp"

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

State::Map& State::getMap() {
	return m_map;			
}

} // End of namespace core.
} // End of namespace road.

