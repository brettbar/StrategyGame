#pragma once

#include "../../common.hpp"
#include "../components/unit.hpp"


namespace Selection {

  inline void UpdateSelection(entt::registry &registry, Vector2 clickPos) {
    auto view = registry.view<Components::Unit>();

    registry.clear<Components::Selected>();

    // use forward iterators and get only the components of interest
    for (auto entity: view) {
      Components::Unit &unit = view.get<Components::Unit>(entity);

      if (CheckCollisionPointCircle(unit.position, clickPos, 64)) {
        registry.emplace<Components::Selected>(entity, true);
      }
    }
  }

};// namespace Selection
