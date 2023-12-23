#pragma once

#include "../irongui/forge.hpp"

namespace UI {

  enum class Action_EditorMode {
    None,
    Tiles,
    GenerateMap,
  };

  inline Action_EditorMode EditorModePanel( Iron::IForge *, Iron::Element * );
  inline bool EditorMapGenerator( Iron::IForge *, Iron::Element * );

  inline bool Editor( f32 waterLvl ) {
    auto f = Iron::Forge();

    auto root_r = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_g = f->Grid( root_r, 4, 4 );

    auto action = EditorModePanel( f, root_g );

    // {
    //   auto grid = f->Grid( root_g->Slot( 7 ), 1, 1 );
    //   f->TextLabel( grid->Slot( 0 ), "Foo", RED );
    // }
    auto grid = f->Grid( root_g->Slot( 7 ), 2, 6 );
    f->TextLabel( grid->Slot( 0 ), "Map Generator", ORANGE );

    f->TextLabel( grid->Slot( 2 ), "waterLvl", GRAY );
    f->TextInput( grid->Slot( 3 ), std::to_string( waterLvl ).c_str(), BLUE );
    f->TextLabel( grid->Slot( 4 ), "seed", GRAY );
    f->TextLabel( grid->Slot( 6 ), "octaves", GRAY );
    f->TextLabel( grid->Slot( 8 ), "bias", GRAY );

    bool generate = f->TextButton( grid->Slot( 10 ), "generate", GREEN );
    return generate;
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