/*
  Authored by Brett Barinaga on 11/29/21. Copyright (c) Brett Barinaga, All
rights reserved.

TEMPORARY TODOS HERE
  @TODO Figure out passing state by ref or val for each function
  @TODO See if excessive State & params can be replaced with only the
      properties that are actually be used. @TODO clean up redunant view captures, maybe one view capture
    per type per frame, then pass that as ref or val?
  @TODO a more general clean up of the code
*/
#include "input.hpp"
#include "renderer/renderer.hpp"
#include "renderer/textures.hpp"
#include "state.hpp"
#include "world/systems/animation_system.hpp"
#include "world/systems/event_system.hpp"
#include "world/systems/map/province_system.hpp"
#include "world/systems/map/terrain_system.hpp"
#include "world/systems/movement_system.hpp"
#include "world/systems/player_system.hpp"
#include "world/systems/selection_system.hpp"
#include "world/systems/spawn_system.hpp"
#include "world/systems/ui/ui_system.hpp"
#include <raylib.h>

void LoadResources( TextureCache & );
bool GameIsRunning();
void CameraUpdate( Camera2D &, f32 );
void ZoomCamera( Camera2D &, f32, Vector2 );

void Init( State &, entt::registry &, TextureCache & );
void Update( State &, entt::registry & );
void LateUpdate( State &, entt::registry & );
void Exit( TextureCache & );

int main( void ) {
  State state = {
    .mapWidth = 128,
    .mapHeight = 128,
    .timeScale = 0.0f,
    .prevTimeScale = 1.0f,
    .gameState = EDITOR,
    .month = 1,
    .year = 4,
    .startYear = 4,
    .currPlayer = std::make_shared<PlayerSystem>(
      PlayerSystem( 0, ROMANS, "Roman Republic" ) ),
  };

  entt::registry reg;
  TextureCache textureCache = {};

  Init( state, reg, textureCache );

  // Main game loop
  f32 MS_PER_UPDATE = 1 / 60.0;
  f32 ONCE_A_SECOND = 1;
  f32 oncelag = 0.0f;

  f32 lag = 0.0f;
  f32 dt = 0.0f;

  while ( GameIsRunning() ) {
    dt = GetFrameTime();

    lag += dt;
    oncelag += dt;

    Input::Handle( state, reg, textureCache );

    while ( lag >= MS_PER_UPDATE ) {
      Update( state, reg );
      lag -= MS_PER_UPDATE;
    }

    while ( oncelag >= ONCE_A_SECOND * ( 1 / state.timeScale ) ) {
      LateUpdate( state, reg );
      oncelag = 0.0f;
    }

    CameraUpdate( state.camera, dt );

    Renderer::Draw( state, reg, textureCache );
  }

  Exit( textureCache );

  return 0;
}

void Init( State &state, entt::registry &reg, TextureCache &cache ) {
  SetConfigFlags( FLAG_WINDOW_RESIZABLE );
  SetTargetFPS( 144 );// Set our game to run at 60 frames-per-second
  const f32 monitor_w = GetMonitorWidth( 0 );


  if ( monitor_w <= 1920 ) {
    InitWindow(
      1280,
      720,
      // GetScreenWidth(),
      // GetScreenHeight(),
      "FieldsOfMars" );
  } else {
    InitWindow(
      1920,
      1080,
      // GetScreenWidth(),
      // GetScreenHeight(),
      "FieldsOfMars" );
  }


  LoadResources( cache );

  // state.camera = Camera2D{
  //   .offset = { (f32) GetScreenWidth() / 2, (f32) GetScreenHeight() / 2 },
  //   .target =
  //     { ( state.mapWidth * 128.0f ) / 2, ( state.mapHeight * 128.0f ) / 2 },
  //   .rotation = 0,
  //   .zoom = 2.0f,
  // };
  // SetCameraMoveControls(KEY_W, KEY_D, KEY_A, KEY_S, 0, 0);

  Terrain::CreateTerrain( reg );
  ProvinceSystem::InitProvinces( reg, cache );
  SpawnSystem::Init();
  UI::Init( reg );
  Renderer::Init( state, cache );
}

void Update( State &state, entt::registry &reg ) {
  MovementSystem::Update( reg, state.timeScale );
  AnimationSystem::UpdateSprites( reg, state.timeScale );
  //  Terrain::UpdateFOW(reg);
  UI::Update( reg );
}

void LateUpdate( State &state, entt::registry &reg ) {
  ProvinceSystem::UpdateProvinces( state, reg );
  SpawnSystem::Update( state, reg );

  state.day++;

  if ( state.month < 12 )
    state.month++;
  else {
    state.year++;
    state.month = 1;
  }
}

void Exit( TextureCache &cache ) {
  // @TODO figure out all deallocs or whatever

  UnloadShader( Renderer::shader );

  UnloadTexture( cache.handle( hstr{ "hexagon" } )->texture );
  UnloadTexture( cache.handle( hstr{ "test" } )->texture );
  UnloadTexture( cache.handle( hstr{ "template" } )->texture );
  UnloadTexture( cache.handle( hstr{ "factionOverlay" } )->texture );
  UnloadTexture( cache.handle( hstr{ "romanVillagerTexture" } )->texture );
  UnloadTexture( cache.handle( hstr{ "greekVillagerTexture" } )->texture );
  UnloadTexture( cache.handle( hstr{ "celtVillagerTexture" } )->texture );
  UnloadTexture( cache.handle( hstr{ "punicVillagerTexture" } )->texture );
  UnloadTexture( cache.handle( hstr{ "persianVillagerTexture" } )->texture );
  UnloadTexture( cache.handle( hstr{ "romanVillageTexture" } )->texture );

  cache.clear();
  CloseWindow();// Close window and OpenGL context
}

