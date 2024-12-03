#include "../irongui/forge.hpp"
#include <filesystem>
#include <raylib.h>
#include <string>

namespace fs = std::filesystem;


namespace UI {
  inline str load_game_menu() {
    auto f = Iron::Forge();

    u32 rows = 12;

    rect root_r = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_g = f->Grid( root_r, 3, 3 );

    auto button_g = f->Grid( root_g->Slot( 7 ), 1, 4 );


    std::string path = "./saves";
    u32 i = 1;

    list<str> options = {};
    for ( const auto &entry: fs::directory_iterator( path ) ) {
      // options.push_back(
      //   f->TextButton( menu_g->Slot( i++ ), entry.path().string(), GRAY, BLUE )
      // );
      options.push_back( entry.path().string() );
    }

    str selected_path = f->SelectMenu( root_g->Slot( 4 ), options, rows );

    if ( ( selected_path.size() > 0 ) && f->TextButton( button_g->Slot( 0 ), "Load", RED ) ) {
      // @todo make actually really get the selected file
      return selected_path;
    }

    return "";
  }

}// namespace UI
