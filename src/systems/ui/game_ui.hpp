#include <map>

#include "../../common.hpp"
#include "../../components/event.hpp"
#include "../event_system.hpp"
#include "ui_system.hpp"

namespace GAME_UI {

struct UiFlag{};

inline UI::Panel CreateRootPanel();
inline entt::entity CreateLeftPanel();
inline entt::entity CreateContextPanel();
inline entt::entity CreateMiniMapPanel();
inline entt::entity CreateSpawnButton();
inline entt::entity CreateContextLabel( const char * );
inline entt::entity CreateSettlementStats();

inline std::map<const char *, entt::entity> ui_lookup;
inline UI::Panel curr_content = CreateRootPanel();

inline UI::Panel CreateRootPanel() {
  return {
    .align_axis = UI::AlignAxis::FLEX_ROW,
    .align_main = UI::Alignment::SPACE_BETWEEN,
    .align_cross = UI::Alignment::FLEX_END,
    .children =
      {
        CreateLeftPanel(),
        CreateContextPanel(),
        CreateMiniMapPanel(),
      },
  };
}

inline entt::entity CreateLeftPanel() {
  entt::entity entity = UI::reg.create();

  UI::Element elem = {
    .type = UI::Type::PANEL,
    .enabled = true,
    .color = BLACK,
    .dmns = Vector2{ 80, 0 },
    .align_self = UI::AlignSelf::STRETCH,
  };

  UI::Panel panel = {
    .align_axis = UI::AlignAxis::FLEX_COLUMN,
    .align_main = UI::Alignment::FLEX_START,
    .align_cross = UI::Alignment::CENTER,
    .children =
      {
        CreateSpawnButton(),
        CreateSpawnButton(),
        CreateSpawnButton(),
      },
  };

  UI::reg.emplace<UiFlag>( entity, UiFlag());
  UI::reg.emplace<UI::Element>( entity, elem );
  UI::reg.emplace<UI::Panel>( entity, panel );

  ui_lookup.insert_or_assign( "left_panel", entity );

  return entity;
}

inline entt::entity CreateContextPanel() {
  entt::entity entity = UI::reg.create();

  UI::Element elem = {
    .type = UI::Type::PANEL,
    .enabled = true,
    .color = RED,
    .dmns = Vector2{ 1000, 300 },
    .align_self = UI::AlignSelf::AUTO,
  };

  UI::Panel panel = {
    .align_axis = UI::AlignAxis::FLEX_ROW,
    .align_main = UI::Alignment::FLEX_START,
    .align_cross = UI::Alignment::FLEX_START,
    .children =
      {
        CreateContextLabel( "" ),
        CreateSettlementStats(),
      },
  };

  UI::reg.emplace<UiFlag>( entity, UiFlag());
  UI::reg.emplace<UI::Element>( entity, elem );
  UI::reg.emplace<UI::Panel>( entity, panel );

  ui_lookup.insert_or_assign( "context_panel", entity );

  return entity;
}

inline entt::entity CreateMiniMapPanel() {
  entt::entity entity = UI::reg.create();

  UI::Element elem = {
    .type = UI::Type::PANEL,
    .enabled = true,
    .color = BLUE,
    .dmns = Vector2{ 300, 300 },
    .align_self = UI::AlignSelf::AUTO,
  };

  UI::Panel panel = {
    .align_axis = UI::AlignAxis::FLEX_ROW,
    .align_main = UI::Alignment::FLEX_START,
    .align_cross = UI::Alignment::FLEX_START,
    .children = {},
  };

  UI::reg.emplace<UiFlag>( entity, UiFlag());
  UI::reg.emplace<UI::Element>( entity, elem );
  UI::reg.emplace<UI::Panel>( entity, panel );

  ui_lookup.insert_or_assign( "minimap_panel", entity );

  return entity;
}

inline entt::entity CreateSpawnButton() {
  entt::entity entity = UI::reg.create();

  UI::TextButton spawn_button = UI::TextButton();
  spawn_button.label = {
    .text = "SpawnSystem",
    .font_size = 20,
    .text_color = RED,
  };

  UI::Margins margins = {
    .top = 5,
    .right = 0,
    .bottom = 5,
    .left = 0,
  };

  UI::Element elem = {
    .type = UI::Type::TEXT_BUTTON,
    .enabled = true,
    .color = WHITE,
    .dmns = Vector2{ 60, 60 },
    .margins = margins,
  };

  spawn_button.action = []() {
    printf( "Spawn Button Action!\n" );

    // if ( ColorToInt( elem.color ) == ColorToInt( WHITE ) )
    //   elem.color = GREEN;
    // else if ( ColorToInt( elem.color ) == ColorToInt( GREEN ) )
    //   elem.color = WHITE;

    EventSystem::dispatcher.trigger<Event::ProvEvent>();
    EventSystem::dispatcher.trigger<Event::SpawnEvent>();
  };

  UI::reg.emplace<UiFlag>( entity, UiFlag());
  UI::reg.emplace<UI::Element>( entity, elem );
  UI::reg.emplace<UI::TextButton>( entity, spawn_button );

  ui_lookup.insert_or_assign( "spawn_button", entity );

  return entity;
}

inline entt::entity CreateContextLabel( const char *text ) {
  entt::entity entity = UI::reg.create();

  UI::Margins margins = {
    .top = 0,
    .right = 5,
    .bottom = 0,
    .left = 0,
  };

  UI::Element elem = {
    .type = UI::Type::TEXT_LABEL,
    .enabled = true,
    .color = BLACK,
    .dmns = { 200, 100 },
    .margins = margins,
  };

  UI::TextLabel label = UI::TextLabel();
  label.text = text;
  label.font_size = 20;
  label.text_color = WHITE;

  UI::reg.emplace<UiFlag>( entity, UiFlag());
  UI::reg.emplace<UI::Element>( entity, elem );
  UI::reg.emplace<UI::TextLabel>( entity, label );

  ui_lookup.insert_or_assign( "context_label", entity );
  return entity;
}

inline entt::entity CreateSettlementStats() {
  entt::entity entity = UI::reg.create();

  UI::Margins margins = {
    .top = 0,
    .right = 5,
    .bottom = 0,
    .left = 5,
  };

  UI::Element elem = {
    .type = UI::Type::TEXT_LABEL,
    .enabled = false,
    .color = BLACK,
    .dmns = { 300, 200 },
    .margins = margins,
  };

  UI::TextLabel label = UI::TextLabel();
  label.text = "";
  label.font_size = 16;
  label.text_color = WHITE;

  UI::reg.emplace<UiFlag>( entity, UiFlag());
  UI::reg.emplace<UI::Element>( entity, elem );
  UI::reg.emplace<UI::TextLabel>( entity, label );

  ui_lookup.insert_or_assign( "settlement_stats", entity );
  return entity;
}

// inline void createFloatingPanel() {
//   entt::entity entity = UI::gui_reg.create();

//   UI::Element elem = {
//     .type = UI::Type::PANEL,
//     .enabled = true,
//     .color = RED,
//     .pos =
//       {
//         (f32) ( GetScreenWidth() / 2.0f ) - 400,
//         (f32) GetScreenHeight() - 200,
//       },
//     .dmns = Vector2{ 800, 200 },
//     .horiz_dimension = UI::Dimension::FIXED,
//     .vert_dimension = UI::Dimension::FIXED,
//   };

//   UI::Panel panel = {
//     .children_horiz_align = UI::HorizAlign::CENTER,
//     .children_vert_align = UI::VertAlign::CENTER,
//     .children =
//       {
//         createContextLabel( elem.pos ),
//       },
//   };

//   UI::gui_reg.emplace<UI::Element>( entity, elem );
//   UI::gui_reg.emplace<UI::Panel>( entity, panel );
// }

};
