#pragma once

#include "../../../shared/common.hpp"
#include "../../library/text_button.hpp"
#include "clay/clay.h"

namespace UI {

  inline int _overview_panel_i = 0;
  inline bool _show_panel = false;

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
        .padding = { 16, 16 },
        .childGap = 8,
        .layoutDirection = CLAY_LEFT_TO_RIGHT,
      } )
    ) {
      CLAY(
        CLAY_ID( "OverviewPanel::Buttons" ),
        CLAY_LAYOUT( {
          .sizing =
            {
              .width = CLAY_SIZING_FIT( { 128 } ),
              .height = CLAY_SIZING_GROW(),
            },
          .childGap = 8,
          .childAlignment =
            {
              .x = CLAY_ALIGN_X_CENTER,
            },
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        } )
      ) {
        for ( u32 i = 0; i < num_buttons; i++ ) {
          CLAY(
            CLAY_IDI( "OverviewPanel::Button", i ),
            CLAY_LAYOUT( {
              .sizing =
                {
                  .width = CLAY_SIZING_FIXED( 240 ),
                },
              .padding = { 16, 16 },
              .childAlignment = Clay_ChildAlignment( CLAY_ALIGN_X_CENTER ),
            } ),
            CLAY_RECTANGLE( {
              .color = { 41, 41, 41, 255 },
              .cornerRadius = { 5 },
            } )
          ) {
            CLAY_TEXT(
              buttons[i].text,
              CLAY_TEXT_CONFIG( {
                .textColor = { 255, 255, 255, 255 },
                .fontId = 0,
                .fontSize = 18,
              } )
            );
          }
        }
      }

      if ( _show_panel ) {
        CLAY(
          CLAY_ID( "OverviewPanel::Content" ),
          CLAY_RECTANGLE(
            { .color = { 41, 41, 41, 255 }, .cornerRadius = { 5 } }
          ),
          CLAY_LAYOUT( {
            .sizing =
              {
                .width = CLAY_SIZING_FIXED( 256 ),
                .height = CLAY_SIZING_FIXED( 512 ),
              },
            .padding = { 16, 16 },
          } )
        ) {
          CLAY(
            CLAY_ID( "OverviewPanel::Content::Banner" ),
            CLAY_LAYOUT( {
              .sizing =
                {
                  .width = CLAY_SIZING_GROW(),
                },
              .childAlignment =
                {
                  .x = CLAY_ALIGN_X_RIGHT,
                },
            } )
          ) {
            CLAY( CLAY_TEXT(
              buttons[_overview_panel_i].text,
              CLAY_TEXT_CONFIG( {
                .textColor = { 255, 255, 255, 255 },
                .fontSize = 16,
              } )
            ) );

            CLAY( CLAY_LAYOUT( {
              .sizing = { CLAY_SIZING_GROW() },
            } ) );

            CLAY(
              CLAY_ID( "X" ),
              CLAY_TEXT(
                CLAY_STRING( "X" ),
                CLAY_TEXT_CONFIG( {
                  .textColor = { 255, 255, 255, 255 },
                  .fontSize = 16,
                } )
              )
            );
          }
        }
      }
    }


    for ( u32 i = 0; i < num_buttons; i++ ) {
      Button button = buttons[i];
      if ( ButtonWasClicked( CLAY_STRING( "OverviewPanel::Button" ), i ) ) {
        // return button.action;
        _overview_panel_i = i;
        _show_panel = true;
      }
    }

    if ( ButtonWasClicked( CLAY_STRING( "X" ) ) ) {
      _show_panel = false;
    }

    // return Action_MainMenu::None;
  }
};// namespace UI
