// @todo
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


#include "interface/pages/campaign/actor_context.hpp"
#include "interface/pages/campaign/map_mode_menu.hpp"
#include "interface/pages/campaign/overview_panel.hpp"
#include "interface/pages/campaign/settlement_context/settlement_context.hpp"
#include "network/client.hpp"
#include "network/host.hpp"
#include "network/network.hpp"
#include "shared/global.hpp"

#include "shared/common.hpp"


#include "world/managers/map_manager.hpp"
#include "world/managers/settlement_manager.hpp"

#include "world/components/player_component.hpp"

#include "world/systems/ai_system.hpp"
#include "world/systems/animation_system.hpp"
#include "world/systems/commands_system.hpp"
#include "world/systems/movement_system.hpp"
#include "world/systems/player_system.hpp"
#include "world/systems/province_system.hpp"
#include "world/systems/selection_system.hpp"
#include "world/systems/settlement_system.hpp"

#include "renderer/renderer.hpp"

#include "interface/irongui/forge.hpp"

#include "interface/pages/campaign/actor_context.hpp"
#include "interface/pages/campaign/settlement_context/settlement_context.hpp"
#include <raylib.h>

// If you ever get strange compile time errors from cereal
// Its probably because you didnt include a type
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>

struct Campaign {
  Campaign( bool is_singleplayer ) {
    _is_singleplayer = is_singleplayer;
    // Start();
  }

  // @todo delete this stuff
  Campaign( bool is_singleplayer, const char * ) {
    _is_singleplayer = is_singleplayer;

    // TODO make take in file path
    // Load();
  }

  str GetLocalPlayerID();

  entt::entity GetLocalPlayerE();

  void start( str );
  void save();
  void load(cstr);
  void CheckForInput();
  void CheckForUIInteractions();
  void UpdateOnFrame( f32 &, f32 &, f32 & );
  void Update60TPS();
  void Update1TPS();
  void PostCommand( Commands::Command );

  void Draw();

  void EvaluteCommands( const Commands::Command & );
  // void HandleSpawnRequest( const Commands::Command & );
  void HandleTimeChangeRequest( const Commands::Command & );

  // void ForwardEvent( const InterfaceEvent::Data & );
  void ConvertCommandRequest( str );


  private:
  bool _is_singleplayer = true;
};

inline entt::entity Campaign::GetLocalPlayerE() {
  return Global::world.view<Player::LocalTag>().front();
}

inline str Campaign::GetLocalPlayerID() {
  if ( _is_singleplayer || Network::is_host ) {
    return "player_0";
  } else {
    return Network::Client()->_local_player_id;
  }
}

inline void Campaign::save() {
  printf( "Saving to output\n" );

  std::ofstream jfile( "output.json" );
  {
    cereal::JSONOutputArchive output{ jfile };

    entt::snapshot{ Global::world }
      .get<Player::Component>( output )
      .get<Faction::Component>( output )
      .get<Actor::Component>( output )
      .get<Animated::Component>( output )
      .get<Tile::Component>( output )
      .get<Province::Component>( output )
      .get<Settlement::Component>( output )
      .get<AI::Component>( output );
  }
  jfile.close();

  std::ofstream file( "output.dat", std::ios::binary );
  {
    cereal::BinaryOutputArchive output{ file };

    entt::snapshot{ Global::world }
      .get<Player::Component>( output )
      .get<Faction::Component>( output )
      .get<Actor::Component>( output )
      .get<Animated::Component>( output )
      .get<Tile::Component>( output )
      .get<Province::Component>( output )
      .get<Settlement::Component>( output )
      .get<AI::Component>( output );
  }
  file.close();
}

inline void common_start() {
  Global::ClearRegistry();
  Renderer::Init();

  // Initialize manager singletons
  // Order matters here
  Faction::Manager::Get();
  Actor::Manager::Get();

  Map::Manager()->Init();
  Settlement::Manager()->on_start();
}

