#pragma once

#include "../../../../shared/common.hpp"
#include "../../../library/text_button.hpp"
#include "construction_tab.hpp"

namespace UI {

  inline int _overview_panel_i = 0;
  inline bool _show_panel = false;

  inline void overview_panel() {
    struct Tab {
      Clay_String text;
      // Action_MainMenu action;
    };

    Tab tabs[] = {
      Tab{
        .text = CLAY_STRING( "Faction" ),
        // .action = Action_MainMenu::StartGame
      },
      Tab{
        .text = CLAY_STRING( "Missions" ),
        // .action = Action_MainMenu::LoadGame,
      },
      Tab{
        .text = CLAY_STRING( "Diplomacy" ),
        // .action = Action_MainMenu::HostGame,
      },
      Tab{
        .text = CLAY_STRING( "Resources" ),
        // .action = Action_MainMenu::JoinGame,
      },
      Tab{
        .text = CLAY_STRING( "Construction" ),
        // .action = Action_MainMenu::Settings,
      },
      Tab{
        .text = CLAY_STRING( "Recruitment" ),
        // .action = Action_MainMenu::ExitGame,
      },
      Tab{
        .text = CLAY_STRING( "Agents" ),
        // .action = Action_MainMenu::ExitGame,
      },
    };
    u32 num_tabs = LEN( tabs );

    CLAY(
      CLAY_ID( "OverviewPanel" ),
      CLAY_LAYOUT( {
        .padding = { 16, 16 },
        .childGap = 8,
        .layoutDirection = CLAY_LEFT_TO_RIGHT,
      } )
    ) {
      CLAY(
        CLAY_ID( "OverviewPanel::tabs" ),
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
        for ( u32 i = 0; i < num_tabs; i++ ) {
          CLAY(
            CLAY_IDI( "OverviewPanel::tab", i ),
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
              tabs[i].text,
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
        Clay_Color white = { 255, 255, 255, 255 };

        CLAY(
          CLAY_ID( "OverviewPanel::Content" ),
          CLAY_RECTANGLE(
            { .color = { 41, 41, 41, 255 }, .cornerRadius = { 5 } }
          ),
          CLAY_LAYOUT( {
            .sizing =
              {
                .width = CLAY_SIZING_FIXED( 384 ),
                .height = CLAY_SIZING_FIXED( 512 ),
              },
            .padding = { 16, 16 },
            .childGap = 8,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          } ),
          CLAY_BORDER_OUTSIDE_RADIUS( 2, white, 5 )
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
            // CLAY_BORDER( {
            //   .bottom =
            //     {
            //       .width = 2,
            //       .color = { 255, 255, 255, 255 },
            //     },
            // } )

          ) {
            CLAY( CLAY_TEXT(
              tabs[_overview_panel_i].text,
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

          Tab current_tab = tabs[_overview_panel_i];
          if ( std::string( current_tab.text.chars ) == "Construction" )
            UI::construction_tab();
        }
      }
    }


    for ( u32 i = 0; i < num_tabs; i++ ) {
      Tab tab = tabs[i];
      if ( ButtonWasClicked( CLAY_STRING( "OverviewPanel::tab" ), i ) ) {
        // return tab.action;
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
