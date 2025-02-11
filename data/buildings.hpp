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

  enum class BuildingName {
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

  inline str building_name_str( BuildingName building ) {
    switch ( building ) {
      case BuildingName::LumberMill:
        return "lumber_mill";
      case BuildingName::Farm:
        return "farm";
      case BuildingName::Fisherman:
        return "fisherman";
      case BuildingName::Quarry:
        return "quarry";
      case BuildingName::Shepherd:
        return "shepherd";
      case BuildingName::Mine:
        return "mine";
      case BuildingName::Mill:
        return "mill";
      case BuildingName::Kiln:
        return "kiln";
      case BuildingName::Stonemason:
        return "stonemason";
      case BuildingName::Spinner:
        return "spinner";
      case BuildingName::Tanner:
        return "tanner";
      case BuildingName::Smithy:
        return "smithy";
      case BuildingName::OlivePress:
        return "olive_press";
      case BuildingName::SwordSmith:
        return "sword_smith";
      case BuildingName::Poleturner:
        return "poleturner";
      case BuildingName::Armourer:
        return "armourer";
      case BuildingName::Jeweler:
        return "jeweler";
      case BuildingName::Tailor:
        return "tailor";
      default:
        return "unknown_building";
    }
  }

  inline map<str, BuildingName> building_lookup = {
    { "lumber_mill", BuildingName::LumberMill },
    { "farm", BuildingName::Farm },
    { "fisherman", BuildingName::Fisherman },
    { "quarry", BuildingName::Quarry },
    { "shepherd", BuildingName::Shepherd },
    { "mine", BuildingName::Mine },
    { "mill", BuildingName::Mill },
    { "kiln", BuildingName::Kiln },
    { "stone_mason", BuildingName::Stonemason },
    { "spinner", BuildingName::Spinner },
    { "tanner", BuildingName::Tanner },
    { "smithy", BuildingName::Smithy },
    { "olive_press", BuildingName::OlivePress },
    { "sword_smith", BuildingName::SwordSmith },
    { "poleturner", BuildingName::Poleturner },
    { "armourer", BuildingName::Armourer },
    { "jeweler", BuildingName::Jeweler },
    { "tailor", BuildingName::Tailor },
  };


  inline list<Recipe> recipes_for_building( BuildingName building ) {
    switch ( building ) {
      case BuildingName::LumberMill:
        return {
          Recipe{
            .inputs = {},
            .outputs = { RecipeItem{ .resource = Resources::Type::Timber } },
          },
        };
      case BuildingName::Farm:
        return {
          Recipe{
            .inputs = {},
            .outputs = { RecipeItem{ .resource = Resources::Type::Wheat } },
          },
        };
      case BuildingName::Fisherman:
        break;
      case BuildingName::Quarry:
        break;
      case BuildingName::Shepherd:
        break;
      case BuildingName::Mine:
        return {
          Recipe{
            .inputs = {},
            .outputs = { RecipeItem{ .resource = Resources::Type::TinOre } },
          },
          Recipe{
            .inputs = {},
            .outputs = { RecipeItem{ .resource = Resources::Type::CopperOre } },
          },
          Recipe{
            .inputs = {},
            .outputs = { RecipeItem{ .resource = Resources::Type::IronOre } },
          },
          Recipe{
            .inputs = {},
            .outputs = { RecipeItem{ .resource = Resources::Type::TinOre } },
          },
          Recipe{
            .inputs = {},
            .outputs = { RecipeItem{ .resource = Resources::Type::SilverOre } },
          },
          Recipe{
            .inputs = {},
            .outputs = { RecipeItem{ .resource = Resources::Type::GoldOre } },
          },
        };
      case BuildingName::Mill:
        break;
      case BuildingName::Kiln:
        break;
      case BuildingName::Stonemason:
        break;
      case BuildingName::Spinner:
        break;
      case BuildingName::Tanner:
        break;
      case BuildingName::Smithy:
        break;
      case BuildingName::OlivePress:
        break;
      case BuildingName::SwordSmith:
        break;
      case BuildingName::Poleturner:
        break;
      case BuildingName::Armourer:
        break;
      case BuildingName::Jeweler:
        break;
      case BuildingName::Tailor:
        break;
    }

    return {};
  }


  struct Building {
    BuildingName name;
    std::string name_str;
    u32 current_recipe = 0;

    template<class Archive>
    void serialize( Archive &ar ) {
      ar( name, name_str, current_recipe );
    }
  };

};// namespace Buildings
