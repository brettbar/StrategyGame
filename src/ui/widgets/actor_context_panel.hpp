#pragma once

#include "../../systems/settlement_system.hpp"

#include "../../renderer/textures.hpp"
#include "../ui_components.hpp"

#include "../../systems/actor_system.hpp"

namespace UI {

inline entt::entity ActorContextPanel( TextureCache & );
inline entt::entity ActorActionsPanel( TextureCache & );
inline entt::entity ActorSpawnSettlementButton( TextureCache & );

inline entt::entity ActorContextPanel( TextureCache &texture_cache ) {
  Element elem = {
    .id = "actor_context_panel",
    .type = Type::BasePanel,
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
    .type = Type::Panel,
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
    .type = Type::TextButton,
    .enabled = false,
  };

  TextButton button = {
    {
      .text = "Spawn Settlement",
      .font_size = 18,
      .text_color = WHITE,
      .background = RED,
    },
    .clickable = false,
    .action =
      []() {
        printf( "Spawn Settlement clicked!!\n" );
        SettlementSystem::SpawnSettlement();
      },
    .check_clickable = []() -> bool {
      return ActorSystem::ColonistCanPlaceSettlement();
    },
  };


  return CreateElement( button, elem );
}

};// namespace UI
