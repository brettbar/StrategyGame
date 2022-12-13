#pragma once

#include "../../common.hpp"
#include "../ui_shared.hpp"

namespace UI {

inline std::vector<entt::entity> CreateCampaignUI() {
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
            TextureButton::Create( "settlement_context_tab_overview", true ),
            TextureButton::Create(
              "settlement_context_tab_construction", true
            ),
            // TextureButton::Create( "settlement_context_tab_population", true ),
            // TextureButton::Create( "settlement_context_tab_resources", true ),
            // TextureButton::Create( "settlement_context_tab_culture", true ),
            // TextureButton::Create( "settlement_context_tab_religion", true ),
            // TextureButton::Create(
            //   "settlement_context_tab_construction", true
            // ),
            // TextureButton::Create( "settlement_context_tab_garrison", true ),
          }
        ),

        StackPanel::Create(
          "settlement_context_content",
          BLACK,
          {
            Panel::Create(
              "settlement_context_overview",
              BLACK,
              Axis::ROW,
              Align::START,
              Align::START,
              {
                TextLabel::Create(
                  "settlement_name", "Uninhabited", 32, BLACK, WHITE, true
                ),
                TextLabel::Create(
                  "settlement_population", "0", 32, BLACK, WHITE, true
                ),
                TextLabel::Create(
                  "settlement_development",
                  "Uninhabited",
                  32,
                  BLACK,
                  WHITE,
                  true
                ),
              }
            ),
            Panel::Create(
              "settlement_context_buildings",
              BLACK,
              Axis::ROW,
              Align::START,
              Align::START,
              {}
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
          "actor_actions_panel",
          BLACK,
          Axis::ROW,
          Align::START,
          Align::START,
          {
            TextButton::Create(
              "actor_spawn_settlement_button",
              "Spawn?",
              26,
              PURPLE,
              WHITE,
              false
            ),
            TextButton::Create(
              "actor_spawn_settlement_button", "IDK?", 26, GREEN, WHITE, false
            ),
          }
        ),
      }
    ),
  };
}

};// namespace UI
