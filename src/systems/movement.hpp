#pragma once

#include "../components/unit.hpp"

namespace Movement {

void SetDestinations(entt::registry&, Camera2D);
void Update(entt::registry&, f32);

}; // namespace Units
