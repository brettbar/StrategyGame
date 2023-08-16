#pragma once

#include "../../shared/common.hpp"

#include "../element.hpp"
#include <raylib.h>

namespace UI
{

  inline sptr<Element> CreateMainMenuUI()
  {
    return GridPanel( "main_menu_root", 3, 3 )
      .FixedSize( GetScreenWidth(), GetScreenHeight() )
      .Background( GRAY )
      .SetChildren( {
        GridPanelElement::Slot{
          { 1, 1, 1, 1 },
          GridPanel( "main_menu", 1, 6 )
            .Background( GREEN )
            .SetChildren( {
              GridPanelElement::Slot{
                { 0, 0, 0, 0 },
                TextLabel( "main_menu_host_game", "Host Game", 32 ),
              },
            } ),
        },
      } );
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
