#pragma once

#include "../../shared/common.hpp"
#include "../irongui/forge.hpp"
#include <raylib.h>


namespace UI
{

enum class Action_ModalMenu {
  None,
  LoadGame,
  SaveGame,
  Settings,
  ExitToMainMenu,
  ExitGame,
};

inline Action_ModalMenu ModalMenu() {
  auto f = Iron::Forge();
  rect root_r = rect{0,0,(f32)GetScreenWidth(), (f32)GetScreenHeight()};
  auto root_g = f->Grid( root_r, 3, 3 );

}
  // inline sptr<Element> CreateModalMenuUI()
  // {
  //   return {
  //     GridPanel( "model_menu_root", 3, 3 )
  //       .FixedSize( GetScreenWidth(), GetScreenHeight() )
  //       .Background( Fade( BLACK, 0.5 ) )
  //       .Children( {
  //         GridPanelElement::Slot{
  //           { 1, 1, 1, 1 },
  //           GridPanel( "modal_menu", 1, 6 )
  //             .Children( {
  //               GridPanelElement::Slot{
  //                 { 0, 0, 0, 0 },
  //                 TextButton( "modal_menu_load_game", "Load Game", 32 )
  //                   .Background( BLUE )
  //                   .SetEvent( InterfaceEvent::ID::ModalMenuLoadGame ),
  //               },
  //               GridPanelElement::Slot{
  //                 { 0, 0, 1, 1 },
  //                 TextButton( "modal_menu_save_game", "Save Game", 32 )
  //                   .Background( BLUE )
  //                   .SetEvent( InterfaceEvent::ID::ModalMenuSaveGame ),
  //               },
  //               GridPanelElement::Slot{
  //                 { 0, 0, 2, 2 },
  //                 TextButton( "modal_menu_settings", "Settings", 32 )
  //                   .Background( BLUE )
  //                   .SetEvent( InterfaceEvent::ID::ModalMenuSettings ),
  //               },
  //               GridPanelElement::Slot{
  //                 { 0, 0, 3, 3 },
  //                 TextButton(
  //                   "modal_menu_save_exit_main",
  //                   "Save and Exit to Main Menu",
  //                   32
  //                 )
  //                   .Background( BLUE )
  //                   .SetEvent( InterfaceEvent::ID::ModalMenuSaveExitMain ),
  //               },
  //               GridPanelElement::Slot{
  //                 { 0, 0, 4, 4 },
  //                 TextButton( "modal_menu_exit_main", "Exit to Main Menu", 32 )
  //                   .Background( BLUE )
  //                   .SetEvent( InterfaceEvent::ID::ModalMenuExitMain ),
  //               },
  //               GridPanelElement::Slot{
  //                 { 0, 0, 5, 5 },
  //                 TextButton( "modal_menu_exit_game", "Exit Game", 32 )
  //                   .Background( BLUE )
  //                   .SetEvent( InterfaceEvent::ID::ModalMenuExitGame ),
  //               },
  //             } ),
  //         },
  //       } ),
  //   };
  // }
};// namespace UI
