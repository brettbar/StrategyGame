#include "spawn.hpp"

namespace Spawn
{

void CreateNew(entt::registry &registry, Vector2 clickPos, Texture2D texture)
{
  entt::entity entity = registry.create();

  Vector2 *spawn = determineTilePos(clickPos);

  Unit actor = Unit{
    .speed = 1.0f,
  };

  Animations animations = {
    {IDLE_DR, 2, 0.2f},
    {IDLE_DL, 2, 0.2f},
    {WALK_DL, 8, 0.8f},
    {WALK_DL, 8, 0.8f},
  };

  Animated animated = Animated{
    .sprite = texture,
    .frameRec = {0, 0, 128, 128},
    .state = IDLE_DR,
    .animations = animations,
    .direction = 0,
    .currFrame = 0,
    .animTime = 0.0f,
    .moving = false,
  };

  if (spawn != nullptr) actor.position = *spawn;
  else
    actor.position = clickPos;

  actor.destination = actor.position;

  registry.emplace<Unit>(entity, actor);
  registry.emplace<Animated>(entity, animated);
}


};// namespace Spawn
