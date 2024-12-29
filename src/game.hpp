#pragma once

#include "interface/irongui/forge.hpp"
#include "interface/pages/editor.hpp"
#include "interface/pages/faction_select_menu.hpp"
#include "interface/pages/lobby_browser.hpp"
#include "interface/pages/main_menu_ui.hpp"
#include "interface/pages/modal_menu_ui.hpp"
#include "interface/pages/multiplayer_lobby.hpp"
#include "interface/pages/save_games.hpp"
#include "interface/pages/singleplayer_lobby.hpp"
#include "network/client.hpp"
#include "network/host.hpp"


#include "network/network.hpp"
#include "shared/common.hpp"


#include "world/systems/player_system.hpp"

#include "campaign.hpp"
#include <cstddef>
#include <optional>
#include <raylib.h>

enum class Scene {
  MainMenu,
  LoadGames,
  SinglePlayerLobby,
  MultiPlayerLobby,
  LobbyBrowser,
  SPFactionSelect,
  MPFactionSelect,
  ModalMenu,
  Campaign,
  Editor,
};

class IGame {
  public:
  static IGame *Game() {
    static IGame instance;
    return &instance;
  }

  void MainLoop() {
    // RegisterEventListeners();

    Global::state.camera = Camera2D{
      .offset = { (f32) GetScreenWidth() / 2, (f32) GetScreenHeight() / 2 },
      .target =
        { ( Global::state.mapWidth * 64.0f ) / 2,
          ( Global::state.mapHeight * 64.0f ) / 2 },
      .rotation = 0,
      .zoom = 2.0f,
    };

    // Initialize manager singletons
    // Order matters here
    Faction::Manager::Get();
    Actor::Manager::Get();

    while ( !WindowShouldClose() && ShouldRun() ) {
      SteamAPI_RunCallbacks();

      // 1. Update Time
      _dt = GetFrameTime();
      _lag += _dt;
      _oncelag += _dt;

      // 5. Run all Updates
      {
        // Update 60 times a second
        while ( _lag >= _MS_PER_UPDATE ) {
          if ( _campaign )
            _campaign->Update60TPS();
          _lag -= _MS_PER_UPDATE;
        }

        // Update once per second no timeScale
        // while ( _oncelag >= _ONCE_A_SECOND )
        // {
        //   std::cout << "What the fuck???" << '\n';

        //   if ( Network::is_host )
        //   {

        //     // Network::Host()->PingAllActiveClients();
        //   }
        //   _oncelag = 0.0f;
        // }

        // Update once per second but modified by timeScale
        // TODO? Do we want to fold this timescale business into campaign itself
        // Since we dont care about timescale in other program_modes
        while ( _oncelag >= _ONCE_A_SECOND * ( 1 / Global::state.timeScale ) ) {
          if ( _campaign )
            _campaign->Update1TPS();
          _oncelag = 0.0f;
        }

        // Update once per frame
        UpdateOnFrame();

        // Update Camera
        CameraUpdate( Global::state.camera, _dt );
      }
    }

    ExitCampaignCleanup();
  }

  Campaign *GetCampaign() {
    return _campaign;
  }

  private:
  Campaign *_campaign;
  bool _single_player = true;
  // bool _creating_lobby = false;
  // bool _looking_for_lobby = false;
  // bool _in_lobby = false;
  bool _hit_exit = false;
  Scene _scene = Scene::MainMenu;

  f32 _oncelag = 0.0f;
  f32 _lag = 0.0f;
  f32 _dt = 0.0f;
  const f32 _MS_PER_UPDATE = 1 / 60.0;
  const f32 _ONCE_A_SECOND = 1;

  // TODO move
  // TODO this should be set from networking
  // std::string player_id = "player_0";

  bool ShouldRun() {
    return !_hit_exit;
  }

  /*=============================================================

                        Begin: Singleplayer
  =============================================================*/
  void StartSingleplayerCampaign( str player_faction ) {
    _scene = Scene::Campaign;

    _campaign = new struct Campaign( true );
    _campaign->start( player_faction );
  }

