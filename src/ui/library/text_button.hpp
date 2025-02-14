#pragma once

#include "../../shared/common.hpp"

#include "../common.h"

namespace UI {

  inline void text_button_lrg(
    Clay_String id_str,
    Clay_String text,
    u32 i = 0,
    Clay_Color color = { 140, 140, 140, 255 }
  ) {
    CLAY( {
      .id = ( Clay__HashString( id_str, i, 0 ) ),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIT( 256 ),
            },
          .padding = { 16, 16 },
          .childAlignment = Clay_ChildAlignment( CLAY_ALIGN_X_CENTER ),
        },
      .backgroundColor = color,
    } ) {
      CLAY_TEXT(
        text,
        CLAY_TEXT_CONFIG( {
          .textColor = COLOR_WHITE,
          .fontId = 0,
          .fontSize = 32,
        } )
      );
    }
  }

  inline void text_button_small(
    Clay_String id_str,
    Clay_String text,
    u32 i = 0,
    Clay_Color color = { 140, 140, 140, 255 }
  ) {
    CLAY( {
      .id = ( Clay__HashString( id_str, i, 0 ) ),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIT( 128 ),
              .height = CLAY_SIZING_FIT( 36 ),
            },
          .padding = { 16, 16 },
          .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
        },
      .backgroundColor = color,
    } ) {
      CLAY_TEXT(
        text,
        CLAY_TEXT_CONFIG( {
          .textColor = COLOR_WHITE,
          .fontId = 0,
          .fontSize = 16,
        } )
      );
    }
  }

  inline bool ButtonWasClicked( Clay_String id_str, u32 index ) {
    auto id = Clay_GetElementIdWithIndex( id_str, index );
    bool buttonIsHovered = Clay_PointerOver( id );
    if ( buttonIsHovered && IsMouseButtonPressed( 0 ) ) {
      return true;
    }
    return false;
  }

  inline bool ButtonWasClicked( Clay_String id_str ) {
    auto id = Clay_GetElementId( id_str );
    bool buttonIsHovered = Clay_PointerOver( id );
    if ( buttonIsHovered && IsMouseButtonPressed( 0 ) ) {
      return true;
    }
    return false;
  }

}// namespace UI
