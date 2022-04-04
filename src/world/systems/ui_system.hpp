#pragma once

#include "../../common.hpp"
#include "../../guilib/gui_system.hpp"
#include "../components/event.hpp"
#include "event_system.hpp"

namespace UI {

inline void createLeftPanel();
inline entt::entity createSpawnButton( Vector2 );

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
  .hot = entt::null,
  .active = entt::null,
};

inline UIListener listener;

inline void createLeftPanel() {
  entt::entity entity = GUI::uiReg.create();
//    GUI::Panel leftPanel = createLeftPanel();

  GUI::Element elem = {
    .enabled = true,
    .color = BLACK,
    .pos = Vector2{0, 0},
    .dmns = Vector2{ 80, (f32) GetScreenHeight() },
    .offset = Vector2{ 0, 0 },
    .horiz_align = GUI::HorizAlign::LEFT,
    .vert_align = GUI::VertAlign::TOP,
    .horiz_dimension = GUI::Dimension::FIXED,
    .vert_dimension = GUI::Dimension::FILL,
  };

  GUI::Panel panel = {
    .elem = elem,
    .move = []() -> Vector2 {
      return { 0, 0 };
    },
    .children = {
      createSpawnButton(elem.pos),
    }
  };

  GUI::uiReg.emplace<GUI::Type>(entity, GUI::Type::PANEL);
  GUI::uiReg.emplace<GUI::Panel>( entity, panel );
}

inline entt::entity createSpawnButton( Vector2 parent ) {
  entt::entity entity = GUI::uiReg.create();

  GUI::TextButton spawnButton = GUI::TextButton();
  spawnButton.label = {
    .text = "SpawnSystem",
    .fontSize = 20,
    .textColor = RED,
  };

  Vector2 offset = { 5, 5 };

  spawnButton.label.elem = {
    .enabled = true,
    .color = WHITE,
    .pos = {
      parent.x + offset.x,
      parent.y + offset.y,
    },
    .dmns = Vector2{ 60, 60 },
    .offset = offset,
  };

  spawnButton.action = [&spawnButton]() {
    printf( "Spawn Button Action!\n" );

    if ( ColorToInt( spawnButton.label.elem.color ) == ColorToInt( WHITE ) )
      spawnButton.label.elem.color = GREEN;
    else if (
      ColorToInt( spawnButton.label.elem.color ) == ColorToInt( GREEN ) )
      spawnButton.label.elem.color = WHITE;

    EventSystem::dispatcher.trigger<Event::ProvEvent>();
    EventSystem::dispatcher.trigger<Event::SpawnEvent>();
  };

  GUI::uiReg.emplace<GUI::Type>(entity, GUI::Type::TEXT_BUTTON);
  GUI::uiReg.emplace<GUI::TextButton>( entity, spawnButton );

  return entity;
}

//inline GUI::Panel createContextPanel() {
//  Vector2 offset = {
//    (f32) ( GetScreenWidth() / 2.0f ) - 400,
//    (f32) GetScreenHeight() - 200,
//  };
//
//  GUI::Element elem = {
//    .type = GUI::PANEL,
//    .enabled = true,
//    .color = RED,
//    .pos = {
//      (f32) ( GetScreenWidth() / 2.0f ) - 400,
//      (f32) GetScreenHeight() - 200,
//    },
//    .dmns = Vector2{ 800, 200 },
//    .offset = offset,
//    .resize = []() -> Vector2 {
//      return { 800, 200 };
//    },
//  };
//
//  GUI::Panel panel = {
//    .elem = elem,
//    .move = []() -> Vector2 {
//      return {
//        (f32) ( GetScreenWidth() / 2.0f ) - 400,
//        (f32) GetScreenHeight() - 200,
//      };
//    },
//    .children = {},
//  };
//
//  return panel;
//}

//inline GUI::TextLabel createContextLabel(Vector2 parent) {
//  GUI::TextLabel contextLabel = GUI::TextLabel();
//  contextLabel.elem = {
//    .type = GUI::TEXT_LABEL,
//    .enabled = true,
//    .color = BLACK,
//    .pos = {
//      parent.x + 5,
//      parent.y + 5,
//    },
//    .dmns = { 200, 100 },
//    .offset = Vector2{ 5, 5 },
//  };
//
//  contextLabel.text = "Poplili";
//  contextLabel.fontSize = 20;
//  contextLabel.textColor = WHITE;
//
//  return contextLabel;
//}

