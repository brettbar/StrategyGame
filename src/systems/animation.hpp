#pragma once

#include "../common.hpp"
#include "movement.hpp"
#include "../components/animated.hpp"

namespace Animation {

void Draw(entt::registry&, bool);
void UpdateSprites(entt::registry &);

};
