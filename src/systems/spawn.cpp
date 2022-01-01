#include "spawn.hpp"
#include "../entities/archetypes.hpp"

namespace Spawn
{

  void CreateNew(entt::registry &reg, TextureCache &cache, Vector2 clickPos, std::shared_ptr<Player> currPlayer)
  {
    std::unique_ptr<Vector2> spawn = DetermineTilePos(clickPos);
    assert(spawn != nullptr);

    entt::entity entity = reg.create();

    Texture tex = cache.handle(currPlayer->textureMap.at("Villager"))->texture;
    Archetypes::Actor actor = Archetypes::Actor(tex, *spawn);

    reg.emplace<Unit>(entity, actor.unit);
    reg.emplace<Animated>(entity, actor.animated);
    reg.emplace<Sight>(entity, actor.sight);
  }
};// namespace Spawn
