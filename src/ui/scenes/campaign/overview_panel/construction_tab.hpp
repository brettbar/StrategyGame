
#include "../../../../shared/common.hpp"
#include "../../../library/text_button.hpp"
#include "../../../library/texture_button.hpp"
#include <optional>
#define CLAY_EXTEND_CONFIG_IMAGE hstr texture_id;
#include "../../../../../data/buildings.hpp"
#include "../../../common.h"

namespace UI {
  inline bool _constructing = false;
  inline i32 _selected_building = -1;

  struct Building {
    Buildings::BuildingType name;
    Clay_String label;
    str path;
  };

  inline list<Building> buildings = {
    {
      .name = Buildings::BuildingType::Farm,
      .label = CLAY_STRING( "Farm" ),
      .path = "farm_icon.png",
    },
    {
      .name = Buildings::BuildingType::LumberMill,
      .label = CLAY_STRING( "Lumber Mill" ),
      .path = "lumber_mill_icon.png",
    },
    {
      .name = Buildings::BuildingType::Mine,
      .label = CLAY_STRING( "Mine" ),
      .path = "mine_icon.png",
    },
  };


  inline u32 construction_browser() {
    CLAY( {
      .layout =
        {
          .childGap = 4,
        },
    } ) {
      for ( u32 i = 0; i < buildings.size(); i++ ) {
        Building building = buildings[i];

        texture_w_tooltip(
          str_to_cs( "BuildingIcon" ),
          str_to_cs( "BuildingIcon::Tooltip" ),
          building.label,
          hstr{ building.path.c_str() },
          { 32, 32 },
          i
        );
      }
    }

    for ( u32 i = 0; i < buildings.size(); i++ ) {
      Building building = buildings[i];

      if ( ButtonWasClicked( CLAY_STRING( "BuildingIcon" ), i ) ) {
        return i;
      }
    }

    return -1;
  }

  enum class Action_ConstructionPreview {
    None,
    Back,
    Build
  };

  inline Action_ConstructionPreview construction_preview( Building building
  ) {//@left off add a back button
    CLAY( {
      .id = CLAY_ID( "Construction::Preview" ),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_GROW(),
              .height = CLAY_SIZING_GROW(),
            },
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
    } ) {

      CLAY( {
        .id = CLAY_ID( "Construction::Preview::TopRow" ),
        .layout = { .childGap = 8 },
      } ) {
        texture_button(
          CLAY_STRING( "Construction::Preview::Back" ),
          hstr{ "back_button.png" },
          { 30, 30 }
        );

        CLAY( {
          .layout =
            {
              .childGap = 8,
              .childAlignment =
                {
                  .y = CLAY_ALIGN_Y_CENTER,
                },
            },
        } ) {

          texture_label( hstr{ building.path.c_str() }, { 64, 64 } );

          CLAY_TEXT(
            building.label,
            CLAY_TEXT_CONFIG( {
              .textColor = { 255, 255, 255, 255 },
              .fontId = 0,
              .fontSize = 32,
            } )
          );
        }
      }

      CLAY( {
        .id = CLAY_ID( "Construction::Preview::Desc" ),
        .layout =
          {
            .sizing =
              {
                .width = CLAY_SIZING_FIXED( 350 ),
              },
          },
      } ) {
        CLAY_TEXT(
          CLAY_STRING( "Description of a farm. Description of a farm. "
                       "Description of a farm." ),
          CLAY_TEXT_CONFIG( {
            .textColor = COLOR_WHITE,
            .fontId = 0,
            .fontSize = 16,
          } )
        );
      }
      CLAY( CLAY_ID( "Construction::Preview::MiddleRow" ) ) {}
      CLAY( CLAY_ID( "Construction::Preview::BottomRow" ) ) {}
    }

    text_button_small(
      CLAY_STRING( "Construction::Build" ), CLAY_STRING( "Build" ), 0
    );

    if ( ButtonWasClicked( CLAY_STRING( "Construction::Preview::Back" ) ) ) {
      return Action_ConstructionPreview::Back;
    }

    if ( ButtonWasClicked( CLAY_STRING( "Construction::Build" ) ) ) {
      return Action_ConstructionPreview::Build;
    }

    return Action_ConstructionPreview::None;
  }

  inline opt<Buildings::BuildingType> construction_tab() {

    if ( _constructing ) {
      Building building = buildings[_selected_building];

      auto action = construction_preview( building );

      // _constructing = false;
      // _selected_building = -1;
      switch ( action ) {
        case UI::Action_ConstructionPreview::None:
          break;
        case UI::Action_ConstructionPreview::Back:
          _constructing = false;
          break;
        case UI::Action_ConstructionPreview::Build:
          return building.name;
      }
    } else {
      _selected_building = construction_browser();

      if ( _selected_building > -1 ) {
        _constructing = true;
      }
    }

    return std::nullopt;
  }
}// namespace UI
