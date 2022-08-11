#include "../ui_components.hpp"

namespace UI {

inline entt::entity CreateContextPanel( entt::registry &reg );
inline entt::entity CreateTabGroup( entt::registry &reg );
inline entt::entity CreateTabButton( entt::registry &reg, hstr );
inline entt::entity CreateTabContent( entt::registry &reg );

inline entt::entity CreateContextPanel( entt::registry &reg ) {
  entt::entity entity = reg.create();

  Element elem = {
    .type = Type::PANEL,
    // .dimensions_type = Dimensions::FIXED,
    .width = 540,
    .height = 203,
    .background = WHITE,
  };

  Panel context_panel = {
    .children_axis = Axis::ROW,
    .children_horiz_align = Align::START,
    .children_vert_align = Align::START,
    .children =
      {
        CreateTabGroup( reg ),
        CreateTabContent( reg ),
      },
    .self_positioned = true,
    .update = []() -> vec2 {
      return {
        ( (f32) GetScreenWidth() / 2 ) - ( 540 / 2.0f ),
        (f32) GetScreenHeight() - 203,
      };
    },
  };

  reg.emplace<Element>( entity, elem );
  reg.emplace<Panel>( entity, context_panel );

  return entity;
}

inline entt::entity CreateTabGroup( entt::registry &reg ) {
  entt::entity entity = reg.create();

  Element elem = {
    .type = Type::PANEL,
    // .dimensions_type = Dimensions::FIX_WIDTH_STRETCH_HEIGHT,
    .width = 45,
    .height = 203,
    .background = GREEN,
  };

  Panel tab_grp = {
    .children_axis = Axis::COLUMN,
    .children_horiz_align = Align::START,
    .children_vert_align = Align::START,
    .children =
      {
        CreateTabButton( reg, "context_tab_overview" ),
        CreateTabButton( reg, "context_tab_population" ),
        CreateTabButton( reg, "context_tab_resources" ),
        CreateTabButton( reg, "context_tab_culture" ),
        CreateTabButton( reg, "context_tab_religion" ),
        CreateTabButton( reg, "context_tab_construction" ),
        CreateTabButton( reg, "context_tab_garrison" ),
      },
  };

  reg.emplace<Element>( entity, elem );
  reg.emplace<Panel>( entity, tab_grp );

  return entity;
}


inline entt::entity CreateTabButton( entt::registry &reg, hstr key ) {
  entt::entity entity = reg.create();

  Element elem = {
    .type = Type::TEXTURE_BUTTON,
    .width = 45,
    .height = 29,
    .background = WHITE,
  };

  TextureButton button = {
    {
      .texture_key = key,
    },
    []() {

    },
  };

  reg.emplace<Element>( entity, elem );
  reg.emplace<TextureButton>( entity, button );

  return entity;
}


inline entt::entity CreateTabContent( entt::registry &reg ) {
  entt::entity entity = reg.create();

  Element elem = {
    .type = Type::TEXT_BUTTON,
    // .dimensions_type = Dimensions::STRETCH,
    .background = BLACK,
  };

  TextButton button = {
    {
      .text = "CONTENT",
      .font_size = 32,
      .text_color = WHITE,
    },
    []() {

    },
  };

  reg.emplace<Element>( entity, elem );
  reg.emplace<TextButton>( entity, button );

  return entity;
}


};// namespace UI
