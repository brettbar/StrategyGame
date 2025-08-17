#pragma once

#include "shared/common.hpp"
#include "shared/global.hpp"
#include "shared/textures.hpp"

namespace fs = std::filesystem;

// @note these work no problem on windows
// something is messing them up on macos tho
// wth is this platform dependent??
inline Image InitTileOutline() {
  Image base = GenImageColor(65, 65, ColorAlpha(WHITE, 0.0));

  // ne border
  ImageDrawLineV(&base, {32, 0}, {63, 16}, YELLOW);

  // e border
  ImageDrawLineV(&base, {63, 16}, {63, 47}, YELLOW);

  // se border
  ImageDrawLineV(&base, {32, 63}, {63, 47}, YELLOW);

  // sw border
  ImageDrawLineV(&base, {32, 63}, {0, 47}, YELLOW);

  // w border
  ImageDrawLineV(&base, {0, 16}, {0, 47}, YELLOW);

  // nw border
  ImageDrawLineV(&base, {0, 16}, {32, 0}, YELLOW);

  return base;
}

// @note these work no problem on windows
// something is messing them up on macos tho
// wth is this platform dependent??
inline void load_borders() {
  std::string asset_folder = "assets";

  for (const auto &pair: color_map) {
    Color color = pair.second;
    str color_str = pair.first;

    LoadTexturePointFilter(
      hstr{("ne_border_" + color_str).c_str()},
      create_border({32, 0}, {63, 16}, color, Global::texture_cache),
      Global::texture_cache
    );
    LoadTexturePointFilter(
      hstr{("e_border_" + color_str).c_str()},
      create_border({63, 16}, {63, 47}, color, Global::texture_cache),
      Global::texture_cache
    );
    LoadTexturePointFilter(
      hstr{("se_border_" + color_str).c_str()},
      create_border({32, 63}, {63, 47}, color, Global::texture_cache),
      Global::texture_cache
    );
    LoadTexturePointFilter(
      hstr{("sw_border_" + color_str).c_str()},
      create_border({32, 63}, {0, 47}, color, Global::texture_cache),
      Global::texture_cache
    );
    LoadTexturePointFilter(
      hstr{("w_border_" + color_str).c_str()},
      create_border({0, 16}, {0, 47}, color, Global::texture_cache),
      Global::texture_cache
    );
    LoadTexturePointFilter(
      hstr{("nw_border_" + color_str).c_str()},
      create_border({0, 16}, {32, 0}, color, Global::texture_cache),
      Global::texture_cache
    );
  }
}

inline void load_all_in_folder(str path) {

  for (const auto &entry: fs::directory_iterator(path)) {
    std::string filename = entry.path().filename().generic_string();

    // std::cout << filename << '\n';

    // LoadAsset(
    //   hstr{ filename.c_str() },
    //   LoadImage( ( path + "/" + filename ).c_str() ),
    //   Global::texture_cache
    // );

    LoadTexturePointFilter(
      hstr{filename.c_str()},
      LoadImage((path + "/" + filename).c_str()),
      Global::texture_cache
    );
  }
}


inline void LoadAssets() {
  std::string asset_folder = "assets";

  LoadAsset(hstr{"tile_outline"}, InitTileOutline(), Global::texture_cache);
  load_borders();


  Global::font_cache.load(
    hstr{"font_default"},
    // LoadFont( ( asset_folder + "/fonts/romulus.png" ).c_str() )
    LoadFontEx("assets/fonts/ONESIZE_.TTF", 48, 0, 0)
  );

  Global::font_cache.load(
    hstr{"font_romulus"},
    LoadFont((asset_folder + "/fonts/romulus.png").c_str())
  );

  // Global::font_cache.load(
  //   hstr{ "font_default" },
  //   LoadFont( ( asset_folder + "/fonts/Perfect-DOS-VGA-437.png" ).c_str() )
  // );

  // load_all_in_folder( asset_folder + "/images/hexagons" );

  LoadAsset(
    hstr{"terrain.png"},
    LoadImage((asset_folder + "/images/terrain.png").c_str()),
    Global::texture_cache
  );

  create_hex_texture(hstr{"red_overlay"}, RED, Global::texture_cache);
  create_hex_texture(hstr{"cyan_overlay"}, BLUE, Global::texture_cache);
  create_hex_texture(hstr{"green_overlay"}, GREEN, Global::texture_cache);
  create_hex_texture(hstr{"yellow_overlay"}, YELLOW, Global::texture_cache);
  create_hex_texture(hstr{"purple_overlay"}, PURPLE, Global::texture_cache);
  create_hex_texture(hstr{"orange_overlay"}, ORANGE, Global::texture_cache);
  create_hex_texture(hstr{"black_overlay"}, BLACK, Global::texture_cache);

  LoadAsset(
    hstr{"template"},
    LoadImage((asset_folder + "/images/Template.png").c_str()),
    Global::texture_cache
  );

  LoadAsset(
    hstr{"romanVillageTexture"},
    LoadImage((asset_folder + "/images/village_roman.png").c_str()),
    Global::texture_cache
  );

  LoadAsset(
    hstr{"buildings"},
    LoadImage((asset_folder + "/images/buildings.png").c_str()),
    Global::texture_cache
  );

  // TODO at somepoint these ui textures should probably
  // be set to bilinear and then run through the fragment shader
  // But before I can do that I need to fix the shader with how
  // it deals with alpha
  LoadTexturePointFilter(
    hstr{"context_panel"},
    LoadImage((asset_folder + "/images/ui/ui_test.png").c_str()),
    Global::texture_cache
  );

  load_all_in_folder(asset_folder + "/images/resources");

  load_all_in_folder(asset_folder + "/images/ui");

  load_all_in_folder(asset_folder + "/images/buildings");
}