  void LoadSinglePlayerCampaign( cstr file_path ) {
    _scene = Scene::Campaign;

    _campaign = new struct Campaign( true, "output.dat" );
    _campaign->load( file_path );
  }
  /*=============================================================
                        End: Singleplayer
  =============================================================*/


  /*=============================================================
                        Begin: Multiplayer
  =============================================================*/
  void HostMultiplayerLobby() {
    _single_player = false;
    Network::is_host = true;
    Network::Host()->Init();
  }

  // @todo maybe can consolidate these
  void HostStartMultiplayerCampaign() {
    if ( _campaign )
      delete _campaign;

    _scene = Scene::Campaign;
    _campaign = new struct Campaign( false );
    _campaign->start_mp();
    // _campaign->start( Network::Host()->GetHostFaction() );
  }
  void ClientStartMultiplayerCampaign() {
    if ( _campaign )
      delete _campaign;

    _scene = Scene::Campaign;
    _campaign = new struct Campaign( false );
    _campaign->start_mp();
    // _campaign->start( Network::Client()->GetClientFaction() );
  }

  void LookForMultiplayerLobby() {
    _single_player = false;
    Network::is_host = false;
    Network::Client()->Init();
  }

  void JoinMultiplayerLobby( CSteamID lobby_id ) {
    if ( Network::Client()->AttemptJoinLobby( lobby_id ) ) {
      printf( "Sending joined lobby event!\n" );
    }
  }

  /*=============================================================
                        End: Multiplayer
  =============================================================*/

  /*=============================================================
                        Begin: Shared
  =============================================================*/

  void ExitGame() {
    _hit_exit = true;
  }


  void ReturnToMain() {
    _scene = Scene::MainMenu;
  }

  void CheckMenuToggle() {
    if ( IsKeyPressed( KEY_CAPS_LOCK ) || IsKeyPressed( KEY_ESCAPE ) ) {
      if ( _scene == Scene::ModalMenu ) {
        _scene = Scene::Campaign;
      } else {
        _scene = Scene::ModalMenu;
      }
    }
  }

  void ExitToMainMenu() {
    _scene = Scene::MainMenu;
    ExitCampaignCleanup();
  }

  void UpdateOnFrame() {
    if ( Network::is_host ) {
      Network::Host()->Update();
    } else {
      auto msg = Network::Client()->CheckForMessages();
      switch ( msg.type ) {
        case Network::IClient::ReceivedMessage_t::None: {
        } break;
        case Network::IClient::ReceivedMessage_t::StartMultiplayer: {
          ClientStartMultiplayerCampaign();
        } break;
        case Network::IClient::ReceivedMessage_t::Command: {
          _campaign->ConvertCommandRequest( msg.body.dump() );
        } break;
      }
    }

    Iron::Forge()->over_any_elem = false;

    switch ( _scene ) {
      case Scene::MainMenu:
        Scene_MainMenu();
        break;
      case Scene::LoadGames:
        Scene_LoadGames();
        break;
      case Scene::SinglePlayerLobby:
        Scene_SinglePlayerLobby();
        break;
      case Scene::MultiPlayerLobby:
        Scene_MultiplayerLobby();
        break;
      case Scene::LobbyBrowser:
        Scene_LobbyBrowser();
        break;
      case Scene::SPFactionSelect:
        Scene_SPFactionSelect();
        break;
      case Scene::MPFactionSelect:
        Scene_MPFactionSelect();
        break;
      case Scene::ModalMenu:
        Scene_ModalMenu();
        break;
      case Scene::Editor:
      case Scene::Campaign:
        Scene_Campaign();
        break;
    }

    if ( !Iron::Forge()->over_any_elem ) {
      Iron::Forge()->context.hot = -1;
      Iron::Forge()->context.active = -1;
    }
  }


