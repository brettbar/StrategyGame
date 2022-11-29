#pragma once

#include "renderer/fonts.hpp"
#include "renderer/textures.hpp"

namespace Global {

enum class ProgramMode {
  MainMenu,
  ModalMenu,
  Game,
  Editor,
};

// TODO delete?
enum class GameMode {
  CAMPAIGN,
  BATTLE,
};

inline ProgramMode program_mode = ProgramMode::Game;
inline FontCache font_cache = {};
inline TextureCache texture_cache = {};
inline entt::registry world;
inline entt::registry local;

// TODO temporary
inline entt::entity host_player;

};// namespace Global
