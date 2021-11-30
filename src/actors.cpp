#include "actors.h"

namespace Actors {

void Draw(entt::registry &registry, bool debug) {
  entt::basic_view villagers = registry.view<Actors::Unit>();
  registry.sort<Actors::Unit>([](const Actors::Unit &lhs, const Actors::Unit &rhs) {
    return rhs.position.y > lhs.position.y;
  });

  villagers.each([debug](Actors::Unit unit) {
    DrawTextureV(
        unit.sprite,
        {unit.position.x - 64.0f, unit.position.y - 64.0f},
        WHITE);
    if (debug && Vector2Distance(unit.position, unit.destination) > 0.5f) {
      DrawLineEx(unit.position, unit.destination, 2, MAGENTA);
    }
  });
}

void CreateNew(entt::registry &registry, Vector2 clickPos, u32 faction,
               std::map<str, Texture2D> textures) {
  entt::entity entity = registry.create();

  Vector2 *spawn = Map::determineTilePos(clickPos);

  Actors::Unit actor = Actors::Unit{
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

  registry.emplace<Actors::Unit>(
      entity, actor.position, actor.destination, actor.speed, actor.sprite);
}

void CreateNew(entt::registry &registry, Vector2 clickPos, Texture2D texture) {
  entt::entity entity = registry.create();

  Vector2 *spawn = Map::determineTilePos(clickPos);

  Actors::Unit actor = Actors::Unit{
      .speed = 1.0f,
      // .sprite = textures.at("romanVillagerTexture"),
  };

  if (spawn != nullptr)
    actor.position = *spawn;
  else
    actor.position = clickPos;

  actor.destination = actor.position;

  actor.sprite = texture;

  registry.emplace<Actors::Unit>(
      entity, actor.position, actor.destination, actor.speed, actor.sprite);
}


void UpdateSelection(entt::registry &registry, Vector2 clickPos) {
  auto view = registry.view<Actors::Unit>();

  registry.clear<Actors::Selected>();

  // use forward iterators and get only the components of interest
  for (auto entity : view) {
    Actors::Unit &unit= view.get<Actors::Unit>(entity);

    if (CheckCollisionPointCircle(unit.position, clickPos, 64)) {
      registry.emplace<Actors::Selected>(entity, true);
    }
  }
}

void SetDestinations(entt::registry &registry, Camera2D camera) {
  entt::basic_view view =
      registry.view<Actors::Unit, Actors::Selected>();

  for (auto entity : view) {
    Actors::Unit &unit= view.get<Actors::Unit>(entity);

    Vector2 *tileOrig =
        Map::determineTilePos(GetScreenToWorld2D(GetMousePosition(), camera));
    if (tileOrig != nullptr)
      unit.destination = *tileOrig;
  }
}

void UpdateMovement(entt::registry &registry, f32 timeScale) {
  entt::basic_view units = registry.view<Actors::Unit>();

  units.each([timeScale](Actors::Unit &unit) {
    if (Vector2Distance(unit.destination, unit.position) >
        0.5f) {
      Vector2 foo = {
          unit.destination.x - unit.position.x,
          unit.destination.y - unit.position.y,
      };

      Vector2 unitVec = Vector2Normalize(foo);
      unit.position.x +=
          unitVec.x * unit.speed * timeScale;
      unit.position.y +=
          unitVec.y * unit.speed * timeScale;
    }
  });
}

}; // namespace Actors
