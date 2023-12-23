#pragma once

#include "../irongui/forge.hpp"

namespace UI {

  enum class Action_EditorMode {
    None,
    Tiles,
  };

  inline Action_EditorMode EditorModePanel( Iron::IForge *, Iron::Element * );

  inline void Editor() {
    auto f = Iron::Forge();

    auto root_r = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_g = f->Grid( root_r, 4, 4 );

    auto action = EditorModePanel( f, root_g );

    {
      auto grid = f->Grid( root_g->Slot( 7 ), 1, 1 );
      f->TextLabel( grid->Slot( 0 ), "Foo", RED );
    }
  }

  inline Action_EditorMode EditorModePanel(
    Iron::IForge *f,
    Iron::Element *root_g
  ) {

    auto grid = f->Grid( root_g->Slot( 3 ), 2, 4 );
    f->TextLabel( grid->Slot( 0 ), "Editor Modes", ORANGE );
    bool tile_mode = f->TextButton( grid->Slot( 2 ), "Tiles", BLUE );
    if ( tile_mode )
      return Action_EditorMode::Tiles;

    return Action_EditorMode::None;
  }

};// namespace UI