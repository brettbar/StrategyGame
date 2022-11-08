#pragma once

#include "../include/cereal/archives/json.hpp"
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace ResourceSystem {

struct RawMaterialRecipe {
  std::string input;
  std::string building;
  std::string biome;

  template<class Archive>
  void serialize( Archive &archive ) {
    archive( input, building, biome );
  }
};

struct RawMaterialRecipes {
  std::map<std::string, std::vector<RawMaterialRecipe>> recipes;

  template<class Archive>
  void serialize( Archive &archive ) {
    archive( recipes );
  }
};

struct RawMaterialsRecipes {
  std::vector<RawMaterialRecipes> recipes;

  template<class Archive>
  void serialize( Archive &archive ) {
    archive( recipes );
  }
};

inline void LoadData() {
  std::ifstream file( "data/raw_materials.json" );
  cereal::JSONInputArchive input{ file };
  RawMaterialsRecipes recipes;

  input( recipes );
}

// MyClass data("hello", 6, true);
// std::stringstream os;
// {
//     cereal::JSONOutputArchive archive_out(os);
//     archive_out(CEREAL_NVP(data));
// }
// string json_str = os.str();
// cout << json_str << endl;

// // deserialize
// std::stringstream is(json_str);
// MyClass data_new;
// {
//     cereal::JSONInputArchive archive_in(is);
//     archive_in(data_new);
//     cout << data_new.y << endl;
// }

};// namespace ResourceSystem
