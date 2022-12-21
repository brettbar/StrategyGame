#pragma once

#include "../common.hpp"
#include "../components/event.hpp"
#include "../components/province.hpp"
#include "../components/selected.hpp"
#include "../components/unit.hpp"
#include "../global.hpp"
#include "../renderer/textures.hpp"
#include "../utils.hpp"


namespace SelectionSystem {

inline entt::entity selected_entity = entt::null;

template<typename T>
inline void ClearSelection( View<T> );
inline void CheckSelectUnits( vec2 );
inline void CheckSelectProvince( vec2 );

inline void Draw( TextureCache &cache, bool isDebug ) {
  auto unitsView = Global::world.view<Selected::Component, Unit::Component>();
  auto provsView =
    Global::world.view<Province::Component, Selected::Component>();

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
    auto &prov = provsView.get<Province::Component>( entity );

    if ( isDebug ) {
      DrawTexture(
        cache[hstr{ "tile_outline" }]->texture,
        prov.tile->position.x,
        prov.tile->position.y,
        WHITE
      );
      // DrawRectangleLinesEx(
      //   { prov.tile->position.x + 32, prov.tile->position.y + 32, 64, 64 },
      //   2,
      //   YELLOW );
    }
  }
}

inline void UpdateSelection( Vector2 click_pos ) {
  View<Unit::Component> units_view = Global::world.view<Unit::Component>();
  View<Province::Component> prov_view =
    Global::world.view<Province::Component>();

  ClearSelection<Unit::Component>( units_view );
  ClearSelection<Province::Component>( prov_view );

  CheckSelectUnits( click_pos );
  CheckSelectProvince( click_pos );
}

template<typename T>
inline void ClearSelection( View<T> component_view ) {
  // reg.clear<Selected::Component>();
  selected_entity = entt::null;

  for ( entt::entity entity: component_view ) {
    T &component = Global::world.get<T>( entity );
    component.selected = false;
    Global::world.remove<Selected::Component>( entity );
  }
}

inline void CheckSelectUnits( vec2 click_pos ) {
  auto units_view = Global::world.view<Unit::Component>();

  // use forward iterators and get only the components of interest
  for ( auto &entity: units_view ) {
    if ( selected_entity != entt::null )
      return;

    Unit::Component &unit = units_view.get<Unit::Component>( entity );

    if ( CheckCollisionPointCircle( unit.position, click_pos, 32 ) ) {
      Global::world.emplace<Selected::Component>( entity, true );

      std::cout << EntityIdToString( entity ) << std::endl;
      std::cout << EntityIdToString( unit.owner ) << std::endl;

      unit.selected = true;
      selected_entity = entity;
    }
  }
}

inline void CheckSelectProvince( vec2 click_pos ) {
  i32 tile_pos_id = DetermineTileIdFromPosition( click_pos );
  auto prov_view = Global::world.view<Province::Component>();

  if ( tile_pos_id == -1 )
    return;

  for ( auto &entity: prov_view ) {
    if ( selected_entity != entt::null )
      return;

    auto &prov = prov_view.get<Province::Component>( entity );

    if ( tile_pos_id == prov.tile->id ) {
      Global::world.emplace<Selected::Component>( entity, true );

      std::cout << EntityIdToString( entity ) << std::endl;
      std::cout << EntityIdToString( prov.owner ) << std::endl;

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
