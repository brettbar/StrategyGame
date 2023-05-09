// TODO
// Add an explicity Eval step and Execute step
// Eval:
// - 0. (happens in game regardless of campaign) Process Events
// 1. Receive Messages
// 2. Receive Commands
// 3. Validate Messages
// 4. Validate Commands (like remove duplicates or something)

// Exec:
// 1. Update the campaign state with changes from Commands
// 2. Update UI with Messages

#pragma once


#include "shared/save.hpp"

#include "world/managers/commands.hpp"

#include "world/systems/animation_system.hpp"
#include "world/systems/faction_system.hpp"
#include "world/systems/map_system.hpp"
#include "world/systems/movement_system.hpp"
#include "world/systems/player_system.hpp"
#include "world/systems/province_system.hpp"
#include "world/systems/settlement_system.hpp"

#include "renderer/renderer.hpp"

#include "interface/ui_system.hpp"

class Campaign
{

  public:
  Campaign()
  {
    Start();
  }

  Campaign( const char * )
  {
    // TODO make take in file path
    Load();
  }

  ~Campaign()
  {
    Global::ClearRegistry();
    delete _command_queue;
  }

  void CheckForInput();
  void UpdateOnFrame( f32 &, f32 &, f32 & );
  void Update60TPS();
  void Update1TPS();
  void PostCommand( Commands::Command );

  private:
  Commands::Queue *_command_queue;

  void Start();
  void Load();
};

inline void Campaign::Start()
{
  MapSystem::Init();
  FactionSystem::Init();
  SettlementSystem::Init();
  ProvinceSystem::Init();
  Renderer::Init();

  // Make sure the singleton is initialized
  _command_queue = new Commands::Queue();

  // Commands::Listen();
  // std::cout << EntityIdToString( Global::host_player ) << std::endl;
}

inline void Campaign::Load()
{
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

  _command_queue = new Commands::Queue();
  // std::cout << EntityIdToString( Global::host_player ) << std::endl;
}

// Runs inside game loop
inline void Campaign::UpdateOnFrame( f32 &dt, f32 &lag, f32 &oncelag )
{
  // 2. Check for Input
  CheckForInput();

  // 3. Process all commands
  _command_queue->FireAll();
  UI::System::UpdateOnFrame();
}


// TODO: look at all of these and see if any belong in UpdateOnFrame
inline void Campaign::Update60TPS()
{
  auto animated_units =
    Global::world.view<Unit::Component, Animated::Component>();

  auto players = Global::world.view<Player::Component>();

  MovementSystem::Update( animated_units, Global::state.timeScale );
  AnimationSystem::Update( animated_units, Global::state.timeScale );
  PlayerSystem::Update( players );
  //  Terrain::UpdateFOW(reg);
}

inline void Campaign::Update1TPS()
{
  auto settlements =
    Global::world.view<Province::Component, Settlement::Component>();

  SettlementSystem::Update( settlements, Global::state );

  Global::state.day++;

  if ( Global::state.month < 12 )
    Global::state.month++;
  else
  {
    Global::state.year++;
    Global::state.month = 1;
  }
}
inline void Campaign::CheckForInput()
{
  Vector2 click_pos =
    GetScreenToWorld2D( GetMousePosition(), Global::state.camera );

  if ( IsKeyPressed( KEY_SPACE ) )
  {
    PostCommand( { Commands::Type::TimeChange, "Player request Pause" } );
  }

  if ( IsKeyPressed( KEY_MINUS ) )
  {
    PostCommand( { Commands::Type::TimeChange, "Player request Slower" } );
  }

  if ( IsKeyPressed( KEY_EQUAL ) )
  {
    PostCommand( { Commands::Type::TimeChange, "Player request Faster" } );
  }

  if ( IsKeyPressed( KEY_V ) )
  {
    PostCommand( { Commands::Type::Spawn, "Player spawn Villager", click_pos }
    );
  }

  if ( IsKeyPressed( KEY_C ) )
  {
    PostCommand( { Commands::Type::Spawn, "Player spawn City", click_pos } );
  }

  if ( IsMouseButtonPressed( 0 ) )
  {
    if ( !UI::Manager()->MouseIsOverUI() )
      PostCommand( { Commands::Type::Selection, "Player select", click_pos } );
  }

  if ( IsMouseButtonPressed( 1 ) )
  {
    if ( !UI::Manager()->MouseIsOverUI() )
      PostCommand( { Commands::Type::Move, "Player move" } );
  }

  if ( IsKeyPressed( KEY_P ) )
  {
    MapSystem::mode = MapSystem::Mode::Political;
  }

  if ( IsKeyPressed( KEY_T ) )
  {
    MapSystem::mode = MapSystem::Mode::Terrain;
  }
}

inline void Campaign::PostCommand( Commands::Command cmd )
{
  _command_queue->Enqueue( cmd );
}
