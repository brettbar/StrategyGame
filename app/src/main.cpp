/*
  Authored by Brett Barinaga on 11/29/21. Copyright (c) Brett Barinaga, All
rights reserved.
*/
#include "steam/isteamnetworkingutils.h"
#include "steam/steam_api.h"


#include "interface/input.hpp"

#include "renderer/renderer.hpp"
#include "shared/commands.hpp"
#include "shared/common.hpp"
#include "shared/events.hpp"
#include "shared/global.hpp"
#include "shared/save.hpp"
#include "shared/textures.hpp"


#include "steam/steam_api_common.h"
#include "world/systems/animation_system.hpp"
#include "world/systems/map_system.hpp"
#include "world/systems/movement_system.hpp"
#include "world/systems/player_system.hpp"
#include "world/systems/province_system.hpp"
#include "world/systems/settlement_system.hpp"
#include "world/systems/spawn_system.hpp"

#include "network/network.hpp"

#include "interface/ui_system.hpp"

#include "shared/utils.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>
#include <stdlib.h>
#include <thread>


namespace fs = std::filesystem;

void LoadResources();
void CameraUpdate( Camera2D &, f32 );

void UpdateOnFrame();
void Update60TPS();
void Update1TPS();

void StartCampaign();
void LoadCampaign();

void Exit( TextureCache & );

void SteamAPIDebugTextHook( int severity, const char *msg ) {
  printf( "S::%d", severity );
  printf( "%s\n", msg );
}

