#pragma once

#include "../components/animated.hpp"
#include "../components/unit.hpp"

namespace Movement
{
  inline void SetDestinations(entt::registry &registry, Camera2D camera)
  {
    entt::basic_view view =
      registry.view<Components::Unit, Components::Animated, Components::Selected>();

    for (auto entity: view)
    {
      Components::Unit &unit = view.get<Components::Unit>(entity);
      Components::Animated &anim = view.get<Components::Animated>(entity);

      std::unique_ptr<Vector2> tileOrig =
        DetermineTilePos(GetScreenToWorld2D(GetMousePosition(), camera));

      if (tileOrig != nullptr)
      {
        unit.destination = *tileOrig;

        if (unit.destination.x > unit.position.x)
          anim.direction = Components::IDLE_DR;
        else if (unit.destination.x < unit.position.x)
          anim.direction = Components::IDLE_DL;
      }
    }
  }

  inline void Update(entt::registry &registry, f32 timeScale)
  {
    entt::basic_view units = registry.view<Components::Unit, Components::Animated>();

    for (auto &entity: units)
    {
      Components::Unit &unit = units.get<Components::Unit>(entity);
      Components::Animated &anim = units.get<Components::Animated>(entity);

      if (Vector2Distance(unit.destination, unit.position) > 0.7f)
      {
        Vector2 unitVec = Vector2Normalize({
          unit.destination.x - unit.position.x,
          unit.destination.y - unit.position.y,
        });

        unit.position.x +=
          unitVec.x * unit.speed * timeScale;
        unit.position.y +=
          unitVec.y * unit.speed * timeScale;

        if (unit.destination.x > unit.position.x)
          anim.state = Components::WALK_DR;
        else if (unit.destination.x < unit.position.x)
          anim.state = Components::WALK_DL;

        if (Vector2Distance(unit.destination, unit.position) <= 0.7f)
        {
          unit.position = unit.destination;

          if (anim.direction == 0)
            anim.state = Components::IDLE_DR;
          else if (anim.direction == 1)
            anim.state = Components::IDLE_DL;
        }
      }
    }
  }

};// namespace Movement