inline void Campaign::start( str player_faction ) {
  common_start();

  PlayerSystem::create_players_for_sp( player_faction );
  ProvinceSystem::Init();
  Actor::System::Init();
  AI::Start();

  Commands::Manager()->init();
  Commands::Manager()
    ->queue.sink<Commands::Command>()
    .connect<&Campaign::EvaluteCommands>( this );
}

inline void Campaign::load(cstr file_path) {
  common_start();

  printf( "Loading from file: %s \n", file_path );

  std::ifstream file( file_path, std::ios::binary );
  {
    cereal::BinaryInputArchive input{ file };

    entt::snapshot_loader{ Global::world }
      .get<Player::Component>( input )
      .get<Faction::Component>( input )
      .get<Actor::Component>( input )
      .get<Animated::Component>( input )
      .get<Tile::Component>( input )
      .get<Province::Component>( input )
      .get<Settlement::Component>( input )
      .get<AI::Component>( input );
    // Sight::Component

    // printf( "%u\n", (int) Global::world.size() );
  }
  file.close();

  // Actor::System::Init();
  // AI::Start();

  Global::world.view<Settlement::Component>().each(
    []( Settlement::Component &settlement ) {
      settlement.texture =
        LoadTextureFromImage( Settlement::Manager()->building_map.at( "roman_m1"
        ) );
    }
  );
  Global::world.view<Player::Component>().each(
    []( entt::entity et, Player::Component &player ) {
      printf( "Player %s\n", player.player_id.c_str() );
      if ( player.player_id == "player_0" && player.is_human ) {
        printf( "Assigning!!!!!!!!!!!!!!!!!!!!!!$$$$$$$$$$$$$$$$$$$$$$$\n" );
        Global::world.emplace<Player::LocalTag>( et );
      }
    }
  );

  Commands::Manager()->init();
  Commands::Manager()
    ->queue.sink<Commands::Command>()
    .connect<&Campaign::EvaluteCommands>( this );
}

// Runs inside game loop
inline void Campaign::UpdateOnFrame( f32 &dt, f32 &lag, f32 &oncelag ) {
  CheckForInput();
  CheckForUIInteractions();
}

// TODO: look at all of these and see if any belong in UpdateOnFrame
inline void Campaign::Update60TPS() {
  auto animated_actors =
    Global::world.view<Actor::Component, Animated::Component>();
  auto players = Global::world.view<Player::Component>();

  MovementSystem::Update( animated_actors, Global::state.timeScale );
  AnimationSystem::Update( animated_actors, Global::state.timeScale );
  Commands::Manager()->poll();
  PlayerSystem::Update( players );
  //  Terrain::UpdateFOW(reg);
}

inline void Campaign::Update1TPS() {
  Settlement::System::update_1tps();

  Global::state.day++;

  if ( Global::state.month < 12 )
    Global::state.month++;
  else {
    Global::state.year++;
    Global::state.month = 1;
  }
}

inline void Campaign::Draw() {
  Renderer::draw_map( Map::Manager()->mode );
}

