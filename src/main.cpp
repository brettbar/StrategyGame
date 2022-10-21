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
#include <raylib.h>

#include "input.hpp"
#include "renderer/fonts.hpp"
#include "renderer/renderer.hpp"
#include "renderer/textures.hpp"
#include "save.hpp"
#include "state.hpp"
#include "systems/animation_system.hpp"
#include "systems/event_system.hpp"
#include "systems/map/province_system.hpp"
#include "systems/map/terrain_system.hpp"
#include "systems/movement_system.hpp"
#include "systems/player_system.hpp"
#include "systems/selection_system.hpp"
#include "systems/settlement_system.hpp"
#include "systems/spawn_system.hpp"
#include "ui/ui_system.hpp"

#include "filesystem"

// namespace fs = std::filesystem;

void LoadResources( TextureCache & );
void CameraUpdate( Camera2D &, f32 );

void Init( State &, entt::registry &, TextureCache & );
void StartGame( entt::registry &, State &, TextureCache & );
void Update( State &, entt::registry & );
void LateUpdate( State &, entt::registry & );
void Exit( TextureCache & );

int main( void ) {
  State state = {
    .mapWidth = 128,
    .mapHeight = 128,
    .timeScale = 0.0f,
    .prevTimeScale = 1.0f,
    .gameState = GameState::EDITOR,
    .month = 1,
    .year = 4,
    .startYear = 4,
    .currPlayer = std::make_shared<PlayerSystem>(
      PlayerSystem( 0, Faction::ROMANS, "Roman Republic" ) ),
  };

  entt::registry reg;
  reg.clear();

  bool game_started = false;

  // MAIN_MENU_UI::emitter.on<MAIN_MENU_UI::ClickEvent>(
  //   [&reg](
  //     const MAIN_MENU_UI::ClickEvent &event,
  //     MAIN_MENU_UI::Emitter &emitter ) {
  //     fs::path f{ "output.json" };
  //     if ( fs::exists( f ) ) {
  //       printf( "Found existing file!\n" );
  //       reg = Save::Load();
  //     }
  //   } );

  // Initialization
  SetConfigFlags( FLAG_WINDOW_RESIZABLE );
  SetTargetFPS( 144 );// Set our game to run at 60 frames-per-second
  SetExitKey( KEY_DELETE );
  InitWindow(
    1920,
    1080,
    // GetScreenWidth(),
    // GetScreenHeight(),
    "FieldsOfMars" );

  LoadResources( Global::texture_cache );


  // Main game loop
  f32 MS_PER_UPDATE = 1 / 60.0;
  f32 ONCE_A_SECOND = 1;
  f32 oncelag = 0.0f;
  f32 lag = 0.0f;
  f32 dt = 0.0f;

  while ( !WindowShouldClose() ) {
    switch ( Global::program_mode ) {
      case Global::ProgramMode::MAIN_MENU:
        Input::CheckMenuToggle();

        BeginDrawing();
        {
          ClearBackground( BLACK );

          // IRONGUI::Draw(
          //   MAIN_MENU_UI::curr_content,
          //   IRONGUI::reg.view<IRONGUI::Element, MAIN_MENU_UI::UiFlag>(),
          //   IRONGUI::reg.view<IRONGUI::Element, MAIN_MENU_UI::UiFlag>(
          //     entt::exclude<IRONGUI::Panel> ),
          //   font_cache );
        }
        EndDrawing();
        break;

      case Global::ProgramMode::MODAL_MENU:
        Input::CheckMenuToggle();

        BeginDrawing();
        {
          Renderer::Draw( state, reg, Global::texture_cache );
          // IRONGUI::Draw(
          //   GAME_UI::curr_content,
          //   IRONGUI::reg.view<IRONGUI::Element, GAME_UI::UiFlag>(),
          //   IRONGUI::reg.view<IRONGUI::Element, GAME_UI::UiFlag>( entt::exclude<IRONGUI::Panel> ),
          //   font_cache
          // );

          DrawRectangle(
            0,
            0,
            GetScreenWidth(),
            GetScreenHeight(),
            Fade( BLACK, 0.33f ) );

          // IRONGUI::Draw(
          //   MODAL_MENU_UI::curr_content,
          //   IRONGUI::reg.view<IRONGUI::Element, MODAL_MENU_UI::UiFlag>(),
          //   IRONGUI::reg.view<IRONGUI::Element, MODAL_MENU_UI::UiFlag>(
          //     entt::exclude<IRONGUI::Panel> ),
          //   font_cache );
        }
        EndDrawing();
        break;

      case Global::ProgramMode::GAME:
        if ( !game_started ) {
          StartGame( reg, state, Global::texture_cache );
          game_started = true;
        }

        // Update Time
        dt = GetFrameTime();
        lag += dt;
        oncelag += dt;

        // Check for Inpu
        Input::CheckMenuToggle();
        Input::Handle( state, reg, Global::texture_cache );


        // Update once per frame
        while ( lag >= MS_PER_UPDATE ) {
          Update( state, reg );
          lag -= MS_PER_UPDATE;
        }

        // Update once per second
        while ( oncelag >= ONCE_A_SECOND * ( 1 / state.timeScale ) ) {
          LateUpdate( state, reg );
          oncelag = 0.0f;
        }

        // Update Camera
        CameraUpdate( state.camera, dt );

        // Draw everything to screen
        BeginDrawing();
        {
          Renderer::Draw( state, reg, Global::texture_cache );
          // IRONGUI::Draw(
          //   GAME_UI::curr_content,
          //   IRONGUI::reg.view<IRONGUI::Element, GAME_UI::UiFlag>(),
          //   IRONGUI::reg.view<IRONGUI::Element, GAME_UI::UiFlag>(
          //     entt::exclude<IRONGUI::Panel> ),
          //   font_cache );
          UI::Draw( Global::texture_cache );
        }
        EndDrawing();
        break;
    }
  }

  // Perform clean up and teardown
  Exit( Global::texture_cache );

  return 0;
}

