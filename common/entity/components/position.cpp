#include "common/entity/components/position.hpp"

namespace entity {

std::string PositionComponent::getComponentName() { return "position"; }

std::string PositionComponent::getName() { return "position"; }

Component * PositionComponent::new_() { return new PositionComponent(); }

} // namespace entity
