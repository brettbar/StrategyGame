/*
  Authored by Brett Barinaga on 11/29/21.
  Copyright (c) Brett Barinaga, All rights reserved.

TEMPORARY TODOS HERE
  @TODO Figure out passing state by ref or val for each function
  @TODO See if excessive State & params can be replaced with only the
      properties that are actually be used.
  @TODO clean up redunant view captures, maybe one view capture
    per type per frame, then pass that as ref or val?
  @TODO a more general clean up of the code
*/
#include "common.hpp"

#include "actors.hpp"
#include "map.hpp"
#include "resource.hpp"
#include "ui/ui.hpp"
#include <cstdio>
#include <raylib.h>

#define MAX_SPRITES 100
#define MAX_BATCH_ELEMENTS 8192

void Init(State &);
void Input(State &);
void Update(State &);
void LateUpdate(State &);
void Draw(State &);
void Exit(State &);

bool GameIsRunning();
void CameraUpdate(Camera2D &);
void ZoomCamera(Camera2D &, f32, Vector2);

State state = {.screenWidth = 1920,
               .screenHeight = 1080,
               .mapWidth = 128,
               .mapHeight = 128,
               .timeScale = 0.0f,
               .prevTimeScale = 1.0f,
               .textures = {},
               .debug = true};
TextureCache textureCache = {};

int main(void) {
  Init(state);

  // Main game loop
  f32 MS_PER_UPDATE = 1 / 60.0;
  f32 ONCE_A_SECOND = 1;
  f32 oncelag = 0.0f;

  f32 lag = 0.0f;
  f32 dt = 0.0f;

  while (GameIsRunning()) {
    dt = GetFrameTime();

    lag += dt;
    oncelag += dt;

    Input(state);

    while (lag >= MS_PER_UPDATE) {
      Update(state);
      lag -= MS_PER_UPDATE;
    }

    while (oncelag >= ONCE_A_SECOND * (1 / state.timeScale)) {
      LateUpdate(state);
      oncelag = 0.0f;
    }

    Draw(state);
  }

  Exit(state);

  return 0;
}

void Init(State &state) {
  InitWindow(state.screenWidth, state.screenHeight,
             "raylib [core] example - basic window");


  constexpr entt::id_type hs = entt::hashed_string{"hexagon"};
  textureCache.load<TextureLoader>(hs, LoadTexture("assets/textures/hexagon.png"));
    
  Texture2D romanVillagerTexture =
      LoadTexture("assets/textures/units/Roman_Villager.png");
  Texture2D greekVillagerTexture =
      LoadTexture("assets/textures/units/Greek_Villager.png");
  Texture2D celtVillagerTexture =
      LoadTexture("assets/textures/units/Celt_Villager.png");
  Texture2D punicVillagerTexture =
      LoadTexture("assets/textures/units/Carthaginian_Villager.png");
  Texture2D persianVillagerTexture =
      LoadTexture("assets/textures/units/Persian_Villager.png");
  Texture2D romanVillageTexture =
      LoadTexture("assets/textures/village_roman.png");
  Texture2D factionOverlay = LoadTexture("assets/textures/overlays.png");

  // state.textures.emplace("hex", hex);
  state.textures.emplace("factionOverlay", factionOverlay);
  state.textures.emplace("romanVillagerTexture", romanVillagerTexture);
  state.textures.emplace("greekVillagerTexture", greekVillagerTexture);
  state.textures.emplace("celtVillagerTexture", celtVillagerTexture);
  state.textures.emplace("punicVillagerTexture", punicVillagerTexture);
  state.textures.emplace("persianVillagerTexture", persianVillagerTexture);
  state.textures.emplace("romanVillageTexture", romanVillageTexture);

  state.camera = Camera2D{
      .offset = {(f32)GetScreenWidth() / 2, (f32)GetScreenHeight() / 2},
      .target = {(state.mapWidth * 128.0f) / 2, (state.mapHeight * 128.0f) / 2},
      .rotation = 0,
      .zoom = 2.0f,
  };
  // SetCameraMoveControls(KEY_W, KEY_D, KEY_A, KEY_S, 0, 0);

  UI::Init(state);
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
    } else if (state.timeScale == 0.0f) {
      state.timeScale = state.prevTimeScale;
    }
  }

  if (IsKeyPressed(KEY_MINUS)) {
    state.timeScale -= 0.5f;
    if (state.timeScale < 0.0f)
      state.timeScale = 0.0f;

    if (state.timeScale == 0.0f && state.prevTimeScale > 0.5f) {
      state.prevTimeScale -= 0.5f;
      state.timeScale = state.prevTimeScale;
    }
  }

  if (IsKeyPressed(KEY_EQUAL)) {
    state.timeScale += 0.5f;
    if (state.timeScale > 1.5f)
      state.timeScale = 1.5f;
  }

  if (IsKeyPressed(KEY_TAB)) {
    Actors::CreateNew(state.registry, clickPos, state.selectedTexture);
  }

  if (IsKeyDown(KEY_LEFT_SHIFT)) {
    if (IsKeyPressed(KEY_ONE)) {
      Map::SetProvinceOwner(state.registry, 0, clickPos);
    }
    if (IsKeyPressed(KEY_TWO)) {
      Map::SetProvinceOwner(state.registry, 1, clickPos);
    }
    if (IsKeyPressed(KEY_THREE)) {
      Map::SetProvinceOwner(state.registry, 2, clickPos);
    }
    if (IsKeyPressed(KEY_FOUR)) {
      Map::SetProvinceOwner(state.registry, 3, clickPos);
    }
    if (IsKeyPressed(KEY_FIVE)) {
      Map::SetProvinceOwner(state.registry, 4, clickPos);
    }
    return;
  }

  if (IsKeyPressed(KEY_GRAVE)) {
    state.debug = !state.debug;
  }

  if (IsMouseButtonPressed(0)) {
    UI::Input(state);
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
  if (IsKeyPressed(KEY_FIVE)) {
    Actors::CreateNew(state.registry, clickPos, 4, state.textures);
  }
}

