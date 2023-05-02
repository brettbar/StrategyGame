#pragma once

#include "interface/ui_manager.hpp"
#include "network/client.hpp"
#include "network/host.hpp"

#include "network/network.hpp"
#include "shared/common.hpp"
#include "shared/save.hpp"

#include "renderer/renderer.hpp"

#include "interface/input.hpp"

#include "campaign.hpp"
#include "signals/updates.hpp"
#include "world/systems/selection_system.hpp"
#include <raylib.h>

enum class ProgramMode
{
  MainMenu,
  ModalMenu,
  Campaign,
  Editor,
};

class IGame
{
  public:
  static IGame *Game()
  {
    static IGame instance;
    return &instance;
  }

  void MainLoop()
  {
    RegisterEventListeners();

    while ( !WindowShouldClose() && ShouldRun() )
    {
      SteamAPI_RunCallbacks();

      // 1. Update Time
      _dt = GetFrameTime();
      _lag += _dt;
      _oncelag += _dt;

      // 5. Run all Updates
      {
        // Update 60 times a second
        while ( _lag >= _MS_PER_UPDATE )
        {
          Update60TPS();
          _lag -= _MS_PER_UPDATE;
        }

        // Update once per second
        while ( _oncelag >= _ONCE_A_SECOND )
        {
          if ( Network::is_host )
          {
            // Network::Host()->PingAllActiveClients();
          }
          _oncelag = 0.0f;
        }

        // Update once per second but modified by timeScale
        // TODO? Do we want to fold this timescale business into campaign itself
        // Since we dont care about timescale in other program_modes
        while ( _oncelag >= _ONCE_A_SECOND * ( 1 / Global::state.timeScale ) )
        {
          if ( _campaign )
          {
            _campaign->Update1TPS();
          }
          _oncelag = 0.0f;
        }

        // Update once per frame
        UpdateOnFrame();

        // Update Camera
        CameraUpdate( Global::state.camera, _dt );
      }
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
  const f32 _MS_PER_UPDATE = 1 / 60.0;
  const f32 _ONCE_A_SECOND = 1;

  // TODO move
  std::string faction = "";
  // TODO this should be set from networking
  // std::string player_id = "player_0";

  IGame( IGame const & ) = delete;
  void operator=( const IGame & ) = delete;

  IGame() {}
  ~IGame() {}

  bool ShouldRun()
  {
    return !_hit_exit;
  }

  /*=============================================================

                        Begin: Singleplayer
  =============================================================*/
  void StartCampaign( std::string player_faction )
  {
    printf( "pending new!!\n" );

    if ( _campaign )
      delete _campaign;

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
  void HostMultiplayerCampaign()
  {
    _single_player = false;
    Network::is_host = true;
    Network::Host()->Init();
    UI::System::SwitchPage( UI::Lobby );

    // InterfaceUpdate::Text( InterfaceUpdate::ID::HostLobby )
    //   .SetText( "Start Game" )
    //   .build()
    //   .send();
    InterfaceUpdate::Text( InterfaceUpdate::ID::HostLobby )
      .SetTarget( Network::Host()->_player_id + "_label" )
      .SetText( Network::Host()->_player_id )
      .send();

    InterfaceUpdate::Clickable( InterfaceUpdate::ID::HostLobby, true )
      .SetTarget( Network::Host()->_player_id + "_faction_selection" )
      .send();
    InterfaceUpdate::Background( InterfaceUpdate::ID::HostLobby, GREEN )
      .SetTarget( Network::Host()->_player_id + "_faction_selection" )
      .send();
  }

  void StartMultiplayerCampaign() {}

  void LookForMultiplayerCampaign()
  {
    _single_player = false;
    Network::is_host = false;
    Network::Client()->Init();
    UI::System::SwitchPage( UI::LobbyBrowser );
  }

  void JoinMultiplayerLobby( CSteamID lobby_id )
  {
    if ( Network::Client()->AttemptJoinLobby( lobby_id ) )
    {
      printf( "Sending joined lobby event!\n" );
      UI::System::SwitchPage( UI::Lobby );


      // InterfaceUpdate::Text( InterfaceUpdate::ID::JoinLobby )
      //   .SetText( "Ready Up" )
      //   .build()
      //   .send();
    }
  }

  /*=============================================================
                        End: Multiplayer
  =============================================================*/

  /*=============================================================
                        Begin: Shared
  =============================================================*/
  void LoadGame()
  {
    if ( _campaign )
      delete _campaign;
    _campaign = new Campaign( "output.dat" );
    UI::System::EnableCampaignUI();

    Game()->_mode = ProgramMode::Campaign;
  }

  void SaveGame()
  {
    SaveSystem::Save();
  }

  void ExitGame()
  {
    _hit_exit = true;
  }

  void ToggleModalMenu()
  {
    if ( _mode == ProgramMode::Campaign )
    {
      _mode = ProgramMode::ModalMenu;
      UI::System::SwitchPage( UI::ModalMenu );
    }
    else if ( _mode == ProgramMode::ModalMenu )
    {
      _mode = ProgramMode::Campaign;
      UI::System::EnableCampaignUI();
    }
  }

  void ReturnToMain()
  {
    UI::System::SwitchPage( UI::MainMenu );
    _mode = ProgramMode::MainMenu;
  }
  /*=============================================================
                        End: Shared
  =============================================================*/


  void UpdateOnFrame()
  {
    if ( Network::is_host )
    {
      Network::Host()->Update();
    }
    else
    {
      Network::Client()->Update();
    }

    switch ( _mode )
    {
      case ProgramMode::MainMenu:
      {
        UI::System::UpdateOnFrame();

        CameraUpdate( Global::state.camera, _dt );

        BeginDrawing();
        {
          ClearBackground( BLACK );
          Renderer::DrawUI();
        }
        EndDrawing();
      }
      break;

      case ProgramMode::ModalMenu:
      {
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
      break;

      case ProgramMode::Campaign:
      {
        if ( _single_player )
        {
          // Singleplayer Campaign
          if ( _campaign )
            _campaign->UpdateOnFrame( _dt, _lag, _oncelag );
        }
        else
        {
          // Multiplayer Campaign
          if ( _campaign )
            _campaign->UpdateOnFrame( _dt, _lag, _oncelag );
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
      break;

      case ProgramMode::Editor:
        break;
    }
  }

  void Update60TPS()
  {
    if ( _campaign )
    {
      _campaign->Update60TPS();
    }
  }


  void ExitGameLoopCleanup()
  {
    if ( Network::is_host )
      Network::Host()->Delete();
    else
      Network::Client()->Delete();

    if ( _campaign )
      delete _campaign;

    delete this;
  }

  void RegisterEventListeners();
};

inline void IGame::RegisterEventListeners()
{
  InterfaceEvent::event_emitter.on<InterfaceEvent::Data>(
    [&](
      const InterfaceEvent::Data &event, InterfaceEvent::EventEmitter &emitter
    ) {
      switch ( event.event_id )
      {
        /// BASIC
        // MainMenu
        case InterfaceEvent::ID::MainMenuHostGame:
          HostMultiplayerCampaign();
          break;
        case InterfaceEvent::ID::MainMenuJoinGame:
          LookForMultiplayerCampaign();
          break;
        case InterfaceEvent::ID::MainMenuStartGame:
        {
          _single_player = true;
          UI::System::SwitchPage( UI::FactionSelectMenu );
        }
        break;
        case InterfaceEvent::ID::MainMenuLoadGame:
          LoadGame();
          break;
        case InterfaceEvent::ID::MainMenuExitGame:
          ExitGame();
          break;
          // FactionSelect
        case InterfaceEvent::ID::OpenFactionSelectPage:
          UI::System::SwitchPage( UI::FactionSelectMenu );
          break;
        case InterfaceEvent::ID::SinglePlayerLobbyStartGame:
          StartCampaign( faction );
          break;
        case InterfaceEvent::ID::ModalMenuLoadGame:
          LoadGame();
          break;
        case InterfaceEvent::ID::ModalMenuSaveGame:
          SaveGame();
          break;
        case InterfaceEvent::ID::ModalMenuExitMain:
          ReturnToMain();
          break;
        case InterfaceEvent::ID::ModalMenuExitGame:
          ExitGame();
          break;
        case InterfaceEvent::ID::ModalMenuToggle:
          ToggleModalMenu();
          break;
        case InterfaceEvent::ID::ReturnToMain:
          ReturnToMain();
          break;
        case InterfaceEvent::ID::PlayerToggledReady:
        {
          if ( Network::is_host )
          {
            Network::Host()->ToggleReady();
          }
          else
          {
            Network::Client()->ToggleReady();
          }
        }
        break;
        case InterfaceEvent::ID::HostStartGame:
        {
          StartCampaign( faction );
        }
        break;
        case InterfaceEvent::ID::JoinLobby:
          if ( event.msg == "lobby_entry_Conquistador's lobby" )
          {
            JoinMultiplayerLobby( event.lobby_id );
          }
          break;

        /// STRING
        case InterfaceEvent::ID::FactionSelected:
        {
          printf(
            "In listener, %s %s\n",
            InterfaceEvent::IDString[(u32) InterfaceEvent::ID::FactionSelected],
            event.msg.c_str()
          );

          faction = event.msg;

          if ( _single_player )
          {
            InterfaceUpdate::Text( InterfaceUpdate::ID::FactionSelected )
              .SetText( faction )
              .send();

            InterfaceUpdate::Background(
              InterfaceUpdate::ID::FactionSelected,
              GetPrimaryFactionColor( faction )
            )
              .send();

            UI::System::SwitchPage( UI::SinglePlayerLobby );
          }
          else
          {
            std::string player_id = "";

            if ( Network::is_host )
            {
              player_id = Network::Host()->_player_id;
              Network::Host()->SetHostFaction( faction );
              Network::Host()->SendMessageToAllClients( Network::Message{
                Network::MessageID::PlayerFactionSelect,
                nlohmann::json{
                  { "player_id", player_id },
                  { "faction", faction },
                },
              } );
            }
            else
            {
              player_id = Network::Client()->_local_player_id;
              Network::Client()->SendMessageToHost( Network::Message{
                Network::MessageID::PlayerFactionSelect,
                nlohmann::json{
                  { "player_id", player_id },
                  { "faction", faction },
                },
              } );
            }

            InterfaceUpdate::Text( InterfaceUpdate::ID::FactionSelected )
              .SetTarget( player_id + "_select_faction" )
              .SetText( faction )
              .send();

            InterfaceUpdate::Background(
              InterfaceUpdate::ID::FactionSelected,
              GetPrimaryFactionColor( faction )
            )
              .SetTarget( player_id + "_select_faction" )
              .send();

            UI::System::SwitchPage( UI::Lobby );
          }
        }
        break;

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
}

inline IGame *Game()
{
  return IGame::Game();
}
