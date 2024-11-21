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

    auto menu_g = f->Grid( root_g->Slot( 4 ), 1, rows );

    f->TextLabel( menu_g->Slot( 0 ), "Select File to Load" );

    std::string path = "./saves";
    u32 i = 1;
    for ( const auto &entry: fs::directory_iterator( path ) ) {
      // std::cout << entry.path() << std::endl;
      f->TextLabel( menu_g->Slot( i++ ), entry.path().string(), GRAY );
    }

    if ( f->TextButton( menu_g->Slot( rows - 1 ), "Load", RED ) ) {
      // @todo make actually really get the selected file
      return "./saves/output.dat";
    }

    return "";
  }

}// namespace UI
