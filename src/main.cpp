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
#include "events.hpp"
#include "input.hpp"
#include "renderer/renderer.hpp"
#include "resource.hpp"
#include "state.hpp"
#include "ui/ui.hpp"
#include "world/player.hpp"
#include "world/systems/animation.hpp"
#include "world/systems/map/provinces.hpp"
#include "world/systems/map/terrain.hpp"
#include "world/systems/movement.hpp"
#include "world/systems/selection.hpp"
#include "world/systems/spawn.hpp"
#include <raylib.h>

void LoadResources( TextureCache & );
bool GameIsRunning();
void CameraUpdate( Camera2D &, f32 );
void ZoomCamera( Camera2D &, f32, Vector2 );

void Init( State &, entt::registry &, TextureCache & );
void Update( State &, entt::registry & );
void LateUpdate( State &, entt::registry & );
void Exit( TextureCache & );

int main( void )
{
  State state = {
    .mapWidth = 128,
    .mapHeight = 128,
    .timeScale = 0.0f,
    .prevTimeScale = 1.0f,
    .gameState = EDITOR,
    .month = 1,
    .year = 4,
    .startYear = 4,
    .currPlayer =
      std::make_shared<Player>( Player( 0, ROMANS, "Roman Republic" ) ),
  };

  entt::registry reg;
  TextureCache textureCache = {};


  Init( state, reg, textureCache );
  UI::Init();
  Renderer::Init( state );


  // auto eventConn = UI::eventEmitter.on<UI::Event>(
  //   [&reg, &state]( const UI::Event &event, UI::EventEmitter &emitter ) {
  //     switch ( event.type )
  //     {
  //       case UI::PROVINCES_SPAWN_PROVINCE:
  //         Provinces::SpawnProvince( reg, state.currPlayer->id );
  //         break;
  //     }
  //   } );

  // Main game loop
  f32 MS_PER_UPDATE = 1 / 60.0;
  f32 ONCE_A_SECOND = 1;
  f32 oncelag = 0.0f;

  f32 lag = 0.0f;
  f32 dt = 0.0f;

  while ( GameIsRunning() )
  {
    dt = GetFrameTime();

    lag += dt;
    oncelag += dt;

    Input::Handle( state, reg, textureCache );

    while ( lag >= MS_PER_UPDATE )
    {
      Update( state, reg );
      lag -= MS_PER_UPDATE;
    }

    while ( oncelag >= ONCE_A_SECOND * ( 1 / state.timeScale ) )
    {
      LateUpdate( state, reg );
      oncelag = 0.0f;
    }

    CameraUpdate( state.camera, dt );

    Renderer::Draw( state, reg, textureCache );
  }

  Exit( textureCache );

  UnloadShader( Renderer::shader );
  // @TODO figure out all deallocs or whatever

  return 0;
}

void Init( State &state, entt::registry &reg, TextureCache &cache )
{
  SetConfigFlags( FLAG_WINDOW_RESIZABLE );
  SetTargetFPS( 144 );// Set our game to run at 60 frames-per-second
  InitWindow(
    1920,
    1080,
    // GetScreenWidth(),
    // GetScreenHeight(),
    "FieldsOfMars" );

  LoadResources( cache );

  state.camera = Camera2D{
    .offset = { (f32) GetScreenWidth() / 2, (f32) GetScreenHeight() / 2 },
    .target =
      { ( state.mapWidth * 128.0f ) / 2, ( state.mapHeight * 128.0f ) / 2 },
    .rotation = 0,
    .zoom = 2.0f,
  };
  // SetCameraMoveControls(KEY_W, KEY_D, KEY_A, KEY_S, 0, 0);

  Terrain::CreateTerrain( reg );
  Provinces::InitProvinces( state, reg );
  Spawn::Init();
}


void Update( State &state, entt::registry &reg )
{
  Movement::Update( reg, state.timeScale );
  Animation::UpdateSprites( reg, state.timeScale );
  //  Terrain::UpdateFOW(reg);
}

void LateUpdate( State &state, entt::registry &reg )
{
  Provinces::UpdateProvinces( state, reg );
  Spawn::Update( state, reg );

  state.day++;

  if ( state.month < 12 )
    state.month++;
  else
  {
    state.year++;
    state.month = 1;
  }
}

void Exit( TextureCache &cache )
{
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

void CameraUpdate( Camera2D &camera, f32 dt )
{
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

void LoadResources( TextureCache &cache )
{
  //  Image hexagon = LoadImage("assets/textures/hexagon.png");
  //  ImageResize(&hexagon, 512, 512);
  //  Texture hexTex = LoadTextureFromImage(hexagon);
  //
  //  cache.load<TextureLoader>(hstr{"hexagon"}, hexTex);

  LoadResource( hstr{ "hexagon" }, "assets/textures/hexagon.png", cache );

  LoadResource(
    hstr{ "test" },
    "assets/textures/hexagons/hexagon5.png",
    cache );
  LoadResource(
    hstr{ "factionOverlay" },
    "assets/textures/overlays.png",
    cache );

  LoadResource( hstr{ "template" }, "assets/textures/Template.png", cache );

  LoadResource(
    hstr{ "romanVillagerTexture" },
    "assets/textures/units/Roman_Villager.png",
    cache );
  LoadResource(
    hstr{ "greekVillagerTexture" },
    "assets/textures/units/Greek_Villager.png",
    cache );
  LoadResource(
    hstr{ "celtVillagerTexture" },
    "assets/textures/units/Celt_Villager.png",
    cache );
  LoadResource(
    hstr{ "punicVillagerTexture" },
    "assets/textures/units/Carthaginian_Villager.png",
    cache );
  LoadResource(
    hstr{ "persianVillagerTexture" },
    "assets/textures/units/Persian_Villager.png",
    cache );
  LoadResource(
    hstr{ "romanVillageTexture" },
    "assets/textures/village_roman.png",
    cache );
}

bool GameIsRunning() { return !WindowShouldClose(); }
