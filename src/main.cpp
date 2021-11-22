#include "../include/entt.hpp"
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
  std::map<std::string, Texture2D> textures;
  Camera2D camera;
};

void Init(State&);
void Exit(State&);
void Update(State&);
void FixedUpdate();
void Draw(State&);
void Input();
bool GameIsRunning();

void CameraUpdate(Camera2D &);
void DrawUI();


int main(void) {
  State state = {
      .screenWidth = 1280,
      .screenHeight = 720,
      .mapWidth = 128,
      .mapHeight = 128,
      .textures = {},
      .camera = {0},
  };

  Init(state);

  // Main game loop
  while (GameIsRunning()) {
    Update(state);
    Draw(state);
  }

  Exit(state);

  return 0;
}

void Init(State &state) {
  InitWindow(state.screenWidth, state.screenHeight,
             "raylib [core] example - basic window");

  Texture2D hex = LoadTexture("assets/textures/hexagon.png");
  Texture2D rawRomanVillTexture =
      LoadTexture("assets/textures/units/Roman_Villager.png");
  Texture2D romanVillageTexture =
      LoadTexture("assets/textures/village_roman.png");

  Image romanVillagerImage = LoadImageFromTexture(rawRomanVillTexture);
  Rectangle rect = {0.0f, 0.0f, 128.0f, 128.0};
  ImageCrop(&romanVillagerImage, rect);
  Texture2D romanVillagerTexture = LoadTextureFromImage(romanVillagerImage);
  UnloadImage(romanVillagerImage);

  state.textures.emplace("hex", hex);
  state.textures.emplace("rawRomanVillTexture", rawRomanVillTexture);
  state.textures.emplace("romanVillageTexture", romanVillageTexture);
  state.textures.emplace("romanVillagerTexture", romanVillagerTexture);

  state.camera.zoom = 2.0f;
  // SetCameraMoveControls(KEY_W, KEY_D, KEY_A, KEY_S, 0, 0);

  Map::CreateTerrain(state.registry, state.mapWidth, state.mapHeight);

  SetTargetFPS(144); // Set our game to run at 60 frames-per-second
}

void Update(State &state) {
    CameraUpdate(state.camera);

    Vector2 clickPos = GetScreenToWorld2D(GetMousePosition(), state.camera);

    if (IsMouseButtonPressed(0)) {
      Actors::UpdateSelection(state.registry, clickPos);
    }
    if (IsMouseButtonPressed(1)) {
      Actors::SetDestinations(state.registry, state.camera);
    }
    if (IsKeyPressed(KEY_ENTER)) {
      Actors::CreateNew(state.registry, clickPos,
                        state.textures.at("romanVillagerTexture"));
    }
    Actors::UpdateMovement(state.registry);

    if (IsKeyPressed(KEY_P)) {
      Map::UpdateProvinces(state.registry, clickPos);
    }
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

    DrawUI();

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

void DrawUI() {
  DrawRectangle(10, 10, 90, 20, BLACK);
  DrawFPS(20, 10);
}

bool GameIsRunning() { return !WindowShouldClose(); }

void Exit(State& state) {
  UnloadTexture(state.textures.at("hex"));
  UnloadTexture(state.textures.at("romanVillageTexture"));
  UnloadTexture(state.textures.at("rawRomanVillTexture"));
  UnloadTexture(state.textures.at("romanVillagerTexture"));

  CloseWindow(); // Close window and OpenGL context
}
