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
  UI::Manager()->EnableCampaignUI();
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
  UI::Manager()->EnableCampaignUI();
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


inline void Campaign::UpdateOnFrame() {
  UI::UpdateOnFrame();
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
