#include "../../../common.hpp"
#include "../../../guilib/gui_system.hpp"
#include "../../components/event.hpp"
#include "../event_system.hpp"

inline GUI::Panel createRootPanel();
inline entt::entity createLeftPanel();
inline entt::entity createContextPanel();
inline entt::entity createMiniMapPanel();
inline entt::entity createSpawnButton();
inline entt::entity createContextLabel();
inline entt::entity createContextTexture();

inline GUI::Panel createRootPanel() {
  return {
    .align_axis = GUI::AlignAxis::FLEX_ROW,
    .align_main = GUI::Alignment::SPACE_BETWEEN,
    .align_cross = GUI::Alignment::FLEX_END,
    .children =
      {
        createLeftPanel(),
        createContextPanel(),
        createMiniMapPanel(),
      },
  };
}

inline entt::entity createLeftPanel() {
  entt::entity entity = GUI::gui_reg.create();

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
        createSpawnButton(),
        createSpawnButton(),
        createSpawnButton(),
      },
  };

  GUI::gui_reg.emplace<GUI::Element>( entity, elem );
  GUI::gui_reg.emplace<GUI::Panel>( entity, panel );

  return entity;
}

inline entt::entity createContextPanel() {
  entt::entity entity = GUI::gui_reg.create();

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
        createContextLabel(),
        createContextTexture(),
      },
  };

  GUI::gui_reg.emplace<GUI::Element>( entity, elem );
  GUI::gui_reg.emplace<GUI::Panel>( entity, panel );

  return entity;
}

inline entt::entity createMiniMapPanel() {
  entt::entity entity = GUI::gui_reg.create();

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

  GUI::gui_reg.emplace<GUI::Element>( entity, elem );
  GUI::gui_reg.emplace<GUI::Panel>( entity, panel );

  return entity;
}

inline entt::entity createSpawnButton() {
  entt::entity entity = GUI::gui_reg.create();

  GUI::TextButton spawnButton = GUI::TextButton();
  spawnButton.label = {
    .text = "SpawnSystem",
    .fontSize = 20,
    .textColor = RED,
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

inline entt::entity createContextLabel() {
  entt::entity entity = GUI::gui_reg.create();

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
  label.text = "Poplili";
  label.fontSize = 20;
  label.textColor = WHITE;

  GUI::gui_reg.emplace<GUI::Element>( entity, elem );
  GUI::gui_reg.emplace<GUI::TextLabel>( entity, label );

  return entity;
}

inline entt::entity createContextTexture() {
  entt::entity entity = GUI::gui_reg.create();

  GUI::Margins margins = {
    .top = 0,
    .right = 5,
    .bottom = 0,
    .left = 5,
  };

  GUI::Element elem = {
    .type = GUI::Type::TEXTURE_LABEL,
    .enabled = true,
    .color = BLACK,
    .dmns = { 200, 200 },
    .margins = margins,
  };

  GUI::TextureLabel label = {};

  GUI::gui_reg.emplace<GUI::Element>( entity, elem );
  GUI::gui_reg.emplace<GUI::TextureLabel>( entity, label );

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
