#include <map>

#include "../../common.hpp"
#include "../../components/event.hpp"
#include "../event_system.hpp"
#include "irongui/irongui.hpp"

namespace GAME_UI {

struct UiFlag {};

inline IRONGUI::Panel CreateRootPanel();
inline entt::entity CreateTopPanel();
inline entt::entity CreateLeftPanel();
inline entt::entity CreateContextPanel();
inline entt::entity CreateMiniMapPanel();
inline entt::entity CreateSpawnButton();
inline entt::entity CreateContextLabel( const char * );
inline entt::entity CreateSettlementStats();

inline std::map<const char *, entt::entity> ui_lookup;
inline IRONGUI::Panel curr_content = CreateRootPanel();

inline IRONGUI::Panel CreateRootPanel() {
  return {
    .children_align_axis = IRONGUI::AlignAxis::ROW,
    .children_main_align = IRONGUI::Alignment::SPACE_BETWEEN,
    .children_cross_align = IRONGUI::Alignment::END,
    .children =
      {
        CreateLeftPanel(),
        CreateTopPanel(),
        CreateContextPanel(),
        CreateMiniMapPanel(),
      },
  };
}

inline entt::entity CreateTopPanel() {
  entt::entity entity = IRONGUI::reg.create();

  IRONGUI::Element elem = {
    .type = IRONGUI::Type::PANEL,
    .enabled = true,
    .color = GREEN,
    .pos = Vector2{ 0, 0 },
    .dmns = Vector2{ 1000, 40 },
    .align_self = IRONGUI::SelfAlign::START,
  };

  IRONGUI::Panel panel = {};

  IRONGUI::reg.emplace<UiFlag>( entity, UiFlag() );
  IRONGUI::reg.emplace<IRONGUI::Element>( entity, elem );
  IRONGUI::reg.emplace<IRONGUI::Panel>( entity, panel );

  ui_lookup.insert_or_assign( "top_panel", entity );

  return entity;
}


inline entt::entity CreateLeftPanel() {
  entt::entity entity = IRONGUI::reg.create();

  IRONGUI::Element elem = {
    .type = IRONGUI::Type::PANEL,
    .enabled = true,
    .color = BLACK,
    .dmns = Vector2{ 80, 0 },
    .align_self = IRONGUI::SelfAlign::STRETCH,
  };

  IRONGUI::Panel panel = {
    .children_align_axis = IRONGUI::AlignAxis::COLUMN,
    .children_main_align = IRONGUI::Alignment::START,
    .children_cross_align = IRONGUI::Alignment::CENTER,
    .children =
      {
        CreateSpawnButton(),
        CreateSpawnButton(),
        CreateSpawnButton(),
      },
  };

  IRONGUI::reg.emplace<UiFlag>( entity, UiFlag() );
  IRONGUI::reg.emplace<IRONGUI::Element>( entity, elem );
  IRONGUI::reg.emplace<IRONGUI::Panel>( entity, panel );

  ui_lookup.insert_or_assign( "left_panel", entity );

  return entity;
}

inline entt::entity CreateContextPanel() {
  entt::entity entity = IRONGUI::reg.create();

  IRONGUI::Element elem = {
    .type = IRONGUI::Type::PANEL,
    .enabled = true,
    .color = RED,
    .dmns = Vector2{ 1000, 300 },
    .align_self = IRONGUI::SelfAlign::AUTO,
  };

  IRONGUI::Panel panel = {
    .children_align_axis = IRONGUI::AlignAxis::ROW,
    .children_main_align = IRONGUI::Alignment::START,
    .children_cross_align = IRONGUI::Alignment::START,
    .children =
      {
        CreateContextLabel( "" ),
        CreateSettlementStats(),
      },
  };

  IRONGUI::reg.emplace<UiFlag>( entity, UiFlag() );
  IRONGUI::reg.emplace<IRONGUI::Element>( entity, elem );
  IRONGUI::reg.emplace<IRONGUI::Panel>( entity, panel );

  ui_lookup.insert_or_assign( "context_panel", entity );

  return entity;
}

inline entt::entity CreateMiniMapPanel() {
  entt::entity entity = IRONGUI::reg.create();

  IRONGUI::Element elem = {
    .type = IRONGUI::Type::PANEL,
    .enabled = true,
    .color = BLUE,
    .dmns = Vector2{ 300, 300 },
    .align_self = IRONGUI::SelfAlign::AUTO,
  };

  IRONGUI::Panel panel = {
    .children_align_axis = IRONGUI::AlignAxis::ROW,
    .children_main_align = IRONGUI::Alignment::START,
    .children_cross_align = IRONGUI::Alignment::START,
    .children = {},
  };

  IRONGUI::reg.emplace<UiFlag>( entity, UiFlag() );
  IRONGUI::reg.emplace<IRONGUI::Element>( entity, elem );
  IRONGUI::reg.emplace<IRONGUI::Panel>( entity, panel );

  ui_lookup.insert_or_assign( "minimap_panel", entity );

  return entity;
}

inline entt::entity CreateSpawnButton() {
  entt::entity entity = IRONGUI::reg.create();

  IRONGUI::TextButton spawn_button = IRONGUI::TextButton();
  spawn_button.label = {
    .text = "SpawnSystem",
    .font_size = 20,
    .text_color = RED,
  };

  IRONGUI::Margins margins = {
    .top = 5,
    .right = 0,
    .bottom = 5,
    .left = 0,
  };

  IRONGUI::Element elem = {
    .type = IRONGUI::Type::TEXT_BUTTON,
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

  IRONGUI::reg.emplace<UiFlag>( entity, UiFlag() );
  IRONGUI::reg.emplace<IRONGUI::Element>( entity, elem );
  IRONGUI::reg.emplace<IRONGUI::TextButton>( entity, spawn_button );

  ui_lookup.insert_or_assign( "spawn_button", entity );

  return entity;
}

inline entt::entity CreateContextLabel( const char *text ) {
  entt::entity entity = IRONGUI::reg.create();

  IRONGUI::Margins margins = {
    .top = 0,
    .right = 5,
    .bottom = 0,
    .left = 0,
  };

  IRONGUI::Element elem = {
    .type = IRONGUI::Type::TEXT_LABEL,
    .enabled = true,
    .color = BLACK,
    .dmns = { 200, 100 },
    .margins = margins,
  };

  IRONGUI::TextLabel label = IRONGUI::TextLabel();
  label.text = text;
  label.font_size = 20;
  label.text_color = WHITE;

  IRONGUI::reg.emplace<UiFlag>( entity, UiFlag() );
  IRONGUI::reg.emplace<IRONGUI::Element>( entity, elem );
  IRONGUI::reg.emplace<IRONGUI::TextLabel>( entity, label );

  ui_lookup.insert_or_assign( "context_label", entity );
  return entity;
}

inline entt::entity CreateSettlementStats() {
  entt::entity entity = IRONGUI::reg.create();

  IRONGUI::Margins margins = {
    .top = 0,
    .right = 5,
    .bottom = 0,
    .left = 5,
  };

  IRONGUI::Element elem = {
    .type = IRONGUI::Type::TEXT_LABEL,
    .enabled = false,
    .color = BLACK,
    .dmns = { 300, 200 },
    .margins = margins,
  };

  IRONGUI::TextLabel label = IRONGUI::TextLabel();
  label.text = "";
  label.font_size = 16;
  label.text_color = WHITE;

  IRONGUI::reg.emplace<UiFlag>( entity, UiFlag() );
  IRONGUI::reg.emplace<IRONGUI::Element>( entity, elem );
  IRONGUI::reg.emplace<IRONGUI::TextLabel>( entity, label );

  ui_lookup.insert_or_assign( "settlement_stats", entity );
  return entity;
}

inline void createFloatingPanel() {
  entt::entity entity = IRONGUI::gui_reg.create();

  IRONGUI::Element elem = {
    .type = IRONGUI::Type::PANEL,
    .enabled = true,
    .color = RED,
    .pos =
      {
        (f32) ( GetScreenWidth() / 2.0f ) - 400,
        (f32) GetScreenHeight() - 200,
      },
    .dmns = Vector2{ 800, 200 },
    .horiz_dimension = IRONGUI::Dimension::FIXED,
    .vert_dimension = IRONGUI::Dimension::FIXED,
  };

  IRONGUI::Panel panel = {
    .children_horiz_align = IRONGUI::HorizAlign::CENTER,
    .children_vert_align = IRONGUI::VertAlign::CENTER,
    .children =
      {
        createContextLabel( elem.pos ),
      },
  };

  IRONGUI::gui_reg.emplace<IRONGUI::Element>( entity, elem );
  IRONGUI::gui_reg.emplace<IRONGUI::Panel>( entity, panel );
}

};// namespace GAME_UI
