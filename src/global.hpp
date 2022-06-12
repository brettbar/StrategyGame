#pragma once 

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

namespace Global {
  inline ProgramMode program_mode = ProgramMode::MAIN_MENU;
};