  void Scene_MainMenu() {
    auto action = UI::DrawMainMenu();

    switch ( action ) {
      case UI::Action_MainMenu::StartGame:
        _scene = Scene::SinglePlayerLobby;
        break;
      case UI::Action_MainMenu::LoadGame: {
        _scene = Scene::LoadGames;
      } break;
      case UI::Action_MainMenu::HostGame:
        HostMultiplayerLobby();
        _scene = Scene::MultiPlayerLobby;
        break;
      case UI::Action_MainMenu::JoinGame:
        LookForMultiplayerLobby();
        _scene = Scene::LobbyBrowser;
        break;
      case UI::Action_MainMenu::Settings:
        break;
      case UI::Action_MainMenu::ExitGame:
        ExitGame();
        return;
      case UI::Action_MainMenu::None:
        break;
    }

    BeginDrawing();
    {
      ClearBackground( BLACK );
      Iron::Forge()->DrawAll();
    }
    EndDrawing();
  }

  void Scene_LoadGames() {
    bool pressed_back = false;
    auto file_to_load = UI::load_game_menu( pressed_back );

    if ( pressed_back ) {
      _scene = Scene::MainMenu;
    }

    if ( file_to_load != "" ) {
      LoadSinglePlayerCampaign( file_to_load.c_str() );
    }

    BeginDrawing();
    {
      ClearBackground( BLACK );
      Iron::Forge()->DrawAll();
    }
    EndDrawing();
  }

  void Scene_SinglePlayerLobby() {
    auto action = UI::SinglePlayerLobby();

    switch ( action ) {
      case UI::Action_SinglePlayerLobby::SelectFaction:
        _scene = Scene::SPFactionSelect;
        break;
      case UI::Action_SinglePlayerLobby::ExitPressed:
        _scene = Scene::MainMenu;
        break;
      case UI::Action_SinglePlayerLobby::None:
        break;
    }
    BeginDrawing();
    {
      ClearBackground( BLACK );
      Iron::Forge()->DrawAll();
    }
    EndDrawing();
  }

  void Scene_MultiplayerLobby() {
    switch ( UI::MultiPlayerLobby() ) {
      case UI::Action_MultiplayerLobby::None:
        break;
      case UI::Action_MultiplayerLobby::HitBack:
        _scene = Scene::MainMenu;
        break;
      case UI::Action_MultiplayerLobby::PickFaction: {
        _scene = Scene::MPFactionSelect;
      } break;
      case UI::Action_MultiplayerLobby::ToggleReady:
        if ( Network::is_host ) {
          Network::Host()->ToggleReady();
        } else {
          Network::Client()->ToggleReady();
        }
        break;
      case UI::Action_MultiplayerLobby::HostStartedGame: {
        Network::Host()->StartHostedCampaign();
        HostStartMultiplayerCampaign();
      } break;
    }

    BeginDrawing();
    {
      ClearBackground( BLACK );
      Iron::Forge()->DrawAll();
    }
    EndDrawing();
  }

  void Scene_LobbyBrowser() {
    CSteamID choice = UI::LobbyBrowser();
    // dont know if this works
    if ( choice.IsValid() ) {
      if ( Network::Client()->AttemptJoinLobby( choice ) ) {
        _scene = Scene::MultiPlayerLobby;
      }
    }

    BeginDrawing();
    {
      ClearBackground( BLACK );
      Iron::Forge()->DrawAll();
    }
    EndDrawing();
  }

  void Scene_SPFactionSelect() {
    auto selection = UI::DrawFactionSelectScreen();

    // @volatile
    // TODO make this a faction enum
    if ( selection != "" ) {
      StartSingleplayerCampaign( selection );
    }

    BeginDrawing();
    {
      ClearBackground( BLACK );
      Iron::Forge()->DrawAll();
    }
    EndDrawing();
  }

