#pragma once

#include "../../shared/common.hpp"

#include "../builders.hpp"
#include "../element.hpp"
#include <raylib.h>

namespace UI
{

  inline sptr<Element> CreateMainMenuUI()
  {
    return GridPanel( "main_menu_root", 3, 3 )
      .FixedSize( GetScreenWidth(), GetScreenHeight() )
      .Background( BLACK )
      // .SetChild(
      //   { 1 },
      //   { 1 },
      //   GridPanel( "main_menu", 1, 3 ).FixedSize( 200, 600 ).Background( GREEN )
      // )
      ;
  }
};// namespace UI

// TextButton( "main_menu_host_game" )
//   .Text( "Host Game", 32 )
//   .Background( BLUE )
//   .SetEvent( InterfaceEvent::ID::MainMenuHostGame ),
// TextButton( "main_menu_join_game" )
//   .Text( "Join Game", 32 )
//   .Background( BLUE )
//   .SetEvent( InterfaceEvent::ID::MainMenuJoinGame ),
// TextButton( "main_menu_start_game" )
//   .Text( "Start Game", 32 )
//   .Background( BLUE )
//   .SetEvent( InterfaceEvent::ID::MainMenuStartGame ),
// TextButton( "main_menu_load_game" )
//   .Text( "Load Game", 32 )
//   .Background( BLUE )
//   .SetEvent( InterfaceEvent::ID::MainMenuLoadGame ),
// TextButton( "main_menu_settings" )
//   .Text( "Settings", 32 )
//   .Background( BLUE )
//   .SetEvent( InterfaceEvent::ID::MainMenuSettings ),
// TextButton( "main_menu_exit_game" )
//   .Text( "Exit Game", 32 )
//   .Background( BLUE )
//   .SetEvent( InterfaceEvent::ID::MainMenuExitGame ),
