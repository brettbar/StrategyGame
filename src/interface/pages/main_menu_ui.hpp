#pragma once

#include "../../shared/common.hpp"

#include "../irongui/forge.hpp"
#include <raylib.h>


namespace UI {

  enum class Action_MainMenu {
    None,
    StartGame,
    LoadGame,
    HostGame,
    JoinGame,
    Settings,
    ExitGame,
  };


  inline Action_MainMenu DrawMainMenu() {
    auto f = Iron::Forge();

    rect root_r = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_g = f->Grid( root_r, 3, 3 );

    auto grid = f->Grid( root_g->Slot( 4 ), 1, 6 );

    bool start_pressed = f->TextButton( grid->Slot( 0 ), "Start Game", BLUE );
    bool load_pressed = f->TextButton( grid->Slot( 1 ), "Load Game", BLUE );
    bool host_pressed = f->TextButton( grid->Slot( 2 ), "Host Game", BLUE );
    bool join_pressed = f->TextButton( grid->Slot( 3 ), "Join Game", BLUE );
    bool settings_pressed = f->TextButton( grid->Slot( 4 ), "Settings", BLUE );
    bool exit_pressed = f->TextButton( grid->Slot( 5 ), "Exit Game", BLUE );


    // str foo = "h: " + std::to_string( Iron::Watcher()->context.hot ) +
    //           ", a: " + std::to_string( Iron::Watcher()->context.active ) +
    //           "\n";

    // printf( "%s", foo.c_str() );


    if ( start_pressed ) {
      printf( "Start Game!!\n" );
      return Action_MainMenu::StartGame;
    }

    if ( load_pressed ) {
      printf( "Load Game!!\n" );
      return Action_MainMenu::LoadGame;
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

    return Action_MainMenu::None;
  }


};// namespace UI