  void Scene_MPFactionSelect() {
    auto selection = UI::DrawFactionSelectScreen();

    if ( selection != "" ) {
      if ( Network::is_host ) {
        Network::Host()->SetHostFaction( selection );
        Network::Host()->SendMessageToAllActiveClients( Network::Message{
          Network::MessageID::PlayerFactionSelect,
          nlohmann::json{
            { "player_id", "player_0" },
            { "faction", selection },
          },
        } );
      } else {
        auto player_id = Network::Client()->_local_player_id;
        Network::Client()->SendMessageToHost( Network::Message{
          Network::MessageID::PlayerFactionSelect,
          nlohmann::json{
            { "player_id", player_id },
            { "faction", selection },
          },
        } );
      }

      _scene = Scene::MultiPlayerLobby;
    }


    BeginDrawing();
    {
      ClearBackground( BLACK );
      Iron::Forge()->DrawAll();
    }
    EndDrawing();
  }

  void Scene_ModalMenu() {
    CheckMenuToggle();

    switch ( UI::DrawModalMenu() ) {
      case UI::Action_ModalMenu::None:
        break;
      case UI::Action_ModalMenu::SaveGame:
        // @todo get actual user input for file name
        _campaign->save( "output" );
        break;
      case UI::Action_ModalMenu::LoadGame:
        // LoadSinglePlayerCampaign();
        break;
      case UI::Action_ModalMenu::Settings:
        break;
      case UI::Action_ModalMenu::ExitToMainMenu:
        ExitToMainMenu();
        break;
      case UI::Action_ModalMenu::ExitGame:
        ExitGame();
        return;
    }

    BeginDrawing();
    {
      Renderer::draw_map( Map::Manager()->mode );
      DrawRectangle(
        0, 0, GetScreenWidth(), GetScreenHeight(), Fade( BLACK, 0.33f )
      );
      Iron::Forge()->DrawAll();
    }
    EndDrawing();
  }

  void Scene_Campaign() {
    CheckMenuToggle();

    _campaign->UpdateOnFrame( _dt, _lag, _oncelag );

    if ( IsKeyPressed( KEY_GRAVE ) ) {
      if ( _scene == Scene::Editor ) {
        _scene = Scene::Campaign;
      } else {
        _scene = Scene::Editor;
      }
    }

    if ( _scene == Scene::Editor ) {

      auto editor_action = UI::panel();

      switch ( editor_action ) {
        case UI::EditorAction::None:
          break;
        case UI::EditorAction::GenerateMap:
          // MapSystem::Init();
          if ( _campaign )
            //@todo actualyl pass in the faction
            _campaign->start( "romans" );
          break;
      }
    }

    // 6. Draw everything
    BeginDrawing();
    {
      _campaign->Draw();
      Iron::Forge()->DrawAll();
      // Iron::Forge()->DrawDebug();
    }
    EndDrawing();
  }

  void ExitCampaignCleanup() {
    // if ( Network::is_host )
    //   Network::Host()->Delete();
    // else
    //   Network::Client()->Delete();

    if ( _campaign )
      delete _campaign;
  }
  /*=============================================================
                        End: Shared
  =============================================================*/
};

