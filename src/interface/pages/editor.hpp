#pragma once

#include "../irongui/forge.hpp"

#include "../../world/managers/map_manager.hpp"

#include "../../world/systems/selection_system.hpp"

#include <raylib.h>
#include <string>

namespace UI {
  enum Mode : u32 {
    UI,
    MapGen,
    Tiles,
    Entities,
  };

  inline str mode_str( Mode mode ) {
    switch ( mode ) {
      case Mode::UI:
        return "UI";
      case Mode::MapGen:
        return "MapGen";
      case Mode::Tiles:
        return "Tiles";
      case Mode::Entities:
        return "Entitites";
    }
  }

  enum class EditorAction {
    None,
    GenerateMap,
  };

  bool mapgen_panel( Iron::Element *editor_g );
  void ui_panel( Iron::Element *editor_g );
  void tiles_panel( Iron::Element *editor_g );
  void entities_panel( Iron::Element *editor_g );

  inline EditorAction panel() {
    auto f = Iron::Forge();

    auto root_g = f->Grid(
      rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() }, 3, 3
    );

    auto editor_g = f->Grid( root_g->Slot( 2 ), 4, 4, Color{ 0, 0, 0, 155 } );

    auto mode_g = f->Grid( editor_g->ColsByRows( 0, 1, 1, 3 ), 1, 4 );
    auto tab_btns = {
      f->TextButton( mode_g->Slot( 0 ), "UI", YELLOW ),
      f->TextButton( mode_g->Slot( 1 ), "MapGen", BLUE ),
      f->TextButton( mode_g->Slot( 2 ), "Tiles", GREEN ),
      f->TextButton( mode_g->Slot( 3 ), "Entities", RED ),
    };
    auto current_tab = f->Tabs( tab_btns );
    Mode mode = (Mode) current_tab;

    // top bar
    {
      auto bar_g = f->Grid( editor_g->Row( 0 ), 3, 1, BLACK );
      f->TextLabel( bar_g->Slot( 0 ), "MODE: " + mode_str( mode ), CR_AQUA );
      f->TextLabel( bar_g->Slot( 1 ), "EDITOR", YELLOW );
      f->TextLabel(
        bar_g->Slot( 2 ), std::to_string( GetFPS() ) + " fps", BLACK
      );
    }

    switch ( mode ) {
      case Mode::UI:
        ui_panel( editor_g );
        break;
      case Mode::MapGen:
        if ( mapgen_panel( editor_g ) ) {
          return EditorAction::GenerateMap;
        }
        break;
      case Mode::Tiles:
        tiles_panel( editor_g );
        break;
      case Mode::Entities:
        entities_panel( editor_g );
        break;
    }

