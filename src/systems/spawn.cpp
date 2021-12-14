#include "spawn.hpp"

namespace Spawn {

void CreateNew(entt::registry &registry, Vector2 clickPos, Texture2D texture) {
  entt::entity entity = registry.create();

  Vector2 *spawn = determineTilePos(clickPos);

  Unit actor = Unit{
      .speed = 1.0f,
      // .sprite = textures.at("romanVillagerTexture"),
  };

  if (spawn != nullptr)
    actor.position = *spawn;
  else
    actor.position = clickPos;

  actor.destination = actor.position;

  actor.sprite = texture;

  registry.emplace<Unit>(
      entity, actor.position, actor.destination, actor.speed, actor.sprite);
}


};

