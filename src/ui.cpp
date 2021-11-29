#include "ui.h"
#include <raylib.h>

namespace UI {

void Init(State &state) {
  auto entity = state.registry.create();

  UI_Element drawer = UI_Element();
  drawer.id = "sprite_list";
  drawer.debugOnly = true;
  drawer.panel = {(f32)state.screenWidth - 256,
                  ((f32)state.screenHeight / 2) - 512, 256, 1024};
  drawer.children = {
      createButton(state.textures.at("romanVillagerTexture"),
                   {drawer.panel.x, drawer.panel.y}, WHITE),
      createButton(state.textures.at("romanVillageTexture"),
                   {drawer.panel.x + 64.0f, drawer.panel.y}, WHITE),

      createButton(state.textures.at("greekVillagerTexture"),
                   {drawer.panel.x, drawer.panel.y + 64.0f}, WHITE),
      createButton(state.textures.at("romanVillageTexture"),
                   {drawer.panel.x + 64.0f, drawer.panel.y + 64.0f}, WHITE),

      createButton(state.textures.at("celtVillagerTexture"),
                   {drawer.panel.x, drawer.panel.y + 128.0f}, WHITE),
      createButton(state.textures.at("romanVillageTexture"),
                   {drawer.panel.x + 64.0f, drawer.panel.y + 128.0f}, WHITE),

      createButton(state.textures.at("punicVillagerTexture"),
                   {drawer.panel.x, drawer.panel.y + 192.0f}, WHITE),
      createButton(state.textures.at("romanVillageTexture"),
                   {drawer.panel.x + 64.0f, drawer.panel.y + 192.0f}, WHITE),

      createButton(state.textures.at("persianVillagerTexture"),
                   {drawer.panel.x, drawer.panel.y + 256.0f}, WHITE),
      createButton(state.textures.at("romanVillageTexture"),
                   {drawer.panel.x + 64.0f, drawer.panel.y + 256.0f}, WHITE),
  };
  state.registry.emplace<UI_Element>(entity, drawer);
}

void Input(State &state) {
  auto view = state.registry.view<UI_Element>();
  Vector2 mousePos = GetMousePosition();

  view.each([&state, mousePos](UI_Element &uiElement) {
    if (CheckCollisionPointRec(mousePos, uiElement.panel)) {
      printf("%s\n", uiElement.id.c_str());

      if (uiElement.id == "sprite_list" && state.debug) {
        for (auto child : uiElement.children) {
          if (CheckCollisionPointRec(mousePos, child.panel)) {
            printf("%s\n", child.id.c_str());
            state.selectedTexture = child.texture;
          }
        }
      }
    }
  });
}

void Update(State &state) {}

void Draw(State &state) {
  auto view = state.registry.view<UI_Element>();

  DrawRectangle(10, 10, 90, 20, BLACK);
  DrawFPS(20, 10);

  DrawRectangle(1170, 10, 90, 20, BLACK);
  DrawText(std::to_string(state.timeScale).c_str(), 1170, 10, 20, WHITE);

  bool debug = state.debug;
  std::map<std::string, Texture2D> textures = state.textures;

  view.each([debug, textures](UI_Element &uiElement) {
    if (debug) {
      if (uiElement.id == "sprite_list") {
        DrawRectangleRounded(uiElement.panel, 0.2f, 1, DARKGRAY);

        for (auto child : uiElement.children) {
          DrawTextureEx(child.texture, child.position, 0.0f, 1.0f, WHITE);
          DrawRectangleLinesEx(child.panel, 2, WHITE);
        }
      }
    }
  });
}

UI_Element createButton(Texture2D texture, Vector2 position, Color color) {
  UI_Element uiElement = UI_Element();
  uiElement.id = "texture_button";
  uiElement.debugOnly = true;
  uiElement.children = {};
  uiElement.position = position;
  uiElement.texture = texture;
  uiElement.panel = {position.x + 32.0f, position.y + 32.0f, 64, 64};
  return uiElement;
}
}; // namespace UI
