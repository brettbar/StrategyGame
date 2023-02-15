#pragma once

#include "world/managers/commands.hpp"

#include "world/systems/animation_system.hpp"
#include "world/systems/map_system.hpp"
#include "world/systems/movement_system.hpp"
#include "world/systems/player.hpp"
#include "world/systems/province_system.hpp"
#include "world/systems/settlement_system.hpp"

#include "renderer/renderer.hpp"

#include "interface/input.hpp"

#include "interface/ui_system.hpp"

class Campaign {

  public:
  Campaign() {
    Start();
  }

  Campaign( const char * ) {
    // TODO make take in file path
    Load();
  }

  ~Campaign() {
    Global::ClearRegistry();
  }

  void Run( f32 &, f32 &, f32 & );

  private:
  const f32 _MS_PER_UPDATE = 1 / 60.0;
  const f32 _ONCE_A_SECOND = 1;

  void Start();
  void Load();
  void UpdateOnFrame();
  void Update60TPS();
  void Update1TPS();
};

inline void Campaign::Start() {
  MapSystem::Init();
  Player::System::Init();
  SettlementSystem::Init();
  ProvinceSystem::Init();
  Renderer::Init();

  // Make sure the singleton is initialized
  Commands::Manager();

  // Commands::Listen();
  // std::cout << EntityIdToString( Global::host_player ) << std::endl;
}

inline void Campaign::Load() {
  SaveSystem::Load();
  MapSystem::Init();
  Renderer::Init();
  // Commands::Listen();
  Global::world.view<Settlement::Component>().each(
    []( Settlement::Component &settlement ) {
      settlement.texture =
        LoadTextureFromImage( Settlement::building_map.at( "roman_m1" ) );
    }
  );

  Commands::Manager();
  // std::cout << EntityIdToString( Global::host_player ) << std::endl;
}

// Runs inside game loop
inline void Campaign::Run( f32 &dt, f32 &lag, f32 &oncelag ) {
  // 1. Update Time
  dt = GetFrameTime();
  lag += dt;
  oncelag += dt;

  // 2. Check for Input
  Input::CheckMenuToggle();
  Input::Handle();

  // 3. Process all commands
  Commands::Manager()->FireAll();

  // TODO
  // Add an explicity Eval step and Execute step
  //
  // Eval:
  // - 0. (happens in game regardless of campaign) Process Events
  // 1. Receive Messages
  // 2. Receive Commands
  // 3. Validate Messages
  // 4. Validate Commands (like remove duplicates or something)

  // Exec:
  // 1. Update the campaign state with changes from Commands
  // 2. Update UI with Messages

  /*================================================================
   *                      EVALUATION 
   ===============================================================*/
  {

  };
  /*================================================================
   *                      /EVALUATION 
   ===============================================================*/

  // 5. Run all Updates
  {
    // Update 60 times a second
    while ( lag >= _MS_PER_UPDATE ) {
      Update60TPS();
      lag -= _MS_PER_UPDATE;
    }

    // Update once per second
    while ( oncelag >= _ONCE_A_SECOND * ( 1 / Global::state.timeScale ) ) {
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
}


inline void Campaign::UpdateOnFrame() {
  UI::System::UpdateOnFrame();
}

// TODO: look at all of these and see if any belong in UpdateOnFrame
inline void Campaign::Update60TPS() {
  auto animated_units =
    Global::world.view<Unit::Component, Animated::Component>();

  auto players = Global::world.view<Player::Component>();

  MovementSystem::Update( animated_units, Global::state.timeScale );
  AnimationSystem::Update( animated_units, Global::state.timeScale );
  Player::System::Update( players );
  //  Terrain::UpdateFOW(reg);
}

inline void Campaign::Update1TPS() {
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
