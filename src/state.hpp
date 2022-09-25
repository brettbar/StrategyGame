//
// Created by brett on 1/1/2022.
//

#pragma once

#include "common.hpp"

enum class GameState {
  MENU,
  EDITOR,
  GAME,
};

enum class RunState {
  RUNNING,
  PAUSE,
};

struct State {
  const u32 mapWidth;
  const u32 mapHeight;
  f32 timeScale;
  f32 prevTimeScale;
  Camera2D camera;
  GameState gameState;

  u32 day;
  u32 month;
  u32 year;
  u32 startYear;

  std::shared_ptr<PlayerSystem> currPlayer;

  Texture2D selectedTexture;
};
