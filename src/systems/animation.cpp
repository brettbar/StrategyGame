#include "animation.hpp"

namespace Animation {

void Draw(entt::registry &registry, bool debug) {
  entt::basic_view villagers = registry.view<Unit>();
  registry.sort<Unit>([](const Unit &lhs, const Unit &rhs) {
    return rhs.position.y > lhs.position.y;
  });

  villagers.each([debug](Unit unit) {
    DrawTextureV(
        unit.sprite,
        {unit.position.x - 64.0f, unit.position.y - 64.0f},
        WHITE);
    if (debug && Vector2Distance(unit.position, unit.destination) > 0.5f) {
      DrawLineEx(unit.position, unit.destination, 2, MAGENTA);
    }
  });
}

void UpdateSprites(entt::registry &reg) {
  auto view = reg.view<Animated, Unit>();

  for (auto &entity : view) {
        
  }
}

}; // namespace Animation
