#pragma once

#include "network/client.hpp"
#include "network/host.hpp"

#include "shared/common.hpp"
#include "shared/events.hpp"
#include "shared/save.hpp"

#include "renderer/renderer.hpp"

#include "interface/input.hpp"

#include "campaign.hpp"


void RunMainMenu( GameState * );
void RunModalMenu();

void CameraUpdate( Camera2D &, f32 );
void Exit( TextureCache & );


/*
========================================================
  Main game loop 
========================================================
*/

inline void RunGameLoop() {
  GameState *state = new GameState();
  Campaign *campaign = nullptr;

  // Network::Host();
  // Network::Client();
  Network::Setup();

  bool creating_lobby = false;
  bool joining_lobby = false;
  bool pending_new_campaign = false;
  bool pending_load_campaign = false;
  bool hit_exit = false;
  bool is_host = false;

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
        else if ( event.msg == "main_menu_resume_game" ) {
          pending_load_campaign = true;
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
        }
      }
    );


    if ( pending_new_campaign ) {
      campaign = new Campaign();
      pending_new_campaign = false;
    }
    else if ( pending_load_campaign ) {
      // campaign = new Campaign();
      // pending_load_campaign = true;
    }

    if ( creating_lobby && is_host ) {
      Network::Host()->Init();
      creating_lobby = false;
    }

    if ( joining_lobby && !state->is_host ) {
      Network::Client()->Init();
      joining_lobby = false;
    }

    if ( state->is_host ) {
      Network::Host()->CheckForMessages();
    }
    else {
      Network::Client()->CheckForMessages();
    }


    SteamAPI_RunCallbacks();

    switch ( Global::program_mode ) {
      case Global::ProgramMode::MainMenu:
        RunMainMenu( state );
        break;

      case Global::ProgramMode::ModalMenu:
        RunModalMenu();
        break;

      case Global::ProgramMode::Campaign:
        assert( campaign );
        campaign->Run( state );
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
  delete state;
}


inline void RunMainMenu( GameState *state ) {
  Input::Handle();

  UI::UpdateOnFrame();

  CameraUpdate( Global::state.camera, state->dt );

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
