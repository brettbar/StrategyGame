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
#include "interface/pages/campaign/settlement_context/settlement_context.hpp"
#include "interface/pages/editor.hpp"
#include "network/client.hpp"
#include "network/host.hpp"
#include "network/network.hpp"
#include "shared/global.hpp"
#include "shared/save.hpp"

#include "shared/common.hpp"

#include "world/components/player.hpp"
#include "world/systems/actor_system.hpp"
#include "world/systems/ai_system.hpp"
#include "world/systems/animation_system.hpp"
#include "world/systems/commands.hpp"
#include "world/systems/faction_system.hpp"
#include "world/systems/map_system.hpp"
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


struct Campaign {
  Campaign( bool is_singleplayer ) {
    _is_singleplayer = is_singleplayer;
    // Start();
  }

  Campaign( bool is_singleplayer, const char * ) {
    _is_singleplayer = is_singleplayer;

    // TODO make take in file path
    // Load();
  }

  ~Campaign() {
    Global::ClearRegistry();
    // delete _command_queue;
  }

  str GetLocalPlayerID();

  entt::entity GetLocalPlayerE();

  void Start( str );
  void Load();
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

inline void Campaign::Start( str player_faction ) {
  FactionSystem::Init();
  PlayerSystem::StartSingleplayer( player_faction );

  MapSystem::Manager()->Init();
  SettlementSystem::Init();
  ProvinceSystem::Init();
  Renderer::Init();

  Actor::System::Init();
  AI::Start();

  Commands::Manager()->init();

  Commands::Manager()
    ->queue.sink<Commands::Command>()
    .connect<&Campaign::EvaluteCommands>( this );
}

inline void Campaign::Load() {
  SaveSystem::Load();
  MapSystem::Manager()->Init();
  Renderer::Init();
  Commands::Manager()->init();
  Global::world.view<Settlement::Component>().each(
    []( Settlement::Component &settlement ) {
      settlement.texture =
        LoadTextureFromImage( Settlement::building_map.at( "roman_m1" ) );
    }
  );
  Commands::Manager()
    ->queue.sink<Commands::Command>()
    .connect<&Campaign::EvaluteCommands>( this );
}

// Runs inside game loop
inline void Campaign::UpdateOnFrame( f32 &dt, f32 &lag, f32 &oncelag ) {
  CheckForInput();
  CheckForUIInteractions();
  Commands::Manager()->poll();
}

// TODO: look at all of these and see if any belong in UpdateOnFrame
inline void Campaign::Update60TPS() {
  auto animated_actors =
    Global::world.view<Actor::Component, Animated::Component>();
  auto players = Global::world.view<Player::Component>();

  MovementSystem::Update( animated_actors, Global::state.timeScale );
  AnimationSystem::Update( animated_actors, Global::state.timeScale );
  PlayerSystem::Update( players );
  //  Terrain::UpdateFOW(reg);
}

inline void Campaign::Update1TPS() {
  SettlementSystem::Update(
    Global::world.view<Province::Component, Settlement::Component>()
  );

  Global::state.day++;

  if ( Global::state.month < 12 )
    Global::state.month++;
  else {
    Global::state.year++;
    Global::state.month = 1;
  }
}

inline void Campaign::Draw() {
  Renderer::draw_map( MapSystem::Manager()->mode );
}

// inline void Campaign::ForwardEvent( const InterfaceEvent::Data &event ) {
//   Vector2 click_pos =
//     GetScreenToWorld2D( GetMousePosition(), Global::state.camera );

//   auto player_e =
//     Global::world.view<Player::Component, Player::LocalTag>().front();

//   if ( player_e == entt::null ) {
//     std::cout << "ERROR"
//               << " no local player was found" << '\n';
//     return;
//   }

//   switch ( event.event_id ) {
//     case InterfaceEvent::ID::SettlementContextMilitaryTab: {
//       InterfaceUpdate::Update{
//         .id = InterfaceUpdate::ID::SettlementContextMilitaryTab,
//       }
//         .Send();
//     }; break;
//     case InterfaceEvent::ID::SettlementContextPopulationTab: {
//       InterfaceUpdate::Update{
//         .id = InterfaceUpdate::ID::SettlementContextPopulationTab,
//       }
//         .Send();
//     }; break;
//     case InterfaceEvent::ID::SettlementContextResourcesTab: {
//       InterfaceUpdate::Update{
//         .id = InterfaceUpdate::ID::SettlementContextResourcesTab,
//       }
//         .Send();
//     }; break;
//     case InterfaceEvent::ID::SettlementContextConstructionTab: {
//       InterfaceUpdate::Update{
//         .id = InterfaceUpdate::ID::SettlementContextConstructionTab,
//       }
//         .Send();
//     }; break;
//     case InterfaceEvent::ID::SettlementContextConstructBuilding: {
//       SettlementSystem::ConstructBuilding( event.msg );
//     }; break;
//     case InterfaceEvent::ID::SettlementContextTrainHastati: {
//       SettlementSystem::TrainRegiment( UnitType::Hastati );
//     }; break;
//     case InterfaceEvent::ID::ActorSpawnSettlment: {
//       PostCommand( Command{
//         .type = CommandType::Spawn,
//         .player_e = player_e,
//         .msg = "Player spawn Settlement",
//         .click_pos = click_pos,
//       } );
//     }; break;
//     default:
//       std::cout << "Unknown event in Campaign::ForwardEvent" << '\n';
//       break;
//   };
// }

inline void Campaign::CheckForUIInteractions() {
  auto change_map_mode = UI::Minimap();
  switch ( change_map_mode ) {
    case UI::Action_MapModeChange::Default:
      MapSystem::Manager()->mode = MapSystem::Mode::Default;
      break;
    case UI::Action_MapModeChange::Terrain:
      MapSystem::Manager()->mode = MapSystem::Mode::Terrain;
      break;
    case UI::Action_MapModeChange::Political:
      MapSystem::Manager()->mode = MapSystem::Mode::Political;
      break;
    case UI::Action_MapModeChange::Resources:
      MapSystem::Manager()->mode = MapSystem::Mode::Resources;
      break;
    default:
      break;
  }

  if ( SelectionSystem::Selected<Province::Component, Settlement::Component>(
       ) ) {
    auto settlement =
      SelectionSystem::GetSelectedComponent<Settlement::Component>();
    auto prov = SelectionSystem::GetSelectedComponent<Province::Component>();
    entt::entity player_e = GetLocalPlayerE();

    if ( !settlement || !prov ) {
      printf( "Got null prov/settlement??\n" );
      return;
    }

    auto action = UI::SettlementContext( settlement );
    switch ( action ) {
      case UI::Action_SettlementContext::SpawnActor:
        PostCommand( Commands::Command::spawn_actor(
          player_e, SettlementSystem::SettlementPosition( *prov )
        ) );
        break;
      case UI::Action_SettlementContext::SpawnHastati:
        break;
      case UI::Action_SettlementContext::BuildFarm:
        break;
      case UI::Action_SettlementContext::None:
        break;
    }
  }

  if ( SelectionSystem::Selected<Actor::Component>() ) {
    auto actor = SelectionSystem::GetSelectedComponent<Actor::Component>();
    entt::entity player_e = GetLocalPlayerE();

    if ( !actor ) {
      printf( "Got null actor??\n" );
      return;
    }

    auto action = UI::ActorContext( actor );

    switch ( action ) {
      case UI::Action_ActorContext::ClaimProvince: {
        PostCommand(
          Commands::Command::claim_province( player_e, actor->position )
        );
      } break;
      case UI::Action_ActorContext::SpawnSettlement: {
        PostCommand( Commands::Command::build_settlement(
          SelectionSystem::GetSelectedEntity()
        ) );
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
    std::cout << "ERROR"
              << " no local player was found" << '\n';
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
    PostCommand( Commands::Command::spawn_actor( player_e, click_pos ) );
  }

  if ( IsKeyPressed( KEY_C ) ) {
    PostCommand( Commands::Command::claim_province( player_e, click_pos ) );
  }

  if ( IsMouseButtonPressed( 0 ) ) {
    // if ( !UI::Manager()->MouseIsOverUI() ) {
    //   SelectionSystem::UpdateSelection( click_pos, GetLocalPlayerID() );
    // }
    if ( !Iron::Forge()->MouseIsOverUI() ) {
      SelectionSystem::UpdateSelection( click_pos, GetLocalPlayerID() );
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
    MapSystem::Manager()->mode = MapSystem::Mode::Political;
  }

  if ( IsKeyPressed( KEY_T ) ) {
    MapSystem::Manager()->mode = MapSystem::Mode::Terrain;
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
      SettlementSystem::spawn_settlement( cmd.entity );
    } break;
    case Commands::Type::ClaimProvince: {
      ProvinceSystem::AssignProvince( cmd.player_e, cmd.click_pos );
    } break;
    case Commands::Type::TimeChange: {
      HandleTimeChangeRequest( cmd );
      return;
    }
    case Commands::Type::SpawnActor: {
      Actor::System::spawn_colonist( cmd.player_e, cmd.click_pos );
      return;
    }
    case Commands::Type::Move: {
      MovementSystem::SetDestinations( cmd.entity, cmd.click_pos );
      return;
    }
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

    if ( Global::state.timeScale == 0.0f && Global::state.prevTimeScale > 0.5f ) {
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
