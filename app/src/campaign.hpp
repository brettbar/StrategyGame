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

#include "app.hpp"

class Campaign {
  public:
  private:
  void Start();
  void Load();
  void UpdateOnFrame();
  void Update60TPS();
  void Update1TPS();

  inline void Run( GameState *state ) {
    // 1. Check for init
    if ( !state->campaign_started && !state->loading_campaign ) {
      Start();
      state->campaign_started = true;
    }
    else if ( !state->campaign_started && state->loading_campaign ) {
      Load();
      state->campaign_started = true;
      state->loading_campaign = false;
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

  inline void Start() {
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

  inline void Load() {
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

  // TODO(rf) can probably go somewhere else
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
};