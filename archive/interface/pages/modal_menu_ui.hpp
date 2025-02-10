#pragma once

#include "../../shared/common.hpp"
#include "../irongui/forge.hpp"
#include <raylib.h>


namespace UI
{

enum class Action_ModalMenu {
  None,
  SaveGame,
  LoadGame,
  Settings,
  ExitToMainMenu,
  ExitGame,
};

inline Action_ModalMenu DrawModalMenu() {
  auto f = Iron::Forge();
  rect root_r = rect{0,0,(f32)GetScreenWidth(), (f32)GetScreenHeight()};
  auto root_g = f->Grid( root_r, 3, 3 );

  auto grid = f->Grid( root_g->Slot( 4 ), 1, 5 );

  bool save_pressed = f->TextButton( grid->Slot( 0 ), "Save Game", BLUE );
  bool load_pressed = f->TextButton( grid->Slot( 1 ), "Load Game", BLUE );
  bool settings_pressed = f->TextButton( grid->Slot( 2 ), "Settings", BLUE );
  bool exit_main_pressed = f->TextButton( grid->Slot( 3 ), "Exit To Main Menu", BLUE );
  bool exit_game_pressed = f->TextButton( grid->Slot( 4 ), "Exit Game", BLUE );

  if (save_pressed) {
    return Action_ModalMenu::SaveGame;
  }
  if (load_pressed) {
    return Action_ModalMenu::LoadGame;
  }
  if (settings_pressed) {
    return Action_ModalMenu::Settings;
  }
  if (exit_main_pressed) {
    return Action_ModalMenu::ExitToMainMenu;
  }
  if (exit_game_pressed) {
    return Action_ModalMenu::ExitGame;
  }
  
  return Action_ModalMenu::None;
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
