#pragma once

#include "../../shared/common.hpp"

#include "../element.hpp"
#include <raylib.h>

namespace UI
{
  inline sptr<Element> CreateModalMenuUI()
  {
    return {
      GridPanel( "model_menu_root", 3, 3 )
        .FixedSize( GetScreenWidth(), GetScreenHeight() )
        .Background( Fade( BLACK, 0.5 ) )
        .SetChildren( {
          GridPanelElement::Slot{
            { 1, 1, 1, 1 },
            GridPanel( "modal_menu", 1, 6 )
              .SetChildren( {
                GridPanelElement::Slot{
                  { 0, 0, 0, 0 },
                  TextButton( "modal_menu_load_game", "Load Game", 32 )
                    .Background( BLUE )
                    .SetEvent( InterfaceEvent::ID::ModalMenuLoadGame ),
                },
                GridPanelElement::Slot{
                  { 0, 0, 1, 1 },
                  TextButton( "modal_menu_save_game", "Save Game", 32 )
                    .Background( BLUE )
                    .SetEvent( InterfaceEvent::ID::ModalMenuSaveGame ),
                },
                GridPanelElement::Slot{
                  { 0, 0, 2, 2 },
                  TextButton( "modal_menu_settings", "Settings", 32 )
                    .Background( BLUE )
                    .SetEvent( InterfaceEvent::ID::ModalMenuSettings ),
                },
                GridPanelElement::Slot{
                  { 0, 0, 3, 3 },
                  TextButton(
                    "modal_menu_save_exit_main",
                    "Save and Exit to Main Menu",
                    32
                  )
                    .Background( BLUE )
                    .SetEvent( InterfaceEvent::ID::ModalMenuSaveExitMain ),
                },
                GridPanelElement::Slot{
                  { 0, 0, 4, 4 },
                  TextButton( "modal_menu_exit_main", "Exit to Main Menu", 32 )
                    .Background( BLUE )
                    .SetEvent( InterfaceEvent::ID::ModalMenuExitMain ),
                },
                GridPanelElement::Slot{
                  { 0, 0, 5, 5 },
                  TextButton( "modal_menu_exit_game", "Exit Game", 32 )
                    .Background( BLUE )
                    .SetEvent( InterfaceEvent::ID::ModalMenuExitGame ),
                },
              } ),
          },
        } ),
    };
  }
};// namespace UI
