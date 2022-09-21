#include "../../renderer/textures.hpp"
#include "../ui_components.hpp"
#include "context_content.hpp"

namespace UI {

inline entt::entity ContextPanel( TextureCache & );
inline entt::entity ContextTabGroup( TextureCache & );
inline entt::entity ContextTabButton( Texture2D );

inline entt::entity ContextPanel( TextureCache &texture_cache ) {
  Element elem = {
    .id = "context_panel",
    .type = Type::BASE_PANEL,
    .enabled = false,
    .transform =
      {
        .width = 540,
        .height = 203,
      },
  };

  BasePanel panel = {
    {
      .background = Fade( BLACK, 0.5 ),
      .children_axis = Axis::ROW,
      .children_horiz_align = Align::START,
      .children_vert_align = Align::START,
      .children =
        {
          ContextTabGroup( texture_cache ),
          ContextContent(),
        },
    },
    .original_size =
      {
        .x = 540,
        .y = 203,
      },
    // TODO cludgy, clean up
    .update =
      []( Element &elem, BasePanel &panel ) {
        elem.transform = {
          ( (f32) GetScreenWidth() / 2 ) -
            ( panel.original_size.x * SCALE / 2.0f ),
          (f32) GetScreenHeight() - panel.original_size.y * SCALE,
          panel.original_size.x * SCALE,
          panel.original_size.y * SCALE,
        };
      },
  };


  return CreateElement( panel, elem );
}

inline entt::entity ContextTabGroup( TextureCache &texture_cache ) {
  Element elem = {
    .id = "context_tab_group",
    .type = Type::PANEL,
  };

  Panel tab_grp = {
    .background = GREEN,
    .children_axis = Axis::COLUMN,
    .children_horiz_align = Align::START,
    .children_vert_align = Align::START,
    .children =
      {
        ContextTabButton(
          texture_cache[hstr{ "context_tab_overview" }]->texture ),
        ContextTabButton(
          texture_cache[hstr{ "context_tab_population" }]->texture ),
        ContextTabButton(
          texture_cache[hstr{ "context_tab_resources" }]->texture ),
        ContextTabButton(
          texture_cache[hstr{ "context_tab_culture" }]->texture ),
        ContextTabButton(
          texture_cache[hstr{ "context_tab_religion" }]->texture ),
        ContextTabButton(
          texture_cache[hstr{ "context_tab_construction" }]->texture ),
        ContextTabButton(
          texture_cache[hstr{ "context_tab_garrison" }]->texture ),
      },
  };

  return CreateElement( tab_grp, elem );
}

inline entt::entity ContextTabButton( Texture2D texture ) {
  Element elem = {
    .id = "context_tab_button",
    .type = Type::TEXTURE_BUTTON,
  };

  TextureButton button = {
    { .texture = texture },
    []() {

    },
  };

  return CreateElement( button, elem );
}

};// namespace UI
