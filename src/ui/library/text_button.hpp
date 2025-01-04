#pragma once

#include "../../shared/common.hpp"
#include "clay/clay.h"

inline void RenderMenuButton(
  Clay_String id_str,
  Clay_String text,
  u32 i,
  Clay_Color color = { 140, 140, 140, 255 }
) {


  CLAY(
    Clay__AttachId( Clay__HashString( id_str, i, 0 ) ),
    CLAY_LAYOUT( {
      .padding = { 16, 16 },
      .sizing =
        {
          .width = CLAY_SIZING_FIT( 240 ),
        },
      .childAlignment = Clay_ChildAlignment( CLAY_ALIGN_X_CENTER ),
    } ),
    CLAY_RECTANGLE( {
      .color = color,
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

inline bool ButtonWasClicked( Clay_String id_str, u32 index ) {
  auto id = Clay_GetElementIdWithIndex( id_str, index );
  bool buttonIsHovered = Clay_PointerOver( id );
  if ( buttonIsHovered && IsMouseButtonPressed( 0 ) ) {
    return true;
  }
  return false;
}