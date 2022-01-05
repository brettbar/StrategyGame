/*
  Authored by Brett Barinaga on 11/29/21. Copyright (c) Brett Barinaga, All
rights reserved.

TEMPORARY TODOS HERE
  @TODO Figure out passing state by ref or val for each function
  @TODO See if excessive State & params can be replaced with only the
      properties that are actually be used.
  @TODO clean up redunant view captures, maybe one view capture
    per type per frame, then pass that as ref or val?
  @TODO a more general clean up of the code
*/
#include "player.hpp"
#include "resource.hpp"
#include "state.hpp"
#include "systems/animation.hpp"
#include "systems/provinces.hpp"
#include "systems/movement.hpp"
#include "systems/selection.hpp"
#include "systems/spawn.hpp"
#include "systems/terrain.hpp"
#include "ui/ui.hpp"

#define MAX_SPRITES 100
#define MAX_BATCH_ELEMENTS 8192

void LoadResources(TextureCache &);
bool GameIsRunning();
void CameraUpdate(Camera2D &);
void ZoomCamera(Camera2D &, f32, Vector2);


void Init(State &, entt::registry &, TextureCache &);
void Input(State &, entt::registry &, TextureCache &);
void Update(State &, entt::registry &);
void LateUpdate(entt::registry &);
void Draw(State &, entt::registry &, TextureCache &);
void Exit(TextureCache &);

int main(void)
{
  State state = {.screenWidth = 1280,
                 .screenHeight = 720,
                 .mapWidth = 128,
                 .mapHeight = 128,
                 .timeScale = 0.0f,
                 .prevTimeScale = 1.0f,
                 .debug = true,
                 .currPlayer = std::make_shared<Player>(Player(0, ROMANS, "Roman Republic"))};
  entt::registry reg;
  TextureCache textureCache = {};

  Init(state, reg, textureCache);

  // Main game loop
  f32 MS_PER_UPDATE = 1 / 60.0;
  f32 ONCE_A_SECOND = 1;
  f32 oncelag = 0.0f;

  f32 lag = 0.0f;
  f32 dt = 0.0f;

  while (GameIsRunning())
  {
    dt = GetFrameTime();

    lag += dt;
    oncelag += dt;

    Input(state, reg, textureCache);

    while (lag >= MS_PER_UPDATE)
    {
      Update(state, reg);
      lag -= MS_PER_UPDATE;
    }

    while (oncelag >= ONCE_A_SECOND * (1 / state.timeScale))
    {
      LateUpdate(reg);
      oncelag = 0.0f;
    }

    Draw(state, reg, textureCache);
  }

  Exit(textureCache);

  return 0;
}

void Init(State &state, entt::registry &reg, TextureCache &cache)
{
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(144);// Set our game to run at 60 frames-per-second
  InitWindow(state.screenWidth, state.screenHeight,
             "raylib [core] example - basic window");

  LoadResources(cache);

  state.camera = Camera2D{
    .offset = {(f32) GetScreenWidth() / 2, (f32) GetScreenHeight() / 2},
    .target = {(state.mapWidth * 128.0f) / 2, (state.mapHeight * 128.0f) / 2},
    .rotation = 0,
    .zoom = 2.0f,
  };
  // SetCameraMoveControls(KEY_W, KEY_D, KEY_A, KEY_S, 0, 0);

  UI::Init(state, reg, cache);
  Terrain::CreateTerrain(reg);
  Provinces::InitProvinces(reg);
}

