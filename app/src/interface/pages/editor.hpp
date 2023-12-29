#pragma once

#include "../irongui/forge.hpp"
#include <raylib.h>

namespace UI {

  enum class ActionEditorMode {
    None,
    GenerateMap,
    ModeTiles,
    ModeUI,
  };

  inline void top_bar( Iron::Element * );
  inline ActionEditorMode EditorModePanel( Iron::Element * );
  inline bool EditorMapGenerator( Iron::IForge *, Iron::Element * );
  inline bool map_gen( Iron::Element *, f32 );

  inline ActionEditorMode Editor( f32 waterLvl ) {
    auto f = Iron::Forge();

    auto root_g = f->Grid(
      rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() }, 3, 3
    );

    auto editor_g = f->Grid( root_g->Slot( 2 ), 4, 4, Color{ 0, 0, 0, 155 } );


    top_bar( editor_g );

    if ( EditorModePanel( editor_g ) != ActionEditorMode::None ) {
      // change mode
    }

    // if ( map_gen( editor_g, waterLvl ) ) {
    //   return ActionEditorMode::GenerateMap;
    // }


    return ActionEditorMode::None;
  }

  inline void top_bar( Iron::Element *editor_g ) {
    auto f = Iron::Forge();

    auto bar_g = f->Grid( editor_g->Row( 0 ), 3, 1, BLACK );

    f->TextLabel( bar_g->Slot( 0 ), "MODE: ", GREEN );
    f->TextLabel( bar_g->Slot( 1 ), "EDITOR", YELLOW );
    f->TextLabel(
      bar_g->Slot( 2 ), std::to_string( GetFPS() ) + " fps", BLACK
    );
  }

  inline ActionEditorMode EditorModePanel( Iron::Element *editor_g ) {
    auto f = Iron::Forge();

    auto mode_g = f->Grid( editor_g->ColsByRows( 0, 1, 1, 3 ), 1, 4 );

    if ( f->TextButton( mode_g->Slot( 0 ), "Tiles", BLUE ) )
      return ActionEditorMode::ModeTiles;

    if ( f->TextButton( mode_g->Slot( 1 ), "UI", YELLOW ) )
      return ActionEditorMode::ModeTiles;

    return ActionEditorMode::None;
  }

  // inline void DebugInfo( Iron::Element *editor_g ) {
  //   auto f = Iron::Forge();

  //   f->TextLabel(
  //     editor_g->Slot( 5 ), ( "h:" + std::to_string( f->context.hot ) ), RED
  //   );
  //   f->TextLabel(
  //     editor_g->Slot( 7 ), ( "a:" + std::to_string( f->context.active ) ), RED
  //   );
  // }

  inline bool map_gen( Iron::Element *root_g, f32 waterLvl ) {
    auto f = Iron::Forge();
    auto editor_g = f->Grid( root_g->Slot( 7 ), 2, 6 );
    f->TextLabel( editor_g->Slot( 0 ), "Map Generator", ORANGE );

    f->TextLabel( editor_g->Slot( 2 ), "waterLvl", GRAY );
    f->TextInput(
      editor_g->Slot( 3 ), std::to_string( waterLvl ).c_str(), BLUE
    );
    f->TextLabel( editor_g->Slot( 4 ), "seed", GRAY );
    f->TextLabel( editor_g->Slot( 6 ), "octaves", GRAY );
    f->TextLabel( editor_g->Slot( 8 ), "bias", GRAY );

    return f->TextButton( editor_g->Slot( 10 ), "generate", GREEN );
  }

};// namespace UI