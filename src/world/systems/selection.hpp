#pragma once

#include "../../common.hpp"
#include "../components/province.hpp"
#include "../components/selected.hpp"
#include "../components/unit.hpp"
#include <raylib.h>


namespace Selection {

inline void Draw( entt::registry &reg, bool isDebug ) {
  auto unitsView = reg.view<c_Selected::Selected, c_Unit::Unit>();
  auto provsView = reg.view<c_Selected::Selected, c_Province::Province>();

  for ( auto entity: unitsView ) {
    c_Unit::Unit &unit = unitsView.get<c_Unit::Unit>( entity );
    if ( isDebug ) {
      DrawRectangleLinesEx(
        { unit.position.x - 32, unit.position.y - 32, 64, 64 },
        2,
        YELLOW );
    }
  }

  for ( auto entity: provsView ) {
    c_Province::Province &prov = provsView.get<c_Province::Province>( entity );
    if ( isDebug ) {
      DrawRectangleLinesEx(
        { prov.tile->position.x - 32, prov.tile->position.y - 64, 64, 64 },
        2,
        YELLOW );
    }
  }
}

inline void UpdateSelection( entt::registry &reg, Vector2 clickPos ) {
  auto unitsView = reg.view<c_Unit::Unit>();
  auto provView = reg.view<c_Province::Province>();

  reg.clear<c_Selected::Selected>();

  bool alreadyFoundOne = false;

  // use forward iterators and get only the components of interest
  for ( auto entity: unitsView ) {
    if ( alreadyFoundOne )
      return;

    c_Unit::Unit &unit = unitsView.get<c_Unit::Unit>( entity );

    if ( CheckCollisionPointCircle( unit.position, clickPos, 64 ) ) {
      reg.emplace<c_Selected::Selected>( entity, true );
      alreadyFoundOne = true;
    }
  }

  for ( auto entity: provView ) {
    if ( alreadyFoundOne )
      return;

    c_Province::Province &prov = provView.get<c_Province::Province>( entity );

    if ( CheckCollisionPointCircle( prov.tile->position, clickPos, 64 ) ) {
      reg.emplace<c_Selected::Selected>( entity, true );
      alreadyFoundOne = true;
    }
  }
}

};// namespace Selection