inline void Campaign::CheckForUIInteractions() {
  auto f = Iron::Forge();
  rect root_r = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
  auto root_g = f->Grid( root_r, 4, 4 );

  auto change_map_mode = UI::Minimap( root_g );
  switch ( change_map_mode ) {
    case UI::Action_MapModeChange::Default:
      Map::Manager()->mode = Map::Mode::Default;
      break;
    case UI::Action_MapModeChange::Terrain:
      Map::Manager()->mode = Map::Mode::Terrain;
      break;
    case UI::Action_MapModeChange::Political:
      Map::Manager()->mode = Map::Mode::Political;
      break;
    case UI::Action_MapModeChange::Resources:
      Map::Manager()->mode = Map::Mode::Resources;
      break;
    default:
      break;
  }

  UI::OverviewPanel();

  if ( Selection::Selected<Province::Component, Settlement::Component>() ) {
    auto settlement = Selection::GetSelectedComponent<Settlement::Component>();
    auto prov = Selection::GetSelectedComponent<Province::Component>();
    entt::entity player_e = GetLocalPlayerE();
    auto faction = Global::world.get<Faction::Component>( player_e );

    if ( !settlement || !prov ) {
      printf( "Got null prov/settlement??\n" );
      return;
    }

    auto action = UI::SettlementContext( root_g, faction, settlement );
    switch ( action ) {
      case UI::Action_SettlementContext::SpawnColonist:
        PostCommand( Commands::Command::spawn_colonist(
          player_e, Settlement::System::settlement_position( *prov )
        ) );
        break;
      case UI::Action_SettlementContext::SpawnArmy:
        PostCommand( Commands::Command::spawn_army(
          player_e, Settlement::System::settlement_position( *prov )
        ) );
        break;
      case UI::Action_SettlementContext::BuildFarm:
        PostCommand( Commands::Command::construct_building( player_e, "farm" )
        );
        break;
      case UI::Action_SettlementContext::None:
        break;
    }
  }

  if ( Selection::Selected<Actor::Component>() ) {
    auto actor = Selection::GetSelectedComponent<Actor::Component>();
    // entt::entity player_e = GetLocalPlayerE();

    if ( !actor ) {
      printf( "Got null actor??\n" );
      return;
    }

    auto action = UI::ActorContext( actor );

    switch ( action ) {
      case UI::Action_ActorContext::ClaimProvince: {
        PostCommand(
          Commands::Command::claim_province( Selection::GetSelectedEntity() )
        );
      } break;
      case UI::Action_ActorContext::SpawnSettlement: {
        PostCommand(
          Commands::Command::build_settlement( Selection::GetSelectedEntity() )
        );
      } break;
      case UI::Action_ActorContext::None:
        break;
    }
  }
}

inline void Campaign::CheckForInput() {
  Vector2 click_pos =
    GetScreenToWorld2D( GetMousePosition(), Global::state.camera );

  auto player_e =
    Global::world.view<Player::Component, Player::LocalTag>().front();

  if ( player_e == entt::null ) {
    std::cout << "ERROR" << " no local player was found" << '\n';
    return;
  }

  if ( IsKeyPressed( KEY_SPACE ) ) {
    PostCommand(
      Commands::Command::time_change( player_e, "Player request Pause" )
    );
  }

  if ( IsKeyPressed( KEY_MINUS ) ) {
    PostCommand(
      Commands::Command::time_change( player_e, "Player request Slower" )
    );
  }

  if ( IsKeyPressed( KEY_EQUAL ) ) {
    PostCommand(
      Commands::Command::time_change( player_e, "Player request Faster" )
    );
  }

  if ( IsKeyPressed( KEY_V ) ) {
    PostCommand( Commands::Command::spawn_colonist( player_e, click_pos ) );
  }


  if ( IsMouseButtonPressed( 0 ) ) {
    if ( !Iron::Forge()->MouseIsOverUI() ) {
      Selection::UpdateSelection( click_pos, GetLocalPlayerID() );
    }
  }

  if ( IsMouseButtonPressed( 1 ) ) {
    if ( !Iron::Forge()->MouseIsOverUI() ) {
      auto selected_e =
        Global::world
          .view<Actor::Component, Animated::Component, Selected::Component>()
          .front();

      if ( selected_e != entt::null ) {
        std::cout << "Moving entity: " << EntityIdToString( selected_e )
                  << '\n';
        PostCommand( Commands::Command::move( player_e, click_pos, selected_e )

        );
      }
    }
  }

  if ( IsKeyPressed( KEY_P ) ) {
    Map::Manager()->mode = Map::Mode::Political;
  }

  if ( IsKeyPressed( KEY_T ) ) {
    Map::Manager()->mode = Map::Mode::Terrain;
  }
}

