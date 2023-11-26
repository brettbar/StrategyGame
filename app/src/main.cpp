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

namespace fs = std::filesystem;

void LoadAssets();


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

  // Perform clean up and teardown
  // @TODO figure out all deallocs or whatever
  UnloadShader( Renderer::shader );
  for ( auto resource: Global::texture_cache ) {
    UnloadTexture( resource.second->texture );
  }
  Global::texture_cache.clear();
  SteamAPI_Shutdown();
  CloseWindow();// Close window and OpenGL context

  return EXIT_SUCCESS;
}

Image InitTileOutline() {
  Image base = GenImageColor( 65, 65, ColorAlpha( WHITE, 0.0 ) );

  ImageDrawLineV( &base, { 0, 16 }, { 32, 0 }, YELLOW );
  ImageDrawLineV( &base, { 64, 16 }, { 64, 48 }, YELLOW );
  ImageDrawLineV( &base, { 32, 0 }, { 64, 16 }, YELLOW );
  ImageDrawLineV( &base, { 64, 48 }, { 32, 64 }, YELLOW );
  ImageDrawLineV( &base, { 32, 64 }, { 0, 48 }, YELLOW );
  ImageDrawLineV( &base, { 0, 48 }, { 0, 16 }, YELLOW );

  return base;
}

