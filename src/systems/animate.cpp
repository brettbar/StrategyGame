#include "animate.hpp"

namespace Animate
{

void Draw(entt::registry &registry, bool debug)
{
  entt::basic_view villagers = registry.view<Unit, Animated>();
  registry.sort<Unit>([](const Unit &lhs, const Unit &rhs) {
    return rhs.position.y > lhs.position.y;
  });

  villagers.each([debug](Unit &unit, Animated &anim) {
    DrawTextureRec(anim.sprite, anim.frameRec,
                   {unit.position.x - 64.0f, unit.position.y - 64.0f}, WHITE);

    if (debug && Vector2Distance(unit.position, unit.destination) > 0.5f)
      DrawLineEx(unit.position, unit.destination, 2, MAGENTA);
  });
}

void UpdateSprites(entt::registry &reg, f32 timeScale)
{
  auto view = reg.view<Animated, Unit>();

  for (auto &entity: view)
  {
    Animated &anim = view.get<Animated>(entity);

    anim.animTime += anim.animations[anim.state].speed * timeScale * 0.2;
    anim.currFrame =
      (i32) anim.animTime % anim.animations[anim.state].frameLength;

    anim.frameRec.x = (f32) anim.currFrame * 128;
    anim.frameRec.y = anim.state * 128;
  }
}

};// namespace Animate
