#pragma once

#include "../../renderer/textures.hpp"
#include "../ui_components.hpp"

namespace UI {

inline entt::entity SettlementContextPanel( TextureCache & );
inline entt::entity SettlementContextTabGroup( TextureCache & );
inline entt::entity SettlementContextTabButton( Texture2D );

inline entt::entity SettlementContextContent();
inline entt::entity SettlementContext();
inline entt::entity SettlementName();
inline entt::entity SettlementPopulation();
inline entt::entity SettlementDevelopment();

inline entt::entity SettlementContextPanel( TextureCache &texture_cache ) {
  Element elem = {
    .id = "settlement_context_panel",
    .type = Type::BasePanel,
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
          SettlementContextTabGroup( texture_cache ),
          SettlementContextContent(),
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

inline entt::entity SettlementContextTabGroup( TextureCache &texture_cache ) {
  Element elem = {
    .id = "context_tab_group",
    .type = Type::Panel,
  };

  Panel tab_grp = {
    .background = Fade( BLACK, 0.5 ),
    .children_axis = Axis::COLUMN,
    .children_horiz_align = Align::START,
    .children_vert_align = Align::START,
    .children =
      {
        SettlementContextTabButton(
          texture_cache[hstr{ "context_tab_overview" }]->texture ),
        SettlementContextTabButton(
          texture_cache[hstr{ "context_tab_population" }]->texture ),
        SettlementContextTabButton(
          texture_cache[hstr{ "context_tab_resources" }]->texture ),
        SettlementContextTabButton(
          texture_cache[hstr{ "context_tab_culture" }]->texture ),
        SettlementContextTabButton(
          texture_cache[hstr{ "context_tab_religion" }]->texture ),
        SettlementContextTabButton(
          texture_cache[hstr{ "context_tab_construction" }]->texture ),
        SettlementContextTabButton(
          texture_cache[hstr{ "context_tab_garrison" }]->texture ),
      },
  };

  return CreateElement( tab_grp, elem );
}

inline entt::entity SettlementContextTabButton( Texture2D texture ) {
  Element elem = {
    .id = "context_tab_button",
    .type = Type::TextureButton,
  };

  TextureButton button = {
    {
      .texture = texture,
    },
    .clickable = true,
  };

  return CreateElement( button, elem );
}

inline entt::entity SettlementContextContent() {
  Element elem = {
    .id = "settlement_context_content",
    .type = Type::Panel,
  };

  Panel panel = {
    .background = BLACK,
    .children_axis = Axis::ROW,
    .children_horiz_align = Align::START,
    .children_vert_align = Align::START,
    .children =
      {
        SettlementContext(),
      },
  };

  return CreateElement( panel, elem );
}

inline entt::entity SettlementContext() {
  Element elem = {
    .id = "settlement_context",
    .type = Type::Panel,
  };

  Panel panel = {
    .background = BLACK,
    .children_axis = Axis::COLUMN,
    .children_horiz_align = Align::START,
    .children_vert_align = Align::START,
    .children =
      {
        SettlementName(),
        SettlementPopulation(),
        SettlementDevelopment(),
      },
  };

  return CreateElement( panel, elem );
}

// TODO this is getting set to the wrong position for the first frame
// To replicate, set a breakpoint on the texture button.Update() call
inline entt::entity SettlementName() {
  Element elem = {
    .id = "settlement_name",
    .type = Type::TextLabel,
    .margins = { 0, 0, 16, 16 },
  };

  TextLabel name = {
    .text = "Uninhabited",
    .font_size = 32,
    .text_color = WHITE,
    .background = BLACK,
    .dynamic = true,
  };


  return CreateElement( name, elem );
}

// TODO this is getting set to the wrong position for the first frame
// To replicate, set a breakpoint on the texture button.Update() call
inline entt::entity SettlementPopulation() {
  Element elem = {
    .id = "settlement_population",
    .type = Type::TextLabel,
    .margins = { 0, 0, 16, 16 },
  };

  TextLabel name = {
    .text = "0",
    .font_size = 32,
    .text_color = WHITE,
    .background = BLACK,
  };

  return CreateElement( name, elem );
}

// TODO this is getting set to the wrong position for the first frame
inline entt::entity SettlementDevelopment() {
  Element elem = {
    .id = "settlement_development",
    .type = Type::TextLabel,
    .margins = { 0, 0, 16, 16 },
  };

  TextLabel name = {
    .text = "SETTLEMENT_PLACEHOLDER",
    .font_size = 32,
    .text_color = WHITE,
    .background = BLACK,
  };


  return CreateElement( name, elem );
}

};// namespace UI
