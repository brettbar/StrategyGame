#include "../../shared/common.hpp"
#include "clay/clay.h"
#include <cstdio>
#include <cstring>
#include <raylib.h>

namespace UI {
  enum class Action_MainMenu {
    None,
    StartGame,
    LoadGame,
    HostGame,
    JoinGame,
    Settings,
    ExitGame,
  };


  // inline void HandleButtonInteraction(
  //   Clay_ElementId elementId,
  //   Clay_PointerData pointerData,
  //   intptr_t userData
  // ) {
  //   printf( "cwddasndakshdlkashdlaksjhdlkajshd ourse there we go!\n" );
  //   if ( pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME ) {
  //     printf( "Of course there we go!\n" );
  //   }
  // }

  // size_t size = strlen( text ) + 1;
  // char fixedArray[size];
  // strncpy( fixedArray, text, size - 1 );
  // fixedArray[size - 1] = '\0';

  inline void RenderMenuButton( Clay_String text, u32 i ) {
    CLAY(
      CLAY_IDI( "MainMenuButton", i ),
      CLAY_LAYOUT( {
        .padding = { 16, 16 },
        .sizing =
          {
            .width = CLAY_SIZING_FIXED( 240 ),
          },
        .childAlignment = Clay_ChildAlignment( CLAY_ALIGN_X_CENTER ),
      } ),
      CLAY_RECTANGLE( {
        .color{ 140, 140, 140, 255 },
      } )
    ) {
      CLAY_TEXT(
        text,
        CLAY_TEXT_CONFIG( {
          .fontId = 0,
          .fontSize = 32,
          .textColor = { 255, 255, 255, 255 },
        } )
      );
    }
  }

  inline bool ButtonWasClicked( u32 index ) {
    auto id =
      Clay_GetElementIdWithIndex( CLAY_STRING( "MainMenuButton" ), index );
    bool buttonIsHovered = Clay_PointerOver( id );
    if ( buttonIsHovered && IsMouseButtonPressed( 0 ) ) {
      return true;
    }
    return false;
  }

  inline Action_MainMenu main_menu() {
    struct TextButton {
      Clay_String text;
      Action_MainMenu action;
    };

    TextButton buttons[] = {
      TextButton{
        .text = CLAY_STRING( "Start Game" ),
        .action = Action_MainMenu::StartGame
      },
      TextButton{
        .text = CLAY_STRING( "Load Game" ),
        .action = Action_MainMenu::LoadGame,
      },
      TextButton{
        .text = CLAY_STRING( "Host Game" ),
        .action = Action_MainMenu::HostGame,
      },
      TextButton{
        .text = CLAY_STRING( "Join Game" ),
        .action = Action_MainMenu::JoinGame,
      },
      TextButton{
        .text = CLAY_STRING( "Settings" ),
        .action = Action_MainMenu::Settings,
      },
      TextButton{
        .text = CLAY_STRING( "Exit Game" ),
        .action = Action_MainMenu::ExitGame,
      },
    };
    u32 num_buttons = sizeof( buttons ) / sizeof( buttons[0] );

    CLAY(
      CLAY_ID( "MainMenu::Buttons" ),
      CLAY_RECTANGLE( { .color = { 43, 41, 51, 255 } } ),
      CLAY_LAYOUT( {
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
        .sizing =
          {
            .width = CLAY_SIZING_GROW(),
            .height = CLAY_SIZING_GROW(),
          },
        .padding = { 16, 16 },
        .childGap = 8,
        .childAlignment =
          {
            .y = CLAY_ALIGN_Y_CENTER,
            .x = CLAY_ALIGN_X_CENTER,
          },
      } )
    ) {

      for ( u32 i = 0; i < num_buttons; i++ ) {
        RenderMenuButton( buttons[i].text, i );
      }
    }

    for ( u32 i = 0; i < num_buttons; i++ ) {
      TextButton button = buttons[i];
      if ( ButtonWasClicked( i ) ) {
        return button.action;
      }
    }


    // if ( RenderMenuButton( "Load Game", 1 ) ) {
    //   return Action_MainMenu::LoadGame;
    // }
    // if ( RenderMenuButton( "Host Game", 2 ) ) {
    //   return Action_MainMenu::HostGame;
    // }
    // if ( RenderMenuButton( "Join Game", 3 ) ) {
    //   return Action_MainMenu::JoinGame;
    // }
    // if ( RenderMenuButton( "Settings", 4 ) ) {
    //   return Action_MainMenu::Settings;
    // }
    // if ( RenderMenuButton( "Exit Game", 5 ) ) {
    //   return Action_MainMenu::ExitGame;
    // }
    return Action_MainMenu::None;
  }


};// namespace UI