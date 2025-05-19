#pragma once

#define CLAY_EXTEND_CONFIG_IMAGE hstr texture_id;
#include "../../../../../data/resources.hpp"
#include "../../../../shared/common.hpp"
#include "../../../common.h"
#include "../../../library/texture_button.hpp"
#include "../../../library/text_label.hpp"

#include "../../../../world/systems/player_system.hpp"

namespace UI {

  inline void resource_icon( Resources::Type type, vec2f dimensions, u32 i ) {
    const char *resource = Resources::ResourceStr( type );

    Clay_String cs = {
      .length = static_cast<int32_t>( strlen( resource ) ),
      .chars = resource,
    };

    CLAY( {
      .id = Clay__HashString( cs, i, 0 ),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIXED( dimensions.x * UI_SCALE ),
              .height = CLAY_SIZING_FIXED( dimensions.y * UI_SCALE ),
            },
        },
    } ) {
      CLAY( {
        .layout =
          {
            .sizing =
              {
                .width = CLAY_SIZING_FIXED( 32 * UI_SCALE ),
                .height = CLAY_SIZING_FIXED( 32 * UI_SCALE ),
              },
          },
        .image = image(resource_icon_path(type), { 32, 32 })
      } );

      auto id = Clay_GetElementIdWithIndex( cs, i );

      if ( Clay_PointerOver( id ) ) {
        CLAY( {
          .id = CLAY_IDI( "ResourceIcon::Tooltip", i ),
          .layout = { .padding = { 8, 8 } },
          .backgroundColor = COLOR_BLACK,
          .cornerRadius = { 5 },
          .floating =
            {
              .attachPoints =
                {
                  .element = CLAY_ATTACH_POINT_LEFT_TOP,
                  .parent = CLAY_ATTACH_POINT_LEFT_BOTTOM,
                },
              .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
              .attachTo = CLAY_ATTACH_TO_PARENT,
            },
        } ) {

          text_label(cs, 12);
        }
      }
    }
  }

  inline void resource_quantity( Clay_String amount_cs, vec2f dimensions ) {
    CLAY({
      .layout = {
        .sizing = {
          .width = CLAY_SIZING_FIXED( dimensions.x * UI_SCALE ),
          .height = CLAY_SIZING_FIXED( dimensions.y * UI_SCALE ),
        },
        .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
      }
    }) {
      text_label(amount_cs, 12);
    }
  }

  inline void resource_rows( Resources::Type resources_count ) {
    vec2f dimensions = { 32, 32 };

    entt::entity local_player = Player::System::GetEntityOfPlayer( "player_0" );

    auto resource_map =
      Resource::System::get_resources_for_player( local_player );

    for ( u32 i = 0; i < (u32) resources_count; i++ ) {
      Resources::Type resource_t = (Resources::Type) i;

      u32 amount_stored = resource_map[resource_t];

      cstr amount_stored_s = u32_to_cstr( amount_stored );

      Clay_String amount_cs = {
        .length = static_cast<int32_t>( strlen( amount_stored_s ) ),
        .chars = amount_stored_s,
      };

      Clay_String temp_else = {
        .length = static_cast<int32_t>( strlen( "0" ) ),
        .chars = "0",
      };


      CLAY( {
        .layout =
          {
            .childGap = 4,
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
          },
      } ) {
        // 1. Icon
        resource_icon( resource_t, dimensions, i );
        // 2. Stored
        resource_quantity( amount_cs, dimensions );
        // 3. Increase Per Tick
        // resource_quantity( temp_else, dimensions );
        // 4. Decrease Per Tick
        // resource_quantity( temp_else, dimensions );
        // 5. Importing Per Tick
        // resource_quantity( temp_else, dimensions );
        // 6. Exporting Per Tick
        // resource_quantity( temp_else, dimensions );
      }
    }
  }

  inline void resources_headers() {
    list<hstr> resource_headers = {
      "resources_header_icons.png",
      "resources_header_stockpile.png",
      "resources_header_increase.png",
      "resources_header_decrease.png",
      "resources_header_incoming.png",
      "resources_header_outgoing.png",
    };

    CLAY( {
      .id = CLAY_ID( "ResourceHeaders" ),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_GROW(),
              .height = CLAY_SIZING_FIXED(18 * UI_SCALE),
            },
          .childGap = 4,
          .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
    } ) {
      for ( u32 i = 0; i < resource_headers.size(); i++ ) {
        texture_label( resource_headers[i], { 34 , 18} );
      }
    }
  }

  inline void resources_tab() {
    CLAY( {
      .layout =
        {
          .childGap = 4,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
    } ) {
      resources_headers();

      CLAY( {
        .id = CLAY_ID( "ResourceRows" ),
        .layout =
          {
            .sizing =
              {
                .width = CLAY_SIZING_GROW(),
              },
            .childGap = 4,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          },
        .scroll = { .vertical = true },
      } ) {
        resource_rows( Resources::Type::COUNT );
      }
    }
  }
}// namespace UI
