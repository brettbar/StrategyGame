#pragma once

#include "../events.hpp"
#include "guilib.hpp"

inline std::vector<GUI::Panel> InitUI()
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

  GUI::TextButton *spawnButton = new GUI::TextButton();
  spawnButton->enabled = true;
  spawnButton->color = WHITE;
  spawnButton->position = Vector2{};
  spawnButton->dimensions = Vector2{ 60, 60 };

  spawnButton->text = "Spawn";
  spawnButton->fontSize = 20;
  spawnButton->textColor = RED;

  spawnButton->type = GUI::TEXT_BUTTON;
  spawnButton->offset = Vector2{ 5, 5 };
  spawnButton->action = [spawnButton]() {
    if ( ColorToInt( spawnButton->color ) == ColorToInt( WHITE ) )
      spawnButton->color = GREEN;
    else if ( ColorToInt( spawnButton->color ) == ColorToInt( GREEN ) )
      spawnButton->color = WHITE;

    Events::dispatcher.trigger<Events::ProvEvent>();
    Events::dispatcher.trigger<Events::SpawnEvent>();
  };


  leftPanel.children = { spawnButton };


  GUI::Panel contextPanel = GUI::Panel();
  contextPanel.enabled = true;
  contextPanel.color = RED;
  contextPanel.position = Vector2{
    (f32) ( GetScreenWidth() / 2.0f ) - 200,
    (f32) GetScreenHeight() - 200,
  };
  contextPanel.dimensions = Vector2{ 400, 200 };
  contextPanel.floating = false;
  contextPanel.oldOffset = Vector2{};
  contextPanel.update = []() -> Rectangle {
    return {
      ( GetScreenWidth() / 2.0f ) - 200,
      (f32) GetScreenHeight() - 200,
      400,
      200,
    };
  };

  GUI::TextLabel *contextLabel = new GUI::TextLabel();
  contextLabel->enabled = true;
  contextLabel->color = BLACK;
  contextLabel->dimensions = { 200, 100 };
  contextLabel->text = "Poplili";
  contextLabel->fontSize = 20;
  contextLabel->textColor = WHITE;


  // GUI::TextureButton *fooButton = new GUI::TextureButton();
  // fooButton->enabled = false;
  // fooButton->color = WHITE;
  // fooButton->dimensions = Vector2{ 128, 128 };

  // fooButton->type = GUI::TEXTURE_BUTTON;
  // fooButton->offset = Vector2{ 10, 10 };
  // fooButton->action = std::function<void()>{};

  contextPanel.children = { contextLabel };


  // GUI::Panel floatingPanel = GUI::Panel();
  // floatingPanel.enabled = true;
  // floatingPanel.color = BLUE;
  // floatingPanel.position = Vector2{ 500, 100 };
  // floatingPanel.dimensions = Vector2{ 200, 400 };
  // floatingPanel.floating = true;
  // floatingPanel.oldOffset = {};
  // floatingPanel.update = []() -> Rectangle { return { 500, 100, 200, 400 }; };
  // floatingPanel.children = {};

  std::vector<GUI::Panel> ui = {
    leftPanel,
    contextPanel,
    // floatingPanel,
  };

  i32 currId = -1;

  for ( GUI::Panel &panel: ui )
  {
    currId++;
    panel.index = currId;

    for ( GUI::Item *child: panel.children )
    {
      currId++;
      child->index = currId;
    }
  }

  return ui;
}