int main( void ) {
  bool campaign_started = false;
  bool campaign_to_load = false;
  bool hit_exit = false;
  bool fresh_start = true;
  bool creating_lobby = false;
  bool joining_lobby = false;

  f32 MS_PER_UPDATE = 1 / 60.0;
  f32 ONCE_A_SECOND = 1;
  f32 oncelag = 0.0f;
  f32 lag = 0.0f;
  f32 dt = 0.0f;

  SetConfigFlags( FLAG_WINDOW_RESIZABLE );
  SetTargetFPS( 200 );// Set our game to run at 60 frames-per-second

  if ( SteamAPI_RestartAppIfNecessary( 480 ) ) {
    return EXIT_FAILURE;
  }

  if ( !SteamAPI_Init() ) {
    printf( "SteamAPI_Init() failed!\n" );
    return EXIT_FAILURE;
  }

  if ( !SteamUser()->BLoggedOn() ) {
    printf( "Steam user is not logged in\n" );
    return EXIT_FAILURE;
  }

  SteamClient()->SetWarningMessageHook( &SteamAPIDebugTextHook );

  printf( "Starting game as %s.\n", SteamFriends()->GetPersonaName() );

  InitWindow( 1920, 1080, "FieldsOfMars" );
  LoadResources();
  UI::EnableMainMenuUI();

  // this has to be right before WindowShouldClose() for some reason
  SetExitKey( KEY_NULL );

  // TODO this really doesnt need to be in SettlementSystem
  // its just loading images into textures, it should
  // be organized differently
  SettlementSystem::Init();
  Commands::Listen();

  Network net = Network();

  if ( !SteamInput()->Init( false ) ) {
    printf( "SteamInput()->Init failed.\n" );
    return EXIT_FAILURE;
  }

  while ( !WindowShouldClose() && !hit_exit ) {
    // TODO this monolithic event handler needs to be handled differently
    Events::event_emitter.on<Events::UIEvent>(
      [&]( const Events::UIEvent &event, Events::EventEmitter &emitter ) {
        if ( event.msg == "main_menu_host_game" ) {
          creating_lobby = true;
        }
        else if ( event.msg == "main_menu_join_game" ) {
          joining_lobby = true;
        }
        else if ( event.msg == "main_menu_resume_game" ) {
          UI::EnableCampaignUI();
        }
        else if ( event.msg == "main_menu_start_game" ) {
          Global::ClearRegistry();

          campaign_started = false;
          UI::EnableCampaignUI();
          fresh_start = true;
        }
        else if ( event.msg == "main_menu_load_game" ) {
          campaign_to_load = true;
        }
        else if ( event.msg == "main_menu_exit_game" ) {
          hit_exit = true;
        }
        else if ( event.msg == "modal_menu_load_game" ) {
          campaign_to_load = true;
        }
        else if ( event.msg == "modal_menu_save_game" ) {
          SaveSystem::Save();
        }
        else if ( event.msg == "modal_menu_exit_main" ) {
          UI::EnableMainMenuUI();
        }
      }
    );

    if ( creating_lobby ) {
      // net_client.Host();
      net.Host();
      creating_lobby = false;
    }

    if ( joining_lobby ) {
      net.Client();
      joining_lobby = false;
    }

    SteamAPI_RunCallbacks();

    // Check and prep for campaign load
    if ( campaign_to_load ) {
      Global::ClearRegistry();
      SaveSystem::Load();

      UI::EnableCampaignUI();
      campaign_started = false;
      campaign_to_load = false;
      fresh_start = false;
    }

    switch ( Global::program_mode ) {
      case Global::ProgramMode::MainMenu: {
        Input::Handle();

        UpdateOnFrame();

        CameraUpdate( Global::state.camera, dt );

        BeginDrawing();
        {
          ClearBackground( BLACK );
          Renderer::DrawUI();
        }
        EndDrawing();
      } break;

      case Global::ProgramMode::ModalMenu: {
        Input::CheckMenuToggle();

        UpdateOnFrame();

        BeginDrawing();
        {
          Renderer::Draw( Global::texture_cache );
          DrawRectangle(
            0, 0, GetScreenWidth(), GetScreenHeight(), Fade( BLACK, 0.33f )
          );
          Renderer::DrawUI();
        }
        EndDrawing();
      } break;

      case Global::ProgramMode::Campaign: {
        // 1. Check for init
        if ( !campaign_started && fresh_start ) {
          StartCampaign();
          campaign_started = true;
        }
        else if ( !campaign_started && !fresh_start ) {
          LoadCampaign();
          campaign_started = true;
        }

        // Update Time
        dt = GetFrameTime();
        lag += dt;
        oncelag += dt;

        // 2. Check for Input
        Input::CheckMenuToggle();
        Input::Handle();

        // 3. Check for network traffic if multiplayer

        // 4. Process all commands
        Commands::FireAll();

        // 5. Run all Updates
        {
          // Update 60 times a second
          while ( lag >= MS_PER_UPDATE ) {
            Update60TPS();
            lag -= MS_PER_UPDATE;
          }

          // Update once per second
          while ( oncelag >= ONCE_A_SECOND * ( 1 / Global::state.timeScale ) ) {
            Update1TPS();
            oncelag = 0.0f;
          }

          // Update once per frame
          UpdateOnFrame();

          // Update Camera
          CameraUpdate( Global::state.camera, dt );
        }

        // 6. Draw everything
        BeginDrawing();
        {
          Renderer::Draw( Global::texture_cache );
          Renderer::DrawUI();

          DrawRectangle( GetScreenWidth() - 120, 2, 100, 24.0f, BLACK );
          DrawFPS( GetScreenWidth() - 100, 2 );
        }
        EndDrawing();
      } break;
    }
  }

  // TODO move this into the Exit function
  net.Exit();

  // Perform clean up and teardown
  Exit( Global::texture_cache );

  return EXIT_SUCCESS;
}

void StartCampaign() {
  MapSystem::Init();
  PlayerSystem::Init();
  ProvinceSystem::Init();
  Renderer::Init();
  // std::cout << EntityIdToString( Global::host_player ) << std::endl;
}

