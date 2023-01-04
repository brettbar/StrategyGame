#pragma once

#include "network/client.hpp"
#include "network/host.hpp"

#include "shared/common.hpp"
#include "shared/events.hpp"
#include "shared/save.hpp"

#include "renderer/renderer.hpp"

#include "interface/input.hpp"

#include "world/managers/commands.hpp"

#include "world/systems/animation_system.hpp"
#include "world/systems/map_system.hpp"
#include "world/systems/movement_system.hpp"
#include "world/systems/player_system.hpp"
#include "world/systems/province_system.hpp"
#include "world/systems/settlement_system.hpp"
#include "world/systems/spawn_system.hpp"

namespace fs = std::filesystem;

struct AppState {
  bool campaign_started = false;
  bool campaign_to_load = false;
  bool hit_exit = false;
  bool fresh_start = true;

  bool creating_lobby = false;
  bool joining_lobby = false;
  // TODO could cause issues by not getting reset
  bool is_host = false;

  f32 MS_PER_UPDATE = 1 / 60.0;
  f32 ONCE_A_SECOND = 1;
  f32 oncelag = 0.0f;
  f32 lag = 0.0f;
  f32 dt = 0.0f;
};


void RunMainMenu( AppState * );
void RunModalMenu();
void RunCampaign( AppState * );

void CheckForEvents( AppState * );
void CameraUpdate( Camera2D &, f32 );
void UpdateOnFrame();
void Update60TPS();
void Update1TPS();
void StartCampaign();
void LoadCampaign();
void Exit( TextureCache & );

/*
========================================================
  Main game loop 
========================================================
*/
inline void RunGameLoop() {
  AppState *state = new AppState();

  // TODO(refactor)
  // Network::Host *host = nullptr;
  Network::Host();
  // Network::Client *client = nullptr;
  Network::Client();
  Network::Setup();


  while ( !WindowShouldClose() && !state->hit_exit ) {
    CheckForEvents( state );

    if ( state->creating_lobby && state->is_host ) {
      // host = new Network::Host();
      Network::Host()->Init();
      state->creating_lobby = false;
    }

    if ( state->joining_lobby && !state->is_host ) {
      // client = new Network::Client();
      // client->Init();
      Network::Client()->Init();
      state->joining_lobby = false;
    }

    SteamAPI_RunCallbacks();

    if ( state->is_host ) {
      Network::Host()->CheckForMessages();
    }
    else {
      Network::Client()->CheckForMessages();
    }


    // // Check and prep for campaign load
    // TODO idk why this has to be called here
    // or else loading doesnt work
    if ( state->campaign_to_load ) {
      Global::ClearRegistry();
      SaveSystem::Load();

      UI::EnableCampaignUI();
      state->campaign_started = false;
      state->campaign_to_load = false;
      state->fresh_start = false;
    }

    switch ( Global::program_mode ) {
      case Global::ProgramMode::MainMenu:
        RunMainMenu( state );
        break;

      case Global::ProgramMode::ModalMenu:
        RunModalMenu();
        break;

      case Global::ProgramMode::Campaign:
        RunCampaign( state );
        break;
    }
  }

  // TODO move this into the Exit function
  // Delete host and client
  // if ( host )
  //   delete host;

  Network::Host()->Delete();
  Network::Client()->Delete();

  // if ( client )
  //   delete client;

  delete state;
}

