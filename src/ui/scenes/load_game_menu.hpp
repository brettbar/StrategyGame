#include "../../src/shared/common.hpp"
#include "../common.h"
#include "../library/text_button.hpp"

namespace UI {
  inline str _selected_path = "";

  enum class Action_LoadGameMenu_t {
    None,
    Back,
    LoadGame,
  };

  struct Action_LoadGameMenu {
    Action_LoadGameMenu_t type = Action_LoadGameMenu_t::None;
    str selection = "";
  };

  inline Action_LoadGameMenu load_game_menu(list<str> &paths) {
    Action_LoadGameMenu action = Action_LoadGameMenu();

    CLAY({
      .id = CLAY_ID("LoadGameMenu"),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_GROW(),
              .height = CLAY_SIZING_GROW(),
            },
          .padding = {16, 16},
          .childGap = 8,
          .childAlignment =
            {
              .x = CLAY_ALIGN_X_CENTER,
              .y = CLAY_ALIGN_Y_CENTER,
            },
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
      .backgroundColor = {43, 41, 51, 255},
    }) {

      for (u32 i = 0; i < paths.size(); i++) {
        Clay_String cs = Clay_String{
          .length = static_cast<int32_t>(strlen(paths[i].c_str())),
          .chars = paths[i].c_str()
        };

        if (text_button_small(cs, cs, 0, COLOR_BLUE)) {
          _selected_path = cs.chars;
        }
      }


      CLAY({
        .layout =
          {
            .childGap = 4,
          },
      }) {
        if (text_button_small(
              CLAY_STRING("LoadGameMenu::Back"), CLAY_STRING("Back")
            )) {
          action = Action_LoadGameMenu{.type = Action_LoadGameMenu_t::Back};
        }

        if (text_button_small(
              CLAY_STRING("LoadGameMenu::Load"), CLAY_STRING("Load")
            ) &&
            _selected_path != "") {
          action = Action_LoadGameMenu{
            .type = Action_LoadGameMenu_t::LoadGame,
            .selection = _selected_path,
          };
        }
      }
    }

    // for (u32 i = 0; i < paths.size(); i++) {
    //   Clay_String cs = Clay_String{
    //     .length = static_cast<int32_t>(strlen(paths[i].c_str())),
    //     .chars = paths[i].c_str()
    //   };
    //
    //   if (button_was_clicked(cs)) {
    //     // @todo
    //     _selected_path = cs.chars;
    //   }
    // }

    // if (button_was_clicked(CLAY_STRING("LoadGameMenu::Back"))) {
    //   return "";
    // }
    //
    // if (_selected_path != "" &&
    //     button_was_clicked(CLAY_STRING("LoadGameMenu::Load"))) {
    //   // @todo make actually really get the selected file
    //   return _selected_path;
    // }


    return action;
  }
}// namespace UI