void LoadCampaign() {
  MapSystem::Init();
  Renderer::Init();
  Global::world.view<Settlement::Component>().each(
    []( Settlement::Component &settlement ) {
      settlement.texture =
        LoadTextureFromImage( Settlement::building_map.at( "roman_m1" ) );
    }
  );
  // std::cout << EntityIdToString( Global::host_player ) << std::endl;
}

void UpdateOnFrame() {
  UI::UpdateOnFrame();
}

// TODO: look at all of these and see if any belong in UpdateOnFrame
void Update60TPS() {
  auto animated_units =
    Global::world.view<Unit::Component, Animated::Component>();

  auto players = Global::world.view<Player::Component>();

  MovementSystem::Update( animated_units, Global::state.timeScale );
  AnimationSystem::Update( animated_units, Global::state.timeScale );
  PlayerSystem::Update( players );
  //  Terrain::UpdateFOW(reg);
}

void Update1TPS() {
  auto settlements =
    Global::world.view<Province::Component, Settlement::Component>();

  SettlementSystem::Update( settlements, Global::state );

  Global::state.day++;

  if ( Global::state.month < 12 )
    Global::state.month++;
  else {
    Global::state.year++;
    Global::state.month = 1;
  }
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

Image InitTileOutline() {
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

void LoadResources() {
  std::string asset_folder = "app/assets/";

  LoadResource(
    hstr{ "tile_outline" }, InitTileOutline(), Global::texture_cache
  );

  Global::font_cache.load(
    hstr{ "font_romulus" },
    LoadFont( ( asset_folder + "/fonts/romulus.png" ).c_str() )
  );

  Global::font_cache.load(
    hstr{ "font_default" },
    LoadFont( ( asset_folder + "/fonts/Perfect-DOS-VGA-437.png" ).c_str() )
  );

  LoadResource(
    hstr{ "land_tile" },
    LoadImage( ( asset_folder + "/images/hexagons/Grass.bmp" ).c_str() ),
    Global::texture_cache
  );
  LoadResource(
    hstr{ "water_tile" },
    LoadImage( ( asset_folder + "/images/hexagons/Ocean.bmp" ).c_str() ),
    Global::texture_cache
  );
  LoadResource(
    hstr{ "hills_tile" },
    LoadImage( ( asset_folder + "/images/hexagons/Hills.bmp" ).c_str() ),
    Global::texture_cache
  );
  LoadResource(
    hstr{ "sand_tile" },
    LoadImage( ( asset_folder + "/images/hexagons/Sand.bmp" ).c_str() ),
    Global::texture_cache
  );
  LoadResource(
    hstr{ "snow_tile" },
    LoadImage( ( asset_folder + "/images/hexagons/Snow.bmp" ).c_str() ),
    Global::texture_cache
  );

  LoadResource(
    hstr{ "redOverlay" },
    LoadImage( ( asset_folder + "/images/overlays/Red.png" ).c_str() ),
    Global::texture_cache
  );
  LoadResource(
    hstr{ "blueOverlay" },
    LoadImage( ( asset_folder + "/images/overlays/Blue.png" ).c_str() ),
    Global::texture_cache
  );
  LoadResource(
    hstr{ "greenOverlay" },
    LoadImage( ( asset_folder + "/images/overlays/Green.png" ).c_str() ),
    Global::texture_cache
  );
  LoadResource(
    hstr{ "purpleOverlay" },
    LoadImage( ( asset_folder + "/images/overlays/Purple.png" ).c_str() ),
    Global::texture_cache
  );
  LoadResource(
    hstr{ "orangeOverlay" },
    LoadImage( ( asset_folder + "/images/overlays/Orange.png" ).c_str() ),
    Global::texture_cache
  );

  LoadResource(
    hstr{ "template" },
    LoadImage( ( asset_folder + "/images/Template.png" ).c_str() ),
    Global::texture_cache
  );

  LoadResource(
    hstr{ "romans_villager_texture" },
    LoadImage( ( asset_folder + "/images/units/RomanVillager.png" ).c_str() ),
    Global::texture_cache
  );
  LoadResource(
    hstr{ "greeks_villager_texture" },
    LoadImage( ( asset_folder + "/images/units/GreekVillager.png" ).c_str() ),
    Global::texture_cache
  );
  LoadResource(
    hstr{ "celts_villager_texture" },
    LoadImage( ( asset_folder + "/images/units/CelticVillager.png" ).c_str() ),
    Global::texture_cache
  );
  LoadResource(
    hstr{ "punics_villager_texture" },
    LoadImage(
      ( asset_folder + "/images/units/Carthaginian_Villager.png" ).c_str()
    ),
    Global::texture_cache
  );
  LoadResource(
    hstr{ "persians_villager_texture" },
    LoadImage( ( asset_folder + "/images/units/Persian_Villager.png" ).c_str()
    ),
    Global::texture_cache
  );
  LoadResource(
    hstr{ "romanVillageTexture" },
    LoadImage( ( asset_folder + "/images/village_roman.png" ).c_str() ),
    Global::texture_cache
  );

  LoadResource(
    hstr{ "buildings" },
    LoadImage( ( asset_folder + "/images/buildings.png" ).c_str() ),
    Global::texture_cache
  );

  // TODO at somepoint these UI textures should probably
  // be set to bilinear and then run through the fragment shader
  // But before I can do that I need to fix the shader with how
  // it deals with alpha
  LoadTexturePointFilter(
    hstr{ "context_panel" },
    LoadImage( ( asset_folder + "/images/UI/UI_test.png" ).c_str() ),
    Global::texture_cache
  );

  LoadTexturePointFilter(
    hstr{ "settlement_context_tab_overview" },
    LoadImage( ( asset_folder + "/images/UI/Overview.png" ).c_str() ),
    Global::texture_cache
  );
  LoadTexturePointFilter(
    hstr{ "settlement_context_tab_population" },
    LoadImage( ( asset_folder + "/images/UI/Population.png" ).c_str() ),
    Global::texture_cache
  );
  LoadTexturePointFilter(
    hstr{ "settlement_context_tab_culture" },
    LoadImage( ( asset_folder + "/images/UI/Culture.png" ).c_str() ),
    Global::texture_cache
  );
  LoadTexturePointFilter(
    hstr{ "settlement_context_tab_religion" },
    LoadImage( ( asset_folder + "/images/UI/Religion.png" ).c_str() ),
    Global::texture_cache
  );
  LoadTexturePointFilter(
    hstr{ "settlement_context_tab_resources" },
    LoadImage( ( asset_folder + "/images/UI/Resources.png" ).c_str() ),
    Global::texture_cache
  );
  LoadTexturePointFilter(
    hstr{ "settlement_context_tab_construction" },
    LoadImage( ( asset_folder + "/images/UI/Construction.png" ).c_str() ),
    Global::texture_cache
  );
  LoadTexturePointFilter(
    hstr{ "settlement_context_tab_garrison" },
    LoadImage( ( asset_folder + "/images/UI/Garrison.png" ).c_str() ),
    Global::texture_cache
  );

  std::string path = asset_folder + "/images/resources";
  for ( const auto &entry: fs::directory_iterator( path ) ) {
    std::cout << entry.path().filename() << std::endl;

    std::string filename = entry.path().filename().generic_string();

    LoadResource(
      hstr{ filename.c_str() },
      LoadImage( ( path + "/" + filename ).c_str() ),
      Global::texture_cache
    );
  }
}

void Exit( TextureCache &cache ) {

  // @TODO figure out all deallocs or whatever

  UnloadShader( Renderer::shader );

  for ( auto resource: cache ) {
    UnloadTexture( resource.second->texture );
  }

  cache.clear();

  SteamAPI_Shutdown();

  CloseWindow();// Close window and OpenGL context
}
