#pragma once

#include "../../shared/common.hpp"

#include "../immediate.hpp"
#include <raylib.h>


namespace UI {

  enum class Action_MainMenu {
    None,
    StartGame,
    HostGame,
    JoinGame,
    Settings,
    ExitGame,
  };


  inline Action_MainMenu DrawMainMenu() {
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
      return Action_MainMenu::StartGame;
    }


    if ( host_pressed ) {
      printf( "Host Game!!\n" );
      return Action_MainMenu::HostGame;
    }

    if ( join_pressed ) {
      printf( "Join Game!!\n" );
      return Action_MainMenu::JoinGame;
    }

    if ( settings_pressed ) {
      printf( "Settings!!\n" );
      return Action_MainMenu::Settings;
    }

    if ( exit_pressed ) {
      printf( "Exit Game!!\n" );
      return Action_MainMenu::ExitGame;
    }

    f.Draw();

    return Action_MainMenu::None;
  }


};// namespace UI
