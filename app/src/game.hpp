#pragma once

#include "interface/ui_system.hpp"
#include "network/client.hpp"
#include "network/host.hpp"

#include "shared/common.hpp"
#include "shared/events.hpp"
#include "shared/save.hpp"

#include "renderer/renderer.hpp"

#include "interface/input.hpp"

#include "campaign.hpp"

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
  Campaign,
  Editor,
};

struct ProgramState {
  bool creating_lobby = false;
  bool joining_lobby = false;
  bool pending_new_campaign = false;
  bool pending_load_campaign = false;
  bool hit_exit = false;
  bool is_host = false;
  ProgramMode mode = ProgramMode::MainMenu;
};

inline void RunGameLoop() {
  Campaign *campaign = nullptr;
  ProgramState *pstate = new ProgramState();

  Network::Setup();


  // TODO these lags might need to be moved to Campaign
  f32 oncelag = 0.0f;
  f32 lag = 0.0f;
  f32 dt = 0.0f;

  std::string choice = "";

  while ( !WindowShouldClose() && !pstate->hit_exit ) {
    Events::event_emitter.on<Events::UIEvent>(
      [&]( const Events::UIEvent &event, Events::EventEmitter &emitter ) {
        if ( event.msg == "main_menu_host_game" ) {
          pstate->creating_lobby = true;
          // TODO make it so you can host an existing game
          // pstate->pending_new_campaign = true;
          pstate->is_host = true;
        }
        else if ( event.msg == "main_menu_join_game" ) {
          pstate->joining_lobby = true;
          // pstate->pending_new_campaign = true;
        }
        else if ( event.msg == "main_menu_start_game" ) {
          // pending_new_campaign = true;
          UI::EnableFactionSelectMenuUI();
        }
        else if ( event.msg == "main_menu_load_game" ) {
          pstate->pending_load_campaign = true;
        }
        else if ( event.msg == "main_menu_exit_game" ) {
          pstate->hit_exit = true;
        }
        else if ( event.msg == "modal_menu_load_game" ) {
          pstate->pending_load_campaign = true;
        }
        else if ( event.msg == "modal_menu_save_game" ) {
          SaveSystem::Save();
        }
        else if ( event.msg == "modal_menu_exit_main" ) {
          UI::EnableMainMenuUI();
          pstate->mode = ProgramMode::MainMenu;
        }
        else if ( event.msg == "toggle_modal_menu" ) {
          if ( pstate->mode == ProgramMode::Campaign ) {
            pstate->mode = ProgramMode::ModalMenu;
            UI::EnableModalMenuUI();
          }
          else if ( pstate->mode == ProgramMode::ModalMenu ) {
            pstate->mode = ProgramMode::Campaign;
            UI::EnableCampaignUI();
          }
        }
        // TODO refactor this to not be so repetitive
        else if ( event.msg == "faction_select_romans" ) {
          printf( "faction_select_romans \n" );
          pstate->pending_new_campaign = true;
          choice = "romans";
        }
        else if ( event.msg == "faction_select_greeks" ) {
          pstate->pending_new_campaign = true;
          choice = "greeks";
        }
        else if ( event.msg == "faction_select_celts" ) {
          pstate->pending_new_campaign = true;
          choice = "celts";
        }
        else if ( event.msg == "faction_select_punics" ) {
          pstate->pending_new_campaign = true;
          choice = "punics";
        }
        else if ( event.msg == "faction_select_persians" ) {
          pstate->pending_new_campaign = true;
          choice = "persians";
        }
        else if ( event.msg == "faction_select_scythians" ) {
          pstate->pending_new_campaign = true;
          choice = "scythians";
        }
        else if ( event.msg == "faction_select_germans" ) {
          pstate->pending_new_campaign = true;
          choice = "germans";
        }
      }
    );

    if ( pstate->pending_new_campaign ) {
      printf( "pending new!!\n" );
      if ( campaign )
        delete campaign;
      campaign = new Campaign();

      Global::host_player = Global::world.create();
      Global::world.emplace<Player::Component>(
        Global::host_player, Global::host_player, true, choice
      );

      pstate->mode = ProgramMode::Campaign;
      pstate->pending_new_campaign = false;
    }
    else if ( pstate->pending_load_campaign ) {
      if ( campaign )
        delete campaign;
      campaign = new Campaign( "output.dat" );
      pstate->mode = ProgramMode::Campaign;
      pstate->pending_load_campaign = false;
    }

    if ( pstate->creating_lobby && pstate->is_host ) {
      UI::EnableLobby();
      Network::Host()->Init();
      pstate->creating_lobby = false;
    }

    if ( pstate->joining_lobby && !pstate->is_host ) {
      UI::EnableLobby();
      Network::Client()->Init();
      pstate->joining_lobby = false;
    }

    if ( pstate->is_host ) {
      Network::Host()->CheckForMessages();
    }
    else {
      Network::Client()->CheckForMessages();
    }


    SteamAPI_RunCallbacks();

    switch ( pstate->mode ) {
      case ProgramMode::MainMenu:
        RunMainMenu( dt );
        break;

      case ProgramMode::ModalMenu:
        RunModalMenu();
        break;

      case ProgramMode::Campaign:
        assert( campaign );
        campaign->Run( dt, lag, oncelag );
        break;
    }
  }

  // TODO move this into the Exit function
  // Delete host and client
  // if ( host )
  //   delete host;

  Network::Host()->Delete();
  Network::Client()->Delete();

  // if ( client )
  //   delete client;

  if ( campaign )
    delete campaign;
  if ( pstate )
    delete pstate;
}


inline void RunMainMenu( f32 dt ) {
  // Input::Handle();

  UI::UpdateOnFrame();

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

  UI::UpdateOnFrame();

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
