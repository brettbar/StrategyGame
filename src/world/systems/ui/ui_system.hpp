#pragma once

#include "../../../common.hpp"
#include "../../../guilib/gui_system.hpp"
#include "../../components/actor.hpp"
#include "../../components/event.hpp"
#include "../../components/province.hpp"
#include "../../components/selected.hpp"
#include "../event_system.hpp"

#include "content.hpp"

namespace UI {

inline void HandleFloatingPanel( GUI::Panel &, Vector2 );

inline GUI::Context context = {
  .hot = entt::null,
  .active = entt::null,
};

inline void InitUI() {
  createLeftPanel();
  createContextPanel();
}

inline void SelectListener( entt::registry &reg, entt::entity entity ) {
  if ( reg.all_of<Province::Component>( entity ) ) {

    auto &province = reg.get<Province::Component>( entity );
    printf( "Settlement: %s \n", province.settlement->name );

    entt::entity text_label_entt = GUI::gui_reg.view<GUI::TextLabel>().front();
    GUI::gui_reg.get<GUI::TextLabel>( text_label_entt ).text =
      province.settlement->name;

  } else if ( reg.all_of<Actor::Component>( entity ) ) {

    auto actor = reg.get<Actor::Component>( entity );
    printf( "Actor: %s \n", actor.name );

    entt::entity text_label_entt = GUI::gui_reg.view<GUI::TextLabel>().front();
    GUI::gui_reg.get<GUI::TextLabel>( text_label_entt ).text = actor.name;
  }
}

inline void Init( entt::registry &reg ) {
  InitUI();
  reg.on_construct<Selected::Component>().connect<&SelectListener>();
}

inline void Update( State &state, entt::registry &reg ) {}

inline void Draw() {
  Vector2 mousePos = GetMousePosition();
  bool mouseWentUp = IsMouseButtonReleased( 0 );
  bool mouseWentDown = IsMouseButtonPressed( 0 );
  bool mouseHeldDown = IsMouseButtonDown( 0 );
  bool overAnyElem = false;

  // 0. Get screen dims for current frame
  const f32 screen_width = GetScreenWidth();
  const f32 screen_height = GetScreenHeight();

  auto all_view = GUI::gui_reg.view<GUI::Element>();
  auto items_view =
    GUI::gui_reg.view<GUI::Element>( entt::exclude<GUI::Panel> );

  GUI::Layout( screen_width, screen_height );

  for ( auto entity: items_view ) {
    GUI::Element &elem = GUI::gui_reg.get<GUI::Element>( entity );

    if ( !elem.enabled )
      continue;

    bool inside = CheckCollisionPointRec(
      GetMousePosition(),
      GUI::RectangleFromVectors( { elem.pos.x, elem.pos.y }, elem.dmns ) );

    if ( !overAnyElem )
      overAnyElem = inside;


    if ( GUI::DoItem( context, entity, inside, mouseWentUp, mouseWentDown ) ) {
      switch ( elem.type ) {
        case GUI::Type::TEXT_BUTTON: {
          auto &button = GUI::gui_reg.get<GUI::TextButton>( entity );
          button.action();
        } break;
        case GUI::Type::TEXTURE_BUTTON: {
        } break;
        default:
          break;
      }
    }
  }


  // 6. Draw Everything
  for ( auto &entity: all_view ) {
    GUI::Element &elem = GUI::gui_reg.get<GUI::Element>( entity );
    if ( !elem.enabled )
      return;

    switch ( elem.type ) {
      case GUI::Type::PANEL: {
        auto &panel = GUI::gui_reg.get<GUI::Panel>( entity );

        DrawRectangleV( elem.pos, elem.dmns, elem.color );

      } break;

      case GUI::Type::TEXT_LABEL: {
        auto &label = GUI::gui_reg.get<GUI::TextLabel>( entity );
        DrawRectangleV( elem.pos, elem.dmns, elem.color );

        DrawText(
          label.text.c_str(),
          elem.pos.x,
          elem.pos.y + ( 0.5 * elem.dmns.y ),
          label.fontSize,
          label.textColor );
      } break;

      case GUI::Type::TEXT_BUTTON: {
        auto &button = GUI::gui_reg.get<GUI::TextButton>( entity );
        DrawRectangleV( elem.pos, elem.dmns, elem.color );

        DrawText(
          button.label.text.c_str(),
          elem.pos.x,
          elem.pos.y + ( 0.5 * elem.dmns.y ),
          button.label.fontSize,
          button.label.textColor );
      } break;

      default:
        break;
    }
  }

  std::string hot_str = std::to_string( (std::uint32_t) context.hot );
  std::string active_str = std::to_string( (std::uint32_t) context.active );

  if ( !overAnyElem ) {
    context.hot = entt::null;
    context.active = entt::null;
  }

  if ( context.hot == entt::null ) {
    hot_str = "-1";
  }

  if ( context.active == entt::null ) {
    active_str = "-1";
  }

  DrawRectangle( GetScreenWidth() - 120, 2, 100, 24.0f, BLACK );
  DrawFPS( GetScreenWidth() - 100, 2 );

  DrawRectangle( GetScreenWidth() - 200, 102, 200, 24.0f, BLACK );
  std::string foo = "hot: " + hot_str;
  DrawText( foo.c_str(), GetScreenWidth() - 200, 102, 24.0f, RED );

  DrawRectangle( GetScreenWidth() - 200, 152, 200, 24.0f, BLACK );
  std::string bar = "active: " + active_str;
  DrawText( bar.c_str(), GetScreenWidth() - 200, 152, 24.0f, RED );
}


// inline void HandleFloatingPanel( GUI::Panel &panel, Vector2 mousePos ) {
//   // printf( "DoFloatingPanel: %d\n", panel.index );
//   panel.pos.x = panel.oldOffset.x + mousePos.x;
//   panel.pos.y = panel.oldOffset.y + mousePos.y;

//   if ( ( panel.pos.x + panel.dmns.x ) > GetScreenWidth() ) {
//     panel.pos.x = GetScreenWidth() - panel.dmns.x;
//   } else if ( ( panel.pos.x ) < 0 ) {
//     panel.pos.x = 0;
//   }

//   if ( ( panel.pos.y + panel.dmns.y ) > GetScreenHeight() ) {
//     panel.pos.y = GetScreenHeight() - panel.dmns.y;
//   } else if ( ( panel.pos.y ) < 0 ) {
//     panel.pos.y = 0;
//   }
// }
//  for ( GUI::Panel &panel: floating ) {
//    if ( !panel.floating )
//      return;
//
//    bool inside = CheckCollisionPointRec(
//      GetMousePosition(),
//      GUI::GetAbsoluteRectangle( panel.pos, panel.dmns ) );
//
//    if ( !overAnyElem )
//      overAnyElem = inside;
//
//    if ( GUI::DoFloatingPanel(
//           context,
//           panel,
//           inside,
//           mouseWentUp,
//           mouseWentDown,
//           mouseHeldDown ) ) {
//      HandleFloatingPanel( panel, mousePos );
//    } else {
//      panel.oldOffset = {
//        panel.pos.x - mousePos.x,
//        panel.pos.y - mousePos.y,
//      };
//    }
//  }

};// namespace UI

//    for ( auto it = items_view.rbegin(); it != items_view.rend(); ++it ) {
