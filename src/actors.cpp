#include "actors.h"

namespace Actors {

void Draw(entt::registry &registry, bool debug) {
  entt::basic_view villagers = registry.view<Actors::RomanVillager>();
  registry.sort<Actors::RomanVillager>([](const Actors::RomanVillager &lhs, const Actors::RomanVillager &rhs) {
    return rhs.position.y > lhs.position.y;
  });

  villagers.each([debug](Actors::RomanVillager romanVillager) {
    DrawTextureV(
        romanVillager.sprite,
        {romanVillager.position.x - 64.0f, romanVillager.position.y - 64.0f},
        WHITE);
    if (debug && Vector2Distance(romanVillager.position, romanVillager.destination) > 0.5f) {
      DrawLineEx(romanVillager.position, romanVillager.destination, 2, MAGENTA);
    }
  });
}

void CreateNew(entt::registry &registry, Vector2 clickPos, u32 faction,
               std::map<std::string, Texture2D> textures) {
  entt::entity entity = registry.create();

  Vector2 *spawn = Map::determineTilePos(clickPos);

  Actors::RomanVillager actor = Actors::RomanVillager{
      .speed = 1.0f,
      // .sprite = textures.at("romanVillagerTexture"),
  };

  if (spawn != nullptr)
    actor.position = *spawn;
  else
    actor.position = clickPos;

  actor.destination = actor.position;

  switch (faction) {
  case 0:
    actor.sprite = textures.at("romanVillagerTexture");
    break;
  case 1:
    actor.sprite = textures.at("greekVillagerTexture");
    break;
  case 2:
    actor.sprite = textures.at("celtVillagerTexture");
    break;
  case 3:
    actor.sprite = textures.at("punicVillagerTexture");
    break;
  case 4:
    actor.sprite = textures.at("persianVillagerTexture");
    break;

  default:
    actor.sprite = textures.at("romanVillagerTexture");
    break;
  }

  registry.emplace<Actors::RomanVillager>(
      entity, actor.position, actor.destination, actor.speed, actor.sprite);
}

void UpdateSelection(entt::registry &registry, Vector2 clickPos) {
  auto view = registry.view<Actors::RomanVillager>();

  registry.clear<Actors::Selected>();

  // use forward iterators and get only the components of interest
  for (auto entity : view) {
    Actors::RomanVillager &villager = view.get<Actors::RomanVillager>(entity);

    if (CheckCollisionPointCircle(villager.position, clickPos, 64)) {
      registry.emplace<Actors::Selected>(entity, true);
    }
  }
}

void SetDestinations(entt::registry &registry, Camera2D camera) {
  entt::basic_view view =
      registry.view<Actors::RomanVillager, Actors::Selected>();

  for (auto entity : view) {
    Actors::RomanVillager &villager = view.get<Actors::RomanVillager>(entity);

    Vector2 *tileOrig =
        Map::determineTilePos(GetScreenToWorld2D(GetMousePosition(), camera));
    if (tileOrig != nullptr)
      villager.destination = *tileOrig;
  }
}

void UpdateMovement(entt::registry &registry, f32 timeScale) {
  entt::basic_view villagers = registry.view<Actors::RomanVillager>();

  villagers.each([timeScale](Actors::RomanVillager &romanVillager) {
    if (Vector2Distance(romanVillager.destination, romanVillager.position) >
        0.5f) {
      Vector2 foo = {
          romanVillager.destination.x - romanVillager.position.x,
          romanVillager.destination.y - romanVillager.position.y,
      };

      Vector2 unitVec = Vector2Normalize(foo);
      romanVillager.position.x +=
          unitVec.x * romanVillager.speed * timeScale;
      romanVillager.position.y +=
          unitVec.y * romanVillager.speed * timeScale;
    }
  });
}

}; // namespace Actors