inline void InitUI() {
  createLeftPanel();

//  entt::entity leftPanelEntt = GUI::uiReg.create();
//  GUI::Panel leftPanel = createLeftPanel();
//
//  entt::entity contextPanelEntt = GUI::uiReg.create();
//  GUI::Panel contextPanel = createContextPanel();
//
//  entt::entity spawnButtonEntt = GUI::uiReg.create();
//  GUI::TextButton spawnButton = createSpawnButton( leftPanel.dyn.elem.pos );
//
//  entt::entity contextLabelEntt = GUI::uiReg.create();
//  GUI::TextLabel contextLabel = createContextLabel(contextPanel.dyn.elem.pos);
//
//  GUI::uiReg.emplace<GUI::Element>( leftPanelEntt, leftPanel.dyn.elem );
//  GUI::uiReg.emplace<GUI::Panel>( leftPanelEntt, leftPanel );
//
//  GUI::uiReg.emplace<GUI::Element>( contextPanelEntt, contextPanel.dyn.elem );
//  GUI::uiReg.emplace<GUI::Panel>( contextPanelEntt, contextPanel );
//
//  GUI::uiReg.emplace<GUI::Element>( spawnButtonEntt, spawnButton.label.elem );
//  GUI::uiReg.emplace<GUI::TextButton>( spawnButtonEntt, spawnButton );
//
//  GUI::uiReg.emplace<GUI::Element>( contextLabelEntt, contextLabel.elem );
//  GUI::uiReg.emplace<GUI::TextLabel>( contextLabelEntt, contextLabel );


  // GUI::TextureButton *fooButton = new GUI::TextureButton();
  // fooButton->enabled = false;
  // fooButton->color = WHITE;
  // fooButton->dimensions = Vector2{ 128, 128 };

  // fooButton->type = GUI::TEXTURE_BUTTON;
  // fooButton->offset = Vector2{ 10, 10 };
  // fooButton->action = std::function<void()>{};

  // leftPanel.children = { spawnButton };
  // contextPanel.children = { contextLabel };
}

inline void Init() {
  InitUI();
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

  // 0. Get screen dims for current frame
  const f32 screenWidth = GetScreenWidth();
  const f32 screenHeight = GetScreenHeight();

  auto all_view = GUI::uiReg.view<GUI::Type>();
  auto items_view = GUI::uiReg.view<GUI::Type>(entt::exclude<GUI::Panel>);

  GUI::Layout( screenWidth, screenHeight );

  for (auto entity : items_view) {
//    if ( !elem.enabled )
//      continue;

  switch (GUI::uiReg.get<GUI::Type>(entity)) {
    case GUI::Type::TEXT_LABEL: {
      auto &label = GUI::uiReg.get<GUI::TextLabel>(entity);
    } break;
    case GUI::Type::TEXTURE_LABEL: {
    } break;
    case GUI::Type::TEXT_BUTTON: {
      auto &button = GUI::uiReg.get<GUI::TextButton>(entity);

      bool inside = CheckCollisionPointRec(
        GetMousePosition(),
        GUI::GetAbsoluteRectangle(
          {
            button.label.elem.offset.x + button.label.elem.pos.x,
            button.label.elem.offset.y + button.label.elem.pos.y
          }, button.label.elem.dmns ) );

      if ( !overAnyElem )
        overAnyElem = inside;

      if ( GUI::DoItem( context, entity, inside, mouseWentUp, mouseWentDown ) ) {
            auto &button = GUI::uiReg.get<GUI::TextButton>( entity );
            button.action();
        }
    } break;
    case GUI::Type::TEXTURE_BUTTON: {
    } break;
  }



  }







  // 6. Draw
  for (auto &entity: all_view) {
    switch ( GUI::uiReg.get<GUI::Type>(entity) ) {
      case GUI::Type::PANEL: {
        auto &panel = GUI::uiReg.get<GUI::Panel>( entity );
        DrawRectangleV( panel.elem.pos, panel.elem.dmns, panel.elem.color );
      } break;

      case GUI::Type::TEXT_BUTTON: {
        auto &button = GUI::uiReg.get<GUI::TextButton>( entity );
        DrawRectangleV( button.label.elem.pos, button.label.elem.dmns, button.label.elem.color );

        DrawText(
          button.label.text.c_str(),
          button.label.elem.pos.x,
          button.label.elem.pos.y + ( 0.5 * button.label.elem.dmns.y ),
          button.label.fontSize,
          button.label.textColor );
      }
        break;

      default:
        break;
    }
  }


//  for ( auto it = panelView.rbegin(); it != panelView.rend(); ++it ) {
//    auto &panel = panelView.get<GUI::Panel>(*it);
//
//    panel.dyn.elem.pos = panel.move();
//
//    for (auto &child : panel.children)
//    {
//      GUI::Element &elem = GUI::uiReg.get<GUI::Element>(child);
//      elem.Update(panel.dyn.elem.pos);
//    }
//  }
//
//  for ( auto it = view.rbegin(); it != view.rend(); ++it ) {
//    entt::entity entity = *it;
//
//    GUI::Element elem = GUI::uiReg.get<GUI::Element>( entity );
//
//    if ( !elem.enabled )
//      continue;
//
//    bool inside = CheckCollisionPointRec(
//      GetMousePosition(),
//      GUI::GetAbsoluteRectangle(
//        {
//          elem.offset.x + elem.pos.x,
//          elem.offset.y + elem.pos.y
//      }, elem.dmns ) );
//
//    if ( !overAnyElem )
//      overAnyElem = inside;
//
//    if ( GUI::DoItem( context, entity, inside, mouseWentUp, mouseWentDown ) ) {
//      switch ( GUI::uiReg.get<GUI::Element>( entity ).type ) {
//        case GUI::PANEL: {
//          auto &panel = GUI::uiReg.get<GUI::Panel>( entity );
//        } break;
//        case GUI::TEXT_LABEL: {
//          auto &label = GUI::uiReg.get<GUI::TextLabel>( entity );
//          label.Draw();
//        } break;
//        case GUI::TEXTURE_LABEL: {
//        } break;
//        case GUI::TEXTURE_BUTTON: {
//        } break;
//        case GUI::TEXT_BUTTON: {
//          auto &button = GUI::uiReg.get<GUI::TextButton>( entity );
//          button.action();
//        } break;
//      }
//    }
//  }


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
