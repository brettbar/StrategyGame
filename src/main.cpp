#include "../include/entt/entt.hpp"
#include "../include/raygui/raygui.h"
#include "actors.h"
#include "map.h"
#include "raylib.h"
#include "raymath.h"
#include <map>

#define MAX_SPRITES 100
#define MAX_BATCH_ELEMENTS 8192

using u32 = unsigned int;
using i32 = int;
using f32 = float;

struct State {
  entt::registry registry;
  u32 screenWidth;
  u32 screenHeight;
  const u32 mapWidth;
  const u32 mapHeight;
  f32 timeScale;
  f32 prevTimeScale;
  std::map<std::string, Texture2D> textures;
  Camera2D camera;
};

void Init(State &);
void Exit(State &);
void Input(State &);
void FixedUpdate(State &, f32);
void Draw(State &);
bool GameIsRunning();

void CameraUpdate(Camera2D &);
void DrawUI(State &);

int main(void) {
  State state = {
      .screenWidth = 1280,
      .screenHeight = 720,
      .mapWidth = 128,
      .mapHeight = 128,
      .timeScale = 0.0f,
      .prevTimeScale = 0.0f,
      .textures = {},
      .camera = {0},
  };

  Init(state);

  // Main game loop
  f32 lag = 0.0f;
  f32 MS_PER_UPDATE = 1000;

  while (GameIsRunning()) {
    f32 dt = GetFrameTime() * 1000;
    lag += dt;

    Input(state);

    while (lag >= MS_PER_UPDATE) {
      FixedUpdate(state, dt);
      lag -= MS_PER_UPDATE;
    }

    Draw(state);
  }

  Exit(state);

  return 0;
}

void Init(State &state) {
  InitWindow(state.screenWidth, state.screenHeight,
             "raylib [core] example - basic window");

  Texture2D hex = LoadTexture("assets/textures/hexagon.png");
  Texture2D romanVillagerTexture =
      LoadTexture("assets/textures/units/Roman_Villager.png");
  Texture2D greekVillagerTexture =
      LoadTexture("assets/textures/units/Greek_Villager.png");
  Texture2D celtVillagerTexture =
      LoadTexture("assets/textures/units/Celt_Villager.png");
  Texture2D punicVillagerTexture =
      LoadTexture("assets/textures/units/Carthaginian_Villager.png");

  Texture2D romanVillageTexture =
      LoadTexture("assets/textures/village_roman.png");

  state.textures.emplace("hex", hex);
  state.textures.emplace("romanVillagerTexture", romanVillagerTexture);
  state.textures.emplace("greekVillagerTexture", greekVillagerTexture);
  state.textures.emplace("celtVillagerTexture", celtVillagerTexture);
  state.textures.emplace("punicVillagerTexture", punicVillagerTexture);
  state.textures.emplace("romanVillageTexture", romanVillageTexture);

  state.camera.zoom = 2.0f;
  // SetCameraMoveControls(KEY_W, KEY_D, KEY_A, KEY_S, 0, 0);

  Map::CreateTerrain(state.registry, state.mapWidth, state.mapHeight);

  SetTargetFPS(144); // Set our game to run at 60 frames-per-second
}

void Input(State &state) {
  CameraUpdate(state.camera);

  Vector2 clickPos = GetScreenToWorld2D(GetMousePosition(), state.camera);

  if (IsKeyPressed(KEY_SPACE)) {
    if (state.timeScale > 0.0f) {
      state.prevTimeScale = state.timeScale;
      state.timeScale = 0.0f;
    } else {
      state.timeScale = state.prevTimeScale;
    }
  }

  if (IsKeyPressed(KEY_MINUS)) {
    state.timeScale -= 0.5f;
    if (state.timeScale < 0.0f)
      state.timeScale = 0.0f;
  }

  if (IsKeyPressed(KEY_EQUAL)) {
    state.timeScale += 0.5f;
    if (state.timeScale > 1.5f)
      state.timeScale = 1.5f;
  }

  if (IsMouseButtonPressed(0)) {
    Actors::UpdateSelection(state.registry, clickPos);
  }
  if (IsMouseButtonPressed(1)) {
    Actors::SetDestinations(state.registry, state.camera);
  }
  if (IsKeyPressed(KEY_ONE)) {
    Actors::CreateNew(state.registry, clickPos, 0, state.textures);
  }
  if (IsKeyPressed(KEY_TWO)) {
    Actors::CreateNew(state.registry, clickPos, 1, state.textures);
  }
  if (IsKeyPressed(KEY_THREE)) {
    Actors::CreateNew(state.registry, clickPos, 2, state.textures);
  }
  if (IsKeyPressed(KEY_FOUR)) {
    Actors::CreateNew(state.registry, clickPos, 3, state.textures);
  }
}

void FixedUpdate(State &state, f32 deltaTime) {
  Actors::UpdateMovement(state.registry, deltaTime * 100, state.timeScale);
  Map::UpdateProvinces(state.registry);
}

void Draw(State &state) {
  BeginDrawing();

  ClearBackground(DARKGRAY);

  BeginMode2D(state.camera);

  Texture2D &hex = state.textures.at("hex");
  Rectangle frameRec = {0.0f, 0.0f, (f32)hex.width / 5, (f32)hex.height};
  Map::DrawTerrain(state.registry, hex, frameRec);

  Texture2D &romanVillageTexture = state.textures.at("romanVillageTexture");
  Map::DrawProvinces(state.registry, romanVillageTexture);
  Actors::Draw(state.registry);

  EndMode2D();

  DrawUI(state);

  EndDrawing();
}

void CameraUpdate(Camera2D &camera) {
  f32 cameraSpeed = 4.0f;

  if (IsKeyDown(KEY_D))
    camera.offset.x -= cameraSpeed;
  if (IsKeyDown(KEY_A))
    camera.offset.x += cameraSpeed;
  if (IsKeyDown(KEY_W))
    camera.offset.y += cameraSpeed;
  if (IsKeyDown(KEY_S))
    camera.offset.y -= cameraSpeed;

  if (IsKeyDown(KEY_Z))
    camera.zoom -= 0.05f;
  if (IsKeyDown(KEY_X))
    camera.zoom += 0.05f;

  camera.zoom += ((f32)GetMouseWheelMove() * 0.05f);
  if (camera.zoom > 3.0f)
    camera.zoom = 3.0f;
  else if (camera.zoom < 0.1f)
    camera.zoom = 0.1f;
}

void DrawUI(State &state) {
  DrawRectangle(10, 10, 90, 20, BLACK);
  DrawFPS(20, 10);

  DrawRectangle(1170, 10, 90, 20, BLACK);
  DrawText(std::to_string(state.timeScale).c_str(), 1170, 10, 20, WHITE);
}

bool GameIsRunning() { return !WindowShouldClose(); }

void Exit(State &state) {
  UnloadTexture(state.textures.at("hex"));
  UnloadTexture(state.textures.at("romanVillagerTexture"));
  UnloadTexture(state.textures.at("greekVillagerTexture"));
  UnloadTexture(state.textures.at("celtVillagerTexture"));
  UnloadTexture(state.textures.at("punicVillagerTexture"));
  UnloadTexture(state.textures.at("romanVillagerTexture"));
  UnloadTexture(state.textures.at("romanVillageTexture"));

  CloseWindow(); // Close window and OpenGL context
}
