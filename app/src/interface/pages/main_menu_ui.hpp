#pragma once

#include "../../shared/common.hpp"

#include "../element.hpp"

namespace UI
{

  inline std::vector<Element> CreateMainMenuUI()
  {
    return {
      Panel( "main_menu" )
        .Axis( Axis::Column )
        .SetAnchor( Anchor::Centered )
        .Background( GREEN )
        .Children( {
          TextButton( "main_menu_host_game" )
            .SetText( "Host Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::MainMenuHostGame ),
          TextButton( "main_menu_join_game" )
            .SetText( "Join Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::MainMenuJoinGame ),
          TextButton( "main_menu_start_game" )
            .SetText( "Start Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::MainMenuStartGame ),
          TextButton( "main_menu_load_game" )
            .SetText( "Load Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::MainMenuLoadGame ),
          TextButton( "main_menu_settings" )
            .SetText( "Settings", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::MainMenuSettings ),
          TextButton( "main_menu_exit_game" )
            .SetText( "Exit Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::MainMenuExitGame ),
        } ),
    };
  }
};// namespace UI
