#include "actors.h"

namespace Actors {

void Draw(entt::registry &registry) {
  entt::basic_view villagers = registry.view<Actors::RomanVillager>();
  villagers.each([](Actors::RomanVillager romanVillager) {
    DrawTextureV(
        romanVillager.sprite,
        {romanVillager.position.x - 64.0f, romanVillager.position.y - 64.0f},
        WHITE);
  });
}

void CreateNew(entt::registry &registry, Vector2 clickPos,
               Texture2D romanVillagerTexture) {
  entt::entity entity = registry.create();

  Actors::RomanVillager roman = Actors::RomanVillager{
      .position = clickPos,
      .destination = clickPos,
      .speed = 1.0f,
      .sprite = romanVillagerTexture,
  };

  registry.emplace<Actors::RomanVillager>(
      entity, roman.position, roman.destination, roman.speed, roman.sprite);
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

void UpdateMovement(entt::registry &registry) {
  entt::basic_view villagers = registry.view<Actors::RomanVillager>();

  villagers.each([](Actors::RomanVillager &romanVillager) {
    if (Vector2Distance(romanVillager.destination, romanVillager.position) >
        0.5f) {
      Vector2 foo = {
          romanVillager.destination.x - romanVillager.position.x,
          romanVillager.destination.y - romanVillager.position.y,
      };

      Vector2 unitVec = Vector2Normalize(foo);
      romanVillager.position.x += unitVec.x * romanVillager.speed;
      romanVillager.position.y += unitVec.y * romanVillager.speed;
    }
  });
}


}; // namespace Actors
