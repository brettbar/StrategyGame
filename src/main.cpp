#include "../include/entt.hpp"
#include "actors.h"
#include "map.h"
#include "raylib.h"
#include "raymath.h"

#define MAX_SPRITES 100
#define MAX_BATCH_ELEMENTS 8192

using u32 = unsigned int;
using i32 = int;
using f32 = float;

void CameraUpdate(Camera2D &);
void DrawUI();

int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  const u32 screenWidth = 1280;
  const u32 screenHeight = 720;

  const u32 mapWidth = 128;
  const u32 mapHeight = 128;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

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

  Rectangle frameRec = {0.0f, 0.0f, (f32)hex.width / 5, (f32)hex.height};

  Camera2D camera = {0};
  camera.zoom = 2.0f;
  // SetCameraMoveControls(KEY_W, KEY_D, KEY_A, KEY_S, 0, 0);
  entt::registry registry;

  Map::CreateTerrain(registry, mapWidth, mapHeight);

  SetTargetFPS(144); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) { // Detect window close button or ESC key
    // Update
    CameraUpdate(camera);

    Vector2 clickPos = GetScreenToWorld2D(GetMousePosition(), camera);

    if (IsMouseButtonPressed(0)) {
      Actors::UpdateSelection(registry, clickPos);
    }
    if (IsMouseButtonPressed(1)) {
      Actors::SetDestinations(registry, camera);
    }
    if (IsKeyPressed(KEY_ENTER)) {
      Actors::CreateNew(registry, clickPos, romanVillagerTexture);
    }
    Actors::UpdateMovement(registry);

    if (IsKeyPressed(KEY_P)) {
      Map::UpdateProvinces(registry, clickPos);
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(DARKGRAY);

    BeginMode2D(camera);

    Map::DrawTerrain(registry, hex, frameRec);
    Map::DrawProvinces(registry, romanVillageTexture);
    Actors::Draw(registry);

    EndMode2D();

    DrawUI();

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  UnloadTexture(hex);
  UnloadTexture(romanVillageTexture);
  UnloadTexture(rawRomanVillTexture);
  UnloadTexture(romanVillagerTexture);

  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
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
