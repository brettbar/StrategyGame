#include "../../../common.hpp"
#include "../../../guilib/gui_system.hpp"
#include "../../components/event.hpp"
#include "../event_system.hpp"
#include <map>

inline std::map<const char *, entt::entity> ui_lookup;

inline GUI::Panel CreateRootPanel();
inline entt::entity CreateLeftPanel();
inline entt::entity CreateContextPanel();
inline entt::entity CreateMiniMapPanel();
inline entt::entity CreateSpawnButton();
inline entt::entity CreateContextLabel( const char * );
inline entt::entity CreateSettlementStats();


inline GUI::Panel CreateRootPanel() {
  return {
    .align_axis = GUI::AlignAxis::FLEX_ROW,
    .align_main = GUI::Alignment::SPACE_BETWEEN,
    .align_cross = GUI::Alignment::FLEX_END,
    .children =
      {
        CreateLeftPanel(),
        CreateContextPanel(),
        CreateMiniMapPanel(),
      },
  };
}

inline entt::entity CreateLeftPanel() {
  entt::entity entity = GUI::reg.create();

  GUI::Element elem = {
    .type = GUI::Type::PANEL,
    .enabled = true,
    .color = BLACK,
    .dmns = Vector2{ 80, 0 },
    .align_self = GUI::AlignSelf::STRETCH,
  };

  GUI::Panel panel = {
    .align_axis = GUI::AlignAxis::FLEX_COLUMN,
    .align_main = GUI::Alignment::FLEX_START,
    .align_cross = GUI::Alignment::CENTER,
    .children =
      {
        CreateSpawnButton(),
        CreateSpawnButton(),
        CreateSpawnButton(),
      },
  };

  GUI::reg.emplace<GUI::Element>( entity, elem );
  GUI::reg.emplace<GUI::Panel>( entity, panel );

  ui_lookup.insert_or_assign( "left_panel", entity );

  return entity;
}

inline entt::entity CreateContextPanel() {
  entt::entity entity = GUI::reg.create();

  GUI::Element elem = {
    .type = GUI::Type::PANEL,
    .enabled = true,
    .color = RED,
    .dmns = Vector2{ 1000, 300 },
    .align_self = GUI::AlignSelf::AUTO,
  };

  GUI::Panel panel = {
    .align_axis = GUI::AlignAxis::FLEX_ROW,
    .align_main = GUI::Alignment::FLEX_START,
    .align_cross = GUI::Alignment::FLEX_START,
    .children =
      {
        CreateContextLabel( "" ),
        CreateSettlementStats(),
      },
  };

  GUI::reg.emplace<GUI::Element>( entity, elem );
  GUI::reg.emplace<GUI::Panel>( entity, panel );

  ui_lookup.insert_or_assign( "context_panel", entity );

  return entity;
}

inline entt::entity CreateMiniMapPanel() {
  entt::entity entity = GUI::reg.create();

  GUI::Element elem = {
    .type = GUI::Type::PANEL,
    .enabled = true,
    .color = BLUE,
    .dmns = Vector2{ 300, 300 },
    .align_self = GUI::AlignSelf::AUTO,
  };

  GUI::Panel panel = {
    .align_axis = GUI::AlignAxis::FLEX_ROW,
    .align_main = GUI::Alignment::FLEX_START,
    .align_cross = GUI::Alignment::FLEX_START,
    .children = {},
  };

  GUI::reg.emplace<GUI::Element>( entity, elem );
  GUI::reg.emplace<GUI::Panel>( entity, panel );

  ui_lookup.insert_or_assign( "minimap_panel", entity );

  return entity;
}

inline entt::entity CreateSpawnButton() {
  entt::entity entity = GUI::reg.create();

  GUI::TextButton spawn_button = GUI::TextButton();
  spawn_button.label = {
    .text = "SpawnSystem",
    .font_size = 20,
    .text_color = RED,
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

  GUI::reg.emplace<GUI::Element>( entity, elem );
  GUI::reg.emplace<GUI::TextButton>( entity, spawn_button );

  ui_lookup.insert_or_assign( "spawn_button", entity );

  return entity;
}

inline entt::entity CreateContextLabel( const char *text ) {
  entt::entity entity = GUI::reg.create();

  GUI::Margins margins = {
    .top = 0,
    .right = 5,
    .bottom = 0,
    .left = 5,
  };

  GUI::Element elem = {
    .type = GUI::Type::TEXT_LABEL,
    .enabled = true,
    .color = BLACK,
    .dmns = { 200, 100 },
    .margins = margins,
  };

  GUI::TextLabel label = GUI::TextLabel();
  label.text = text;
  label.font_size = 20;
  label.text_color = WHITE;

  GUI::reg.emplace<GUI::Element>( entity, elem );
  GUI::reg.emplace<GUI::TextLabel>( entity, label );

  ui_lookup.insert_or_assign( "context_label", entity );
  return entity;
}

inline entt::entity CreateSettlementStats() {
  entt::entity entity = GUI::reg.create();

  GUI::Margins margins = {
    .top = 0,
    .right = 5,
    .bottom = 0,
    .left = 5,
  };

  GUI::Element elem = {
    .type = GUI::Type::TEXT_LABEL,
    .enabled = false,
    .color = BLACK,
    .dmns = { 300, 200 },
    .margins = margins,
  };

  GUI::TextLabel label = GUI::TextLabel();
  label.text = "";
  label.font_size = 14;
  label.text_color = WHITE;

  GUI::reg.emplace<GUI::Element>( entity, elem );
  GUI::reg.emplace<GUI::TextLabel>( entity, label );

  ui_lookup.insert_or_assign( "settlement_stats", entity );
  return entity;
}

// inline void createFloatingPanel() {
//   entt::entity entity = GUI::gui_reg.create();

//   GUI::Element elem = {
//     .type = GUI::Type::PANEL,
//     .enabled = true,
//     .color = RED,
//     .pos =
//       {
//         (f32) ( GetScreenWidth() / 2.0f ) - 400,
//         (f32) GetScreenHeight() - 200,
//       },
//     .dmns = Vector2{ 800, 200 },
//     .horiz_dimension = GUI::Dimension::FIXED,
//     .vert_dimension = GUI::Dimension::FIXED,
//   };

//   GUI::Panel panel = {
//     .children_horiz_align = GUI::HorizAlign::CENTER,
//     .children_vert_align = GUI::VertAlign::CENTER,
//     .children =
//       {
//         createContextLabel( elem.pos ),
//       },
//   };

//   GUI::gui_reg.emplace<GUI::Element>( entity, elem );
//   GUI::gui_reg.emplace<GUI::Panel>( entity, panel );
// }
