#pragma once

#include "../common.hpp"
#include "../components/event.hpp"
#include "../components/province.hpp"
#include "../components/selected.hpp"
#include "../components/unit.hpp"
#include "../events.hpp"
#include "../global.hpp"
#include "../renderer/textures.hpp"
#include <raylib.h>


namespace SelectionSystem {

inline entt::entity selected_entity = entt::null;

template<typename T>
inline void ClearSelection( View<T> );
inline void CheckSelectUnits( View<Unit::Component>, vec2 );
inline void CheckSelectProvince( View<Province::Component>, vec2 );

inline void Draw( TextureCache &cache, bool isDebug ) {
  auto unitsView =
    Global::registry.view<Selected::Component, Unit::Component>();
  auto provsView =
    Global::registry.view<Selected::Component, Province::Component>();

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

inline void UpdateSelection( Vector2 click_pos ) {
  View<Unit::Component> units_view = Global::registry.view<Unit::Component>();
  View<Province::Component> prov_view =
    Global::registry.view<Province::Component>();

  ClearSelection( units_view );
  ClearSelection( prov_view );

  CheckSelectUnits( units_view, click_pos );
  CheckSelectProvince( prov_view, click_pos );
}

template<typename T>
inline void ClearSelection( View<T> component_view ) {
  // reg.clear<Selected::Component>();
  selected_entity = entt::null;

  for ( entt::entity entity: component_view ) {
    T &component = Global::registry.get<T>( entity );
    component.selected = false;

    Global::registry.remove<Selected::Component>( entity );
  }
}

inline void
CheckSelectUnits( View<Unit::Component> units_view, vec2 click_pos ) {
  // use forward iterators and get only the components of interest
  for ( auto &entity: units_view ) {
    if ( selected_entity != entt::null )
      return;

    Unit::Component &unit = units_view.get<Unit::Component>( entity );

    if ( CheckCollisionPointCircle( unit.position, click_pos, 32 ) ) {
      Global::registry.emplace<Selected::Component>( entity, true );


      unit.selected = true;
      selected_entity = entity;
    }
  }
}

inline void
CheckSelectProvince( View<Province::Component> prov_view, vec2 click_pos ) {
  u32 tile = DetermineTileIdFromClick( click_pos );

  for ( auto &entity: prov_view ) {
    if ( selected_entity != entt::null )
      return;

    Province::Component &prov = prov_view.get<Province::Component>( entity );

    if ( tile == prov.tile->id ) {
      Global::registry.emplace<Selected::Component>( entity, true );

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
