#pragma once

#define CLAY_EXTEND_CONFIG_IMAGE hstr texture_id;
#include "../../../../shared/common.hpp"
#include "../../../library/texture_button.hpp"

#include "../../../common.h"


namespace UI {

  inline int _overview_panel_i = 0;
  inline Clay_String tab = CLAY_STRING( "" );


  inline Clay_String overview_tabs() {
    struct Tab {
      Clay_String ref;
      hstr icon;
    };

    Tab tabs[] = {
      Tab{
        .ref = CLAY_STRING( "Overview" ),
        .icon = hstr{ "rome_banner.png" },
      },
      // Tab{
      //   .ref = CLAY_STRING( "Missions" ),
      //   .icon = hstr{ "long_slot.png" },
      // },
      // Tab{
      //   .ref = CLAY_STRING( "Objectives" ),
      //   .icon = hstr{ "long_slot.png" },
      // },
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

    f32 scale = 2.0;

    CLAY( {
      .id = CLAY_ID( "OverviewPanel" ),
      .layout =
        {
          .sizing = { .width = CLAY_SIZING_FIT( 128 ) },
          .childGap = 4,
          .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
    } ) {
      for ( u32 i = 0; i < num_tabs; i++ ) {
        vec2f dims = { 66 * UI_SCALE, 32 * UI_SCALE };
        if ( std::string( tabs[i].ref.chars ) == "Overview" ) {
          dims = { 66 * UI_SCALE, 64 * UI_SCALE };
        }

        CLAY( {
          .id = CLAY_IDI( "OverviewPanel::tab", i ),
          .layout =
            {
              .sizing =
                {
                  .width = CLAY_SIZING_FIXED( dims.x ),
                },
              .childAlignment = Clay_ChildAlignment( CLAY_ALIGN_X_CENTER ),
            },
        } ) {
          texture_label( tabs[i].icon, dims );
        }
      }
    }


    for ( u32 i = 0; i < num_tabs; i++ ) {
      if ( ButtonWasClicked( CLAY_STRING( "OverviewPanel::tab" ), i ) ) {
        // return tab.action;
        _overview_panel_i = i;
        tab = tabs[_overview_panel_i].ref;
        return tab;
      }
    }

    if ( ButtonWasClicked( CLAY_STRING( "X" ) ) ) {
      tab = CLAY_STRING( "" );
    }

    return tab;
  }

}// namespace UI
