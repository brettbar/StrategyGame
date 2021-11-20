#include "../include/entt.hpp"
#include "map.h"
#include "raylib.h"
#include "raymath.h"

#define MAX_SPRITES 100
#define MAX_BATCH_ELEMENTS 8192

using u32 = unsigned int;
using i32 = int;
using f32 = float;

struct RomanVillager {
  Vector2 position;
  Vector2 destination;
  f32 speed;
  Texture2D sprite;
};

void CameraUpdate(Camera2D &);

int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  const u32 screenWidth = 1920;
  const u32 screenHeight = 1080;

  const u32 mapWidth = 128;
  const u32 mapHeight = 128;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

  Texture2D hex = LoadTexture("assets/textures/hexagon.png");

  Texture2D rawRomanVillTexture =
      LoadTexture("assets/textures/units/Roman_Villager.png");

  Image romanVillagerImage = LoadImageFromTexture(rawRomanVillTexture);
  Rectangle rect = {0.0f, 0.0f, 128.0f, 128.0};
  ImageCrop(&romanVillagerImage, rect);
  Texture2D romanVillagerTexture = LoadTextureFromImage(romanVillagerImage);
  UnloadImage(romanVillagerImage);

  Rectangle frameRec = {0.0f, 0.0f, (f32)hex.width / 5, (f32)hex.height};

  Camera2D camera = {0};
  // SetCameraMoveControls(KEY_W, KEY_D, KEY_A, KEY_S, 0, 0);

  entt::registry registry;

  Map::CreateMap(registry, mapWidth, mapHeight);

  SetTargetFPS(144); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) { // Detect window close button or ESC key
    // Update
    CameraUpdate(camera);

    auto villagers = registry.view<RomanVillager>();

    if (IsMouseButtonPressed(0)) {
      entt::entity entity = registry.create();
      Vector2 clickPos = GetScreenToWorld2D(GetMousePosition(), camera);
      RomanVillager roman = RomanVillager{
          .position = clickPos,
          .destination = clickPos,
          .speed = 1.0f,
          .sprite = romanVillagerTexture,
      };

      registry.emplace<RomanVillager>(entity, roman.position, roman.destination,
                                      roman.speed, roman.sprite);
    }

    if (IsMouseButtonPressed(1)) {
      villagers.each([camera](RomanVillager &romanVillager) {
        romanVillager.destination =
            GetScreenToWorld2D(GetMousePosition(), camera);
      });
    }

    villagers.each([](RomanVillager &romanVillager) {
      if (Vector2Distance(romanVillager.destination, romanVillager.position) >
          0.5f) {
        Vector2 foo = {
            romanVillager.destination.x - romanVillager.position.x,
            romanVillager.destination.y - romanVillager.position.y,
        };

        Vector2 unitVec = Vector2Normalize(foo);
        romanVillager.position.x += unitVec.x * romanVillager.speed;
        romanVillager.position.y += unitVec.y * romanVillager.speed;
      }
    });

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(DARKGRAY);

    BeginMode2D(camera);

    Map::DrawMap(registry, hex, frameRec);

    villagers.each([](RomanVillager romanVillager) {
      DrawTextureV(
          romanVillager.sprite,
          {romanVillager.position.x - 64.0f, romanVillager.position.y - 64.0f},
          WHITE);
    });
    EndMode2D();


    DrawRectangle(10, 10, 90, 20, BLACK);


    DrawFPS(20, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  UnloadTexture(hex);
  UnloadTexture(rawRomanVillTexture);
  UnloadTexture(romanVillagerTexture);

  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

void CameraUpdate(Camera2D &camera) {
  f32 cameraSpeed = 4.0f;
  camera.zoom = 2.0f;

  if (IsKeyDown(KEY_D))
    camera.offset.x -= cameraSpeed;
  if (IsKeyDown(KEY_A))
    camera.offset.x += cameraSpeed;
  if (IsKeyDown(KEY_W))
    camera.offset.y += cameraSpeed;
  if (IsKeyDown(KEY_S))
    camera.offset.y -= cameraSpeed;

  camera.zoom += ((f32)GetMouseWheelMove() * 0.05f);
  if (camera.zoom > 3.0f)
    camera.zoom = 3.0f;
  else if (camera.zoom < 0.1f)
    camera.zoom = 0.1f;
}
