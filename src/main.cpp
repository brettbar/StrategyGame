/*
  Authored by Brett Barinaga on 11/29/21. Copyright (c) Brett Barinaga, All
rights reserved.
*/


#include <raylib.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"

#include "shared/common.hpp"
#include "shared/global.hpp"
#include "steam/steam_api.h"
#include "steam/steam_api_common.h"
#include <nlohmann/json.hpp>
#include <stdlib.h>

#include "assets.hpp"
#include "game.hpp"
#include "network/network.hpp"


void SteamAPIDebugTextHook( int severity, const char *msg ) {
  printf( "S::%d", severity );
  printf( "%s\n", msg );
}

void HandleClayErrors( Clay_ErrorData errorData ) {
  printf( "%s", errorData.errorText.chars );
  if ( errorData.errorType == CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED ) {
    // reinitializeClay = true;
    Clay_SetMaxElementCount( Clay_GetMaxElementCount() * 2 );
  } else if ( errorData.errorType ==
              CLAY_ERROR_TYPE_TEXT_MEASUREMENT_CAPACITY_EXCEEDED ) {
    // reinitializeClay = true;
    Clay_SetMaxMeasureTextCacheWordCount(
      Clay_GetMaxMeasureTextCacheWordCount() * 2
    );
  }
}

/*
========================================================
  Main application entrypoint
========================================================
*/
int main() {
  Global::mp_capable = true;

  if ( SteamAPI_RestartAppIfNecessary( 2296090 ) ) {
    Global::mp_capable = false;
  }

  if ( !SteamAPI_Init() ) {
    printf( "SteamAPI_Init() failed!\n" );
    Global::mp_capable = false;
  }

  SteamClient()->SetWarningMessageHook( &SteamAPIDebugTextHook );

  if ( !SteamUser()->BLoggedOn() ) {
    printf( "Steam user is not logged in\n" );
    Global::mp_capable = false;
  }

  if ( !SteamInput()->Init( false ) ) {
    printf( "SteamInput()->Init failed.\n" );
    Global::mp_capable = false;
  }


  printf( "Starting game as %s.\n", SteamFriends()->GetPersonaName() );

  // SetConfigFlags( FLAG_WINDOW_RESIZABLE );
  SetTargetFPS( 200 );// Set our game to run at 60 frames-per-second

  // InitWindow( 1920, 1080, "FieldsOfMars" );
  u64 clay_required_memory = Clay_MinMemorySize();
  Clay_Arena clay_memory = Clay_Arena{
    .capacity = clay_required_memory,
    .memory = (char *) malloc( clay_required_memory ),
  };
  Clay_Initialize(
    clay_memory,
    Clay_Dimensions{
      .width = (f32) GetScreenWidth(),
      .height = (f32) GetScreenHeight(),
    },
    Clay_ErrorHandler{ HandleClayErrors, 0 }
  );
  Clay_Raylib_Initialize( 1920, 1080, "FieldsOfMars", FLAG_WINDOW_RESIZABLE );


  LoadAssets();

  Global::fonts.push_back( Global::font_cache[hstr{ "font_default" }]->font );

  Clay_SetMeasureTextFunction( Raylib_MeasureText, Global::fonts.data() );

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
