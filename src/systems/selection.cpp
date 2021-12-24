#include "selection.hpp"

namespace Selection {

void UpdateSelection(entt::registry &registry, Vector2 clickPos) {
  auto view = registry.view<Unit>();

  registry.clear<Selected>();

  // use forward iterators and get only the components of interest
  for (auto entity : view) {
    Unit &unit = view.get<Unit>(entity);

    if (CheckCollisionPointCircle(unit.position, clickPos, 64)) {
      registry.emplace<Selected>(entity, true);
    }
  }
}

};
