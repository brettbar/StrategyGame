#pragma once

#include "network/client.hpp"
#include "network/host.hpp"

#include "shared/common.hpp"
#include "shared/events.hpp"
#include "shared/save.hpp"

#include "renderer/renderer.hpp"

#include "interface/input.hpp"

#include "app.hpp"
#include "campaign.hpp"


namespace fs = std::filesystem;

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

  // TODO(refactor)
  // Network::Host *host = nullptr;
  Network::Host();
  // Network::Client *client = nullptr;
  Network::Client();
  Network::Setup();

  while ( !WindowShouldClose() && state->GameShouldRun() ) {
    Events::event_emitter.on<Events::UIEvent>(
      [&]( const Events::UIEvent &event, Events::EventEmitter &emitter ) {
        if ( event.msg == "main_menu_host_game" ) {
          // state->creating_lobby = true;
          // state->is_host = true;
          state->HostCampaign();
        }
        else if ( event.msg == "main_menu_join_game" ) {
          // state->joining_lobby = true;
          // state->is_host = false;
          state->JoinCampaign();
        }
        else if ( event.msg == "main_menu_resume_game" ) {
          // state->pending_load_campaign = true;
          state->LoadCampaign();
        }
        else if ( event.msg == "main_menu_start_game" ) {
          // state->pending_new_campaign = true;
          state->StartCampaign();
        }
        else if ( event.msg == "main_menu_load_game" ) {
          // state->pending_load_campaign = true;
          state->LoadCampaign();
        }
        else if ( event.msg == "main_menu_exit_game" ) {
          // state->hit_exit = true;
          state->ExitCampaign();
        }
        else if ( event.msg == "modal_menu_load_game" ) {
          // state->pending_load_campaign = true;

          // TODO prompt for save before exiting
          state->LoadCampaign();
        }
        else if ( event.msg == "modal_menu_save_game" ) {
          SaveSystem::Save();
          //state->SaveCampaign();
        }
        else if ( event.msg == "modal_menu_exit_main" ) {
          UI::EnableMainMenuUI();
          // TODO prompt for save before exiting
          //state->ExitCampaign():
        }
      }
    );


    // if ( state->creating_lobby && state->is_host ) {
    //   // host = new Network::Host();
    //   Network::Host()->Init();
    //   state->creating_lobby = false;
    // }

    // if ( state->joining_lobby && !state->is_host ) {
    //   // client = new Network::Client();
    //   // client->Init();
    //   Network::Client()->Init();
    //   state->joining_lobby = false;
    // }

    // if ( state->is_host ) {
    //   Network::Host()->CheckForMessages();
    // }
    // else {
    //   Network::Client()->CheckForMessages();
    // }


    // If we are starting a new campaign
    // if ( !state->in_a_campaign && state->pending_new_campaign ) {
    // }

    // If we are loading an existing campaign
    // else if ( !state->in_a_campaign && state->pending_load_campaign ) {
    // }

    SteamAPI_RunCallbacks();

    switch ( Global::program_mode ) {
      case Global::ProgramMode::MainMenu:
        RunMainMenu( state );
        break;

      case Global::ProgramMode::ModalMenu:
        RunModalMenu();
        break;

      case Global::ProgramMode::Campaign:
        Campaign::Run( state );
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
