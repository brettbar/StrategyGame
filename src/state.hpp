//
// Created by brett on 1/1/2022.
//

#pragma once

#include "common.hpp"

struct State {
  u32 screenWidth;
  u32 screenHeight;
  const u32 mapWidth;
  const u32 mapHeight;
  f32 timeScale;
  f32 prevTimeScale;
  Camera2D camera;
  bool debug;

  std::shared_ptr<Player> currPlayer;

  Texture2D selectedTexture;
};
