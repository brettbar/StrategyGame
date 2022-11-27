#pragma once
#include "../common.hpp"
#include "ui_components.hpp"

namespace UI {
inline std::vector<entt::entity> content;

inline std::vector<entt::entity> CreateContent() {
  return {
    Panel::Create(
      "settlement_context_panel",
      Fade( BLACK, 0.5 ),
      Axis::ROW,
      Align::START,
      Align::START,
      true,
      true,
      []() -> Vector2 {
        return {
          ( (f32) GetScreenWidth() / 2 ) - ( 500 * SCALE / 2.0f ),
          (f32) GetScreenHeight() - 200 * SCALE,
        };
      },
      []() -> Vector2 {
        return {
          500 * SCALE,
          200 * SCALE,
        };
      },
      {
        Panel::Create(
          "settlement_context_tab_group",
          BLUE,
          Axis::COLUMN,
          Align::START,
          Align::START,
          {
            TextureButton::Create( "settlement_context_tab_overview" ),
            TextureButton::Create( "settlement_context_tab_population" ),
            TextureButton::Create( "settlement_context_tab_resources" ),
            TextureButton::Create( "settlement_context_tab_culture" ),
            TextureButton::Create( "settlement_context_tab_religion" ),
            TextureButton::Create( "settlement_context_tab_construction" ),
            TextureButton::Create( "settlement_context_tab_garrison" ),
          }
        ),
        Panel::Create(
          "settlement_context_content",
          BLACK,
          Axis::ROW,
          Align::START,
          Align::START,
          {
            TextLabel::Create(
              "settlement_name",
              BLACK,
              "Uninhabited",
              32,
              WHITE,
              true
            ),
            TextLabel::
              Create( "settlement_population", BLACK, "0", 32, WHITE, true ),
            TextLabel::Create(
              "settlement_development",
              BLACK,
              "Uninhabited",
              32,
              WHITE,
              true
            ),
          }
        ),
      }
    ),
    Panel::Create(
      "actor_context_panel",
      Fade( BLACK, 0.5 ),
      Axis::ROW,
      Align::START,
      Align::START,
      true,
      true,
      []() -> Vector2 {
        return {
          ( (f32) GetScreenWidth() / 2 ) - ( 500 * SCALE / 2.0f ),
          (f32) GetScreenHeight() - 200 * SCALE,
        };
      },
      []() -> Vector2 {
        return {
          500 * SCALE,
          200 * SCALE,
        };
      },
      {
        Panel::Create(
          "actor_context_panel",
          BLACK,
          Axis::ROW,
          Align::START,
          Align::START,
          {
            TextButton::Create(
              "actor_actions_panel",
              PURPLE,
              "Spawn?",
              26,
              WHITE,
              false
            ),
          }
        ),
      }
    ),
  };
}

};// namespace UI
