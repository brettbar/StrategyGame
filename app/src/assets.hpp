#pragma once

#include "shared/common.hpp"
#include "shared/global.hpp"
#include "shared/textures.hpp"
#include "world/systems/faction_system.hpp"

namespace fs = std::filesystem;

inline Image InitTileOutline() {
  Image base = GenImageColor( 65, 65, ColorAlpha( WHITE, 0.0 ) );

  ImageDrawLineV( &base, { 0, 16 }, { 32, 0 }, YELLOW );
  ImageDrawLineV( &base, { 64, 16 }, { 64, 48 }, YELLOW );
  ImageDrawLineV( &base, { 32, 0 }, { 64, 16 }, YELLOW );
  ImageDrawLineV( &base, { 64, 48 }, { 32, 64 }, YELLOW );
  ImageDrawLineV( &base, { 32, 64 }, { 0, 48 }, YELLOW );
  ImageDrawLineV( &base, { 0, 48 }, { 0, 16 }, YELLOW );

  return base;
}

inline void load_borders() {
  std::string asset_folder = "app/assets/";

  for ( const auto &pair: FactionSystem::color_map ) {
    Color color = pair.second;
    str color_str = pair.first;

    LoadAsset(
      hstr{ ( "ne_border_" + color_str ).c_str() },
      create_border( { 32, 0 }, { 63, 16 }, color, Global::texture_cache ),
      Global::texture_cache
    );
    LoadAsset(
      hstr{ ( "e_border_" + color_str ).c_str() },
      create_border( { 63, 16 }, { 63, 47 }, color, Global::texture_cache ),
      // LoadImage( ( asset_folder + "/images/overlays-E.png" ).c_str() ),
      Global::texture_cache
    );
    LoadAsset(
      hstr{ ( "se_border_" + color_str ).c_str() },
      create_border( { 63, 47 }, { 32, 63 }, color, Global::texture_cache ),
      Global::texture_cache
    );
    LoadAsset(
      hstr{ ( "sw_border_" + color_str ).c_str() },
      create_border( { 31, 63 }, { 0, 47 }, color, Global::texture_cache ),
      Global::texture_cache
    );
    LoadAsset(
      hstr{ ( "w_border_" + color_str ).c_str() },
      create_border( { 0, 47 }, { 0, 16 }, color, Global::texture_cache ),
      Global::texture_cache
    );
    LoadAsset(
      hstr{ ( "nw_border_" + color_str ).c_str() },
      create_border( { 0, 16 }, { 31, 0 }, color, Global::texture_cache ),
      Global::texture_cache
    );
  }
}

inline void load_all_in_folder( str path ) {

  for ( const auto &entry: fs::directory_iterator( path ) ) {
    std::string filename = entry.path().filename().generic_string();

    std::cout << filename << '\n';

    LoadAsset(
      hstr{ filename.c_str() },
      LoadImage( ( path + "/" + filename ).c_str() ),
      Global::texture_cache
    );
  }
}

inline void LoadAssets() {
  std::string asset_folder = "app/assets/";

  LoadAsset( hstr{ "tile_outline" }, InitTileOutline(), Global::texture_cache );
  load_borders();

  Global::font_cache.load(
    hstr{ "font_romulus" },
    LoadFont( ( asset_folder + "/fonts/romulus.png" ).c_str() )
  );

  Global::font_cache.load(
    hstr{ "font_default" },
    LoadFont( ( asset_folder + "/fonts/Perfect-DOS-VGA-437.png" ).c_str() )
  );

  // LoadAsset(
  //   hstr{ "land_tile" },
  //   LoadImage( ( asset_folder + "/images/hexagons/Grass.bmp" ).c_str() ),
  //   Global::texture_cache
  // );
  // LoadAsset(
  //   hstr{ "forest_tile" },
  //   LoadImage( ( asset_folder + "/images/hexagons/Forest.bmp" ).c_str() ),
  //   Global::texture_cache
  // );
  // LoadAsset(
  //   hstr{ "mountains_tile" },
  //   LoadImage( ( asset_folder + "/images/hexagons/Mountains.bmp" ).c_str() ),
  //   Global::texture_cache
  // );
  // LoadAsset(
  //   hstr{ "water_tile" },
  //   LoadImage( ( asset_folder + "/images/hexagons/Ocean.bmp" ).c_str() ),
  //   Global::texture_cache
  // );
  // LoadAsset(
  //   hstr{ "hills_tile" },
  //   LoadImage( ( asset_folder + "/images/hexagons/Hills.bmp" ).c_str() ),
  //   Global::texture_cache
  // );
  // LoadAsset(
  //   hstr{ "sand_tile" },
  //   LoadImage( ( asset_folder + "/images/hexagons/Sand.bmp" ).c_str() ),
  //   Global::texture_cache
  // );
  // LoadAsset(
  //   hstr{ "snow_tile" },
  //   LoadImage( ( asset_folder + "/images/hexagons/Snow.bmp" ).c_str() ),
  //   Global::texture_cache
  // );

  load_all_in_folder( asset_folder + "/images/hexagons" );

  create_hex_texture( hstr{ "red_overlay" }, RED, Global::texture_cache );
  create_hex_texture( hstr{ "cyan_overlay" }, BLUE, Global::texture_cache );
  create_hex_texture( hstr{ "green_overlay" }, GREEN, Global::texture_cache );
  create_hex_texture( hstr{ "purple_overlay" }, PURPLE, Global::texture_cache );
  create_hex_texture( hstr{ "orange_overlay" }, ORANGE, Global::texture_cache );

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

  load_all_in_folder( asset_folder + "/images/resources" );
}