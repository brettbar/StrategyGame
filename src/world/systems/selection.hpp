#pragma once

#include "../../common.hpp"
#include "../components/unit.hpp"
#include <raylib.h>


namespace Selection
{

inline void Draw( entt::registry &reg, bool isDebug )
{
  auto view = reg.view<Components::Selected, Components::Unit>();

  for ( auto entity: view )
  {
    Components::Unit &unit = view.get<Components::Unit>( entity );
    if ( isDebug )
      DrawRectangleLinesEx(
        { unit.position.x - 32, unit.position.y - 32, 64, 64 },
        2,
        YELLOW );
  }
}

inline void UpdateSelection( entt::registry &reg, Vector2 clickPos )
{
  auto view = reg.view<Components::Unit>();

  reg.clear<Components::Selected>();

  bool alreadyFoundOne = false;

  // use forward iterators and get only the components of interest
  for ( auto entity: view )
  {
    if ( alreadyFoundOne ) return;

    Components::Unit &unit = view.get<Components::Unit>( entity );

    if ( CheckCollisionPointCircle( unit.position, clickPos, 64 ) )
    {
      reg.emplace<Components::Selected>( entity, true );
      alreadyFoundOne = true;
    }
  }
}

};// namespace Selection
