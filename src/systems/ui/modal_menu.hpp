#include "../../../include/entt/entt.hpp"
#include "../../global.hpp"
#include "ui_system.hpp"

namespace MODAL_MENU_UI {

struct UiFlag{};

inline UI::Panel CreateRootPanel();
inline entt::entity CreateMenuButton( std::string );

struct Emitter: entt::emitter<Emitter>{};
inline std::map<const char *, entt::entity> ui_lookup;
inline UI::Panel curr_content = CreateRootPanel();
inline Emitter emitter{};

inline UI::Panel CreateRootPanel() {
  return {
    .align_axis = UI::AlignAxis::FLEX_COLUMN,
    .align_main = UI::Alignment:: CENTER,
    .align_cross = UI::Alignment::CENTER,
    .children = {
      CreateMenuButton("Save game"),
      CreateMenuButton("Load game"),
      CreateMenuButton("Settings"),
      CreateMenuButton("Save & Quit"),
      CreateMenuButton("Quit Without Saving"),
    }
  };
}


inline entt::entity CreateMenuButton( std::string label ) {
  entt::entity entity = UI::reg.create();

  UI::TextButton menu_button = UI::TextButton();
  menu_button.label = {
    .text = label,
    .font_size = 32,
    .text_color = WHITE,
  };

  UI::Margins margins = {
    .top = 15,
    .right = 0,
    .bottom = 0,
    .left = 0,
  };

  UI::Element elem = {
    .type = UI::Type::TEXT_BUTTON,
    .enabled = true,
    .color = BLUE,
    .dmns = Vector2{ 200, 80 },
    .margins = margins,
  };

  if ( label == "Save game" ) {
    menu_button.action = []() {
      printf("save game\n");
    };
  } else if ( label == "Load game" ) {
    menu_button.action = []() {
      printf("load game\n");
    };
  } else if ( label == "Settings" ) {
    menu_button.action = []() {
      printf("settings\n");
    };
  } else if ( label == "Save & Quit" ) {
    menu_button.action = []() {
      printf("save & quit\n");
    };
  } else if ( label == "Quit Without Saving" ) {
    menu_button.action = []() {
      printf("quit without saving\n");
      CloseWindow();
    };
  }


  UI::reg.emplace<UiFlag>( entity, UiFlag());
  UI::reg.emplace<UI::Element>( entity, elem );
  UI::reg.emplace<UI::TextButton>( entity, menu_button );

  ui_lookup.insert_or_assign( ("menu_button" + std::string(label)).c_str(), entity );

  return entity;
}

};
