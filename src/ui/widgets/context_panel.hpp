#include "../../renderer/textures.hpp"
#include "../ui_components.hpp"

namespace UI {

inline entt::entity CreateContextPanel( entt::registry &, TextureCache & );
inline entt::entity CreateTabGroup( entt::registry &, TextureCache & );
inline entt::entity CreateTabButton( entt::registry &reg, Texture2D );
inline entt::entity CreateTabContent( entt::registry & );

inline entt::entity
CreateContextPanel( entt::registry &reg, TextureCache &texture_cache ) {
  entt::entity entity = reg.create();

  Element elem = {
    .type = Type::BASE_PANEL,
    .id = "context_panel",
  };

  BasePanel context_panel = {
    {
      .transform =
        {
          .width = 540,
          .height = 203,
        },
      .background = Fade( BLACK, 0.5 ),
      .children_axis = Axis::ROW,
      .children_horiz_align = Align::START,
      .children_vert_align = Align::START,
      .children =
        {
          CreateTabGroup( reg, texture_cache ),
        },
    },
    .original_size =
      {
        .x = 540,
        .y = 203,
      },
    .update =
      []( BasePanel &panel ) {
        panel.transform = {
          ( (f32) GetScreenWidth() / 2 ) -
            ( panel.original_size.x * SCALE / 2.0f ),
          (f32) GetScreenHeight() - panel.original_size.y * SCALE,
          panel.original_size.x * SCALE,
          panel.original_size.y * SCALE,
        };
      },
  };

  reg.emplace<Element>( entity, elem );
  reg.emplace<BasePanel>( entity, context_panel );

  ui_lookup.insert_or_assign( elem.id, entity );

  return entity;
}

inline entt::entity
CreateTabGroup( entt::registry &reg, TextureCache &texture_cache ) {
  entt::entity entity = reg.create();

  Element elem = {
    .type = Type::PANEL,
    .id = "context_tab_group",
  };

  Panel tab_grp = {
    .background = GREEN,
    .children_axis = Axis::COLUMN,
    .children_horiz_align = Align::START,
    .children_vert_align = Align::START,
    .children =
      {
        CreateTabButton(
          reg,
          texture_cache[hstr{ "context_tab_overview" }]->texture ),
        CreateTabButton(
          reg,
          texture_cache[hstr{ "context_tab_population" }]->texture ),
        CreateTabButton(
          reg,
          texture_cache[hstr{ "context_tab_resources" }]->texture ),
        CreateTabButton(
          reg,
          texture_cache[hstr{ "context_tab_culture" }]->texture ),
        CreateTabButton(
          reg,
          texture_cache[hstr{ "context_tab_religion" }]->texture ),
        CreateTabButton(
          reg,
          texture_cache[hstr{ "context_tab_construction" }]->texture ),
        CreateTabButton(
          reg,
          texture_cache[hstr{ "context_tab_garrison" }]->texture ),
      },
  };

  reg.emplace<Element>( entity, elem );
  reg.emplace<Panel>( entity, tab_grp );

  ui_lookup.insert_or_assign( elem.id, entity );

  return entity;
}


inline entt::entity CreateTabButton( entt::registry &reg, Texture2D texture ) {
  entt::entity entity = reg.create();

  Element elem = {
    .type = Type::TEXTURE_BUTTON,
    .id = "context_tab_button",
  };

  TextureButton button = {
    {
      .transform =
        {
          .width = (f32) texture.width,
          .height = (f32) texture.height,
        },
      .texture = texture,
    },
    []() {

    },
  };

  reg.emplace<Element>( entity, elem );
  reg.emplace<TextureButton>( entity, button );

  ui_lookup.insert_or_assign( elem.id, entity );

  return entity;
}


inline entt::entity CreateTabContent( entt::registry &reg ) {
  entt::entity entity = reg.create();

  Element elem = {
    .type = Type::TEXT_BUTTON,
    .id = "context_tab_content",
  };

  TextButton button = {
    {
      .transform =
        {
          .width = 100,
          .height = 50,
        },
      .text = "CONTENT",
      .font_size = 32,
      .text_color = WHITE,
      .background = BLACK,
    },
    []() {

    },
  };

  reg.emplace<Element>( entity, elem );
  reg.emplace<TextButton>( entity, button );

  ui_lookup.insert_or_assign( elem.id, entity );

  return entity;
}
};// namespace UI
