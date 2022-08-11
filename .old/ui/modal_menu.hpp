#include "../../../include/entt/entt.hpp"
#include "../../global.hpp"
#include "irongui/irongui.hpp"

namespace MODAL_MENU_UI {

struct UiFlag {};

inline IRONGUI::Panel CreateRootPanel();
inline entt::entity CreateMenuButton( std::string );

struct Emitter : entt::emitter<Emitter> {};
inline std::map<const char *, entt::entity> ui_lookup;
inline IRONGUI::Panel curr_content = CreateRootPanel();
inline Emitter emitter{};

inline IRONGUI::Panel CreateRootPanel() {
  return {
    .children_align_axis = IRONGUI::AlignAxis::COLUMN,
    .children_main_align = IRONGUI::Alignment::CENTER,
    .children_cross_align = IRONGUI::Alignment::CENTER,
    .children = {
      CreateMenuButton( "Save game" ),
      CreateMenuButton( "Load game" ),
      CreateMenuButton( "Settings" ),
      CreateMenuButton( "Save & Quit" ),
      CreateMenuButton( "Quit Without Saving" ),
    } };
}


inline entt::entity CreateMenuButton( std::string label ) {
  entt::entity entity = IRONGUI::reg.create();

  IRONGUI::TextButton menu_button = IRONGUI::TextButton();
  menu_button.label = {
    .text = label,
    .font_size = 32,
    .text_color = WHITE,
  };

  IRONGUI::Margins margins = {
    .top = 15,
    .right = 0,
    .bottom = 0,
    .left = 0,
  };

  IRONGUI::Element elem = {
    .type = IRONGUI::Type::TEXT_BUTTON,
    .enabled = true,
    .color = BLUE,
    .dmns = Vector2{ 200, 80 },
    .margins = margins,
  };

  if ( label == "Save game" ) {
    menu_button.action = []() { printf( "save game\n" ); };
  } else if ( label == "Load game" ) {
    menu_button.action = []() { printf( "load game\n" ); };
  } else if ( label == "Settings" ) {
    menu_button.action = []() { printf( "settings\n" ); };
  } else if ( label == "Save & Quit" ) {
    menu_button.action = []() { printf( "save & quit\n" ); };
  } else if ( label == "Quit Without Saving" ) {
    menu_button.action = []() {
      printf( "quit without saving\n" );
      CloseWindow();
    };
  }


  IRONGUI::reg.emplace<UiFlag>( entity, UiFlag() );
  IRONGUI::reg.emplace<IRONGUI::Element>( entity, elem );
  IRONGUI::reg.emplace<IRONGUI::TextButton>( entity, menu_button );

  ui_lookup.insert_or_assign(
    ( "menu_button" + std::string( label ) ).c_str(),
    entity );

  return entity;
}

};// namespace MODAL_MENU_UI
