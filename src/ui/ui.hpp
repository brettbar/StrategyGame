#pragma once

#include "../common.hpp"
#include "../world/components/event.hpp"
#include "../world/systems/event_system.hpp"
#include "./guilib.hpp"

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
  .hot = entt::null,
  .active = entt::null,
};

inline UIListener listener;

inline void createLeftPanel() {
  entt::entity entity = GUI::uiReg.create();

  GUI::Panel panel = GUI::Panel();
  panel.elem = { .type = GUI::PANEL };
  panel.elem = {
    .enabled = true,
    .color = BLACK,
    .pos = Vector2{ 0, 0 },
    .dmns = Vector2{ 80, (f32) GetScreenHeight() },
    .offset = Vector2{ 0, 0 },
    .resize = []() -> Rectangle {
      return { 0, 0, 80, (f32) GetScreenHeight() };
    },
  };
  panel.floating = false;
  panel.oldOffset = {};

  GUI::uiReg.emplace<GUI::Element>( entity, panel.elem );
  GUI::uiReg.emplace<GUI::Panel>( entity, panel );
}

inline void InitUI() {

  createLeftPanel();
  // GUI::TextButton *spawnButton = new GUI::TextButton();
  // spawnButton->enabled = true;
  // spawnButton->color = WHITE;

  // spawnButton->type = GUI::TEXT_BUTTON;
  // spawnButton->offset = Vector2{ 5, 5 };

  // spawnButton->pos = {
  //   leftPanel.pos.x + spawnButton->offset.x,
  //   leftPanel.pos.y + spawnButton->offset.y,
  // };
  // spawnButton->dmns = Vector2{ 60, 60 };

  // spawnButton->text = "SpawnSystem";
  // spawnButton->fontSize = 20;
  // spawnButton->textColor = RED;

  // spawnButton->action = [spawnButton]() {
  //   if ( ColorToInt( spawnButton->color ) == ColorToInt( WHITE ) )
  //     spawnButton->color = GREEN;
  //   else if ( ColorToInt( spawnButton->color ) == ColorToInt( GREEN ) )
  //     spawnButton->color = WHITE;

  //   EventSystem::dispatcher.trigger<Event::ProvEvent>();
  //   EventSystem::dispatcher.trigger<Event::SpawnEvent>();
  // };

  // GUI::Panel contextPanel = GUI::Panel();
  // contextPanel.enabled = true;
  // contextPanel.color = RED;
  // contextPanel.pos = Vector2{
  //   (f32) ( GetScreenWidth() / 2.0f ) - 200,
  //   (f32) GetScreenHeight() - 200,
  // };
  // contextPanel.dmns = Vector2{ 400, 200 };
  // contextPanel.floating = false;
  // contextPanel.oldOffset = Vector2{};
  // contextPanel.update = [contextPanel]() {
  //   contextPanel.pos = Vector2{
  //     ( GetScreenWidth() / 2.0f ) - 200,
  //     (f32) GetScreenHeight() - 200 };
  //   contextPanel.dmns = Vector2{ 400, 200 };
  // };

  // GUI::TextLabel *contextLabel = new GUI::TextLabel();
  // contextLabel->type = GUI::TEXT_LABEL;
  // contextLabel->offset = Vector2{ 5, 5 };

  // contextLabel->enabled = true;
  // contextLabel->color = BLACK;
  // contextLabel->pos = {
  //   contextPanel.pos.x + contextLabel->offset.x,
  //   contextPanel.pos.y + contextLabel->offset.y,
  // };
  // contextLabel->dmns = { 200, 100 };
  // contextLabel->text = "Poplili";
  // contextLabel->fontSize = 20;
  // contextLabel->textColor = WHITE;

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

  for ( auto &entity: GUI::uiReg.view<GUI::Element>() ) {
    GUI::Element elem = GUI::uiReg.get<GUI::Element>( entity );

    bool inside = CheckCollisionPointRec(
      GetMousePosition(),
      GUI::GetAbsoluteRectangle( elem.pos, elem.dmns ) );

    if ( !overAnyElem )
      overAnyElem = inside;

    if ( GUI::DoItem(
           context,
           entity,
           elem.enabled,
           inside,
           mouseWentUp,
           mouseWentDown ) ) {
    }
  }


  if ( !overAnyElem ) {
    context.hot = entt::null;
    context.active = entt::null;
  }

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

  DrawRectangle( GetScreenWidth() - 120, 2, 100, 24.0f, BLACK );
  DrawFPS( GetScreenWidth() - 100, 2 );

  DrawRectangle( GetScreenWidth() - 200, 102, 200, 24.0f, BLACK );
  // std::string foo = "hot: " + std::to_string( context.hot ) + " " +
  //                   "active: " + std::to_string( context.active );

  // DrawText( foo.c_str(), GetScreenWidth() - 200, 102, 24.0f, RED );
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

};// namespace UI
