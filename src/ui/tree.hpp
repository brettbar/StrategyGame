#pragma once

#include "../events.hpp"
#include "guilib.hpp"

inline std::vector<GUI::Panel> InitTree()
{
  std::vector<GUI::Panel> tree = {
    GUI::Panel{
      {
        .enabled = true,
        .color = BLACK,
        .position = Vector2{ 0, 0 },
        .dimensions = Vector2{ 80, (f32) GetScreenHeight() },
      },
      .floating = false,
      .oldOffset = {},
      .update = []() -> Rectangle {
        return { 0, 0, 80, (f32) GetScreenHeight() };
      },
      .children =
        {
          GUI::Item{
            {
              .enabled = true,
              .color = WHITE,
              .position = {},
              .dimensions = { 60, 60 },
            },
            .type = GUI::TEXT_BUTTON,
            .offset = Vector2{ 5, 5 },
            .action =
              []() {
                Events::dispatcher.trigger<Events::ProvEvent>();
                Events::dispatcher.trigger<Events::SpawnEvent>();
              },
            .text = "Spawn",
          },
        },
    },

    GUI::Panel{
      {
        .enabled = true,
        .color = RED,
        .position =
          Vector2{
            (f32) ( GetScreenWidth() / 2.0f ) - 200,
            (f32) GetScreenHeight() - 200,
          },
        .dimensions = Vector2{ 400, 200 },
      },
      .floating = false,
      .oldOffset = {},
      .update = []() -> Rectangle {
        return {
          ( GetScreenWidth() / 2.0f ) - 200,
          (f32) GetScreenHeight() - 200,
          400,
          200,
        };
      },
      .children =
        {
          GUI::Item{
            {
              .enabled = false,
              .color = WHITE,
              .position = {},
              .dimensions = { 128, 128 },
            },
            .type = GUI::TEXTURE_BUTTON,
            .offset = Vector2{ 10, 10 },
            .action = {},
            .text = "",
          },
        },
    },

    GUI::Panel{
      {
        .enabled = true,
        .color = BLUE,
        .position = Vector2{ 500, 100 },
        .dimensions = Vector2{ 200, 400 },
      },
      .floating = true,
      .oldOffset = {},
      .update = []() -> Rectangle {
        return { 500, 100, 200, 400 };
      },
      .children = {},
    },
  };

  i32 currId = -1;

  for ( GUI::Panel &panel: tree )
  {
    currId++;
    panel.index = currId;

    for ( GUI::Item &child: panel.children )
    {
      currId++;
      child.index = currId;
    }
  }

  return tree;
}