void CameraUpdate( Camera2D &camera, f32 dt ) {
  f32 cameraSpeed = 500.0f;
  // Vector2 screenCenter = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  // Vector2 target = GetScreenToWorld2D(screenCenter, camera);
  // PrintVec2(target);

  // camera.offset = target;

  if ( IsKeyDown( KEY_D ) )
    camera.target.x += dt * cameraSpeed / camera.zoom;
  if ( IsKeyDown( KEY_A ) )
    camera.target.x -= dt * cameraSpeed / camera.zoom;
  if ( IsKeyDown( KEY_W ) )
    camera.target.y -= dt * cameraSpeed / camera.zoom;
  if ( IsKeyDown( KEY_S ) )
    camera.target.y += dt * cameraSpeed / camera.zoom;

  if ( IsKeyDown( KEY_Z ) )
    camera.zoom -= 0.05f;
  if ( IsKeyDown( KEY_X ) )
    camera.zoom += 0.05f;

  f32 mouseWheelDelta = GetMouseWheelMove();

  camera.zoom += ( mouseWheelDelta * 0.05f );
  if ( camera.zoom > 8.0f )
    camera.zoom = 8.0f;
  else if ( camera.zoom < 0.08f )
    camera.zoom = 0.08f;

  camera.offset = { (f32) GetScreenWidth() / 2, (f32) GetScreenHeight() / 2 };
}

inline Texture2D InitTileOutline() {
  Image base = GenImageColor( 129, 129, ColorAlpha( WHITE, 0.0 ) );

  // N -> NE
  ImageDrawLineV( &base, { 0, 32 }, { 64, 0 }, YELLOW );
  // NE -> SE
  ImageDrawLineV( &base, { 128, 32 }, { 128, 96 }, YELLOW );
  // SE -> S
  ImageDrawLineV( &base, { 64, 0 }, { 128, 32 }, YELLOW );
  // S -> SW
  ImageDrawLineV( &base, { 128, 96 }, { 64, 128 }, YELLOW );
  // SW -> NW
  ImageDrawLineV( &base, { 64, 128 }, { 0, 96 }, YELLOW );
  // NW -> N
  ImageDrawLineV( &base, { 0, 96 }, { 0, 32 }, YELLOW );

  Texture2D texture = LoadTextureFromImage( base );

  return texture;
}

void LoadResources( TextureCache &cache ) {
  //  Image hexagon = LoadImage("assets/textures/hexagon.png");
  //  ImageResize(&hexagon, 512, 512);
  //  Texture hexTex = LoadTextureFromImage(hexagon);
  //
  //  cache.load<TextureLoader>(hstr{"hexagon"}, hexTex);
  // LoadResource( hstr{ "hexagon" }, "assets/textures/hexagon.png", cache );

  cache.load<TextureLoader>( hstr{ "tile_outline" }, InitTileOutline() );

  // Hexes
  Image hexes = LoadImage( "assets/textures/hexagon.png" );

  Image land_hex = ImageCopy( hexes );
  ImageCrop( &land_hex, { 0, 0, 130, 130 } );
  LoadResource( hstr{ "land_tile" }, land_hex, cache );

  Image water_hex = ImageCopy( hexes );
  ImageCrop( &water_hex, { 130, 0, 130, 130 } );
  LoadResource( hstr{ "water_tile" }, water_hex, cache );

  Image hills_hex = ImageCopy( hexes );
  ImageCrop( &hills_hex, { 260, 0, 130, 130 } );
  LoadResource( hstr{ "hills_tile" }, hills_hex, cache );

  Image sand_hex = ImageCopy( hexes );
  ImageCrop( &sand_hex, { 390, 0, 130, 130 } );
  LoadResource( hstr{ "sand_tile" }, sand_hex, cache );

  Image snow_hex = ImageCopy( hexes );
  ImageCrop( &snow_hex, { 520, 0, 130, 130 } );
  LoadResource( hstr{ "snow_tile" }, snow_hex, cache );

  LoadResource(
    hstr{ "factionOverlay" },
    LoadImage( "assets/textures/overlays.png" ),
    cache );

  LoadResource(
    hstr{ "template" },
    LoadImage( "assets/textures/Template.png" ),
    cache );

  LoadResource(
    hstr{ "romanVillagerTexture" },
    LoadImage( "assets/textures/units/Roman_Villager.png" ),
    cache );
  LoadResource(
    hstr{ "greekVillagerTexture" },
    LoadImage( "assets/textures/units/Greek_Villager.png" ),
    cache );
  LoadResource(
    hstr{ "celtVillagerTexture" },
    LoadImage( "assets/textures/units/Celt_Villager.png" ),
    cache );
  LoadResource(
    hstr{ "punicVillagerTexture" },
    LoadImage( "assets/textures/units/Carthaginian_Villager.png" ),
    cache );
  LoadResource(
    hstr{ "persianVillagerTexture" },
    LoadImage( "assets/textures/units/Persian_Villager.png" ),
    cache );
  LoadResource(
    hstr{ "romanVillageTexture" },
    LoadImage( "assets/textures/village_roman.png" ),
    cache );

  LoadResource(
    hstr{ "buildings" },
    LoadImage( "assets/textures/buildings.png" ),
    cache );
}

bool GameIsRunning() { return !WindowShouldClose(); }
