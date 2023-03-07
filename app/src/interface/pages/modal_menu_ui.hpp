#pragma once

#include "../../shared/common.hpp"

#include "../ui_builder.hpp"

namespace UI
{
  inline std::vector<Element> CreateModalMenuUI()
  {
    return {
      Panel( "modal_menu" )
        .SetAxis( Axis::Column )
        .SetAnchor( Anchor::Centered )
        .Children( {
          TextButton( "modal_menu_load_game" )
            .SetText( "Load Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::ModalMenuLoadGame )
            .build(),

          TextButton( "modal_menu_save_game" )
            .SetText( "Save Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::ModalMenuSaveGame )
            .build(),

          TextButton( "modal_menu_settings" )
            .SetText( "Settings", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::ModalMenuSettings )
            .build(),

          TextButton( "modal_menu_save_exit_main" )
            .SetText( "Save and Exit to Main Menu", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::ModalMenuSaveExitMain )
            .build(),

          TextButton( "modal_menu_exit_main" )
            .SetText( "Exit to Main Menu", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::ModalMenuExitMain )
            .build(),

          TextButton( "modal_menu_exit_game" )
            .SetText( "Exit Game", 32 )
            .Background( BLUE )
            .SetEvent( InterfaceEvent::ID::ModalMenuExitGame )
            .build(),
        } )
        .build(),
    };
  }
};// namespace UI