inline void Campaign::ConvertCommandRequest( str cmd ) {
  nlohmann::json body = nlohmann::json::parse( cmd );

  std::cout << "BODY" << body << '\n';

  str cmd_player_id = body["cmd_player"];
  Commands::Type cmd_type = body["cmd_type"];
  str cmd_msg = body["cmd_msg"];
  f32 cmd_click_pos_x = body["cmd_pos.x"];
  f32 cmd_click_pos_y = body["cmd_pos.y"];
  entt::entity entity = body["entity"];

  auto players = Global::world.view<Player::Component>();

  for ( entt::entity player: players ) {
    Player::Component pc = Global::world.get<Player::Component>( player );

    std::cout << "pc.player_id: " << pc.player_id << '\n';
    std::cout << "cmd_player_id: " << cmd_player_id << '\n';

    if ( pc.player_id == cmd_player_id ) {
      // @todo Gonna need to redo converting net command requests to local again now that
      // I changed how commands work
      // auto cmd = Commands::Command{
      //   .type = cmd_type,
      //   .player_e = player,
      //   .msg = cmd_msg,
      //   .click_pos = { cmd_click_pos_x, cmd_click_pos_y },
      //   .entity = entity,
      // };

      // Commands::Manager()->enqueue( cmd );
    }
  }
}


inline void Campaign::PostCommand( Commands::Command cmd ) {
  if ( _is_singleplayer ) {
    Commands::Manager()->enqueue( cmd );
  } else {
    auto body = nlohmann::json{
      { "cmd_player", GetLocalPlayerID() },
      { "cmd_type", cmd.type },
      { "cmd_msg", cmd.msg },
      { "cmd_pos.x", cmd.click_pos.x },
      { "cmd_pos.y", cmd.click_pos.y },
      { "entity", cmd.entity },
    };

    if ( Network::is_host ) {
      Network::Host()->SendMessageToAllActiveClients( Network::Message{
        Network::MessageID::Command,
        body,
      } );
      Commands::Manager()->enqueue( cmd );
    } else {
      Network::Client()->SendMessageToHost( Network::Message{
        Network::MessageID::Command,
        body,
      } );
    }
  }
}


// @todo This should be split to explicit Evaluate and Execute steps.
inline void Campaign::EvaluteCommands( const Commands::Command &cmd ) {
  switch ( cmd.type ) {
    case Commands::Type::BuildSettlement: {
      Settlement::System::spawn_settlement( cmd.entity );
      return;
    }
    case Commands::Type::ClaimProvince: {
      ProvinceSystem::colonist_claim_province( cmd.entity );
      return;
    }
    case Commands::Type::TimeChange: {
      HandleTimeChangeRequest( cmd );
      return;
    }
    case Commands::Type::SpawnColonist: {
      Actor::System::spawn_colonist( cmd.player_e, cmd.click_pos );
      return;
    }
    case Commands::Type::SpawnArmy: {
      Actor::System::spawn_army( cmd.player_e, cmd.click_pos );
      return;
    }
    case Commands::Type::Move: {
      MovementSystem::SetDestinations( cmd.entity, cmd.click_pos );
      return;
    }
    case Commands::Type::ConstructBuilding: {
      Settlement::System::construct_building( cmd.msg );
      return;
    };
  }
}


inline void Campaign::HandleTimeChangeRequest( const Commands::Command &cmd ) {
  if ( cmd.msg == "Player request Pause" ) {
    std::cout << cmd.msg << std::endl;

    if ( Global::state.timeScale > 0.0f ) {
      Global::state.prevTimeScale = Global::state.timeScale;
      Global::state.timeScale = 0.0f;
    } else if ( Global::state.timeScale == 0.0f ) {
      Global::state.timeScale = Global::state.prevTimeScale;
    }

    return;
  }

  if ( cmd.msg == "Player request Slower" ) {
    Global::state.timeScale -= 0.5f;
    if ( Global::state.timeScale < 0.0f )
      Global::state.timeScale = 0.0f;

    if ( Global::state.timeScale == 0.0f &&
         Global::state.prevTimeScale > 0.5f ) {
      Global::state.prevTimeScale -= 0.5f;
      Global::state.timeScale = Global::state.prevTimeScale;
    }

    return;
  }

  if ( cmd.msg == "Player request Faster" ) {
    Global::state.timeScale += 0.5f;
    if ( Global::state.timeScale > 1.5f )
      Global::state.timeScale = 1.5f;

    return;
  }
}
