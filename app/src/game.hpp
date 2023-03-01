#pragma once

#include "interface/ui_manager.hpp"
#include "network/client.hpp"
#include "network/host.hpp"

#include "shared/common.hpp"
#include "shared/save.hpp"

#include "renderer/renderer.hpp"

#include "interface/input.hpp"

#include "campaign.hpp"
#include "world/systems/selection_system.hpp"
#include <raylib.h>

enum class ProgramMode {
  MainMenu,
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
    RegisterEventListeners();

    while ( !WindowShouldClose() && ShouldRun() ) {

      HandleMessages();

      SteamAPI_RunCallbacks();

      RunFrame();
    }

    ExitGameLoopCleanup();
  }

  private:
  Campaign *_campaign = nullptr;
  bool _single_player = true;
  // bool _creating_lobby = false;
  // bool _looking_for_lobby = false;
  // bool _in_lobby = false;
  bool _hit_exit = false;
  ProgramMode _mode = ProgramMode::MainMenu;

  f32 _oncelag = 0.0f;
  f32 _lag = 0.0f;
  f32 _dt = 0.0f;

  // TODO move
  std::string faction = "";

  IGame( IGame const & ) = delete;
  void operator=( const IGame & ) = delete;

  IGame() {}
  ~IGame() {}

  bool ShouldRun() {
    return !_hit_exit;
  }

  /*=============================================================
                        Begin: Singleplayer
  =============================================================*/
  void StartCampaign( std::string player_faction ) {
    printf( "pending new!!\n" );

    DeleteCampaignInstance();
    _campaign = new Campaign();

    UI::System::EnableCampaignUI();

    Global::host_player = Global::world.create();
    Global::world.emplace<Player::Component>(
      Global::host_player, Global::host_player, true, player_faction
    );

    SelectionSystem::Start();

    Game()->_mode = ProgramMode::Campaign;
  }
  /*=============================================================
                        End: Singleplayer
  =============================================================*/


  /*=============================================================
                        Begin: Multiplayer
  =============================================================*/
  void HostMultiplayerCampaign() {
    _single_player = false;
    Network::is_host = true;
    Network::Host()->Init();
    UI::System::SwitchPage( UI::Lobby );
  }

  void StartMultiplayerCampaign() {}

  void LookForMultiplayerCampaign() {
    _single_player = false;
    Network::is_host = false;
    Network::Client()->Init();
    UI::System::SwitchPage( UI::LobbyBrowser );
  }

  void JoinMultiplayerLobby( CSteamID lobby_id ) {
    if ( Network::Client()->AttemptJoinLobby( lobby_id ) ) {
      printf( "Sending joined lobby event!\n" );
      UI::System::SwitchPage( UI::Lobby );
    }
  }

  void HandleMessages() {
    if ( Network::is_host ) {
      // Network::Host()->SendPing();
      Network::Host()->CheckForMessages();
      Network::Host()->EvaluateMessages();
    }
    else {
      Network::Client()->CheckForMessage();
    }
  }
  /*=============================================================
                        End: Multiplayer
  =============================================================*/

  /*=============================================================
                        Begin: Shared
  =============================================================*/
  void LoadGame() {
    if ( _campaign )
      delete _campaign;
    _campaign = new Campaign( "output.dat" );
    UI::System::EnableCampaignUI();

    Game()->_mode = ProgramMode::Campaign;
  }

  void SaveGame() {
    SaveSystem::Save();
  }

  void ExitGame() {
    _hit_exit = true;
  }

  void ToggleModalMenu() {
    if ( _mode == ProgramMode::Campaign ) {
      _mode = ProgramMode::ModalMenu;
      UI::System::SwitchPage( UI::ModalMenu );
    }
    else if ( _mode == ProgramMode::ModalMenu ) {
      _mode = ProgramMode::Campaign;
      UI::System::EnableCampaignUI();
    }
  }

  void ReturnToMain() {
    UI::System::SwitchPage( UI::MainMenu );
    _mode = ProgramMode::MainMenu;
  }
  /*=============================================================
                        End: Shared
  =============================================================*/


  void DeleteCampaignInstance() {
    if ( _campaign )
      delete _campaign;
  }

  void RunFrame() {
    switch ( _mode ) {
      case ProgramMode::MainMenu:
        RunMainMenu( _dt );
        break;

      case ProgramMode::ModalMenu:
        RunModalMenu();
        break;

      case ProgramMode::Campaign: {
        if ( _single_player ) {
          // Singleplayer Campaign
          if ( _campaign )
            _campaign->Run( _dt, _lag, _oncelag );
        }
        else {
          // Multiplayer Campaign
          if ( _campaign )
            _campaign->Run( _dt, _lag, _oncelag );
        }
      } break;

      case ProgramMode::Editor:
        break;
    }
  }

  void RunMainMenu( f32 dt ) {
    UI::System::UpdateOnFrame();

    CameraUpdate( Global::state.camera, dt );

    BeginDrawing();
    {
      ClearBackground( BLACK );
      Renderer::DrawUI();
    }
    EndDrawing();
  }

  void RunModalMenu() {
    Input::CheckMenuToggle();

    UI::System::UpdateOnFrame();

    BeginDrawing();
    {
      Renderer::Draw( Global::texture_cache );
      DrawRectangle(
        0, 0, GetScreenWidth(), GetScreenHeight(), Fade( BLACK, 0.33f )
      );
      Renderer::DrawUI();
    }
    EndDrawing();
  }


  void ExitGameLoopCleanup() {
    if ( Network::is_host )
      Network::Host()->Delete();
    else
      Network::Client()->Delete();

    DeleteCampaignInstance();

    delete this;
  }

  void RegisterEventListeners();
};

