// TODO, probably all the stuff hardcoded in the these data/*.hpp files should
// be in json
#pragma once

#include "../src/shared/common.hpp"
#include "clay/clay.h"
#include "resources.hpp"
#include <cereal/types/vector.hpp>

namespace Buildings {

  struct RecipeItem {
    u32 quantity = 1;// typically 1 at a time, but can be overloaded
    Resources::Type resource;

    template<class Archive>
    void serialize(Archive &ar) {
      ar(quantity, resource);
    }
  };

  struct Recipe {
    u32 id;
    list<RecipeItem> inputs;
    list<RecipeItem> outputs;// usually just one

    bool operator==(const Recipe &other) const {
      return id == other.id;
    }

    template<class Archive>
    void serialize(Archive &ar) {
      ar(id, inputs, outputs);
    }
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

  struct Building {
    BuildingType type;
    BuildingCategory category;
    Clay_String label;
    str path;
    Recipe current_recipe;
  };

  inline const char *building_name_cstr(BuildingType building) {
    switch (building) {
      case BuildingType::LumberMill:
        return "Lumber Mill";
      case BuildingType::Farm:
        return "Farm";
      case BuildingType::Fisherman:
        return "Fisherman";
      case BuildingType::Quarry:
        return "Quarry";
      case BuildingType::Shepherd:
        return "Shepherd";
      case BuildingType::Mine:
        return "Mine";
      case BuildingType::Mill:
        return "Mill";
      case BuildingType::Kiln:
        return "Kiln";
      case BuildingType::Stonemason:
        return "Stonemason";
      case BuildingType::Spinner:
        return "Spinner";
      case BuildingType::Tanner:
        return "Tanner";
      case BuildingType::Smithy:
        return "Smithy";
      case BuildingType::OlivePress:
        return "Olive Press";
      case BuildingType::SwordSmith:
        return "Sword Smith";
      case BuildingType::Poleturner:
        return "Poleturner";
      case BuildingType::Armourer:
        return "Armourer";
      case BuildingType::Jeweler:
        return "Jeweler";
      case BuildingType::Tailor:
        return "Tailor";
      default:
        return "Unknown Building";
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
            .id = 0,
            .inputs = {},
            .outputs = {RecipeItem{.resource = Resources::Type::Timber}},
          },
        };
      case BuildingType::Farm:
        return {
          Recipe{
            .id = 0,
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
            .id = 0,
            .inputs = {},
            .outputs = {RecipeItem{.resource = Resources::Type::TinOre}},
          },
          Recipe{
            .id = 1,
            .inputs = {},
            .outputs = {RecipeItem{.resource = Resources::Type::CopperOre}},
          },
          Recipe{
            .id = 2,
            .inputs = {},
            .outputs = {RecipeItem{.resource = Resources::Type::IronOre}},
          },
          Recipe{
            .id = 3,
            .inputs = {},
            .outputs = {RecipeItem{.resource = Resources::Type::SilverOre}},
          },
          Recipe{
            .id = 4,
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


  // @refactor make this a util, alongside the one in resources.hpp
  inline hstr building_icon_path(BuildingType type) {
    return file_path_str_format(
      Buildings::building_name_cstr(type), "_icon.png"
    );
  }

};// namespace Buildings
