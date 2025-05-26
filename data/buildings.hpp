// TODO, probably all the stuff hardcoded in the these data/*.hpp files should
// be in json
#pragma once

#include "../src/shared/common.hpp"
#include "resources.hpp"
#include <cereal/types/vector.hpp>

namespace Buildings {

  struct RecipeItem {
    u32 quantity = 1;// typically 1 at a time, but can be overloaded
    Resources::Type resource;
  };
  struct Recipe {
    list<RecipeItem> inputs;
    list<RecipeItem> outputs;// usually just one
  };

  enum class BuildingCategory {
    Agricultural,
    Gathering,
    Refinement,
    Production,
    Diplomatic,
    Martial,
    Naval,
    Civic,
    Religious
  };

  enum class BuildingType {
    // Gathering
    LumberMill,// nc
    Farm,
    Fisherman,// nc
    Quarry,
    Shepherd,
    Mine,

    // Refinement
    Mill,
    Kiln,// nc
    Stonemason,
    Spinner,
    Tanner,
    Smithy,

    // Production
    OlivePress,
    SwordSmith,//nc?
    Poleturner,
    Armourer,
    Jeweler,
    Tailor,
  };

  inline str building_name_str(BuildingType building) {
    switch (building) {
      case BuildingType::LumberMill:
        return "lumber_mill";
      case BuildingType::Farm:
        return "farm";
      case BuildingType::Fisherman:
        return "fisherman";
      case BuildingType::Quarry:
        return "quarry";
      case BuildingType::Shepherd:
        return "shepherd";
      case BuildingType::Mine:
        return "mine";
      case BuildingType::Mill:
        return "mill";
      case BuildingType::Kiln:
        return "kiln";
      case BuildingType::Stonemason:
        return "stonemason";
      case BuildingType::Spinner:
        return "spinner";
      case BuildingType::Tanner:
        return "tanner";
      case BuildingType::Smithy:
        return "smithy";
      case BuildingType::OlivePress:
        return "olive_press";
      case BuildingType::SwordSmith:
        return "sword_smith";
      case BuildingType::Poleturner:
        return "poleturner";
      case BuildingType::Armourer:
        return "armourer";
      case BuildingType::Jeweler:
        return "jeweler";
      case BuildingType::Tailor:
        return "tailor";
      default:
        return "unknown_building";
    }
  }

  inline map<str, BuildingType> building_lookup = {
    {"lumber_mill", BuildingType::LumberMill},
    {"farm", BuildingType::Farm},
    {"fisherman", BuildingType::Fisherman},
    {"quarry", BuildingType::Quarry},
    {"shepherd", BuildingType::Shepherd},
    {"mine", BuildingType::Mine},
    {"mill", BuildingType::Mill},
    {"kiln", BuildingType::Kiln},
    {"stone_mason", BuildingType::Stonemason},
    {"spinner", BuildingType::Spinner},
    {"tanner", BuildingType::Tanner},
    {"smithy", BuildingType::Smithy},
    {"olive_press", BuildingType::OlivePress},
    {"sword_smith", BuildingType::SwordSmith},
    {"poleturner", BuildingType::Poleturner},
    {"armourer", BuildingType::Armourer},
    {"jeweler", BuildingType::Jeweler},
    {"tailor", BuildingType::Tailor},
  };

  inline list<Recipe> recipes_for_building(BuildingType building) {
    switch (building) {
      case BuildingType::LumberMill:
        return {
          Recipe{
            .inputs = {},
            .outputs = {RecipeItem{.resource = Resources::Type::Timber}},
          },
        };
      case BuildingType::Farm:
        return {
          Recipe{
            .inputs = {},
            .outputs = {RecipeItem{.resource = Resources::Type::Wheat}},
          },
        };
      case BuildingType::Fisherman:
        break;
      case BuildingType::Quarry:
        break;
      case BuildingType::Shepherd:
        break;
      case BuildingType::Mine:
        return {
          Recipe{
            .inputs = {},
            .outputs = {RecipeItem{.resource = Resources::Type::TinOre}},
          },
          Recipe{
            .inputs = {},
            .outputs = {RecipeItem{.resource = Resources::Type::CopperOre}},
          },
          Recipe{
            .inputs = {},
            .outputs = {RecipeItem{.resource = Resources::Type::IronOre}},
          },
          Recipe{
            .inputs = {},
            .outputs = {RecipeItem{.resource = Resources::Type::TinOre}},
          },
          Recipe{
            .inputs = {},
            .outputs = {RecipeItem{.resource = Resources::Type::SilverOre}},
          },
          Recipe{
            .inputs = {},
            .outputs = {RecipeItem{.resource = Resources::Type::GoldOre}},
          },
        };
      case BuildingType::Mill:
        break;
      case BuildingType::Kiln:
        break;
      case BuildingType::Stonemason:
        break;
      case BuildingType::Spinner:
        break;
      case BuildingType::Tanner:
        break;
      case BuildingType::Smithy:
        break;
      case BuildingType::OlivePress:
        break;
      case BuildingType::SwordSmith:
        break;
      case BuildingType::Poleturner:
        break;
      case BuildingType::Armourer:
        break;
      case BuildingType::Jeweler:
        break;
      case BuildingType::Tailor:
        break;
    }

    return {};
  }

  inline hstr building_icon_path(BuildingType type) {
    return hstr{(Buildings::building_name_str(type) + "_icon.png").c_str()};
  }

};// namespace Buildings