// TODO this monolithic event handler needs to be handled differently
inline void CheckForEvents( AppState *state ) {
  Events::event_emitter.on<Events::UIEvent>(
    [&]( const Events::UIEvent &event, Events::EventEmitter &emitter ) {
      if ( event.msg == "main_menu_host_game" ) {
        state->creating_lobby = true;
        state->is_host = true;
      }
      else if ( event.msg == "main_menu_join_game" ) {
        state->joining_lobby = true;
        state->is_host = false;
      }
      else if ( event.msg == "main_menu_resume_game" ) {
        state->campaign_to_load = true;
      }
      else if ( event.msg == "main_menu_start_game" ) {
        Global::ClearRegistry();

        state->campaign_started = false;
        UI::EnableCampaignUI();
        state->fresh_start = true;
      }
      else if ( event.msg == "main_menu_load_game" ) {
        state->campaign_to_load = true;
      }
      else if ( event.msg == "main_menu_exit_game" ) {
        state->hit_exit = true;
      }
      else if ( event.msg == "modal_menu_load_game" ) {
        state->campaign_to_load = true;
      }
      else if ( event.msg == "modal_menu_save_game" ) {
        SaveSystem::Save();
      }
      else if ( event.msg == "modal_menu_exit_main" ) {
        UI::EnableMainMenuUI();
      }
    }
  );
}


inline void RunMainMenu( AppState *state ) {
  Input::Handle();

  UpdateOnFrame();

  CameraUpdate( Global::state.camera, state->dt );

  BeginDrawing();
  {
    ClearBackground( BLACK );
    Renderer::DrawUI();
  }
  EndDrawing();
}

inline void RunModalMenu() {
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
}

inline void RunCampaign( AppState *state ) {
  // 1. Check for init
  if ( !state->campaign_started && state->fresh_start ) {
    StartCampaign();
    state->campaign_started = true;
  }
  else if ( !state->campaign_started && !state->fresh_start ) {
    LoadCampaign();
    state->campaign_started = true;
  }

  // Update Time
  state->dt = GetFrameTime();
  state->lag += state->dt;
  state->oncelag += state->dt;

  // 2. Check for Input
  Input::CheckMenuToggle();
  Input::Handle();

  // 3. Check for network traffic if multiplayer

  // 4. Process all commands
  Commands::Manager()->FireAll();

  // 5. Run all Updates
  {
    // Update 60 times a second
    while ( state->lag >= state->MS_PER_UPDATE ) {
      Update60TPS();
      state->lag -= state->MS_PER_UPDATE;
    }

    // Update once per second
    while ( state->oncelag >=
            state->ONCE_A_SECOND * ( 1 / Global::state.timeScale ) ) {
      Update1TPS();
      state->oncelag = 0.0f;
    }

    // Update once per frame
    UpdateOnFrame();

    // Update Camera
    CameraUpdate( Global::state.camera, state->dt );
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
}

inline void StartCampaign() {
  MapSystem::Init();
  PlayerSystem::Init();
  SettlementSystem::Init();
  ProvinceSystem::Init();
  Renderer::Init();

  // Make sure the singleton is initialized
  Commands::Manager();

  // Commands::Listen();
  // std::cout << EntityIdToString( Global::host_player ) << std::endl;
}

inline void LoadCampaign() {
  MapSystem::Init();
  Renderer::Init();
  // Commands::Listen();
  Global::world.view<Settlement::Component>().each(
    []( Settlement::Component &settlement ) {
      settlement.texture =
        LoadTextureFromImage( Settlement::building_map.at( "roman_m1" ) );
    }
  );

  // Make sure the singleton is initialized
  Commands::Manager();
  // std::cout << EntityIdToString( Global::host_player ) << std::endl;
}


inline void UpdateOnFrame() {
  UI::UpdateOnFrame();
}

// TODO: look at all of these and see if any belong in UpdateOnFrame
inline void Update60TPS() {
  auto animated_units =
    Global::world.view<Unit::Component, Animated::Component>();

  auto players = Global::world.view<Player::Component>();

  MovementSystem::Update( animated_units, Global::state.timeScale );
  AnimationSystem::Update( animated_units, Global::state.timeScale );
  PlayerSystem::Update( players );
  //  Terrain::UpdateFOW(reg);
}

inline void Update1TPS() {
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

inline void CameraUpdate( Camera2D &camera, f32 dt ) {
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