// inline void IGame::RegisterEventListeners() {
//   InterfaceEvent::event_emitter.on<InterfaceEvent::Data>(
//     [&](
//       const InterfaceEvent::Data &event, InterfaceEvent::EventEmitter &emitter
//     ) {
//       switch ( event.event_id ) {
//         /// BASIC
//         // MainMenu
//         case InterfaceEvent::ID::MainMenuHostGame:
//           HostMultiplayerLobby();
//           break;
//         case InterfaceEvent::ID::MainMenuJoinGame:
//           LookForMultiplayerLobby();
//           break;
//         case InterfaceEvent::ID::MainMenuStartGame: {
//           _single_player = true;
//           // UI::System::SwitchPage( UI::SinglePlayerLobby );
//         } break;
//         case InterfaceEvent::ID::MainMenuLoadGame:
//           LoadGame();
//           break;
//         case InterfaceEvent::ID::MainMenuExitGame:
//           ExitGame();
//           break;
//           // FactionSelect
//         case InterfaceEvent::ID::OpenFactionSelectPage:
//           // UI::System::SwitchPage( UI::FactionSelectMenu );
//           break;
//         case InterfaceEvent::ID::SinglePlayerLobbyStartGame:
//           StartSingleplayerCampaign( faction );
//           break;
//         case InterfaceEvent::ID::ModalMenuLoadGame:
//           LoadGame();
//           break;
//         case InterfaceEvent::ID::ModalMenuSaveGame:
//           SaveGame();
//           break;
//         case InterfaceEvent::ID::ModalMenuExitMain:
//           ReturnToMain();
//           break;
//         case InterfaceEvent::ID::ModalMenuExitGame:
//           ExitGame();
//           break;
//         case InterfaceEvent::ID::ModalMenuToggle:
//           ToggleModalMenu();
//           break;
//         case InterfaceEvent::ID::ReturnToMain:
//           ReturnToMain();
//           break;
//         case InterfaceEvent::ID::PlayerToggledReady: {
//           if ( Network::is_host ) {
//             Network::Host()->ToggleReady();
//           } else {
//             Network::Client()->ToggleReady();
//           }
//         } break;
//         case InterfaceEvent::ID::HostStartGame: {
//           Network::Host()->StartHostedCampaign();
//           HostStartMultiplayerCampaign();
//         } break;
//         case InterfaceEvent::ID::JoinHostedCampaign: {
//           ClientStartMultiplayerCampaign();
//         } break;
//         case InterfaceEvent::ID::JoinLobby: {
//           if ( event.msg == "lobby_entry_Conquistador's lobby" ) {
//             JoinMultiplayerLobby( event.lobby_id );
//           }
//         } break;
//         case InterfaceEvent::ID::FactionSelected: {
//           faction = event.msg;

//           if ( _single_player ) {
//             InterfaceUpdate::Update{
//               .id = InterfaceUpdate::ID::PlayerSelectedFaction,
//               .update_txt = faction,
//               .player_id = "player_0",
//             }
//               .Send();

//             // UI::System::SwitchPage( UI::SinglePlayerLobby );
//           } else {
//             std::string player_id = "";

//             if ( Network::is_host ) {
//               player_id = Network::Host()->_player_id;
//               Network::Host()->SetHostFaction( faction );
//               Network::Host()->SendMessageToAllActiveClients( Network::Message{
//                 Network::MessageID::PlayerFactionSelect,
//                 nlohmann::json{
//                   { "player_id", player_id },
//                   { "faction", faction },
//                 },
//               } );
//             } else {
//               player_id = Network::Client()->_local_player_id;
//               Network::Client()->SendMessageToHost( Network::Message{
//                 Network::MessageID::PlayerFactionSelect,
//                 nlohmann::json{
//                   { "player_id", player_id },
//                   { "faction", faction },
//                 },
//               } );
//             }

//             InterfaceUpdate::Update{
//               .id = InterfaceUpdate::ID::PlayerSelectedFaction,
//               .update_txt = faction,
//               .player_id = player_id,
//             }
//               .Send();

//             // UI::System::SwitchPage( UI::Lobby );
//           }
//         } break;

//         case InterfaceEvent::ID::ClientReceivedCommand: {
//           if ( _campaign )
//             _campaign->ConvertCommandRequest( event.msg );
//         } break;

//         // TODO(??) maybe we just need a separate queue or something
//         // for the campaign
//         case InterfaceEvent::ID::ActorSpawnSettlment:
//         case InterfaceEvent::ID::SettlementContextMilitaryTab:
//         case InterfaceEvent::ID::SettlementContextPopulationTab:
//         case InterfaceEvent::ID::SettlementContextResourcesTab:
//         case InterfaceEvent::ID::SettlementContextConstructionTab:
//         case InterfaceEvent::ID::SettlementContextConstructBuilding:
//         case InterfaceEvent::ID::SettlementContextTrainHastati: {
//           // if ( _campaign )
//           //   _campaign->ForwardEvent( event );
//         } break;

//         default:
//           printf( "Error, unregistered UI event fired\n" );
//           break;
//       }
//     }
//   );
// }

inline IGame *Game() {
  return IGame::Game();
}
