#pragma once

#include "interface/irongui/forge.hpp"
#include "interface/pages/editor.hpp"
#include "interface/pages/faction_select_menu.hpp"
#include "interface/pages/main_menu_ui.hpp"
#include "interface/pages/modal_menu_ui.hpp"
#include "interface/pages/save_games.hpp"
#include "interface/pages/singleplayer_lobby.hpp"
#include "network/client.hpp"
#include "network/host.hpp"


#include "network/network.hpp"
#include "shared/common.hpp"

#include "signals/updates.hpp"

#include "world/systems/player_system.hpp"

#include "campaign.hpp"
#include <raylib.h>

enum class Scene {
  MainMenu,
  LoadGames,
  SinglePlayerLobby,
  FactionSelect,
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
  Scene _mode = Scene::MainMenu;

  f32 _oncelag = 0.0f;
  f32 _lag = 0.0f;
  f32 _dt = 0.0f;
  const f32 _MS_PER_UPDATE = 1 / 60.0;
  const f32 _ONCE_A_SECOND = 1;

  // TODO move
  std::string faction = "";
  // TODO this should be set from networking
  // std::string player_id = "player_0";

  bool ShouldRun() {
    return !_hit_exit;
  }

  /*=============================================================

                        Begin: Singleplayer
  =============================================================*/
  void StartSingleplayerCampaign( str player_faction ) {
    _mode = Scene::Campaign;

    _campaign = new struct Campaign( true );
    _campaign->start( player_faction );
  }

  void LoadSinglePlayerCampaign( cstr file_path ) {
    _mode = Scene::Campaign;

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
    // UI::System::SwitchPage( UI::Lobby );

    InterfaceUpdate::Update{
      .id = InterfaceUpdate::ID::HostLobby,
      .player_id = Network::Host()->_player_id,
    }
      .Send();
  }

  void HostStartMultiplayerCampaign() {
    if ( _campaign )
      delete _campaign;

    _campaign = new Campaign( false );

    // UI::System::InitCampaignUI();


    PlayerSystem::HostStartMultiplayer();

    Game()->_mode = Scene::Campaign;
  }

  void ClientStartMultiplayerCampaign() {
    if ( _campaign )
      delete _campaign;

    _campaign = new struct Campaign( false );

    // UI::System::InitCampaignUI();

    PlayerSystem::ClientStartMultiplayer();

    Game()->_mode = Scene::Campaign;
  }

  void LookForMultiplayerLobby() {
    _single_player = false;
    Network::is_host = false;
    Network::Client()->Init();
    // UI::System::SwitchPage( UI::LobbyBrowser );
  }

  void JoinMultiplayerLobby( CSteamID lobby_id ) {
    InterfaceUpdate::Update{
      .id = InterfaceUpdate::ID::JoinLobby,
      .player_id = "player_0",
    }
      .Send();

    if ( Network::Client()->AttemptJoinLobby( lobby_id ) ) {
      printf( "Sending joined lobby event!\n" );
      // UI::System::SwitchPage( UI::Lobby );
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
    // UI::System::SwitchPage( UI::MainMenu );
    _mode = Scene::MainMenu;
  }


  void CheckMenuToggle() {
    if ( IsKeyPressed( KEY_CAPS_LOCK ) || IsKeyPressed( KEY_ESCAPE ) ) {
      // InterfaceEvent::event_emitter.publish( InterfaceEvent::Data{
      //   InterfaceEvent::ID::ModalMenuToggle,
      // } );
      if ( _mode == Scene::ModalMenu ) {
        _mode = Scene::Campaign;
      } else {
        _mode = Scene::ModalMenu;
      }
    }
  }

  void ExitToMainMenu() {
    _mode = Scene::MainMenu;
    ExitCampaignCleanup();
  }

  void UpdateOnFrame() {
    if ( Network::is_host ) {
      Network::Host()->Update();
    } else {
      Network::Client()->Update();
    }

    Iron::Forge()->over_any_elem = false;

    switch ( _mode ) {
      case Scene::MainMenu: {
        auto action = UI::DrawMainMenu();

        switch ( action ) {
          case UI::Action_MainMenu::StartGame:
            _mode = Scene::SinglePlayerLobby;
            break;
          case UI::Action_MainMenu::LoadGame: {
            _mode = Scene::LoadGames;
          } break;
          case UI::Action_MainMenu::HostGame:
            break;
          case UI::Action_MainMenu::JoinGame:
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
      } break;
      case Scene::LoadGames: {
        bool pressed_back = false;
        auto file_to_load = UI::load_game_menu( pressed_back );

        if ( pressed_back ) {
          _mode = Scene::MainMenu;
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
      } break;
      case Scene::SinglePlayerLobby: {
        auto action = UI::SinglePlayerLobby();

        switch ( action ) {
          case UI::Action_SinglePlayerLobby::SelectFaction:
            _mode = Scene::FactionSelect;
            break;
          case UI::Action_SinglePlayerLobby::ExitPressed:
            _mode = Scene::MainMenu;
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
      } break;

      case Scene::FactionSelect: {
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
          // Iron::Forge()->DrawDebug();
        }
        EndDrawing();
      } break;

      case Scene::ModalMenu: {
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
      } break;

      case Scene::Editor:
      case Scene::Campaign: {
        CheckMenuToggle();

        if ( _single_player ) {
          // Singleplayer Campaign
          if ( _campaign )
            _campaign->UpdateOnFrame( _dt, _lag, _oncelag );
        } else {
          // Multiplayer Campaign
          if ( _campaign )
            _campaign->UpdateOnFrame( _dt, _lag, _oncelag );
        }

        if ( IsKeyPressed( KEY_GRAVE ) ) {
          if ( _mode == Scene::Editor ) {
            _mode = Scene::Campaign;
          } else {
            _mode = Scene::Editor;
          }
        }

        if ( _mode == Scene::Editor ) {

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
      } break;
    }

    if ( !Iron::Forge()->over_any_elem ) {
      Iron::Forge()->context.hot = -1;
      Iron::Forge()->context.active = -1;
    }
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