void Update(State &state) {
  Actors::UpdateMovement(state.registry, state.timeScale);
  state.screenWidth = GetScreenWidth();
  state.screenHeight = GetScreenHeight();
}

void LateUpdate(State &state) { Map::UpdateProvinces(state.registry); }

void Draw(State &state) {
  BeginDrawing();

  ClearBackground(DARKGRAY);

  BeginMode2D(state.camera);

  // Texture2D &hex = state.textures.at("hex");
  Texture2D hex = textureCache.handle(entt::hashed_string{"hexagon"})->texture;
  Rectangle frameRec = {0.0f, 0.0f, (f32)hex.width / 5, (f32)hex.height};
  Map::DrawTerrain(state.registry, hex, frameRec);

  Map::DrawProvinces(state);
  Actors::Draw(state.registry, state.debug);

  EndMode2D();

  UI::Update(state);
  UI::Draw(state);

  EndDrawing();
}

void CameraUpdate(Camera2D &camera) {
  f32 cameraSpeed = 5.0f;
  // Vector2 screenCenter = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  // Vector2 target = GetScreenToWorld2D(screenCenter, camera);
  // PrintVec2(target);

  // camera.offset = target;

  if (IsKeyDown(KEY_D))
    camera.target.x += cameraSpeed * 1 / camera.zoom;
  if (IsKeyDown(KEY_A))
    camera.target.x -= cameraSpeed * 1 / camera.zoom;
  if (IsKeyDown(KEY_W))
    camera.target.y -= cameraSpeed * 1 / camera.zoom;
  if (IsKeyDown(KEY_S))
    camera.target.y += cameraSpeed * 1 / camera.zoom;

  if (IsKeyDown(KEY_Z))
    camera.zoom -= 0.05f;
  if (IsKeyDown(KEY_X))
    camera.zoom += 0.05f;

  f32 mouseWheelDelta = GetMouseWheelMove();

  camera.zoom += (mouseWheelDelta * 0.05f);
  if (camera.zoom > 3.0f)
    camera.zoom = 3.0f;
  else if (camera.zoom < 0.1f)
    camera.zoom = 0.1f;
}

bool GameIsRunning() { return !WindowShouldClose(); }

void Exit(State &state) {
  // UnloadTexture(state.textures.at("hex"));
  UnloadTexture(textureCache.handle(entt::hashed_string{"hexagon"})->texture);
  UnloadTexture(state.textures.at("factionOverlay"));
  UnloadTexture(state.textures.at("romanVillagerTexture"));
  UnloadTexture(state.textures.at("greekVillagerTexture"));
  UnloadTexture(state.textures.at("celtVillagerTexture"));
  UnloadTexture(state.textures.at("punicVillagerTexture"));
  UnloadTexture(state.textures.at("persianVillagerTexture"));
  UnloadTexture(state.textures.at("romanVillagerTexture"));
  UnloadTexture(state.textures.at("romanVillageTexture"));

  CloseWindow(); // Close window and OpenGL context
}
