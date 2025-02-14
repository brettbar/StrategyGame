
#pragma once

#include "../../common.h"
#include "../../library/text_button.hpp"

namespace UI {

  inline void time_button(
    Clay_String id_str,
    Clay_String text,
    Clay_Color text_color = COLOR_WHITE,
    u32 i = 0
  ) {
    CLAY( {
      .id = Clay__HashString( id_str, i, 0 ),
      .layout =
        {
          .sizing = { .width = CLAY_SIZING_FIT( 64 ) },
          .padding = { 16, 16 },
          .childAlignment = Clay_ChildAlignment( CLAY_ALIGN_X_CENTER ),
        },
      .backgroundColor = COLOR_BLACK,
    } ) {
      CLAY_TEXT(
        text,
        CLAY_TEXT_CONFIG( {
          .textColor = text_color,
          .fontId = 0,
          .fontSize = 24,
        } )
      );
    }
  }


  inline void time_panel( f32 timescale ) {
    CLAY() {

      Clay_Color paused_color = COLOR_WHITE;
      Clay_Color fast_color = COLOR_WHITE;
      Clay_Color faster_color = COLOR_WHITE;
      Clay_Color fastest_color = COLOR_WHITE;

      if ( timescale == 0.0f ) {
        paused_color = COLOR_CYAN;
      } else if ( timescale == 0.5f ) {
        fast_color = COLOR_CYAN;
      } else if ( timescale == 1.0f ) {
        faster_color = COLOR_CYAN;
      } else if ( timescale == 1.5f ) {
        fastest_color = COLOR_CYAN;
      }


      time_button( CLAY_STRING( "Paused" ), CLAY_STRING( "||" ), paused_color );
      time_button( CLAY_STRING( "Fast" ), CLAY_STRING( ">" ), fast_color );
      time_button( CLAY_STRING( "Faster" ), CLAY_STRING( ">>" ), faster_color );
      time_button(
        CLAY_STRING( "Fastest" ), CLAY_STRING( ">|" ), fastest_color
      );
    }
  }
}// namespace UI
