#include "ml/LearnerPolicyDumb.hpp"

namespace road
{
namespace ml
{

void LearnerPolicyDumb::action(core::State& state)
{
  if (state.getDelay() >= PHASE_TIME)
  {
    state.setLights(static_cast<core::State::Lights>(1 - state.getLights())); // Change the lights
  }
  else
  {
    state.setLights(state.getLights());   // Update the delay
  }
}

} // End of namespace ml.
} // End of namespace road.
