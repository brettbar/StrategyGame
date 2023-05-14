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


#include "network/network.hpp"
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
  Move,
};

struct Command
{
  CommandType type;
  entt::entity player_e;

  std::string msg;
  Vector2 click_pos;
  entt::entity entity;
};

class Campaign
{

  public:
  Campaign( bool is_singleplayer )
  {
    _is_singleplayer = is_singleplayer;
    Start();
  }

  Campaign( bool is_singleplayer, const char * )
  {
    _is_singleplayer = is_singleplayer;

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

  void ConvertCommandRequest( std::string );


  private:
  entt::dispatcher _command_queue;
  bool _is_singleplayer = true;

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

  auto player_e =
    Global::world.view<Player::Component, Player::LocalTag>().front();

  if ( player_e == entt::null )
  {
    std::cout << "ERROR"
              << " no local player was found" << '\n';
    return;
  }

  if ( IsKeyPressed( KEY_SPACE ) )
  {
    PostCommand( { CommandType::TimeChange, player_e, "Player request Pause" }
    );
  }

  if ( IsKeyPressed( KEY_MINUS ) )
  {
    PostCommand( { CommandType::TimeChange, player_e, "Player request Slower" }
    );
  }

  if ( IsKeyPressed( KEY_EQUAL ) )
  {
    PostCommand( { CommandType::TimeChange, player_e, "Player request Faster" }
    );
  }

  if ( IsKeyPressed( KEY_V ) )
  {
    PostCommand(
      { CommandType::Spawn, player_e, "Player spawn Villager", click_pos }
    );
  }

  if ( IsKeyPressed( KEY_C ) )
  {
    PostCommand(
      { CommandType::Spawn, player_e, "Player spawn City", click_pos }
    );
  }

  if ( IsMouseButtonPressed( 0 ) )
  {
    if ( !UI::Manager()->MouseIsOverUI() )
    {
      if ( _is_singleplayer )
        SelectionSystem::UpdateSelection( click_pos, "player_0" );
      else if ( Network::is_host )
      {
        SelectionSystem::UpdateSelection( click_pos, "player_0" );
      }
      else
      {
        SelectionSystem::UpdateSelection(
          click_pos, Network::Client()->_local_player_id
        );
      }
    }
  }

  if ( IsMouseButtonPressed( 1 ) )
  {
    if ( !UI::Manager()->MouseIsOverUI() )
    {
      auto selected_e =
        Global::world
          .view<Unit::Component, Animated::Component, Selected::Component>()
          .front();

      if ( selected_e != entt::null )
      {
        std::cout << "Moving entity: " << EntityIdToString( selected_e )
                  << '\n';
        PostCommand( {
          CommandType::Move,
          player_e,
          "Player move",
          click_pos,
          selected_e,
        } );
      }
    }
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

inline void Campaign::ConvertCommandRequest( std::string str )
{
  nlohmann::json body = nlohmann::json::parse( str );

  std::cout << "BODY" << body << '\n';

  std::string cmd_player_id = body["cmd_player"];
  CommandType cmd_type = body["cmd_type"];
  std::string cmd_msg = body["cmd_msg"];
  f32 cmd_click_pos_x = body["cmd_pos.x"];
  f32 cmd_click_pos_y = body["cmd_pos.y"];
  entt::entity entity = body["entity"];

  auto players = Global::world.view<Player::Component>();

  for ( entt::entity player: players )
  {
    Player::Component pc = Global::world.get<Player::Component>( player );

    std::cout << "pc.player_id: " << pc.player_id << '\n';
    std::cout << "cmd_player_id: " << cmd_player_id << '\n';

    if ( pc.player_id == cmd_player_id )
    {
      _command_queue.enqueue( Command{
        .type = cmd_type,
        .player_e = player,
        .msg = cmd_msg,
        .click_pos = { cmd_click_pos_x, cmd_click_pos_y },
        .entity = entity,
      } );
    }
  }
}


inline void Campaign::PostCommand( Command cmd )
{
  if ( _is_singleplayer )
  {
    _command_queue.enqueue( cmd );
  }
  else
  {
    if ( Network::is_host )
    {
      Network::Host()->SendMessageToAllActiveClients( Network::Message{
        Network::MessageID::Command,
        nlohmann::json{
          { "cmd_player", "player_0" },
          { "cmd_type", cmd.type },
          { "cmd_msg", cmd.msg },
          { "cmd_pos.x", cmd.click_pos.x },
          { "cmd_pos.y", cmd.click_pos.y },
          { "entity", cmd.entity },
        },
      } );
      _command_queue.enqueue( cmd );
    }
    else
    {
      Network::Client()->SendMessageToHost( Network::Message{
        Network::MessageID::Command,
        nlohmann::json{
          { "cmd_player", Network::Client()->_local_player_id },
          { "cmd_type", cmd.type },
          { "cmd_msg", cmd.msg },
          { "cmd_pos.x", cmd.click_pos.x },
          { "cmd_pos.y", cmd.click_pos.y },
          { "entity", cmd.entity },
        },
      } );
    }
  }
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
      if ( cmd.msg == "Player spawn Villager" )
      {
        ActorSystem::SpawnColonist( cmd.player_e, cmd.click_pos );
        return;
      }

      if ( cmd.msg == "Player spawn City" )
      {
        ProvinceSystem::AssignProvince( cmd.player_e, cmd.click_pos );
      }
      return;
    }
    case CommandType::Move:
    {
      MovementSystem::SetDestinations( cmd.entity, cmd.click_pos );
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
