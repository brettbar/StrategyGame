#pragma once

#include "../common.hpp"
#include "../components/event.hpp"
#include "../components/province.hpp"
#include "../components/selected.hpp"
#include "../components/unit.hpp"
#include "../renderer/textures.hpp"
#include "event_system.hpp"
#include <raylib.h>


namespace SelectionSystem {

inline entt::entity selected_entity = entt::null;

template<typename T>
inline void ClearSelection( entt::registry &, view<T> );
inline void CheckSelectUnits( entt::registry &, view<Unit::Component>, vec2 );
inline void
CheckSelectProvince( entt::registry &, view<Province::Component>, vec2 );

inline void Draw( entt::registry &reg, TextureCache &cache, bool isDebug ) {
  auto unitsView = reg.view<Selected::Component, Unit::Component>();
  auto provsView = reg.view<Selected::Component, Province::Component>();

  // for ( auto entity: unitsView ) {
  //   Unit::Component &unit = unitsView.get<Unit::Component>( entity );
  //   if ( isDebug ) {

  //     // DrawRectangleLinesEx(
  //     //   { unit.position.x - 32, unit.position.y - 32, 64, 64 },
  //     //   2,
  //     //   YELLOW );
  //   }
  // }

  for ( auto entity: provsView ) {
    Province::Component &prov = provsView.get<Province::Component>( entity );
    if ( isDebug ) {
      DrawTexture(
        cache[hstr{ "tile_outline" }]->texture,
        prov.tile->position.x,
        prov.tile->position.y,
        WHITE );
      // DrawRectangleLinesEx(
      //   { prov.tile->position.x + 32, prov.tile->position.y + 32, 64, 64 },
      //   2,
      //   YELLOW );
    }
  }
}

inline void UpdateSelection( entt::registry &reg, Vector2 click_pos ) {
  view<Unit::Component> units_view = reg.view<Unit::Component>();
  view<Province::Component> prov_view = reg.view<Province::Component>();

  ClearSelection( reg, units_view );
  ClearSelection( reg, prov_view );

  CheckSelectUnits( reg, units_view, click_pos );
  CheckSelectProvince( reg, prov_view, click_pos );
}

template<typename T>
inline void ClearSelection( entt::registry &reg, view<T> component_view ) {
  // reg.clear<Selected::Component>();
  selected_entity = entt::null;

  for ( entt::entity entity: component_view ) {
    T &component = reg.get<T>( entity );
    component.selected = false;

    reg.remove<Selected::Component>( entity );
  }
}

inline void CheckSelectUnits(
  entt::registry &reg,
  view<Unit::Component> units_view,
  vec2 click_pos ) {
  // use forward iterators and get only the components of interest
  for ( auto &entity: units_view ) {
    if ( selected_entity != entt::null )
      return;

    Unit::Component &unit = units_view.get<Unit::Component>( entity );

    if ( CheckCollisionPointCircle( unit.position, click_pos, 32 ) ) {
      reg.emplace<Selected::Component>( entity, true );


      unit.selected = true;
      selected_entity = entity;
    }
  }
}

inline void CheckSelectProvince(
  entt::registry &reg,
  view<Province::Component> prov_view,
  vec2 click_pos ) {
  u32 tile = DetermineTileIdFromClick( click_pos );

  for ( auto &entity: prov_view ) {
    if ( selected_entity != entt::null )
      return;

    Province::Component &prov = prov_view.get<Province::Component>( entity );

    if ( tile == prov.tile->id ) {
      reg.emplace<Selected::Component>( entity, true );

      selected_entity = entity;
    }

    // if ( CheckCollisionPointCircle(
    //        Vector2{ prov.tile->position.x + 32, prov.tile->position.y + 32 },
    //        clickPos,
    //        64 ) ) {


    //   reg.emplace<Selected::Component>( entity, true );

    //   alreadyFoundOne = true;
    // }
  }
}


};// namespace SelectionSystem
