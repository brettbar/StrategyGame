#pragma once

#include "../../shared/common.hpp"

#include "../ui_builder.hpp"

namespace UI
{

  inline std::vector<Element> CreateMainMenuUI()
  {
    return {
      Panel( "main_menu" )
        .SetAxis( Axis::Column )
        .SetAnchor( Anchor::Centered )
        .Background( GREEN )
        .Children( {
          TextButton( "main_menu_host_game" )
            .SetText( "Host Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::MainMenuHostGame )
            .build(),
          TextButton( "main_menu_join_game" )
            .SetText( "Join Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::MainMenuJoinGame )
            .build(),
          TextButton( "main_menu_start_game" )
            .SetText( "Start Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::MainMenuStartGame )
            .build(),
          TextButton( "main_menu_load_game" )
            .SetText( "Load Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::MainMenuLoadGame )
            .build(),
          TextButton( "main_menu_settings" )
            .SetText( "Settings", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::MainMenuSettings )
            .build(),
          TextButton( "main_menu_exit_game" )
            .SetText( "Exit Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::MainMenuExitGame )
            .build(),
        } )
        .build(),
    };
  }
};// namespace UI
