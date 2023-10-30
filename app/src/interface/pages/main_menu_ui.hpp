#pragma once

#include "../../shared/common.hpp"

#include "../builders.hpp"
#include "../immediate.hpp"
#include <raylib.h>


namespace UI {


  inline void DrawMainMenu() {
    auto f = Iron::Forge();

    rect root = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_grid = f.Grid( root, 3, 3 );

    auto grid = f.Grid( root_grid->Slot( 4 ), 1, 5 );

    bool start_pressed = f.TextButton( grid->Slot( 0 ), "Start Game", BLUE );
    bool host_pressed = f.TextButton( grid->Slot( 1 ), "Host Game", BLUE );
    bool join_pressed = f.TextButton( grid->Slot( 2 ), "Join Game", BLUE );
    bool settings_pressed = f.TextButton( grid->Slot( 3 ), "Settings", BLUE );
    bool exit_pressed = f.TextButton( grid->Slot( 4 ), "Exit Game", BLUE );

    if ( start_pressed ) {
      printf( "Start Game!!\n" );
    }


    if ( host_pressed ) {
      printf( "Host Game!!\n" );
    }

    if ( join_pressed ) {
      printf( "Join Game!!\n" );
    }

    if ( settings_pressed ) {
      printf( "Settings!!\n" );
    }

    if ( exit_pressed ) {
      printf( "Exit Game!!\n" );
    }

    f.Draw();
  }


};// namespace UI