void Input(State &state, entt::registry &reg, TextureCache &cache)
{
  CameraUpdate(state.camera);

  Vector2 clickPos = GetScreenToWorld2D(GetMousePosition(), state.camera);

  if (IsKeyPressed(KEY_SPACE))
  {
    if (state.timeScale > 0.0f)
    {
      state.prevTimeScale = state.timeScale;
      state.timeScale = 0.0f;
    }
    else if (state.timeScale == 0.0f)
    {
      state.timeScale = state.prevTimeScale;
    }
  }

  if (IsKeyPressed(KEY_MINUS))
  {
    state.timeScale -= 0.5f;
    if (state.timeScale < 0.0f)
      state.timeScale = 0.0f;

    if (state.timeScale == 0.0f && state.prevTimeScale > 0.5f)
    {
      state.prevTimeScale -= 0.5f;
      state.timeScale = state.prevTimeScale;
    }
  }

  if (IsKeyPressed(KEY_EQUAL))
  {
    state.timeScale += 0.5f;
    if (state.timeScale > 1.5f)
      state.timeScale = 1.5f;
  }

  if (IsKeyPressed(KEY_V))
  {
    Spawn::CreateNew(reg, cache, clickPos, state.currPlayer);
  }

  if (IsKeyPressed(KEY_C))
  {
    Provinces::SetProvinceOwner(reg, state.currPlayer->id, clickPos);
  }

  if (IsKeyPressed(KEY_GRAVE))
  {
    state.debug = !state.debug;
  }

  if (IsMouseButtonPressed(0))
  {
    UI::Input(state, reg);
    Selection::UpdateSelection(reg, clickPos);
  }
  if (IsMouseButtonPressed(1))
  {
    Movement::SetDestinations(reg, state.camera);
  }

  if (IsKeyPressed(KEY_ONE))
  {
    state.currPlayer->id = 0;
    state.currPlayer->faction = ROMANS;
    state.currPlayer->factionName = "Roman Republic";
    state.currPlayer->RefreshTextureMap();
  }

  if (IsKeyPressed(KEY_TWO))
  {
    state.currPlayer->id = 1;
    state.currPlayer->faction = GREEKS;
    state.currPlayer->factionName = "Greek Cities";
    state.currPlayer->RefreshTextureMap();
  }

  if (IsKeyPressed(KEY_THREE))
  {
    state.currPlayer->id = 2;
    state.currPlayer->faction = CELTS;
    state.currPlayer->factionName = "Celtic Tribes";
    state.currPlayer->RefreshTextureMap();
  }

  if (IsKeyPressed(KEY_FOUR))
  {
    state.currPlayer->id = 3;
    state.currPlayer->faction = PUNICS;
    state.currPlayer->factionName = "Punic Colonies";
    state.currPlayer->RefreshTextureMap();
  }

  if (IsKeyPressed(KEY_FIVE))
  {
    state.currPlayer->id = 4;
    state.currPlayer->faction = PERSIANS;
    state.currPlayer->factionName = "Persian Empire";
    state.currPlayer->RefreshTextureMap();
  }
}

void Update(State &state, entt::registry &reg)
{
  Movement::Update(reg, state.timeScale);
  state.screenWidth = GetScreenWidth();
  state.screenHeight = GetScreenHeight();
  Animation::UpdateSprites(reg, state.timeScale);
  //  Terrain::UpdateFOW(reg);
}

void LateUpdate(entt::registry &reg)
{
  Provinces::UpdateProvinces(reg);
}

void Draw(State &state, entt::registry &reg, TextureCache &cache)
{
  BeginDrawing();

  ClearBackground(DARKGRAY);

  BeginMode2D(state.camera);

  Texture2D hex = cache.handle(hstr{"hexagon"})->texture;
  Rectangle frameRec = {0.0f, 0.0f, (f32) hex.width / 5, (f32) hex.height};
  Terrain::DrawTerrain(reg, hex, frameRec);

  Provinces::DrawProvinces(reg, cache);
  Animation::Draw(reg, state.debug);

  EndMode2D();

  UI::Update(state, reg);
  UI::Draw(state, reg);

  EndDrawing();
}

void Exit(TextureCache &cache)
{
  UnloadTexture(cache.handle(hstr{"hexagon"})->texture);
  UnloadTexture(cache.handle(hstr{"template"})->texture);
  UnloadTexture(cache.handle(hstr{"factionOverlay"})->texture);
  UnloadTexture(cache.handle(hstr{"romanVillagerTexture"})->texture);
  UnloadTexture(cache.handle(hstr{"greekVillagerTexture"})->texture);
  UnloadTexture(cache.handle(hstr{"celtVillagerTexture"})->texture);
  UnloadTexture(cache.handle(hstr{"punicVillagerTexture"})->texture);
  UnloadTexture(cache.handle(hstr{"persianVillagerTexture"})->texture);
  UnloadTexture(cache.handle(hstr{"romanVillageTexture"})->texture);

  cache.clear();
  CloseWindow();// Close window and OpenGL context
}

void CameraUpdate(Camera2D &camera)
{
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

void LoadResources(TextureCache &cache)
{
  LoadResource(hstr{"hexagon"}, "assets/textures/hexagon.png", cache);
  LoadResource(hstr{"factionOverlay"}, "assets/textures/overlays.png", cache);

  LoadResource(hstr{"template"}, "assets/textures/Template.png", cache);

  LoadResource(hstr{"romanVillagerTexture"},
               "assets/textures/units/Roman_Villager.png", cache);
  LoadResource(hstr{"greekVillagerTexture"},
               "assets/textures/units/Greek_Villager.png", cache);
  LoadResource(hstr{"celtVillagerTexture"},
               "assets/textures/units/Celt_Villager.png", cache);
  LoadResource(hstr{"punicVillagerTexture"},
               "assets/textures/units/Carthaginian_Villager.png", cache);
  LoadResource(hstr{"persianVillagerTexture"},
               "assets/textures/units/Persian_Villager.png", cache);
  LoadResource(hstr{"romanVillageTexture"}, "assets/textures/village_roman.png",
               cache);
}

bool GameIsRunning() { return !WindowShouldClose(); }
