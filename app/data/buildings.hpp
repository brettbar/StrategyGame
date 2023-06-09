// TODO, probably all the stuff hardcoded in the these data/*.hpp files should
// be in json
#pragma once

#include "resources.hpp"
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

  struct Building
  {
    BuildingName name;
    Type type;
    std::string name_str;

    std::vector<Resources::RawMaterial> producing;
    std::vector<Resources::Natural> consuming;
  };

};// namespace Buildings
