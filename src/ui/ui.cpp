#include "ui.h"
#include <raylib.h>

namespace UI {

void Init(State &state) {
  auto drawerEntity = state.registry.create();
  auto sidebarEntity = state.registry.create();

  int currId = 0;

  Element drawer = CreateDrawer(state);
  Element sidebar = CreateSideBar(state);

  state.registry.emplace<Element>(drawerEntity, drawer);
  state.registry.emplace<Element>(sidebarEntity, sidebar);
}

void Input(State &state) {
  auto view = state.registry.view<Element>();
  Vector2 mousePos = GetMousePosition();

  view.each([&state, mousePos](Element &uiElement) {
    if (CheckCollisionPointRec(mousePos, uiElement.panel)) {
      if (uiElement.name == "sprite_list" && state.debug) {
        for (auto child : uiElement.children) {
          if (CheckCollisionPointRec(mousePos, child.panel)) {
            state.selectedTexture = child.texture;
          }
        }
      }
    }
  });
}

void Update(State &state) {
  auto view = state.registry.view<Element>();

  view.each([&state](Element &drawer) {
    if (drawer.name == "sprite_list") {
      drawer.panel = {(f32)state.screenWidth - 192,
                      ((f32)state.screenHeight / 4), 192,
                      (f32)state.screenHeight / 2};

      f32 offset = 0;
      for (u32 i = 0; i < drawer.children.size(); i++) {
        if (i % 2 == 0) {
          drawer.children[i].position.x = drawer.panel.x;
          drawer.children[i].position.y = drawer.panel.y + 64.0 * offset;
        } else {
          drawer.children[i].position.x = drawer.panel.x + 64.0;
          drawer.children[i].position.y = drawer.panel.y + 64.0 * offset++;
        }
        drawer.children[i].panel = {drawer.children[i].position.x + 32.0f,
                                    drawer.children[i].position.y + 32.0f, 64,
                                    64};
      }
    }
  });
}

void Draw(State &state) {
  auto view = state.registry.view<Element>();


  view.each([&state](Element &element) {
    if (element.name == "sprite_list") {
      if (state.debug)
        DrawElement(element);

      for (auto child : element.children) {
        if (state.debug)
          DrawElement(child);
      }
    } else {
      DrawElement(element);
    }
  });
  DrawTopBar(state);

}

void DrawTopBar(State &state) {
  DrawRectangle(0, 0, (f32)state.screenWidth, 24.0f, BLACK);
  DrawFPS(16, 2);
  DrawText(std::to_string(state.timeScale).c_str(), state.screenWidth - 128, 2,
           20, WHITE);
}

void DrawElement(Element element) {
  switch (element.shape) {
  case Texture:
    DrawTextureEx(element.texture, element.position, 0.0f, 1.0f, WHITE);
    break;
  case Rect:
    DrawRectangleRec(element.panel, element.color);
    break;
  case Rect_Rounded:
    DrawRectangleRounded(element.panel, 0.2f, 1, DARKGRAY);
    break;
  case Rect_Lines:
    DrawRectangleLinesEx(element.panel, 2, WHITE);
    break;
  }
}

Element CreateButton(Texture2D texture, Vector2 position, Color color) {
  Element element = Element();
  element.id = 0;
  element.name = "sprite_button";
  element.shape = Texture;
  element.debugOnly = true;
  element.position = position;
  element.texture = texture;
  element.panel = {position.x + 32.0f, position.y + 32.0f, 64, 64};
  element.children = {};
  return element;
}

Element CreateSideBar(State &state) {
  Element sidebar = Element();
  sidebar.id = 0;
  sidebar.name = "side_bar";
  sidebar.shape = Rect;
  sidebar.debugOnly = false;
  sidebar.panel = {0, 0, 100, (f32)state.screenHeight};
  sidebar.color = DARKGRAY;
  sidebar.children = {};
  return sidebar;
}

Element CreateDrawer(State &state) {
  Element drawer = Element();
  drawer.id = 0;
  drawer.name = "sprite_list";
  drawer.shape = Rect_Rounded;
  drawer.debugOnly = true;
  drawer.panel = {(f32)state.screenWidth - 192, ((f32)state.screenHeight / 4),
                  192, (f32)state.screenHeight / 2};
  drawer.children = {
      CreateButton(state.textures.at("romanVillagerTexture"),
                   {drawer.panel.x, drawer.panel.y}, WHITE),
      CreateButton(state.textures.at("romanVillageTexture"),
                   {drawer.panel.x + 64.0f, drawer.panel.y}, WHITE),

      CreateButton(state.textures.at("greekVillagerTexture"),
                   {drawer.panel.x, drawer.panel.y + 64.0f}, WHITE),
      CreateButton(state.textures.at("romanVillageTexture"),
                   {drawer.panel.x + 64.0f, drawer.panel.y + 64.0f}, WHITE),

      CreateButton(state.textures.at("celtVillagerTexture"),
                   {drawer.panel.x, drawer.panel.y + 128.0f}, WHITE),
      CreateButton(state.textures.at("romanVillageTexture"),
                   {drawer.panel.x + 64.0f, drawer.panel.y + 128.0f}, WHITE),

      CreateButton(state.textures.at("punicVillagerTexture"),
                   {drawer.panel.x, drawer.panel.y + 192.0f}, WHITE),
      CreateButton(state.textures.at("romanVillageTexture"),
                   {drawer.panel.x + 64.0f, drawer.panel.y + 192.0f}, WHITE),

      CreateButton(state.textures.at("persianVillagerTexture"),
                   {drawer.panel.x, drawer.panel.y + 256.0f}, WHITE),
      CreateButton(state.textures.at("romanVillageTexture"),
                   {drawer.panel.x + 64.0f, drawer.panel.y + 256.0f}, WHITE),
  };
  return drawer;
}

}; // namespace UI