void StartGame(
  entt::registry &reg,
  State &state,
  TextureCache &texture_cache ) {
  Terrain::CreateTerrain( reg );
  ProvinceSystem::InitProvinces( reg, texture_cache );
  SettlementSystem::InitSettlements( reg, texture_cache );
  SpawnSystem::Init();
  // GAME_UI::Init( reg );
  UI::Init( reg, texture_cache );
  Renderer::Init( state );
}

void LoadGame() {}

void Update( State &state, entt::registry &reg ) {
  MovementSystem::Update( reg, state.timeScale );
  AnimationSystem::UpdateSprites( reg, state.timeScale );
  UI::Update( reg );
  //  Terrain::UpdateFOW(reg);
  // GAME_UI::Update( reg );
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

  for ( auto resource: cache ) {
    UnloadTexture( resource.second->texture );
  }

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

  camera.zoom += ( mouseWheelDelta * 0.2f );
  if ( camera.zoom > 8.0f )
    camera.zoom = 8.0f;
  else if ( camera.zoom < 0.08f )
    camera.zoom = 0.08f;

  camera.offset = { (f32) GetScreenWidth() / 2, (f32) GetScreenHeight() / 2 };
}

inline Image InitTileOutline() {
  Image base = GenImageColor( 65, 65, ColorAlpha( WHITE, 0.0 ) );

  // N -> NE
  ImageDrawLineV( &base, { 0, 16 }, { 32, 0 }, YELLOW );
  // NE -> SE
  ImageDrawLineV( &base, { 64, 16 }, { 64, 48 }, YELLOW );
  // SE -> S
  ImageDrawLineV( &base, { 32, 0 }, { 64, 16 }, YELLOW );
  // S -> SW
  ImageDrawLineV( &base, { 64, 48 }, { 32, 64 }, YELLOW );
  // SW -> NW
  ImageDrawLineV( &base, { 32, 64 }, { 0, 48 }, YELLOW );
  // NW -> N
  ImageDrawLineV( &base, { 0, 48 }, { 0, 16 }, YELLOW );

  return base;
}

