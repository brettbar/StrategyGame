#pragma once

#include "../../../shared/common.hpp"
#include "../../library/text_button.hpp"
#include "clay/clay.h"

namespace UI {

  inline void overview_panel() {
    struct Button {
      Clay_String text;
      // Action_MainMenu action;
    };

    Button buttons[] = {
      Button{
        .text = CLAY_STRING( "Faction" ),
        // .action = Action_MainMenu::StartGame
      },
      Button{
        .text = CLAY_STRING( "Missions" ),
        // .action = Action_MainMenu::LoadGame,
      },
      Button{
        .text = CLAY_STRING( "Diplomacy" ),
        // .action = Action_MainMenu::HostGame,
      },
      Button{
        .text = CLAY_STRING( "Resources" ),
        // .action = Action_MainMenu::JoinGame,
      },
      Button{
        .text = CLAY_STRING( "Construction" ),
        // .action = Action_MainMenu::Settings,
      },
      Button{
        .text = CLAY_STRING( "Recruitment" ),
        // .action = Action_MainMenu::ExitGame,
      },
      Button{
        .text = CLAY_STRING( "Agents" ),
        // .action = Action_MainMenu::ExitGame,
      },
    };
    u32 num_buttons = LEN( buttons );

    CLAY(
      CLAY_ID( "OverviewPanel" ),
      CLAY_LAYOUT( {
        .layoutDirection = CLAY_LEFT_TO_RIGHT,
        .childGap = 8,
        .padding = { 16, 16 },
      } )
    ) {
      CLAY(
        CLAY_ID( "OverviewPanel::Buttons" ),
        CLAY_LAYOUT( {
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
          .sizing =
            {
              .width = CLAY_SIZING_FIT(),
              .height = CLAY_SIZING_GROW(),
            },
          .childGap = 8,
          .childAlignment =
            {
              .x = CLAY_ALIGN_X_CENTER,
            },
        } )
      ) {
        for ( u32 i = 0; i < num_buttons; i++ ) {
          CLAY(
            CLAY_IDI( "OverviewPanel::Button", i ),
            CLAY_LAYOUT( {
              .padding = { 16, 16 },
              .sizing =
                {
                  .width = CLAY_SIZING_FIXED( 240 ),
                },
              .childAlignment = Clay_ChildAlignment( CLAY_ALIGN_X_CENTER ),
            } ),
            CLAY_RECTANGLE( { .color = { 41, 41, 41, 255 }, .cornerRadius = 5 }
            )
          ) {
            CLAY_TEXT(
              buttons[i].text,
              CLAY_TEXT_CONFIG( {
                .fontId = 0,
                .fontSize = 18,
                .textColor = { 255, 255, 255, 255 },
              } )
            );
          }
        }
      }

      CLAY(
        CLAY_ID( "OverviewPanel::Content" ),
        CLAY_RECTANGLE( { .color = { 41, 41, 41, 255 }, .cornerRadius = 5 } ),
        CLAY_LAYOUT( {
          .sizing =
            {
              .width = CLAY_SIZING_FIXED( 271 ),
              .height = CLAY_SIZING_FIXED( 295 ),
            },
        } )
      ) {
        CLAY( CLAY_TEXT(
          CLAY_STRING( "foo" ),
          CLAY_TEXT_CONFIG(
            { .fontSize = 16, .textColor = { 255, 255, 255, 255 } }
          )
        ) );
      }
    }


    // for ( u32 i = 0; i < num_buttons; i++ ) {
    //   Button button = buttons[i];
    //   if ( ButtonWasClicked( i ) ) {
    //     // return button.action;
    //   }
    // }

    // return Action_MainMenu::None;
  }
};// namespace UI