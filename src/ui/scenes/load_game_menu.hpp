#include "../../src/shared/common.hpp"
#include "../common.h"
#include "../library/text_button.hpp"

namespace UI {
  inline str _selected_path = "";

  inline str load_game_menu( list<str> &paths, bool &pressed_back ) {
    CLAY(
      CLAY_ID( "LoadGameMenu" ),
      CLAY_RECTANGLE( { .color = { 43, 41, 51, 255 } } ),
      CLAY_LAYOUT( {
        .sizing =
          {
            .width = CLAY_SIZING_GROW(),
            .height = CLAY_SIZING_GROW(),
          },
        .padding = { 16, 16 },
        .childGap = 8,
        .childAlignment =
          {
            .x = CLAY_ALIGN_X_CENTER,
            .y = CLAY_ALIGN_Y_CENTER,
          },
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      } )
    ) {

      for ( u32 i = 0; i < paths.size(); i++ ) {
        Clay_String cs = Clay_String{
          .length = strlen( paths[i].c_str() ), .chars = paths[i].c_str()
        };

        text_button_small( cs, cs, 0, COLOR_BLUE );
      }


      CLAY() {
        text_button_small(
          CLAY_STRING( "LoadGameMenu::Back" ), CLAY_STRING( "Back" )
        );

        text_button_small(
          CLAY_STRING( "LoadGameMenu::Load" ), CLAY_STRING( "Load" )
        );
      }
    }

    for ( u32 i = 0; i < paths.size(); i++ ) {
      Clay_String cs = Clay_String{
        .length = strlen( paths[i].c_str() ), .chars = paths[i].c_str()
      };

      if ( ButtonWasClicked( cs ) ) {
        // @todo
        _selected_path = cs.chars;
      }
    }

    if ( ButtonWasClicked( CLAY_STRING( "LoadGameMenu::Back" ) ) ) {
      pressed_back = true;
      return "";
    }

    if ( _selected_path != "" &&
         ButtonWasClicked( CLAY_STRING( "LoadGameMenu::Load" ) ) ) {
      // @todo make actually really get the selected file
      return _selected_path;
    }


    return "";
  }
}// namespace UI