void LoadResources( TextureCache &texture_cache ) {
  // texture_cache.load<TextureLoader>(
  //   hstr{ "tile_outline" },
  //   InitTileOutline() );

  LoadResource( hstr{ "tile_outline" }, InitTileOutline(), texture_cache );

  Global::font_cache.load(
    hstr{ "font_romulus" },
    LoadFont( "assets/fonts/romulus.png" ) );

  Global::font_cache.load(
    hstr{ "font_default" },
    LoadFont( "assets/fonts/Perfect-DOS-VGA-437.png" ) );

  LoadResource(
    hstr{ "land_tile" },
    LoadImage( "assets/textures/hexagons/Grass.bmp" ),
    texture_cache );
  LoadResource(
    hstr{ "water_tile" },
    LoadImage( "assets/textures/hexagons/Ocean.bmp" ),
    texture_cache );
  LoadResource(
    hstr{ "hills_tile" },
    LoadImage( "assets/textures/hexagons/Hills.bmp" ),
    texture_cache );
  LoadResource(
    hstr{ "sand_tile" },
    LoadImage( "assets/textures/hexagons/Sand.bmp" ),
    texture_cache );
  LoadResource(
    hstr{ "snow_tile" },
    LoadImage( "assets/textures/hexagons/Snow.bmp" ),
    texture_cache );

  LoadResource(
    hstr{ "redOverlay" },
    LoadImage( "assets/textures/overlays/Red.png" ),
    texture_cache );
  LoadResource(
    hstr{ "blueOverlay" },
    LoadImage( "assets/textures/overlays/Blue.png" ),
    texture_cache );
  LoadResource(
    hstr{ "greenOverlay" },
    LoadImage( "assets/textures/overlays/Green.png" ),
    texture_cache );
  LoadResource(
    hstr{ "purpleOverlay" },
    LoadImage( "assets/textures/overlays/Purple.png" ),
    texture_cache );
  LoadResource(
    hstr{ "orangeOverlay" },
    LoadImage( "assets/textures/overlays/Orange.png" ),
    texture_cache );

  LoadResource(
    hstr{ "template" },
    LoadImage( "assets/textures/Template.png" ),
    texture_cache );

  LoadResource(
    hstr{ "romanVillagerTexture" },
    LoadImage( "assets/textures/units/Roman_Villager.png" ),
    texture_cache );
  LoadResource(
    hstr{ "greekVillagerTexture" },
    LoadImage( "assets/textures/units/Greek_Villager.png" ),
    texture_cache );
  LoadResource(
    hstr{ "celtVillagerTexture" },
    LoadImage( "assets/textures/units/Celt_Villager.png" ),
    texture_cache );
  LoadResource(
    hstr{ "punicVillagerTexture" },
    LoadImage( "assets/textures/units/Carthaginian_Villager.png" ),
    texture_cache );
  LoadResource(
    hstr{ "persianVillagerTexture" },
    LoadImage( "assets/textures/units/Persian_Villager.png" ),
    texture_cache );
  LoadResource(
    hstr{ "romanVillageTexture" },
    LoadImage( "assets/textures/village_roman.png" ),
    texture_cache );

  LoadResource(
    hstr{ "buildings" },
    LoadImage( "assets/textures/buildings.png" ),
    texture_cache );


  LoadResource(
    hstr{ "context_panel" },
    LoadImage( "assets/textures/UI/UI_test.png" ),
    texture_cache );

  LoadResource(
    hstr{ "context_tab_overview" },
    LoadImage( "assets/textures/UI/Overview.png" ),
    texture_cache );
  LoadResource(
    hstr{ "context_tab_population" },
    LoadImage( "assets/textures/UI/Population.png" ),
    texture_cache );
  LoadResource(
    hstr{ "context_tab_culture" },
    LoadImage( "assets/textures/UI/Culture.png" ),
    texture_cache );
  LoadResource(
    hstr{ "context_tab_religion" },
    LoadImage( "assets/textures/UI/Religion.png" ),
    texture_cache );
  LoadResource(
    hstr{ "context_tab_resources" },
    LoadImage( "assets/textures/UI/Resources.png" ),
    texture_cache );
  LoadResource(
    hstr{ "context_tab_construction" },
    LoadImage( "assets/textures/UI/Construction.png" ),
    texture_cache );
  LoadResource(
    hstr{ "context_tab_garrison" },
    LoadImage( "assets/textures/UI/Garrison.png" ),
    texture_cache );
}
