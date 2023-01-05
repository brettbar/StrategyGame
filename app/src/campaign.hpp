#pragma once

#include "world/managers/commands.hpp"

#include "world/systems/animation_system.hpp"
#include "world/systems/map_system.hpp"
#include "world/systems/movement_system.hpp"
#include "world/systems/player_system.hpp"
#include "world/systems/province_system.hpp"
#include "world/systems/settlement_system.hpp"
#include "world/systems/spawn_system.hpp"

#include "renderer/renderer.hpp"

#include "interface/input.hpp"

// TODO this is a weird spot to put this, but both game and campaign use it? maybe refactor
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

struct GameState {
  const f32 MS_PER_UPDATE = 1 / 60.0;
  const f32 ONCE_A_SECOND = 1;
  f32 oncelag = 0.0f;
  f32 lag = 0.0f;
  f32 dt = 0.0f;

  bool is_host = false;
};

class Campaign {
  public:
  Campaign() {
    Global::program_mode = Global::ProgramMode::Campaign;
    UI::EnableCampaignUI();
    Start();
  }

  Campaign( const char * ) {
    // TODO make take in file path
    SaveSystem::Load();

    Global::program_mode = Global::ProgramMode::Campaign;
    UI::EnableCampaignUI();

    Load();
  }

  ~Campaign() {
    Global::ClearRegistry();
  }

  void HostCampaign() {}

  void Run( GameState *state ) {
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

  private:
  void Start() {
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

  void Load() {
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
};