    return EditorAction::None;
  }


  /*==========================================================================
                                     PANELS
    ========================================================================*/


  inline bool mapgen_panel( Iron::Element *editor_g ) {
    auto f = Iron::Forge();

    auto map_g = f->Grid( editor_g->ColsByRows( 1, 4, 1, 4 ), 2, 8, BLACK );

    f->TextLabel( map_g->Row( 0 ), "Map Generator", ORANGE );


    {
      f->TextLabel( map_g->Slot( 2 ), "waterLvl", GRAY );
      str *new_water_lvl = f->TextInput(
        map_g->Slot( 3 ),
        std::to_string( Map::Manager()->waterLevel ).c_str(),
        WHITE
      );
      if ( new_water_lvl ) {
        Map::Manager()->waterLevel = (f32) atof( ( *new_water_lvl ).c_str() );
      }


      f->TextLabel( map_g->Slot( 4 ), "mtnsLvl", GRAY );
      str *new_mtn_lvl = f->TextInput(
        map_g->Slot( 5 ),
        std::to_string( Map::Manager()->mtnsLevel ).c_str(),
        WHITE
      );
      if ( new_mtn_lvl ) {
        Map::Manager()->mtnsLevel = (f32) atof( ( *new_mtn_lvl ).c_str() );
      }


      f->TextLabel( map_g->Slot( 6 ), "hillsLvl", GRAY );
      str *new_hills_lvl = f->TextInput(
        map_g->Slot( 7 ),
        std::to_string( Map::Manager()->hillsLevel ).c_str(),
        WHITE
      );
      if ( new_hills_lvl ) {
        Map::Manager()->hillsLevel = (f32) atof( ( *new_hills_lvl ).c_str() );
      }
    }

    f->TextLabel( map_g->Slot( 8 ), "seed", GRAY );
    str *new_seed = f->TextInput(
      map_g->Slot( 9 ), std::to_string( Map::Manager()->seed ), WHITE
    );
    if ( new_seed ) {
      Map::Manager()->seed = (f32) atof( ( *new_seed ).c_str() );
    }

    f->TextLabel( map_g->Slot( 10 ), "octaves", GRAY );
    str *new_octaves = f->TextInput(
      map_g->Slot( 11 ), std::to_string( Map::Manager()->octaves ), WHITE
    );
    if ( new_octaves ) {
      Map::Manager()->octaves = (f32) atof( ( *new_octaves ).c_str() );
    }

    f->TextLabel( map_g->Slot( 12 ), "bias", GRAY );
    str *new_bias = f->TextInput(
      map_g->Slot( 13 ), std::to_string( Map::Manager()->scaling_bias ), WHITE
    );
    if ( new_bias ) {
      Map::Manager()->scaling_bias = (f32) atof( ( *new_bias ).c_str() );
    }

    return f->TextButton( map_g->Row( 7 ), "generate", GREEN );
  }

  inline void ui_panel( Iron::Element *editor_g ) {
    auto f = Iron::Forge();

    auto ui_g = f->Grid( editor_g->ColsByRows( 1, 4, 1, 4 ), 1, 4, BLACK );

    f->TextLabel(
      ui_g->Slot( 0 ), ( "h:" + std::to_string( f->context.hot ) ), RED
    );
    f->TextLabel(
      ui_g->Slot( 1 ), ( "a:" + std::to_string( f->context.active ) ), RED
    );
  }

  inline void tiles_panel( Iron::Element *editor_g ) {
    auto f = Iron::Forge();

    auto tiles_g = f->Grid( editor_g->ColsByRows( 1, 4, 1, 4 ), 1, 4, BLACK );

    vec2f mouse_pos = GetMousePosition();
    vec2f world_pos = GetScreenToWorld2D( mouse_pos, Global::state.camera );
    sptr<vec2f> tile_pos = DetermineTilePos( world_pos );
    sptr<vec2u> tile_coords = DetermineTileCoords( world_pos );

    str tile_coords_str = "Nil";
    str tile_pos_str = "Nil";
    str tile_noise_str = "Nil";

    if ( tile_coords != nullptr ) {
      tile_coords_str = std::to_string( tile_coords->x ) + ", " +
                        std::to_string( tile_coords->y );
    }

    if ( tile_pos != nullptr ) {
      tile_pos_str =
        std::to_string( tile_pos->x ) + ", " + std::to_string( tile_pos->y );
    }

    if ( tile_coords != nullptr ) {
      tile_noise_str =
        std::to_string( Map::Manager()->get_noise( *tile_coords ) );
    }

    f->TextLabel( tiles_g->Slot( 0 ), "Tile Coords: " + tile_coords_str, GRAY );
    f->TextLabel( tiles_g->Slot( 1 ), "Tile Pos: " + tile_pos_str, GRAY );
    f->TextLabel( tiles_g->Slot( 2 ), "Tile Noise: " + tile_noise_str, GRAY );
  }

  inline void entities_panel( Iron::Element *editor_g ) {
    auto f = Iron::Forge();

    auto ent_g = f->Grid( editor_g->ColsByRows( 1, 4, 1, 4 ), 1, 4, BLACK );

    entt::entity selected = Selection::GetSelectedEntity();

    if ( Selection::Selected<Actor::Component>() ) {
      f->TextLabel( ent_g->Slot( 0 ), "Actor", RED );
      f->TextLabel(
        ent_g->Slot( 1 ), "ID: " + EntityIdToString( selected ), RED
      );
    } else if ( Selection::Selected<Province::Component>() ) {
      f->TextLabel( ent_g->Slot( 0 ), "Prov", BLUE );
      f->TextLabel(
        ent_g->Slot( 1 ), "ID: " + EntityIdToString( selected ), BLUE
      );
    }
  }
};// namespace UI
