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
                TextButton(
                  "main_menu_host_game",
                  "Host Game",
                  32,
                  InterfaceEvent::ID::MainMenuHostGame
                )
                  .Background( BLUE ),
              },
              GridPanelElement::Slot{
                { 0, 0, 1, 1 },
                TextButton(
                  "main_menu_join_game",
                  "Join Game",
                  32,
                  InterfaceEvent::ID::MainMenuJoinGame
                )
                  .Background( BLUE ),
              },
              GridPanelElement::Slot{
                { 0, 0, 2, 2 },
                TextButton(
                  "main_menu_start_game",
                  "Start Game",
                  32,
                  InterfaceEvent::ID::MainMenuStartGame
                )
                  .Background( BLUE ),
              },
              GridPanelElement::Slot{
                { 0, 0, 3, 3 },
                TextButton(
                  "main_menu_load_game",
                  "Load Game",
                  32,
                  InterfaceEvent::ID::MainMenuLoadGame
                )
                  .Background( BLUE ),
              },
              GridPanelElement::Slot{
                { 0, 0, 4, 4 },
                TextButton(
                  "main_menu_settings",
                  "Settings",
                  32,
                  InterfaceEvent::ID::MainMenuSettings
                )
                  .Background( BLUE ),
              },
              GridPanelElement::Slot{
                { 0, 0, 5, 5 },
                TextButton(
                  "main_menu_exit_game",
                  "Exit Game",
                  32,
                  InterfaceEvent::ID::MainMenuExitGame
                )
                  .Background( BLUE ),
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
