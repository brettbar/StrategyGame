// TODO, probably all the stuff hardcoded in the these data/*.hpp files should
// be in json
#pragma once

#include <map>
#include <string>
#include <vector>
// enum class GatheringBuildingName {
//   WoodCutter,// nc
//   Farm,
//   Fisherman,// nc
//   Quarry,
//   Shepherd,
//   Tanner,
//   Mine,
// };

// enum class ProcessingBuildingName {
//   Sawmill,
//   Mill,
//   Kiln,// nc
//   Stonemason,
//   Spinner,
//   Tanner,
//   Smith,
// };

// enum class ProductionBuildingName {
//   OlivePress,
//   SwordSmith,//nc?
//   Poleturner,
//   Armourer,
//   Jeweler,
//   Tailor,
// };

namespace Buildings {

enum class Type {
  Gathering,
  Processing,
  Producton,
};

using Recipe = std::map<std::string, std::vector<std::string>>;

inline std::vector<std::string> gathering{
  "wood_cutter",
  "farm",
  "fisherman",
  "quarry",
  "shepherd",
  "tanner",
  "mine",
};

inline std::vector<std::string> refining{
  "sawmill",
  "mill",
  "kiln",
  "stonemason",
  "spinner",
  "tanner",
  "smith",
};

inline std::vector<std::string> production{
  "olive_press",
  "sword_smith",
  "poleturner",
  "armourer",
  "jeweler",
  "tailor",
};


struct Building {
  std::string name;
  Type type;

  std::string curr_output;

  std::vector<Recipe> recipes;
};

};// namespace Buildings
