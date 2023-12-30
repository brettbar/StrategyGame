#pragma once

#include "../irongui/forge.hpp"

#include "../../world/systems/map_system.hpp"
#include "../../world/systems/selection_system.hpp"
#include <raylib.h>

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
    auto tabs = f->Tabs( tab_btns );
    Mode mode = (Mode) tabs->t.tabs->current_tab;

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

    auto map_g = f->Grid( editor_g->ColsByRows( 1, 4, 1, 4 ), 2, 6, BLACK );


    f->TextLabel( map_g->Row( 0 ), "Map Generator", ORANGE );

    f->TextLabel( map_g->Slot( 2 ), "waterLvl", GRAY );
    f->TextInput(
      map_g->Slot( 3 ), std::to_string( MapSystem::waterLevel ).c_str(), WHITE
    );
    f->TextLabel( map_g->Slot( 4 ), "seed", GRAY );
    f->TextLabel( map_g->Slot( 6 ), "octaves", GRAY );
    f->TextLabel( map_g->Slot( 8 ), "bias", GRAY );

    return f->TextButton( map_g->Row( 5 ), "generate", GREEN );
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

    std::string tile_coords_str = "Nil";
    std::string tile_pos_str = "Nil";

    if ( tile_coords != nullptr ) {
      tile_coords_str = std::to_string( tile_coords->x ) + ", " +
                        std::to_string( tile_coords->y );
    }

    if ( tile_pos != nullptr ) {
      tile_pos_str =
        std::to_string( tile_pos->x ) + ", " + std::to_string( tile_pos->y );
    }

    f->TextLabel( tiles_g->Slot( 0 ), "Tile Coords: " + tile_coords_str, GRAY );
    f->TextLabel( tiles_g->Slot( 1 ), "Tile Pos: " + tile_pos_str, GRAY );
    // f->TextLabel( tiles_g->Slot( 2 ), "Tile Noise: " + tile_noise_str, GRAY );
  }

  inline void entities_panel( Iron::Element *editor_g ) {
    auto f = Iron::Forge();

    auto ent_g = f->Grid( editor_g->ColsByRows( 1, 4, 1, 4 ), 1, 4, BLACK );

    entt::entity selected = SelectionSystem::GetSelectedEntity();

    if ( SelectionSystem::Selected<Actor::Component>() ) {
      f->TextLabel( ent_g->Slot( 0 ), "Actor", RED );
      f->TextLabel(
        ent_g->Slot( 1 ), "ID: " + EntityIdToString( selected ), RED
      );
    } else if ( SelectionSystem::Selected<Province::Component>() ) {
      f->TextLabel( ent_g->Slot( 0 ), "Prov", BLUE );
      f->TextLabel(
        ent_g->Slot( 1 ), "ID: " + EntityIdToString( selected ), BLUE
      );
    }
  }
};// namespace UI