#include "ui.h"

void DrawUI(State &state) {
  DrawRectangle(10, 10, 90, 20, BLACK);
  DrawFPS(20, 10);

  DrawRectangle(1170, 10, 90, 20, BLACK);
  DrawText(std::to_string(state.timeScale).c_str(), 1170, 10, 20, WHITE);

  // GuiGrid((Rectangle){0, 0, (f32)state.screenWidth,
  // (f32)state.screenHeight}, 20.0f, 2);
  if (state.debug) {
    Rectangle panel = {(f32)state.screenWidth - 200,
                       ((f32)state.screenHeight / 2) - 400, 200, 800};
    DrawRectangleRounded(panel, 0.2f, 3, DARKGRAY);

    DrawTextureEx(state.textures.at("romanVillagerTexture"), {panel.x, panel.y}, 0.0f, 1.0f, WHITE);
  }
}
