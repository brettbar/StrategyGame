#define RAYGUI_IMPLEMENTATION
#pragma once

#include "../../include/raygui.h"
#include "../common.hpp"

namespace Editor {

  inline void Draw() {
    const u32 screenWidth = GetScreenWidth();
    const u32 screenHeight = GetScreenHeight();

    GuiStatusBar((Rectangle){0, 0, (f32) screenWidth, 20}, "This is a status bar.");

    int result = GuiMessageBox((Rectangle){(screenWidth / 2.0f) - 125.0f,
                                           (screenHeight / 2.0f) - 50.0f, 250, 100},
                               GuiIconText(RAYGUI_ICON_200, "Close Window"), "Do you really want to exit?", "Yes;No");
  }

};// namespace Editor
