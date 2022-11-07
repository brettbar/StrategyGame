#pragma once

#include "../include/cereal/archives/json.hpp"
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace ResourceSystem {

/*
TODO this probably should be moved to component
*/
enum class ResourceType {
  RawMaterial,
  ProcessedMaterial,
  Product,
};
struct Recipe {
  const char recipe[10];
};
struct Resource {
  std::string name;
  ResourceType type;
  Recipe recipe;
  // Recipes
};

const std::string raw_materials = {};
const std::string processed_materials = {};
const std::string products = {};

struct RawMatRecipe {
  std::string resource;
  std::vector<std::string> inputs;
  std::string building;
  std::string biome;
};

struct ProcMatRecipe {};

inline std::map<std::string, std::vector<RawMatRecipe>> recipes;

inline void LoadData() {
  std::ifstream file( "data/resources.json" );
  cereal::JSONInputArchive input{ file };
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
