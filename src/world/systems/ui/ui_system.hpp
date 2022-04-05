#pragma once

#include "../../../common.hpp"
#include "../../../guilib/gui_system.hpp"
#include "../../components/event.hpp"
#include "../event_system.hpp"

namespace UI {

inline void createLeftPanel();
inline void createContextPanel();
inline entt::entity createSpawnButton( Vector2 );
inline entt::entity createContextLabel( Vector2 );

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
  entt::entity entity = GUI::gui_reg.create();

  GUI::Element elem = {
    .type = GUI::Type::PANEL,
    .enabled = true,
    .color = BLACK,
    .pos = Vector2{ 0, 0 },
    .dmns = Vector2{ 80, (f32) GetScreenHeight() },
    .margins = Vector2{ 0, 0 },
    .horiz_align = GUI::HorizAlign::LEFT,
    .vert_align = GUI::VertAlign::TOP,
    .horiz_dimension = GUI::Dimension::FIXED,
    .vert_dimension = GUI::Dimension::FILL,
  };

  GUI::Panel panel = {
    .children =
      {
        createSpawnButton( elem.pos ),
      },
  };

  GUI::gui_reg.emplace<GUI::Element>( entity, elem );
  GUI::gui_reg.emplace<GUI::Panel>( entity, panel );
}

inline entt::entity createSpawnButton( Vector2 parent ) {
  entt::entity entity = GUI::gui_reg.create();

  GUI::TextButton spawnButton = GUI::TextButton();
  spawnButton.label = {
    .text = "SpawnSystem",
    .fontSize = 20,
    .textColor = RED,
  };

  Vector2 margins = { 0, 5 };

  GUI::Element elem = {
    .type = GUI::Type::TEXT_BUTTON,
    .enabled = true,
    .color = WHITE,
    .pos =
      {
        parent.x + margins.x,
        parent.y + margins.y,
      },
    .dmns = Vector2{ 60, 60 },
    .margins = margins,
    .horiz_align = GUI::HorizAlign::CENTER,
    .vert_align = GUI::VertAlign::TOP,
    .horiz_dimension = GUI::Dimension::FIXED,
    .vert_dimension = GUI::Dimension::FIXED,
  };

  spawnButton.action = []() {
    printf( "Spawn Button Action!\n" );

    // if ( ColorToInt( elem.color ) == ColorToInt( WHITE ) )
    //   elem.color = GREEN;
    // else if ( ColorToInt( elem.color ) == ColorToInt( GREEN ) )
    //   elem.color = WHITE;

    EventSystem::dispatcher.trigger<Event::ProvEvent>();
    EventSystem::dispatcher.trigger<Event::SpawnEvent>();
  };

  GUI::gui_reg.emplace<GUI::Element>( entity, elem );
  GUI::gui_reg.emplace<GUI::TextButton>( entity, spawnButton );

  return entity;
}

inline void createContextPanel() {
  entt::entity entity = GUI::gui_reg.create();

  GUI::Element elem = {
    .type = GUI::Type::PANEL,
    .enabled = true,
    .color = RED,
    .pos =
      {
        (f32) ( GetScreenWidth() / 2.0f ) - 400,
        (f32) GetScreenHeight() - 200,
      },
    .dmns = Vector2{ 800, 200 },
    .horiz_align = GUI::HorizAlign::CENTER,
    .vert_align = GUI::VertAlign::BOTTOM,
    .horiz_dimension = GUI::Dimension::FIXED,
    .vert_dimension = GUI::Dimension::FIXED,
  };

  GUI::Panel panel = {
    .children =
      {
        createContextLabel( elem.pos ),
      },
  };

  GUI::gui_reg.emplace<GUI::Element>( entity, elem );
  GUI::gui_reg.emplace<GUI::Panel>( entity, panel );
}

inline entt::entity createContextLabel( Vector2 parent ) {
  entt::entity entity = GUI::gui_reg.create();

  Vector2 margins = { 5, 5 };

  GUI::Element elem = {
    .type = GUI::Type::TEXT_LABEL,
    .enabled = true,
    .color = BLACK,
    .pos =
      {
        parent.x + margins.x,
        parent.y + margins.y,
      },
    .dmns = { 200, 100 },
    .margins = margins,
    .horiz_align = GUI::HorizAlign::LEFT,
    .vert_align = GUI::VertAlign::TOP,
    .horiz_dimension = GUI::Dimension::FIXED,
    .vert_dimension = GUI::Dimension::FIXED,
  };

  GUI::TextLabel label = GUI::TextLabel();
  label.text = "Poplili";
  label.fontSize = 20;
  label.textColor = WHITE;

  GUI::gui_reg.emplace<GUI::Element>( entity, elem );
  GUI::gui_reg.emplace<GUI::TextLabel>( entity, label );

  return entity;
}

inline void InitUI() {
  createLeftPanel();
  createContextPanel();

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
        case GUI::Type::TEXT_LABEL: {
          auto &label = GUI::gui_reg.get<GUI::TextLabel>( entity );
        } break;
        case GUI::Type::TEXTURE_LABEL: {
        } break;
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
