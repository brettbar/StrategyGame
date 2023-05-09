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

#include "world/systems/actor_system.hpp"
#include "world/systems/animation_system.hpp"
#include "world/systems/faction_system.hpp"
#include "world/systems/map_system.hpp"
#include "world/systems/movement_system.hpp"
#include "world/systems/player_system.hpp"
#include "world/systems/province_system.hpp"
#include "world/systems/settlement_system.hpp"

#include "renderer/renderer.hpp"

#include "interface/ui_system.hpp"


enum class CommandType
{
  TimeChange,
  Spawn,
  Selection,
  Move,
};

struct Command
{
  CommandType type;

  std::string msg;
  Vector2 click_pos;
};

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
    // delete _command_queue;
  }

  void CheckForInput();
  void UpdateOnFrame( f32 &, f32 &, f32 & );
  void Update60TPS();
  void Update1TPS();
  void PostCommand( Command );

  void Receive( const Command & );
  void HandleSpawnRequest( const Command & );
  void HandleTimeChangeRequest( const Command & );


  private:
  entt::dispatcher _command_queue;

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

  _command_queue = entt::dispatcher{};
  _command_queue.sink<Command>().connect<&Campaign::Receive>( this );
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

  _command_queue = entt::dispatcher{};
  _command_queue.sink<Command>().connect<&Campaign::Receive>( this );
}

// Runs inside game loop
inline void Campaign::UpdateOnFrame( f32 &dt, f32 &lag, f32 &oncelag )
{
  // 2. Check for Input
  CheckForInput();

  // 3. Process all commands
  _command_queue.update();
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
    PostCommand( { CommandType::TimeChange, "Player request Pause" } );
  }

  if ( IsKeyPressed( KEY_MINUS ) )
  {
    PostCommand( { CommandType::TimeChange, "Player request Slower" } );
  }

  if ( IsKeyPressed( KEY_EQUAL ) )
  {
    PostCommand( { CommandType::TimeChange, "Player request Faster" } );
  }

  if ( IsKeyPressed( KEY_V ) )
  {
    PostCommand( { CommandType::Spawn, "Player spawn Villager", click_pos } );
  }

  if ( IsKeyPressed( KEY_C ) )
  {
    PostCommand( { CommandType::Spawn, "Player spawn City", click_pos } );
  }

  if ( IsMouseButtonPressed( 0 ) )
  {
    if ( !UI::Manager()->MouseIsOverUI() )
      PostCommand( { CommandType::Selection, "Player select", click_pos } );
  }

  if ( IsMouseButtonPressed( 1 ) )
  {
    if ( !UI::Manager()->MouseIsOverUI() )
      PostCommand( { CommandType::Move, "Player move" } );
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

inline void Campaign::PostCommand( Command cmd )
{
  _command_queue.enqueue( cmd );
}

inline void Campaign::Receive( const Command &cmd )
{
  switch ( cmd.type )
  {
    case CommandType::TimeChange:
    {
      HandleTimeChangeRequest( cmd );
      return;
    }
    case CommandType::Spawn:
    {
      HandleSpawnRequest( cmd );
      return;
    }
    case CommandType::Selection:
    {
      SelectionSystem::UpdateSelection( cmd.click_pos );
      return;
    }
    case CommandType::Move:
    {
      MovementSystem::SetDestinations( Global::state.camera );
      return;
    }
  }
}


inline void Campaign::HandleTimeChangeRequest( const Command &cmd )
{
  if ( cmd.msg == "Player request Pause" )
  {
    std::cout << cmd.msg << std::endl;

    if ( Global::state.timeScale > 0.0f )
    {
      Global::state.prevTimeScale = Global::state.timeScale;
      Global::state.timeScale = 0.0f;
    }
    else if ( Global::state.timeScale == 0.0f )
    {
      Global::state.timeScale = Global::state.prevTimeScale;
    }

    return;
  }

  if ( cmd.msg == "Player request Slower" )
  {
    Global::state.timeScale -= 0.5f;
    if ( Global::state.timeScale < 0.0f )
      Global::state.timeScale = 0.0f;

    if ( Global::state.timeScale == 0.0f && Global::state.prevTimeScale > 0.5f )
    {
      Global::state.prevTimeScale -= 0.5f;
      Global::state.timeScale = Global::state.prevTimeScale;
    }

    return;
  }

  if ( cmd.msg == "Player request Faster" )
  {
    Global::state.timeScale += 0.5f;
    if ( Global::state.timeScale > 1.5f )
      Global::state.timeScale = 1.5f;

    return;
  }
}

inline void Campaign::HandleSpawnRequest( const Command &cmd )
{

  if ( cmd.msg == "Player spawn Villager" )
  {
    ActorSystem::SpawnColonist( Global::host_player, cmd.click_pos );
    return;
  }

  if ( cmd.msg == "Player spawn City" )
  {
    ProvinceSystem::AssignProvince( Global::host_player, cmd.click_pos );
  }
};
