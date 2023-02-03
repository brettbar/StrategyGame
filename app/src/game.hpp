#pragma once

#include "interface/ui_manager.hpp"
#include "network/client.hpp"
#include "network/host.hpp"

#include "shared/common.hpp"
#include "shared/events.hpp"
#include "shared/save.hpp"

#include "renderer/renderer.hpp"

#include "interface/input.hpp"

#include "campaign.hpp"
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
    while ( !WindowShouldClose() && ShouldRun() ) {
      CheckForEvents();

      UI::System::CheckForMessages();

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
  bool _is_host = false;
  bool _hit_exit = false;
  ProgramMode _mode = ProgramMode::MainMenu;

  f32 _oncelag = 0.0f;
  f32 _lag = 0.0f;
  f32 _dt = 0.0f;

  // TODO move
  std::string faction_str = "";

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
  /*=============================================================
                        End: Singleplayer
  =============================================================*/


  /*=============================================================
                        Begin: Multiplayer
  =============================================================*/
  void HostMultiplayerCampaign() {
    _single_player = false;
    _is_host = true;
    Network::Host()->Init();
    UI::System::SwitchPage( UI::Lobby );
  }

  void StartMultiplayerCampaign() {}

  void LookForMultiplayerCampaign() {
    _single_player = false;
    _is_host = false;
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
    if ( _is_host ) {
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
      UI::System::SwitchPage( UI::Campaign );
    }
  }

  void ReturnToMain() {
    UI::System::SwitchPage( UI::MainMenu );
    _mode = ProgramMode::MainMenu;
  }
  /*=============================================================
                        End: Shared
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

    Global::world.on_construct<Selected::Component>()
      .connect<&UI::System::ListenForSelect>();
    Global::world.on_destroy<Selected::Component>()
      .connect<&UI::System::ListenForDeselect>();

    Game()->_mode = ProgramMode::Campaign;
  }

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
        }
      } break;

      case ProgramMode::Editor:
        break;
    }
  }

  void RunMainMenu( f32 dt ) {
    // Input::Handle();

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


  void CheckForEvents() {
    Events::event_emitter.on<Events::ButtonClick>(
      [&]( const Events::ButtonClick &event, Events::EventEmitter &emitter ) {
        if ( event.origin == "main_menu_host_game" ) {
          HostMultiplayerCampaign();
        }
        else if ( event.origin == "main_menu_join_game" ) {
          LookForMultiplayerCampaign();
        }
        else if ( event.origin == "main_menu_start_game" ) {
          _single_player = true;
          UI::System::SwitchPage( UI::FactionSelectMenu );
        }
        else if ( event.origin == "main_menu_load_game" ) {
          LoadGame();
        }
        else if ( event.origin == "main_menu_exit_game" ) {
          ExitGame();
        }
        else if ( event.origin == "player_select_faction" ) {
          UI::System::SwitchPage( UI::FactionSelectMenu );
        }
        else if ( event.origin == "singleplayer_faction_label" ) {
          UI::System::SwitchPage( UI::FactionSelectMenu );
        }
        else if ( event.origin == "singleplayer_lobby_start_game" ) {
          StartCampaign( faction_str );
        }
        else if ( event.origin == "modal_menu_load_game" ) {
          LoadGame();
        }
        else if ( event.origin == "modal_menu_save_game" ) {
          SaveGame();
        }
        else if ( event.origin == "modal_menu_exit_main" ) {
          ReturnToMain();
        }
        else if ( event.origin == "modal_menu_exit_game" ) {
          ExitGame();
        }
        else if ( event.origin == "toggle_modal_menu" ) {
          ToggleModalMenu();
        }
        else if ( event.origin == "faction_select" ) {
          // StartCampaign( event.msg );
          if ( _single_player ) {
            UI::System::SwitchPage( UI::SinglePlayerLobby );
            faction_str = event.msg;

            Messages::message_emitter.publish( Messages::FactionSelected{
              { "singleplayer_faction_selected", event.msg },
              [&]() -> Color {
                if ( event.msg == "romans" )
                  return RED;
                if ( event.msg == "greeks" )
                  return BLUE;
                if ( event.msg == "celts" )
                  return GREEN;
                if ( event.msg == "punics" )
                  return PURPLE;
                if ( event.msg == "germans" )
                  return GRAY;
                if ( event.msg == "scythians" )
                  return PINK;
                if ( event.msg == "persians" )
                  return ORANGE;
                else
                  return BLACK;
              }(),
            } );
          }
          else {
            UI::System::SwitchPage( UI::Lobby );
          }
        }
        else {
          printf(
            "Error, unregistered UI event fired: %s\n", event.origin.c_str()
          );
        }
      }
    );

    Events::event_emitter.on<Events::JoinLobby>(
      [&]( const Events::JoinLobby &event, Events::EventEmitter &emitter ) {
        printf( "JoinLobby!!! origin %s\n", event.origin.c_str() );

        if ( event.origin == "lobby_entry_Conquistador's lobby" ) {
          JoinMultiplayerLobby( event.lobby_id );
        }
      }
    );
  }

  void ExitGameLoopCleanup() {
    if ( Network::is_host )
      Network::Host()->Delete();
    else
      Network::Client()->Delete();

    DeleteCampaignInstance();

    delete this;
  }
};

inline IGame *Game() {
  return IGame::Game();
}
