#pragma once

#include "network/client.hpp"
#include "network/host.hpp"

#include "shared/common.hpp"
#include "shared/events.hpp"
#include "shared/save.hpp"

#include "renderer/renderer.hpp"

#include "interface/input.hpp"

#include "campaign.hpp"


void RunMainMenu( f32 );
void RunModalMenu();

void CameraUpdate( Camera2D &, f32 );
void Exit( TextureCache & );


/*
========================================================
  Main game loop 
========================================================
*/

enum class ProgramMode {
  MainMenu,
  ModalMenu,
  Campaign,
  Editor,
};

inline void RunGameLoop() {
  Campaign *campaign = nullptr;

  Network::Setup();

  bool creating_lobby = false;
  bool joining_lobby = false;
  bool pending_new_campaign = false;
  bool pending_load_campaign = false;
  bool hit_exit = false;
  bool is_host = false;

  // TODO these lags might need to be moved to Campaign
  f32 oncelag = 0.0f;
  f32 lag = 0.0f;
  f32 dt = 0.0f;

  ProgramMode program_mode = ProgramMode::MainMenu;

  while ( !WindowShouldClose() && !hit_exit ) {
    Events::event_emitter.on<Events::UIEvent>(
      [&]( const Events::UIEvent &event, Events::EventEmitter &emitter ) {
        if ( event.msg == "main_menu_host_game" ) {
          creating_lobby = true;
          // TODO make it so you can host an existing game
          pending_new_campaign = true;
          is_host = true;
        }
        else if ( event.msg == "main_menu_join_game" ) {
          joining_lobby = true;
          pending_new_campaign = true;
        }
        else if ( event.msg == "main_menu_start_game" ) {
          pending_new_campaign = true;
        }
        else if ( event.msg == "main_menu_load_game" ) {
          pending_load_campaign = true;
        }
        else if ( event.msg == "main_menu_exit_game" ) {
          hit_exit = true;
        }
        else if ( event.msg == "modal_menu_load_game" ) {
          pending_load_campaign = true;
        }
        else if ( event.msg == "modal_menu_save_game" ) {
          SaveSystem::Save();
        }
        else if ( event.msg == "modal_menu_exit_main" ) {
          UI::EnableMainMenuUI();
          program_mode = ProgramMode::MainMenu;
        }
        else if ( event.msg == "toggle_modal_menu" ) {
          if ( program_mode == ProgramMode::Campaign ) {
            program_mode = ProgramMode::ModalMenu;
            UI::EnableModalMenuUI();
          }
          else if ( program_mode == ProgramMode::ModalMenu ) {
            program_mode = ProgramMode::Campaign;
            UI::EnableCampaignUI();
          }
        }
      }
    );


    if ( pending_new_campaign ) {
      if ( campaign )
        delete campaign;
      campaign = new Campaign();
      program_mode = ProgramMode::Campaign;
      pending_new_campaign = false;
    }
    else if ( pending_load_campaign ) {
      if ( campaign )
        delete campaign;
      campaign = new Campaign( "output.dat" );
      program_mode = ProgramMode::Campaign;
      pending_load_campaign = false;
    }

    if ( creating_lobby && is_host ) {
      Network::Host()->Init();
      creating_lobby = false;
    }

    if ( joining_lobby && !is_host ) {
      Network::Client()->Init();
      joining_lobby = false;
    }

    if ( is_host ) {
      Network::Host()->CheckForMessages();
    }
    else {
      Network::Client()->CheckForMessages();
    }


    SteamAPI_RunCallbacks();

    switch ( program_mode ) {
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

  delete campaign;
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
