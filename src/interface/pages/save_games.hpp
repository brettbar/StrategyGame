#include "../irongui/forge.hpp"
#include <filesystem>
#include <raylib.h>
#include <string>

namespace fs = std::filesystem;


namespace UI {
  inline str load_game_menu( bool &pressed_back ) {
    auto f = Iron::Forge();

    u32 rows = 12;

    rect root_r = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_g = f->Grid( root_r, 3, 3 );

    auto button_g = f->Grid( root_g->Slot( 7 ), 2, 4 );


    // @todo probably make this absolute like paradox games do, in documents for example
    std::string path = "./saves";

    list<str> options = {};
    for ( const auto &entry: fs::directory_iterator( path ) ) {
      // options.push_back(
      //   f->TextButton( menu_g->Slot( i++ ), entry.path().string(), GRAY, BLUE )
      // );
      if ( entry.path().extension() == ".dat" )
        options.push_back( entry.path().string() );
    }

    str selected_path = f->SelectMenu( root_g->Slot( 4 ), options, rows );

    if ( f->TextButton( button_g->Slot( 0 ), "Back", RED ) ) {
      pressed_back = true;
      return "";
    }

    if ( ( selected_path.size() > 0 ) &&
         f->TextButton( button_g->Slot( 1 ), "Load", RED ) ) {
      // @todo make actually really get the selected file
      return selected_path;
    }

    return "";
  }

}// namespace UI
