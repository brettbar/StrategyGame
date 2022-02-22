#pragma once

#include "../../common.hpp"
#include "../components/animated.hpp"
#include "../components/sight.hpp"
#include "../components/unit.hpp"
#include "../entities/archetypes.hpp"

#include "../../resource.hpp"
#include "../player.hpp"

namespace Spawn
{

Texture2D DetermineTexture( Faction, TextureCache & );

inline void CreateNew(
  entt::registry &reg,
  TextureCache &cache,
  Vector2 clickPos,
  std::shared_ptr<Player> currPlayer )
{
  std::unique_ptr<Vector2> spawn = DetermineTilePos( clickPos );
  assert( spawn != nullptr );

  entt::entity entity = reg.create();

  Texture tex =
    cache.handle( currPlayer->textureMap.at( "Villager" ) )->texture;
  Archetypes::Actor actor = Archetypes::Actor( tex, *spawn );

  reg.emplace<Components::Unit>( entity, actor.unit );
  reg.emplace<Components::Animated>( entity, actor.animated );
  reg.emplace<Components::Sight>( entity, actor.sight );
}
};// namespace Spawn
