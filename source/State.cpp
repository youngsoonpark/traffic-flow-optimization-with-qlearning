#include "State.hpp"

namespace road {
    namespace core {

        State::State() : m_running(true), m_lights(Lights::Horisontal), m_maxCars(50) {
        }

        State::~State() {
        }

        bool State::isRunning() {
            return m_running;
        }

        void State::setRunning(bool running) {
            m_running = running;
        }

        Graph* State::getGraph() {
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
} // End of namespace road.

