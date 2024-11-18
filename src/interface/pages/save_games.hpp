#include "../irongui/forge.hpp"
#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

namespace UI {
  inline void load_game_menu() {
    auto f = Iron::Forge();
    std::string path = "./saves";

    rect root_r = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_g = f->Grid( root_r, 1, 10 );

    for (const auto & entry : fs::directory_iterator(path)) {
      std::cout << entry.path() << std::endl;
      f->


    }

  }

}// namespace UI