void LoadAssets() {
  std::string asset_folder = "app/assets/";

  LoadAsset( hstr{ "tile_outline" }, InitTileOutline(), Global::texture_cache );

  Global::font_cache.load(
    hstr{ "font_romulus" },
    LoadFont( ( asset_folder + "/fonts/romulus.png" ).c_str() )
  );

  Global::font_cache.load(
    hstr{ "font_default" },
    LoadFont( ( asset_folder + "/fonts/Perfect-DOS-VGA-437.png" ).c_str() )
  );

  LoadAsset(
    hstr{ "land_tile" },
    LoadImage( ( asset_folder + "/images/hexagons/Grass.bmp" ).c_str() ),
    Global::texture_cache
  );
  LoadAsset(
    hstr{ "forest_tile" },
    LoadImage( ( asset_folder + "/images/hexagons/Forest.bmp" ).c_str() ),
    Global::texture_cache
  );
  LoadAsset(
    hstr{ "mountains_tile" },
    LoadImage( ( asset_folder + "/images/hexagons/Mountains.bmp" ).c_str() ),
    Global::texture_cache
  );
  LoadAsset(
    hstr{ "water_tile" },
    LoadImage( ( asset_folder + "/images/hexagons/Ocean.bmp" ).c_str() ),
    Global::texture_cache
  );
  LoadAsset(
    hstr{ "hills_tile" },
    LoadImage( ( asset_folder + "/images/hexagons/Hills.bmp" ).c_str() ),
    Global::texture_cache
  );
  LoadAsset(
    hstr{ "sand_tile" },
    LoadImage( ( asset_folder + "/images/hexagons/Sand.bmp" ).c_str() ),
    Global::texture_cache
  );
  LoadAsset(
    hstr{ "snow_tile" },
    LoadImage( ( asset_folder + "/images/hexagons/Snow.bmp" ).c_str() ),
    Global::texture_cache
  );

  // LoadAsset(
  //   hstr{ "redOverlay" },
  //   LoadImage( ( asset_folder + "/images/overlays/Red.png" ).c_str() ),
  //   Global::texture_cache
  // );
  create_hex_texture(
    hstr{ "redOverlay" }, Fade( RED, 0.35 ), Global::texture_cache
  );
  create_hex_texture(
    hstr{ "cyanOverlay" }, Fade( BLUE, 0.35 ), Global::texture_cache
  );
  create_hex_texture(
    hstr{ "greenOverlay" }, Fade( GREEN, 0.35 ), Global::texture_cache
  );
  create_hex_texture(
    hstr{ "purpleOverlay" }, Fade( PURPLE, 0.35 ), Global::texture_cache
  );
  create_hex_texture(
    hstr{ "orangeOverlay" }, Fade( ORANGE, 0.35 ), Global::texture_cache
  );

  LoadAsset(
    hstr{ "template" },
    LoadImage( ( asset_folder + "/images/Template.png" ).c_str() ),
    Global::texture_cache
  );

  LoadAsset(
    hstr{ "romans_villager_texture" },
    LoadImage( ( asset_folder + "/images/units/RomanVillager.png" ).c_str() ),
    Global::texture_cache
  );

  LoadAsset(
    hstr{ "romans_hastati_texture" },
    LoadImage( ( asset_folder + "/images/units/RomanHastati.png" ).c_str() ),
    Global::texture_cache
  );
  LoadTexturePointFilter(
    hstr{ "romans_hastati_texture_overview" },
    CropUnitImage( ( asset_folder + "/images/units/RomanHastati.png" ).c_str()
    ),
    Global::texture_cache
  );

  LoadAsset(
    hstr{ "greeks_villager_texture" },
    LoadImage( ( asset_folder + "/images/units/GreekVillager.png" ).c_str() ),
    Global::texture_cache
  );
  LoadAsset(
    hstr{ "celts_villager_texture" },
    LoadImage( ( asset_folder + "/images/units/CelticVillager.png" ).c_str() ),
    Global::texture_cache
  );
  LoadAsset(
    hstr{ "punics_villager_texture" },
    LoadImage(
      ( asset_folder + "/images/units/Carthaginian_Villager.png" ).c_str()
    ),
    Global::texture_cache
  );
  LoadAsset(
    hstr{ "persians_villager_texture" },
    LoadImage( ( asset_folder + "/images/units/Persian_Villager.png" ).c_str()
    ),
    Global::texture_cache
  );
  LoadAsset(
    hstr{ "romanVillageTexture" },
    LoadImage( ( asset_folder + "/images/village_roman.png" ).c_str() ),
    Global::texture_cache
  );

  LoadAsset(
    hstr{ "buildings" },
    LoadImage( ( asset_folder + "/images/buildings.png" ).c_str() ),
    Global::texture_cache
  );

  // TODO at somepoint these UI textures should probably
  // be set to bilinear and then run through the fragment shader
  // But before I can do that I need to fix the shader with how
  // it deals with alpha
  LoadTexturePointFilter(
    hstr{ "context_panel" },
    LoadImage( ( asset_folder + "/images/UI/UI_test.png" ).c_str() ),
    Global::texture_cache
  );

  LoadTexturePointFilter(
    hstr{ "settlement_context_tab_overview" },
    LoadImage( ( asset_folder + "/images/UI/Overview.png" ).c_str() ),
    Global::texture_cache
  );
  LoadTexturePointFilter(
    hstr{ "settlement_context_tab_population" },
    LoadImage( ( asset_folder + "/images/UI/Population.png" ).c_str() ),
    Global::texture_cache
  );
  LoadTexturePointFilter(
    hstr{ "settlement_context_tab_culture" },
    LoadImage( ( asset_folder + "/images/UI/Culture.png" ).c_str() ),
    Global::texture_cache
  );
  LoadTexturePointFilter(
    hstr{ "settlement_context_tab_religion" },
    LoadImage( ( asset_folder + "/images/UI/Religion.png" ).c_str() ),
    Global::texture_cache
  );
  LoadTexturePointFilter(
    hstr{ "settlement_context_tab_resources" },
    LoadImage( ( asset_folder + "/images/UI/Resources.png" ).c_str() ),
    Global::texture_cache
  );
  LoadTexturePointFilter(
    hstr{ "settlement_context_tab_construction" },
    LoadImage( ( asset_folder + "/images/UI/Construction.png" ).c_str() ),
    Global::texture_cache
  );
  LoadTexturePointFilter(
    hstr{ "settlement_context_tab_garrison" },
    LoadImage( ( asset_folder + "/images/UI/Garrison.png" ).c_str() ),
    Global::texture_cache
  );

  std::string path = asset_folder + "/images/resources";
  for ( const auto &entry: fs::directory_iterator( path ) ) {
    std::cout << entry.path().filename() << std::endl;

    std::string filename = entry.path().filename().generic_string();

    std::cout << filename << '\n';

    LoadAsset(
      hstr{ filename.c_str() },
      LoadImage( ( path + "/" + filename ).c_str() ),
      Global::texture_cache
    );
  }
}
