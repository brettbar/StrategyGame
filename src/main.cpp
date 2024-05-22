/*
  Authored by Brett Barinaga on 11/29/21. Copyright (c) Brett Barinaga, All
rights reserved.
*/

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"

#include "steam/isteamnetworkingutils.h"
#include "steam/steam_api.h"


#include "shared/common.hpp"
#include "shared/global.hpp"
#include "shared/textures.hpp"

#include "assets.hpp"

#include "steam/steam_api_common.h"

#include "network/client.hpp"
#include "network/host.hpp"
#include "network/network.hpp"

#include "game.hpp"

#include "shared/utils.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>
#include <stdlib.h>
#include <thread>


void SteamAPIDebugTextHook( int severity, const char *msg ) {
  printf( "S::%d", severity );
  printf( "%s\n", msg );
}


/*
========================================================
  Main application entrypoint
========================================================
*/
int main() {
  Global::mp_capable = true;

  if ( SteamAPI_RestartAppIfNecessary( 480 ) ) {
    Global::mp_capable = false;
  }

  if ( !SteamAPI_Init() ) {
    printf( "SteamAPI_Init() failed!\n" );
    Global::mp_capable = false;
  }

  if ( !SteamUser()->BLoggedOn() ) {
    printf( "Steam user is not logged in\n" );
    Global::mp_capable = false;
  }

  if ( !SteamInput()->Init( false ) ) {
    printf( "SteamInput()->Init failed.\n" );
    Global::mp_capable = false;
  }

  SteamClient()->SetWarningMessageHook( &SteamAPIDebugTextHook );

  printf( "Starting game as %s.\n", SteamFriends()->GetPersonaName() );

  SetConfigFlags( FLAG_WINDOW_RESIZABLE );
  SetTargetFPS( 200 );// Set our game to run at 60 frames-per-second
  InitWindow( 1920, 1080, "FieldsOfMars" );
  LoadAssets();
  // UI::System::Init();

  SetExitKey( KEY_NULL );

  Network::Setup();

  // This call will block and run until the game exists
  Game()->MainLoop();

  CloseWindow();// Close window and OpenGL context

  // Perform clean up and teardown
  // @TODO figure out all deallocs or whatever
  UnloadShader( Renderer::shader );
  for ( auto resource: Global::texture_cache ) {
    UnloadTexture( resource.second->texture );
  }
  // Global::texture_cache.clear();
  SteamAPI_Shutdown();

  return EXIT_SUCCESS;
}
