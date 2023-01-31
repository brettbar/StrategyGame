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

/*
========================================================
  Main game loop 
========================================================
*/

void CameraUpdate( Camera2D &, f32 );
void RunMainMenu( f32 );
void RunModalMenu();

enum class ProgramMode {
  MainMenu,
  ModalMenu,
  SinglePlayerCampaign,
  MultiPlayerCampaign,
  Editor,
};

class IGame {
  Campaign *_campaign = nullptr;
  // bool _creating_lobby = false;
  // bool _looking_for_lobby = false;
  // bool _in_lobby = false;
  // bool _is_host = false;
  bool _pending_new_campaign = false;
  bool _pending_load_campaign = false;
  bool _hit_exit = false;
  ProgramMode _mode = ProgramMode::MainMenu;

  std::string choice = "";

  IGame( IGame const & ) = delete;
  void operator=( const IGame & ) = delete;

  IGame() {}

  public:
  ~IGame() {}

  static IGame *Game() {
    static IGame instance;
    return &instance;
  }

  void DeleteCampaignInstance() {
    if ( _campaign )
      delete _campaign;
  }

  bool ShouldRun() {
    return !_hit_exit;
  }

  void StartGame() {
    printf( "pending new!!\n" );

    DeleteCampaignInstance();
    _campaign = new Campaign();

    UI::System::EnableCampaignUI();

    Global::host_player = Global::world.create();
    Global::world.emplace<Player::Component>(
      Global::host_player, Global::host_player, true, choice
    );

    Global::world.on_construct<Selected::Component>()
      .connect<&UI::System::ListenForSelect>();
    Global::world.on_destroy<Selected::Component>()
      .connect<&UI::System::ListenForDeselect>();

    Game()->_mode = ProgramMode::SinglePlayerCampaign;
    Game()->_pending_new_campaign = false;
  }

  void HostGame() {
    Network::Host()->Init();
    UI::System::SwitchPage( UI::Lobby );
  }

  void LookForGame() {
    Network::Client()->Init();
    UI::System::SwitchPage( UI::LobbyBrowser );
  }

  void JoinGame() {
    UI::System::SwitchPage( UI::Lobby );
  }

  void JoinLobby( CSteamID lobby_id ) {
    if ( Network::Client()->AttemptJoinLobby( lobby_id ) ) {
      printf( "Sending joined lobby event!\n" );
    }
  }

  void LoadGame() {
    if ( _campaign )
      delete _campaign;
    _campaign = new Campaign( "output.dat" );
    UI::System::EnableCampaignUI();

    Game()->_mode = ProgramMode::SinglePlayerCampaign;
    Game()->_pending_load_campaign = false;
  }

  void SaveGame() {
    SaveSystem::Save();
  }

  void ExitGame() {
    _hit_exit = true;
  }

  void ToggleModalMenu() {
    if ( _mode == ProgramMode::SinglePlayerCampaign ) {
      _mode = ProgramMode::ModalMenu;
      UI::System::SwitchPage( UI::ModalMenu );
    }
    else if ( _mode == ProgramMode::ModalMenu ) {
      _mode = ProgramMode::SinglePlayerCampaign;
      UI::System::SwitchPage( UI::Campaign );
    }
  }

  void ReturnToMain() {
    UI::System::SwitchPage( UI::MainMenu );
    _mode = ProgramMode::MainMenu;
  }

  // while

  void RunCampaign( f32 &dt, f32 &lag, f32 &oncelag ) {
    assert( _campaign );
    _campaign->Run( dt, lag, oncelag );
  }
};

inline IGame *Game() {
  return IGame::Game();
}

