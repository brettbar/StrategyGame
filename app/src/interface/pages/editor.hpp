#pragma once

#include "../irongui/forge.hpp"
#include <raylib.h>

namespace UI {

  struct Editor {
    Editor() {}
    ~Editor() {}

    Editor( Editor const & ) = delete;
    void operator=( const Editor & ) = delete;


    static Editor *Instance() {
      static Editor instance;
      return &instance;
    }

    enum class Mode {
      UI,
      Tiles,
    };

    str mode_str( Mode mode ) {
      switch ( mode ) {
        case Mode::UI:
          return "UI";
        case Mode::Tiles:
          return "Tiles";
      }
    }

    enum class Action {
      None,
      GenerateMap,
    };

    Mode _mode = Mode::UI;


    Action Panel( f32 waterLvl ) {
      auto f = Iron::Forge();

      auto root_g = f->Grid(
        rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() }, 3, 3
      );

      auto editor_g = f->Grid( root_g->Slot( 2 ), 4, 4, Color{ 0, 0, 0, 155 } );


      top_bar( editor_g );

      mode_panel( editor_g );

      switch ( _mode ) {
        case Mode::UI:
          break;
        case Mode::Tiles:
          if ( map_gen( root_g, waterLvl ) ) {
            return Action::GenerateMap;
          }
          break;
      }


      return Action::None;
    }

    void top_bar( Iron::Element *editor_g ) {
      auto f = Iron::Forge();

      auto bar_g = f->Grid( editor_g->Row( 0 ), 3, 1, BLACK );


      f->TextLabel( bar_g->Slot( 0 ), "MODE: " + mode_str( _mode ), GREEN );
      f->TextLabel( bar_g->Slot( 1 ), "EDITOR", YELLOW );
      f->TextLabel(
        bar_g->Slot( 2 ), std::to_string( GetFPS() ) + " fps", BLACK
      );
    }

    void mode_panel( Iron::Element *editor_g ) {
      auto f = Iron::Forge();

      auto mode_g = f->Grid( editor_g->ColsByRows( 0, 1, 1, 3 ), 1, 4 );

      if ( f->TextButton( mode_g->Slot( 0 ), "UI", YELLOW ) ) {
        _mode = Mode::UI;
      }

      if ( f->TextButton( mode_g->Slot( 1 ), "Tiles", BLUE ) ) {
        _mode = Mode::Tiles;
        return;
      }
    }

    bool map_gen( Iron::Element *root_g, f32 waterLvl ) {
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
  };


  // inline void DebugInfo( Iron::Element *editor_g ) {
  //   auto f = Iron::Forge();

  //   f->TextLabel(
  //     editor_g->Slot( 5 ), ( "h:" + std::to_string( f->context.hot ) ), RED
  //   );
  //   f->TextLabel(
  //     editor_g->Slot( 7 ), ( "a:" + std::to_string( f->context.active ) ), RED
  //   );
  // }


};// namespace UI