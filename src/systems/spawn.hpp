#pragma once

#include "../common.hpp"
#include "../components/animated.hpp"
#include "../components/sight.hpp"
#include "../components/unit.hpp"
#include "../player.hpp"
#include "../resource.hpp"

namespace Spawn
{

  void CreateNew(entt::registry &, TextureCache &, Vector2, std::shared_ptr<Player>);

  Texture2D DetermineTexture(Faction, TextureCache &);
};// namespace Spawn
