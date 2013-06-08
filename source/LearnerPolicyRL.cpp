#include "LearnerPolicyRL.hpp"
#include "assert.h"

namespace road {
namespace ml {

LearnerPolicyRL::LearnerPolicyRL(): learning_rate(DEFAULT_LEARNING_RATE),
                                    discount_factor(DEFAULT_DISCOUNT_FACTOR),
                                    exploration_rate(DEFAULT_EXPLORATION_RATE) {
    // TODO Initialise the huge array
}

LearnerPolicyRL::~LearnerPolicyRL() {
    // TODO Free the huge array
}

void LearnerPolicyRL::setLearningRate(double new_learning_rate) {
    assert(new_learning_rate >= 0 && new_learning_rate <= 1);
    learning_rate = new_learning_rate;
}

void LearnerPolicyRL::setExplorationRate(double new_exploration_rate) {
    assert(new_exploration_rate >= 0 && new_exploration_rate <= 1);
    exploration_rate = new_exploration_rate;
}

void LearnerPolicyRL::setDiscountFactor(double new_discount_factor) {
    assert(new_discount_factor >= 0 && new_discount_factor < 1);
    discount_factor = new_discount_factor;
}

void LearnerPolicyRL::action(core::State& state) {
    // TODO Do q-learning
}	

} // End of namespace ml.
} // End of namespace road.