inline void IGame::RegisterEventListeners() {
  Events::event_emitter.on<Events::EventUnion>(
    [&]( const Events::EventUnion &event, Events::EventEmitter &emitter ) {
      switch ( event.id ) {
        /// BASIC
        // MainMenu
        case Events::ID::MainMenuHostGame: {
          HostMultiplayerCampaign();
          Messages::dispatcher.enqueue( Messages::UpdateText{
            Messages::ID::HostLobby,
            "Start Game",
          } );
        } break;
        case Events::ID::MainMenuJoinGame: {
          LookForMultiplayerCampaign();
          Messages::dispatcher.enqueue( Messages::UpdateText{
            Messages::ID::JoinLobby,
            "Ready Up",
          } );
        } break;
        case Events::ID::MainMenuStartGame: {
          _single_player = true;
          UI::System::SwitchPage( UI::FactionSelectMenu );
        } break;
        case Events::ID::MainMenuLoadGame:
          LoadGame();
          break;
        case Events::ID::MainMenuExitGame:
          ExitGame();
          break;
          // FactionSelect
        case Events::ID::OpenFactionSelectPage:
          UI::System::SwitchPage( UI::FactionSelectMenu );
          break;
        case Events::ID::SinglePlayerLobbyStartGame:
          StartCampaign( faction );
          break;
        case Events::ID::ModalMenuLoadGame:
          LoadGame();
          break;
        case Events::ID::ModalMenuSaveGame:
          SaveGame();
          break;
        case Events::ID::ModalMenuExitMain:
          ReturnToMain();
          break;
        case Events::ID::ModalMenuExitGame:
          ExitGame();
          break;
        case Events::ID::ModalMenuToggle:
          ToggleModalMenu();
          break;
        case Events::ID::ReturnToMain:
          ReturnToMain();
          break;
        case Events::ID::ReadyUp:
          StartCampaign( faction );
          break;
        case Events::ID::JoinLobby:
          if ( event.msg == "lobby_entry_Conquistador's lobby" ) {
            JoinMultiplayerLobby( event.lobby_id );
          }
          break;

        /// STRING
        case Events::ID::FactionSelected: {
          printf(
            "In listener, %s %s\n",
            Events::IDString[(u32) Events::ID::FactionSelected],
            event.msg.c_str()
          );

          faction = event.msg;

          Messages::dispatcher.enqueue( Messages::UpdateText{
            Messages::ID::FactionSelected,
            faction,
          } );
          Messages::dispatcher.enqueue( Messages::UpdateBackground{
            Messages::ID::FactionSelected,
            // TODO replace with json stuff
            [&]() -> Color {
              if ( faction == "romans" )
                return RED;
              if ( faction == "greeks" )
                return BLUE;
              if ( faction == "celts" )
                return GREEN;
              if ( faction == "punics" )
                return PURPLE;
              if ( faction == "germans" )
                return GRAY;
              if ( faction == "scythians" )
                return PINK;
              if ( faction == "persians" )
                return ORANGE;
              else
                return BLACK;
            }(),
          } );

          UI::System::SwitchPage( UI::SinglePlayerLobby );

        } break;
        case Events::ID::MPFactionSelected: {
          faction = event.msg;

          Messages::dispatcher.enqueue( Messages::UpdateText{
            Messages::ID::FactionSelected,
            faction,
          } );

          Messages::dispatcher.enqueue( Messages::UpdateBackground{
            Messages::ID::FactionSelected,
            // TODO replace with json stuff
            [&]() -> Color {
              if ( faction == "romans" )
                return RED;
              if ( faction == "greeks" )
                return BLUE;
              if ( faction == "celts" )
                return GREEN;
              if ( faction == "punics" )
                return PURPLE;
              if ( faction == "germans" )
                return GRAY;
              if ( faction == "scythians" )
                return PINK;
              if ( faction == "persians" )
                return ORANGE;
              else
                return BLACK;
            }(),
          } );

          UI::System::SwitchPage( UI::Lobby );
        } break;

        default:
          printf( "Error, unregistered UI event fired\n" );
          break;
      }
    }
  );


  // Events::event_emitter.on<Events::ButtonClick>(
  //   [&]( const Events::ButtonClick &event, Events::EventEmitter &emitter ) {
  //     // else if ( event.origin_id == "faction_select" ) {
  //     //   //// StartCampaign( event.msg );
  //     // }
  //     if ( event.origin_id == "faction_selected" ) {}
  //     else if ( event.origin_id == "mp_faction_select" ) {
  //       UI::System::SwitchPage( UI::FactionSelectMenu );
  //     }
  //     else if ( event.origin_id == "actor_spawn_settlement" ) {
  //       printf(
  //         "In listener, %s %s\n", event.origin_id.c_str(), event.msg.c_str()
  //       );

  //       SettlementSystem::SpawnSettlement();
  //     }
  //   }
  // );

  // Events::event_emitter.on<Events::JoinLobby>(
  //   [&]( const Events::JoinLobby &event, Events::EventEmitter &emitter ) {
  //     printf( "JoinLobby!!! origin %s\n", event.origin_id.c_str() );

  //   }
  // );
}

inline IGame *Game() {
  return IGame::Game();
}
