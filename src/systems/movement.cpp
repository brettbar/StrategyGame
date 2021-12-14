#include "movement.hpp"

namespace Movement {

void SetDestinations(entt::registry &registry, Camera2D camera) {
  entt::basic_view view =
      registry.view<Unit, Selected>();

  for (auto entity : view) {
    Unit &unit= view.get<Unit>(entity);

    Vector2 *tileOrig =
        determineTilePos(GetScreenToWorld2D(GetMousePosition(), camera));
    if (tileOrig != nullptr)
      unit.destination = *tileOrig;
  }
}

void Update(entt::registry &registry, f32 timeScale) {
  entt::basic_view units = registry.view<Unit>();

  units.each([timeScale](Unit &unit) {
    if (Vector2Distance(unit.destination, unit.position) >
        0.7f) {
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
