#include "../../../include/entt/entt.hpp"
#include <map>

#include "../../guilib/gui_system.hpp"

namespace MAIN_MENU_UI {
struct UiFlag{};

inline GUI::Panel CreateRootPanel();
inline entt::entity CreateMenuButton( const char* );

inline std::map<const char *, entt::entity> ui_lookup;
inline GUI::Panel curr_content = CreateRootPanel();

inline GUI::Panel CreateRootPanel() {
  return {
    .align_axis = GUI::AlignAxis::FLEX_COLUMN,
    .align_main = GUI::Alignment:: CENTER,
    .align_cross = GUI::Alignment::CENTER,
    .children = {
      CreateMenuButton("New game"),
      CreateMenuButton("Load game"),
      CreateMenuButton("Settings"),
      CreateMenuButton("Exit"),
    }
  };
}

inline entt::entity CreateMenuButton( const char * label ) {
  entt::entity entity = GUI::reg.create();

  GUI::TextButton menu_button = GUI::TextButton();
  menu_button.label = {
    .text = label,
    .font_size = 32,
    .text_color = WHITE,
  };

  GUI::Margins margins = {
    .top = 5,
    .right = 0,
    .bottom = 5,
    .left = 0,
  };

  GUI::Element elem = {
    .type = GUI::Type::TEXT_BUTTON,
    .enabled = true,
    .color = BLUE,
    .dmns = Vector2{ 200, 80 },
    .margins = margins,
  };


  GUI::reg.emplace<UiFlag>( entity, UiFlag());
  GUI::reg.emplace<GUI::Element>( entity, elem );
  GUI::reg.emplace<GUI::TextButton>( entity, menu_button );

  ui_lookup.insert_or_assign( ("menu_button" + std::string(label)).c_str(), entity );

  return entity;
}

};
