#pragma once

#include "../common.hpp"
#include "../world/components/event.hpp"
#include "../world/systems/event_system.hpp"
#include "./guilib.hpp"
#include "content.hpp"


namespace UI {
inline void HandleFloatingPanel( GUI::Panel &, Vector2 );

struct UIListener : EventSystem::Listener {
  inline void Receive( const Event::SelectionEvent &e ) {
    if ( currState == nullptr || currReg == nullptr )
      return;

    printf( "UI found selection event! %s\n", e.name.c_str() );
  }

  inline void Listen() {
    EventSystem::dispatcher.sink<Event::SelectionEvent>()
      .connect<&UIListener::Receive>( this );
  }
};

inline GUI::Context context = {
  .hot = -1,
  .active = -1,
};

inline std::vector<GUI::Panel> content;
inline std::vector<GUI::Panel> floating;

inline UIListener listener;

inline void Init() {
  content = InitUI();
  listener.Listen();
}

inline void Update( State &state, entt::registry &reg ) {
  listener.Update( state, reg );
}

inline void Draw() {
  Vector2 mousePos = GetMousePosition();
  bool mouseWentUp = IsMouseButtonReleased( 0 );
  bool mouseWentDown = IsMouseButtonPressed( 0 );
  bool mouseHeldDown = IsMouseButtonDown( 0 );
  bool overAnyElem = false;

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

  for ( GUI::Panel &panel: content ) {
    panel.Update();
    DrawRectangleV( panel.pos, panel.dmns, panel.color );

    for ( GUI::Item *child: panel.children ) {
      GUI::Item item = *child;

      item.Update( panel.pos );

      bool inside = CheckCollisionPointRec(
        GetMousePosition(),
        GUI::GetAbsoluteRectangle( item.pos, item.dmns ) );

      if ( !overAnyElem )
        overAnyElem = inside;

      if ( DoItem( context, child, inside, mouseWentUp, mouseWentDown ) ) {
        switch ( child->type ) {
          case GUI::PANEL: {
          } break;
          case GUI::TEXT_LABEL: {
          } break;
          case GUI::TEXTURE_LABEL: {
          } break;
          case GUI::TEXTURE_BUTTON: {
            GUI::TextButton *button = dynamic_cast<GUI::TextButton *>( child );
            button->action();
          } break;
          case GUI::TEXT_BUTTON: {
            GUI::TextButton *button = dynamic_cast<GUI::TextButton *>( child );
            button->action();
          } break;
        }
      }
    }

    DrawRectangle( GetScreenWidth() - 120, 2, 100, 24.0f, BLACK );
    DrawFPS( GetScreenWidth() - 100, 2 );

    DrawRectangle( GetScreenWidth() - 200, 102, 200, 24.0f, BLACK );
    std::string foo = "hot: " + std::to_string( context.hot ) + " " +
                      "active: " + std::to_string( context.active );

    DrawText( foo.c_str(), GetScreenWidth() - 200, 102, 24.0f, RED );
  }

  if ( !overAnyElem ) {
    context.hot = -1;
    context.active = -1;
  }
}

inline void HandleFloatingPanel( GUI::Panel &panel, Vector2 mousePos ) {
  // printf( "DoFloatingPanel: %d\n", panel.index );
  panel.pos.x = panel.oldOffset.x + mousePos.x;
  panel.pos.y = panel.oldOffset.y + mousePos.y;

  if ( ( panel.pos.x + panel.dmns.x ) > GetScreenWidth() ) {
    panel.pos.x = GetScreenWidth() - panel.dmns.x;
  } else if ( ( panel.pos.x ) < 0 ) {
    panel.pos.x = 0;
  }

  if ( ( panel.pos.y + panel.dmns.y ) > GetScreenHeight() ) {
    panel.pos.y = GetScreenHeight() - panel.dmns.y;
  } else if ( ( panel.pos.y ) < 0 ) {
    panel.pos.y = 0;
  }
}

inline bool MouseWasOverUI() {
  bool inside = false;

  for ( GUI::Panel &panel: content ) {
    inside = CheckCollisionPointRec(
      GetMousePosition(),
      GUI::GetAbsoluteRectangle( panel.pos, panel.dmns ) );

    if ( inside )
      return true;

    for ( GUI::Item *child: panel.children ) {
      GUI::Item item = *child;

      inside = CheckCollisionPointRec(
        GetMousePosition(),
        GUI::GetAbsoluteRectangle( item.pos, item.dmns ) );

      if ( inside )
        return true;
    }
  }

  return inside;
}

};// namespace UI
