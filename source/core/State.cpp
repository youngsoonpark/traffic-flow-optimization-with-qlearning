#include "core/State.hpp"

namespace road
{
namespace core
{

State::State() : m_running(true), m_paused(false), m_lights(Lights::HORIZONTAL), m_delay(0), m_maxCars(50)
{
}

State::~State()
{
}

bool State::isRunning()
{
  return m_running;
}

void State::setRunning(bool running)
{
  m_running = running;
}

bool State::isPaused() {
  return m_paused;
}

void State::setPaused(bool paused) {
  m_paused = paused;
}

Graph* State::getGraph()
{
  return &m_graph;
}

State::Lights State::getLights() const
{
  return m_lights;
}

void State::setLights(State::Lights lights)
{
  if (m_lights == lights)
  {
    m_delay++;
  }
  else
  {
    m_lights = lights;
    m_delay = 0;
  }
}

uint8_t State::getDelay() {
  return m_delay;
}

int8_t State::getMaxCars() const
{
  return m_maxCars;
}
} // End of namespace core.
} // End of namespace road.

