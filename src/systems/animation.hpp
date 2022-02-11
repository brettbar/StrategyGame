#pragma once

#include "../common.hpp"
#include "../components/animated.hpp"
#include "movement.hpp"

namespace Animation
{

  // void Draw(entt::registry &, bool);
  // void UpdateSprites(entt::registry &, f32);

  inline void Draw(entt::registry &registry, bool debug)
  {
    entt::basic_view villagers = registry.view<Unit, Animated>();
    registry.sort<Unit>([](const Unit &lhs, const Unit &rhs) {
      return rhs.position.y > lhs.position.y;
    });

    villagers.each([debug](Unit &unit, Animated &anim) {
      //    DrawTextureV(
      //        unit.sprite,
      //        {unit.position.x - 64.0f, unit.position.y - 64.0f},
      //        WHITE);

      DrawTextureRec(
        anim.sprite,
        anim.frameRec,
        {unit.position.x - 64.0f, unit.position.y - 64.0f},
        WHITE);

      if (debug && Vector2Distance(unit.position, unit.destination) > 0.5f)
      {
        DrawLineEx(unit.position, unit.destination, 2, MAGENTA);
      }
    });
  }

  inline void UpdateSprites(entt::registry &reg, f32 timeScale)
  {
    auto view = reg.view<Animated, Unit>();

    for (auto &entity: view)
    {
      Animated &anim = view.get<Animated>(entity);

      float animSpeed = 0.18f;
      anim.animTime += anim.animations[anim.state].speed * timeScale * animSpeed;
      anim.currFrame = (int) anim.animTime % anim.animations[anim.state].frameLength;

      anim.frameRec.x = (f32) anim.currFrame * 128;
      anim.frameRec.y = anim.state * 128;
    }
  }

};// namespace Animation
