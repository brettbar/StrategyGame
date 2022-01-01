#pragma once

#include "../common.hpp"
#include "../player.hpp"
#include "../resource.hpp"
#include "../components/unit.hpp"
#include "../components/animated.hpp"
#include "../components/sight.hpp"

namespace Spawn {

void CreateNew(entt::registry&, TextureCache&, Vector2, std::shared_ptr<Player>);

Texture2D DetermineTexture(Faction, TextureCache&);
};

