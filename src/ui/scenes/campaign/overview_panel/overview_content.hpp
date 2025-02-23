#pragma once

#define CLAY_EXTEND_CONFIG_IMAGE hstr texture_id;
#include "construction_tab.hpp"
#include "resources_tab.hpp"

namespace UI {
  enum class OverviewAction_t {
    None,
    Construction,
  };
  struct OverviewAction {
    OverviewAction_t type;
    Buildings::BuildingType building;
  };

  inline OverviewAction overview_content( Clay_String current_tab ) {
    opt<Buildings::BuildingType> building_to_build = std::nullopt;

    CLAY( {
      .id = CLAY_ID( "OverviewPanel::Content" ),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIT( 384 ),
              // .height = CLAY_SIZING_PERCENT( 0.5 ),
              .height = CLAY_SIZING_FIT( 786 ),
            },
          .padding = { 16, 16 },
          .childGap = 8,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
      .backgroundColor = { 0, 0, 0, 200 },
      .cornerRadius = { 5, 5, 5, 5 },
      .border =
        {
          .color = COLOR_WHITE,
          .width = { 2, 2, 2, 2 },
        },
    } ) {
      CLAY( {
        .id = CLAY_ID( "OverviewPanel::Content::Banner" ),
        .layout =
          {
            .sizing =
              {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_FIT( 48 ),
              },
            .childAlignment =
              {
                .x = CLAY_ALIGN_X_RIGHT,
                .y = CLAY_ALIGN_Y_CENTER,
              },
          },
      } ) {
        CLAY() {
          CLAY_TEXT(
            current_tab,
            CLAY_TEXT_CONFIG( {
              .textColor = COLOR_WHITE,
              .fontSize = 16,
            } )
          );
        }

        CLAY( {
          .layout =
            {
              .sizing = { CLAY_SIZING_GROW() },
            },
        } );

        CLAY( CLAY_ID( "X" ), ) {
          CLAY_TEXT(
            CLAY_STRING( "X" ),
            CLAY_TEXT_CONFIG( {
              .textColor = COLOR_WHITE,
              .fontSize = 16,
            } )
          );
        }
      }

      if ( std::string( current_tab.chars ) == "Construction" ) {
        building_to_build = UI::construction_tab();
      } else if ( std::string( current_tab.chars ) == "Resources" ) {
        UI::resources_tab();
      }
    }


    if ( building_to_build.has_value() ) {
      return OverviewAction{
        .type = OverviewAction_t::Construction,
        .building = building_to_build.value(),
      };
    }

    return { .type = OverviewAction_t::None };
  }
}// namespace UI
