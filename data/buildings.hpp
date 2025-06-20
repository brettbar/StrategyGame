// TODO, probably all the stuff hardcoded in the these data/*.hpp files should
// be in json
#pragma once

#include "../src/shared/common.hpp"
#include "biomes.hpp"
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

  enum class Type {
    // Gathering
    LumberMill,// nc
    Farm,
    Orchard,
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
    Type type;
    BuildingCategory category;
    Clay_String label;
    str path;
    Recipe current_recipe;

    template<class Archive>
    void serialize(Archive &ar) {
      ar(type, category, current_recipe);
    }
  };

  struct ConstructionRecipe {
    u32 id;
    list<RecipeItem> inputs;

    bool operator==(const Recipe &other) const {
      return id == other.id;
    }

    template<class Archive>
    void serialize(Archive &ar) {
      ar(id, inputs);
    }
  };

  inline const char *building_name_cstr(Type building) {
    switch (building) {
      case Type::LumberMill:
        return "Lumber Mill";
      case Type::Farm:
        return "Farm";
      case Type::Fisherman:
        return "Fisherman";
      case Type::Quarry:
        return "Quarry";
      case Type::Shepherd:
        return "Shepherd";
      case Type::Mine:
        return "Mine";
      case Type::Mill:
        return "Mill";
      case Type::Kiln:
        return "Kiln";
      case Type::Stonemason:
        return "Stonemason";
      case Type::Spinner:
        return "Spinner";
      case Type::Tanner:
        return "Tanner";
      case Type::Smithy:
        return "Smithy";
      case Type::OlivePress:
        return "Olive Press";
      case Type::SwordSmith:
        return "Sword Smith";
      case Type::Poleturner:
        return "Poleturner";
      case Type::Armourer:
        return "Armourer";
      case Type::Jeweler:
        return "Jeweler";
      case Type::Tailor:
        return "Tailor";
      default:
        return "Unknown Building";
    }
  }

  inline map<str, Type> building_lookup = {
    {"lumber_mill", Type::LumberMill},
    {"farm", Type::Farm},
    {"fisherman", Type::Fisherman},
    {"quarry", Type::Quarry},
    {"shepherd", Type::Shepherd},
    {"mine", Type::Mine},
    {"mill", Type::Mill},
    {"kiln", Type::Kiln},
    {"stone_mason", Type::Stonemason},
    {"spinner", Type::Spinner},
    {"tanner", Type::Tanner},
    {"smithy", Type::Smithy},
    {"olive_press", Type::OlivePress},
    {"sword_smith", Type::SwordSmith},
    {"poleturner", Type::Poleturner},
    {"armourer", Type::Armourer},
    {"jeweler", Type::Jeweler},
    {"tailor", Type::Tailor},
  };

  inline list<Recipe> recipes_for_building(Type building) {
    switch (building) {
      case Type::LumberMill:
        return {
          Recipe{
            .id = 0,
            .inputs = {},
            .outputs = {RecipeItem{.resource = Resources::Type::Timber}},
          },
        };
      case Type::Farm:
        return {
          Recipe{
            .id = 0,
            .inputs = {},
            .outputs = {RecipeItem{.resource = Resources::Type::Wheat}},
          },
        };
      case Type::Fisherman:
        break;
      case Type::Quarry:
        break;
      case Type::Shepherd:
        break;
      case Type::Mine:
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
      case Type::Mill:
        break;
      case Type::Kiln:
        break;
      case Type::Stonemason:
        break;
      case Type::Spinner:
        break;
      case Type::Tanner:
        break;
      case Type::Smithy:
        break;
      case Type::OlivePress:
        break;
      case Type::SwordSmith:
        break;
      case Type::Poleturner:
        break;
      case Type::Armourer:
        break;
      case Type::Jeweler:
        break;
      case Type::Tailor:
        break;
    }

    return {};
  }

  inline list<ConstructionRecipe> construction_recipes(Type type) {
    switch (type) {
      case Type::LumberMill:
      case Type::Farm:
      case Type::Orchard:
      case Type::Fisherman:
      case Type::Quarry:
      case Type::Shepherd:
      case Type::Mine:
      case Type::Mill:
      case Type::Kiln:
      case Type::Stonemason:
      case Type::Spinner:
      case Type::Tanner:
      case Type::Smithy:
      case Type::OlivePress:
      case Type::SwordSmith:
      case Type::Poleturner:
      case Type::Armourer:
      case Type::Jeweler:
      case Type::Tailor:
        return {
          ConstructionRecipe{
            .id = 0,
            .inputs =
              {
                RecipeItem{
                  .quantity = 1,
                  .resource = Resources::Type::Bricks,
                },
                RecipeItem{
                  .quantity = 1,
                  .resource = Resources::Type::Timber,
                },
              },
          },
          ConstructionRecipe{
            .id = 1,
            .inputs =
              {
                RecipeItem{
                  .quantity = 1,
                  .resource = Resources::Type::Stone,
                },
                RecipeItem{
                  .quantity = 1,
                  .resource = Resources::Type::Timber,
                },
              },
          },
        };
    };

    return {};
  }

  // @todo make a function for actually checking the the tile has the required resource
  inline list<Biome> eligible_biomes_for_building(Type type) {

    auto land_tiles = {
      Biome::Desert,
      Biome::Plains,
      Biome::Hills,
      Biome::Mountains,
      Biome::Forest,
      Biome::Taiga,
      Biome::Tundra,
      Biome::Steppe,
    };

    switch (type) {
      case Type::LumberMill:
        return {Biome::Forest, Biome::Taiga};
      case Type::Farm:
        return {Biome::Plains};
      case Type::Orchard:
        return {Biome::Plains, Biome::Forest, Biome::Hills};
      case Type::
        Fisherman:// @todo this isnt really determined by the biome, but rather the proximity to the sea/rivers
        return {};
      case Type::Quarry:
        return {Biome::Hills, Biome::Mountains};
      case Type::Shepherd:
        return {Biome::Plains, Biome::Hills};
      case Type::Mine:
        return {Biome::Mountains};

      case Type::Mill:
      case Type::Kiln:
      case Type::Stonemason:
      case Type::Spinner:
      case Type::Tanner:
      case Type::Smithy:
      case Type::OlivePress:
      case Type::SwordSmith:
      case Type::Poleturner:
      case Type::Armourer:
      case Type::Jeweler:
      case Type::Tailor:
        return land_tiles;
    }

    return {};
  }


  // @refactor make this a util, alongside the one in resources.hpp
  inline hstr building_icon_path(Type type) {
    return file_path_str_format(
      Buildings::building_name_cstr(type), "_icon.png"
    );
  }

};// namespace Buildings
