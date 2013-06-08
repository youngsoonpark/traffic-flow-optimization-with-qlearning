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

State::Graph* State::getGraph() {
    return &m_graph;
}

State::Lights State::getLights() const {
    return m_lights;
}

void State::setLights(Lights lights) {
    m_lights = lights;
}

int8_t State::feedback() const {
    if (m_lights == Lights::Horisontal) {
        // return the number of cars stopped in vertical edges * -1
    } else if (m_lights == Lights::Vertical) {
        // return the number of cars stopped in horisontal edges * -1
    } else {
        return 0;
    }
}

} // End of namespace core.
} // End of namespace road.

