#pragma once

#include "../common.hpp"
#include "../components/animated.hpp"
#include "movement.hpp"

namespace Animate
{

void Draw(entt::registry &, bool);
void UpdateSprites(entt::registry &, f32);

};// namespace Animate
