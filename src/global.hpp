#pragma once

#include "renderer/fonts.hpp"
#include "renderer/textures.hpp"

namespace Global {

enum class ProgramMode {
  MAIN_MENU,
  MODAL_MENU,
  GAME,
  EDITOR,
};

enum class GameMode {
  CAMPAIGN,
  BATTLE,
};

inline ProgramMode program_mode = ProgramMode::GAME;
inline FontCache font_cache = {};
inline TextureCache texture_cache = {};
inline entt::registry world;
inline entt::registry local;

};// namespace Global