inline void RunGameLoop() {

  Network::Setup();


  // TODO these lags might need to be moved to Campaign
  f32 oncelag = 0.0f;
  f32 lag = 0.0f;
  f32 dt = 0.0f;


  while ( !WindowShouldClose() && Game()->ShouldRun() ) {
    Events::event_emitter.on<Events::ButtonClick>(
      [&]( const Events::ButtonClick &event, Events::EventEmitter &emitter ) {
        if ( event.origin == "main_menu_host_game" ) {
          Game()->HostGame();
        }
        else if ( event.origin == "main_menu_join_game" ) {
          Game()->LookForGame();
        }
        else if ( event.origin == "main_menu_start_game" ) {
          // pending_new_campaign = true;
          UI::System::SwitchPage( UI::FactonSelectMenu );
        }
        else if ( event.origin == "main_menu_load_game" ) {
          Game()->LoadGame();
        }
        else if ( event.origin == "main_menu_exit_game" ) {
          Game()->ExitGame();
        }
        else if ( event.origin == "player_select_faction" ) {
          UI::System::SwitchPage( UI::FactonSelectMenu );
        }
        else if ( event.origin == "modal_menu_load_game" ) {
          Game()->LoadGame();
        }
        else if ( event.origin == "modal_menu_save_game" ) {
          Game()->SaveGame();
        }
        else if ( event.origin == "modal_menu_exit_main" ) {
          Game()->ReturnToMain();
        }
        else if ( event.origin == "modal_menu_exit_game" ) {
          Game()->ExitGame();
        }
        else if ( event.origin == "toggle_modal_menu" ) {
          Game()->ToggleModalMenu();
        }
        else if ( event.origin == "faction_select" ) {
          choice = event.msg;
          Game()->StartGame();
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
          Game()->JoinLobby( event.lobby_id );
        }
      }
    );


    // if ( Game()->_pending_new_campaign ) {}
    // else if ( Game()->_pending_load_campaign ) {}
    // if ( pstate->creating_lobby && pstate->is_host ) {
    //   pstate->creating_lobby = false;
    // }
    // if ( pstate->looking_for_lobby && !pstate->is_host ) {
    //   // UI::EnableLobby();
    //   pstate->looking_for_lobby = false;
    // }

    // if ( pstate->in_lobby && !pstate->is_host ) {
    //   pstate->in_lobby = false;
    // }

    if ( pstate->is_host ) {
      // Network::Host()->SendPing();
      Network::Host()->CheckForMessages();
      Network::Host()->EvaluateMessages();
    }
    else {
      Network::Client()->CheckForMessage();
    }


    SteamAPI_RunCallbacks();

    switch ( pstate->mode ) {
      case ProgramMode::MainMenu:
        RunMainMenu( dt );
        break;

      case ProgramMode::ModalMenu:
        RunModalMenu();
        break;

      case ProgramMode::SinglePlayerCampaign:
        Game()->RunCampaign( dt, lag, oncelag );
        break;
    }
  }

  // TODO move this into the Exit function

  if ( Network::is_host )
    Network::Host()->Delete();
  else
    Network::Client()->Delete();

  Game()->DeleteCampaignInstance();

  delete Game();
}


inline void RunMainMenu( f32 dt ) {
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

inline void RunModalMenu() {
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

// TODO A bit weird to put this here instead of in campaign
inline void Campaign::Run( f32 &dt, f32 &lag, f32 &oncelag ) {
  // 1. Update Time
  dt = GetFrameTime();
  lag += dt;
  oncelag += dt;

  // 2. Check for Input
  Input::CheckMenuToggle();
  Input::Handle();

  // 3. Process all commands
  Commands::Manager()->FireAll();

  // 5. Run all Updates
  {
    // Update 60 times a second
    while ( lag >= _MS_PER_UPDATE ) {
      Update60TPS();
      lag -= _MS_PER_UPDATE;
    }

    // Update once per second
    while ( oncelag >= _ONCE_A_SECOND * ( 1 / Global::state.timeScale ) ) {
      Update1TPS();
      oncelag = 0.0f;
    }

    // Update once per frame
    UpdateOnFrame();

    // Update Camera
    CameraUpdate( Global::state.camera, dt );
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

inline void CameraUpdate( Camera2D &camera, f32 dt ) {
  f32 cameraSpeed = 500.0f;
  // Vector2 screenCenter = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  // Vector2 target = GetScreenToWorld2D(screenCenter, camera);
  // PrintVec2(target);

  // camera.offset = target;

  if ( IsKeyDown( KEY_D ) )
    camera.target.x += dt * cameraSpeed / camera.zoom;
  if ( IsKeyDown( KEY_A ) )
    camera.target.x -= dt * cameraSpeed / camera.zoom;
  if ( IsKeyDown( KEY_W ) )
    camera.target.y -= dt * cameraSpeed / camera.zoom;
  if ( IsKeyDown( KEY_S ) )
    camera.target.y += dt * cameraSpeed / camera.zoom;

  if ( IsKeyDown( KEY_Z ) )
    camera.zoom -= 0.05f;
  if ( IsKeyDown( KEY_X ) )
    camera.zoom += 0.05f;

  f32 mouseWheelDelta = GetMouseWheelMove();

  camera.zoom += ( mouseWheelDelta * 0.2f );
  if ( camera.zoom > 8.0f )
    camera.zoom = 8.0f;
  else if ( camera.zoom < 0.08f )
    camera.zoom = 0.08f;

  camera.offset = { (f32) GetScreenWidth() / 2, (f32) GetScreenHeight() / 2 };
}
