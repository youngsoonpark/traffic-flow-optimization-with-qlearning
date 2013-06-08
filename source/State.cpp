#include "State.hpp"

namespace road {
<<<<<<< HEAD
    namespace core {

        State::State(const int& maxCars) : m_running(true), m_lights(Lights::Horisontal), m_maxCars(maxCars) {
            noCar.badboyFactor = 1.0;
            noCar.speed = 1;
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

        State::Lights State::getLights() const
        {
            return m_lights;
        }

        void State::setLights(State::Lights lights)
        {
            m_lights = lights;
        }

        /*std::vector<int8_t> State::identity() const
        {
            std::vector<int8_t>() x;
            return x;
        }*/

    } // End of namespace core.
=======
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
>>>>>>> 2e33383836b177744c7e9f6d026b803a3527297a
} // End of namespace road.

