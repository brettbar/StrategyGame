#include "../../../common.hpp"
#include "../../../guilib/gui_system.hpp"
#include "../../components/event.hpp"
#include "../event_system.hpp"

inline entt::entity createLeftPanel();
inline entt::entity createContextPanel();
inline entt::entity createSpawnButton( Vector2 );
inline entt::entity createContextLabel( Vector2 );

inline entt::entity createLeftPanel() {
  entt::entity entity = GUI::gui_reg.create();

  GUI::Element elem = {
    .type = GUI::Type::PANEL,
    .enabled = true,
    .color = BLACK,
    .pos = Vector2{ 0, 0 },
    .dmns = Vector2{ 80, (f32) GetScreenHeight() },
    .margins = Vector2{ 0, 0 },
    .horiz_dimension = GUI::Dimension::FIXED,
    .vert_dimension = GUI::Dimension::FILL,
  };

  GUI::Panel panel = {
    .children_horiz_align = GUI::HorizAlign::LEFT,
    .children_vert_align = GUI::VertAlign::TOP,
    .children =
      {
        createSpawnButton( elem.pos ),
      },
  };

  GUI::gui_reg.emplace<GUI::Element>( entity, elem );
  GUI::gui_reg.emplace<GUI::Panel>( entity, panel );

  return entity;
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

inline entt::entity createContextPanel() {
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
    .dmns = Vector2{ 1000, 300 },
    .horiz_dimension = GUI::Dimension::FIXED,
    .vert_dimension = GUI::Dimension::FIXED,
  };

  GUI::Panel panel = {
    .children_horiz_align = GUI::HorizAlign::CENTER,
    .children_vert_align = GUI::VertAlign::CENTER,
    .children =
      {
        createContextLabel( elem.pos ),
        createContextLabel( elem.pos ),
      },
  };

  GUI::gui_reg.emplace<GUI::Element>( entity, elem );
  GUI::gui_reg.emplace<GUI::Panel>( entity, panel );

  return entity;
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
