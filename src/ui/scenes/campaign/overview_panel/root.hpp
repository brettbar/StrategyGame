#pragma once

#define CLAY_EXTEND_CONFIG_IMAGE hstr texture_id;
#include "../../../../shared/common.hpp"
#include "../../../library/text_button.hpp"
#include "../../../library/texture_button.hpp"
#include "construction_tab.hpp"
#include "resources_tab.hpp"

namespace UI {

  inline int _overview_panel_i = 0;
  inline bool _show_panel = false;

  enum class Action_Overview {
    None,
    ConstructBuilding,
  };

  inline void overview_content( Clay_String current_tab );

  inline Action_Overview overview_panel() {
    struct Tab {
      Clay_String ref;
      hstr icon;
    };

    Tab tabs[] = {
      Tab{
        .ref = CLAY_STRING( "Overview" ),
        .icon = hstr{ "rome_banner.png" },
      },
      Tab{
        .ref = CLAY_STRING( "Missions" ),
        .icon = hstr{ "long_slot.png" },
      },
      Tab{
        .ref = CLAY_STRING( "Objectives" ),
        .icon = hstr{ "long_slot.png" },
      },
      Tab{
        .ref = CLAY_STRING( "Innovations" ),
        .icon = hstr{ "long_slot.png" },
      },
      Tab{
        .ref = CLAY_STRING( "Laws & Customs" ),
        .icon = hstr{ "long_slot.png" },
      },
      Tab{
        .ref = CLAY_STRING( "Diplomacy" ),
        .icon = hstr{ "diplomacy_tab.png" },
      },
      Tab{
        .ref = CLAY_STRING( "Resources" ),
        .icon = hstr{ "resources_tab.png" },
      },
      Tab{
        .ref = CLAY_STRING( "Construction" ),
        .icon = hstr{ "construction_tab.png" },
      },
      Tab{
        .ref = CLAY_STRING( "Agents" ),
        .icon = hstr{ "agents_tab.png" },
      },
      Tab{
        .ref = CLAY_STRING( "Military" ),
        .icon = hstr{ "military_tab.png" },
      },
    };
    u32 num_tabs = LEN( tabs );

    CLAY(
      CLAY_ID( "OverviewPanel" ),
      CLAY_LAYOUT( {
        .padding = { 4, 4 },
        .childGap = 4,
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
          .childGap = 4,
          .childAlignment =
            {
              .x = CLAY_ALIGN_X_CENTER,
            },
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        } )
      ) {
        for ( u32 i = 0; i < num_tabs; i++ ) {
          vec2f dims = { 132, 64 };
          if ( std::string( tabs[i].ref.chars ) == "Overview" ) {
            dims = { 132, 128 };
          }

          CLAY(
            CLAY_IDI( "OverviewPanel::tab", i ),
            CLAY_LAYOUT( {
              .sizing =
                {
                  .width = CLAY_SIZING_FIXED( dims.x ),
                },
              .childAlignment = Clay_ChildAlignment( CLAY_ALIGN_X_CENTER ),
            } )
          ) {
            texture_button( tabs[i].icon, dims );
          }
        }
      }

      if ( _show_panel ) {
        overview_content( tabs[_overview_panel_i].ref );
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

    return Action_Overview::None;
  }

  inline void overview_content( Clay_String current_tab ) {
    CLAY(
      CLAY_ID( "OverviewPanel::Content" ),
      CLAY_RECTANGLE( { .color = { 0, 0, 0, 200 }, .cornerRadius = { 5 } } ),
      CLAY_LAYOUT( {
        .sizing =
          {
            .width = CLAY_SIZING_FIT( { 384 } ),
            .height = CLAY_SIZING_FIXED( 604 ),
          },
        .padding = { 16, 16 },
        .childGap = 8,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      } ),
      CLAY_BORDER_OUTSIDE_RADIUS( 2, COLOR_WHITE, 5 )
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
          current_tab,
          CLAY_TEXT_CONFIG( {
            .textColor = COLOR_WHITE,
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
              .textColor = COLOR_WHITE,
              .fontSize = 16,
            } )
          )
        );
      }

      if ( std::string( current_tab.chars ) == "Construction" )
        UI::construction_tab();
      else if ( std::string( current_tab.chars ) == "Resources" )
        UI::resources_tab();
    }
  }
};// namespace UI
