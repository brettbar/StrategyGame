#pragma once

#include "../events.hpp"
#include "guilib.hpp"

inline std::vector<GUI::Panel> InitTree()
{

  GUI::Panel leftPanel = GUI::Panel();
  leftPanel.enabled = true;
  leftPanel.color = BLACK;
  leftPanel.position = Vector2{ 0, 0 };
  leftPanel.dimensions = Vector2{ 80, (f32) GetScreenHeight() };
  leftPanel.floating = false;
  leftPanel.oldOffset = {};
  leftPanel.update = []() -> Rectangle {
    return { 0, 0, 80, (f32) GetScreenHeight() };
  };

  GUI::TextButton spawnButton = GUI::TextButton();
  spawnButton.enabled = true;
  spawnButton.color = WHITE;
  spawnButton.position = Vector2{};
  spawnButton.dimensions = Vector2{ 60, 60 };

  spawnButton.type = GUI::TEXT_BUTTON;
  spawnButton.offset = Vector2{ 5, 5 };
  spawnButton.action = []() {
    Events::dispatcher.trigger<Events::ProvEvent>();
    Events::dispatcher.trigger<Events::SpawnEvent>();
  };
  spawnButton.text = "Spawn";


  leftPanel.children = { spawnButton };


  GUI::Panel bottomPanel = GUI::Panel();
  bottomPanel.enabled = true;
  bottomPanel.color = RED;
  bottomPanel.position = Vector2{
    (f32) ( GetScreenWidth() / 2.0f ) - 200,
    (f32) GetScreenHeight() - 200,
  };
  bottomPanel.dimensions = Vector2{ 400, 200 };
  bottomPanel.floating = false;
  bottomPanel.oldOffset = Vector2{};
  bottomPanel.update = []() -> Rectangle {
    return {
      ( GetScreenWidth() / 2.0f ) - 200,
      (f32) GetScreenHeight() - 200,
      400,
      200,
    };
  };

  GUI::TextureButton fooButton = GUI::TextureButton();
  fooButton.enabled = false;
  fooButton.color = WHITE;
  fooButton.position = Vector2{};
  fooButton.dimensions = Vector2{ 128, 128 };

  fooButton.type = GUI::TEXTURE_BUTTON;
  fooButton.offset = Vector2{ 10, 10 };
  fooButton.action = std::function<void()>{};

  bottomPanel.children = { fooButton };


  GUI::Panel floatingPanel = GUI::Panel();
  floatingPanel.enabled = true;
  floatingPanel.color = BLUE;
  floatingPanel.position = Vector2{ 500, 100 };
  floatingPanel.dimensions = Vector2{ 200, 400 };
  floatingPanel.floating = true;
  floatingPanel.oldOffset = {};
  floatingPanel.update = []() -> Rectangle { return { 500, 100, 200, 400 }; };
  floatingPanel.children = {};

  std::vector<GUI::Panel> tree = {
    leftPanel,
    bottomPanel,
    floatingPanel,
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
