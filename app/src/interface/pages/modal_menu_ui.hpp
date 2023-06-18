#pragma once

#include "../../shared/common.hpp"

#include "../element.hpp"

namespace UI
{
  inline std::vector<Element> CreateModalMenuUI()
  {
    return {
      Panel( "modal_menu" )
        .Axis( Axis::Column )
        .Anchor( Anchor::Centered )
        .Children( {
          TextButton( "modal_menu_load_game" )
            .Text( "Load Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::ModalMenuLoadGame ),

          TextButton( "modal_menu_save_game" )
            .Text( "Save Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::ModalMenuSaveGame ),

          TextButton( "modal_menu_settings" )
            .Text( "Settings", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::ModalMenuSettings ),

          TextButton( "modal_menu_save_exit_main" )
            .Text( "Save and Exit to Main Menu", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::ModalMenuSaveExitMain ),

          TextButton( "modal_menu_exit_main" )
            .Text( "Exit to Main Menu", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::ModalMenuExitMain ),

          TextButton( "modal_menu_exit_game" )
            .Text( "Exit Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::ModalMenuExitGame ),
        } ),
    };
  }
};// namespace UI
