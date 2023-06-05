// TODO, probably all the stuff hardcoded in the these data/*.hpp files should
// be in json
#pragma once

#include "settlement.hpp"
#include <map>
#include <string>
#include <vector>

namespace Buildings
{

  enum class Type
  {
    Gathering,
    Processing,
    Producton,
  };

  using Recipe = std::map<std::string, std::vector<std::string>>;

  enum class BuildingName
  {
    // Gathering
    WoodCutter,// nc
    Farm,
    Fisherman,// nc
    Quarry,
    Shepherd,
    Mine,

    // Refinement
    Sawmill,
    Mill,
    Kiln,// nc
    Stonemason,
    Spinner,
    Tanner,
    Smith,

    // Production
    OlivePress,
    SwordSmith,//nc?
    Poleturner,
    Armourer,
    Jeweler,
    Tailor,
  };
  //
  // inline std::vector<std::string> gathering{
  //   "wood_cutter",
  //   "farm",
  //   "fisherman",
  //   "quarry",
  //   "shepherd",
  //   "tanner",
  //   "mine",
  // };

  // inline std::vector<std::string> refining{
  //   "sawmill",
  //   "mill",
  //   "kiln",
  //   "stonemason",
  //   "spinner",
  //   "tanner",
  //   "smith",
  // };

  // inline std::vector<std::string> production{
  //   "olive_press",
  //   "sword_smith",
  //   "poleturner",
  //   "armourer",
  //   "jeweler",
  //   "tailor",
  // };

  struct Building
  {
    BuildingName name;
    Type type;
    std::string name_str;
  };

};// namespace Buildings
