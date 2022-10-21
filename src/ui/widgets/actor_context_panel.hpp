#pragma once

#include "../../renderer/textures.hpp"
#include "../ui_components.hpp"


namespace UI {

inline entt::entity ActorContextPanel( TextureCache & );
inline entt::entity ActorActionsPanel( TextureCache & );
inline entt::entity ActorSpawnSettlementButton( TextureCache & );

inline entt::entity ActorContextPanel( TextureCache &texture_cache ) {
  Element elem = {
    .id = "actor_context_panel",
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
          ActorActionsPanel( texture_cache ),
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


inline entt::entity ActorActionsPanel( TextureCache &texture_cache ) {
  Element elem = {
    .id = "actor_actions_panel",
    .type = Type::PANEL,
    .enabled = false,
  };

  Panel panel = {
    .children =
      {

        ActorSpawnSettlementButton( texture_cache ),
      },
  };

  return CreateElement( panel, elem );
}


inline entt::entity ActorSpawnSettlementButton( TextureCache &texture_cache ) {
  Element elem = {
    .id = "actor_spawn_settlement_button",
    .type = Type::TEXT_BUTTON,
    .enabled = false,
    .transform = { 200, 100 },
  };

  TextButton button = {
    {
      .text = "Spawn Settlement",
      .font_size = 14,
      .text_color = WHITE,
      .background = BLACK,
    },
    .clickable = true,
    .action = []() { printf( "Spawn Settlement clicked!!\n" ); } };


  return CreateElement( button, elem );
}

};// namespace UI
