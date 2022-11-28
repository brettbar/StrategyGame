// TODO this should be moved to json instead some time later,
// but cereal library sucks and I am out of patience

#pragma once

#include <map>
#include <string>
#include <vector>

#include "biomes.hpp"

enum class NaturalResource {
  Soil,// not sure about this one
  Trees,
  Fish,
  Clay,
  Stone,
  Sheep,
  Cattle,
  SilverVein,
  GoldVein,
  IronVein,
  TinVein,
  CopperVein,
  Gemstones,
};

enum class RawMaterial {
  Timber,
  Wheat,
  Olives,
  Fish,
  Clay,
  Stone,
  Wool,
  Flax,
  Hide,
  SilverOre,
  GoldOre,
  IronOre,
  TinOre,
  CopperOre,
  Gems,
};

enum class ProcessedMaterial {
  Lumber,
  Flour,
  Bricks,
  StoneBricks,
  Pottery,
  Cloth,
  Linen,
  Leather,
  Silver,
  Gold,
  Iron,
  Bronze,
};

enum class Product {
  OliveOil,
  Swords,
  Spears,
  Shields,
  Helmets,
  Armour,
  Clothing,
  Jewelry,
};

enum class GatheringBuildingName {
  WoodCutter,// nc
  Farm,
  Fisherman,// nc
  Quarry,
  Shepherd,
  Tanner,
  Mine,
};

enum class ProcessingBuildingName {
  Sawmill,
  Mill,
  Kiln,// nc
  Stonemason,
  Spinner,
  Tanner,
  Smith,
};

enum class ProductionBuildingName {
  OlivePress,
  SwordSmith,//nc?
  Poleturner,
  Armourer,
  Jeweler,
  Tailor,
};

struct RawMaterialRecipe {
  NaturalResource input;
  GatheringBuildingName building;
  std::vector<Biome> biome;
};

struct ProcessedMaterialRecipe {
  std::vector<RawMaterial> inputs;
  ProcessingBuildingName building;
};

struct ProductRecipe {
  std::vector<RawMaterial> raw_inputs;
  std::vector<ProcessedMaterial> processed_inputs;
  ProductionBuildingName building;
};

inline std::map<RawMaterial, std::vector<RawMaterialRecipe>> raw_materials = {
  {
    RawMaterial::Timber,
    {
      {
        NaturalResource::Trees,
        GatheringBuildingName::WoodCutter,
        { Biome::Forest },
      },
    },
  },

  {
    RawMaterial::Wheat,
    {
      {
        NaturalResource::Soil,
        GatheringBuildingName::Farm,
        { Biome::Plains },
      },
    },
  },

  {
    RawMaterial::Olives,
    {
      {
        NaturalResource::Soil,
        GatheringBuildingName::Farm,
        { Biome::Hills },
      },
    },
  },

  {
    RawMaterial::Fish,
    {
      {
        NaturalResource::Fish,
        GatheringBuildingName::Fisherman,
        { Biome::Sea },// TODO this needs to be something next to the sea
      },
    },
  },

  {
    RawMaterial::Clay,
    {
      {
        NaturalResource::Clay,
        GatheringBuildingName::Quarry,
        { Biome::Hills, Biome::Mountains },
      },
    },
  },

  {
    RawMaterial::Stone,
    {
      {
        NaturalResource::Stone,
        GatheringBuildingName::Quarry,
        { Biome::Hills, Biome::Mountains },
      },
    },
  },

  {
    RawMaterial::Wool,
    {
      {
        NaturalResource::Sheep,
        GatheringBuildingName::Shepherd,
        {
          Biome::Hills,
          Biome::Plains,
        },
      },
    },
  },

  {
    RawMaterial::Flax,
    {
      {
        NaturalResource::Soil,
        GatheringBuildingName::Farm,
        {
          Biome::Hills,
          Biome::Plains,
        },
      },
    },
  },

  {
    RawMaterial::Hide,
    {
      {
        NaturalResource::Cattle,
        GatheringBuildingName::Tanner,
        { Biome::Plains },
      },
    },
  },

  {
    RawMaterial::SilverOre,
    {
      {
        NaturalResource::SilverVein,
        GatheringBuildingName::Mine,
        { Biome::Mountains },
      },
    },
  },

  {
    RawMaterial::GoldOre,
    {
      {
        NaturalResource::GoldVein,
        GatheringBuildingName::Mine,
        { Biome::Mountains },
      },
    },
  },

  {
    RawMaterial::IronOre,
    {
      {
        NaturalResource::IronVein,
        GatheringBuildingName::Mine,
        { Biome::Mountains },
      },
    },
  },

  {
    RawMaterial::TinOre,
    {
      {
        NaturalResource::TinVein,
        GatheringBuildingName::Mine,
        { Biome::Mountains },
      },
    },
  },

  {
    RawMaterial::CopperOre,
    {
      {
        NaturalResource::CopperVein,
        GatheringBuildingName::Mine,
        { Biome::Mountains },
      },
    },
  },
};


inline std::map<ProcessedMaterial, std::vector<ProcessedMaterialRecipe>>
  processed_materials = {
    {
      ProcessedMaterial::Lumber,
      {
        {
          { RawMaterial::Timber },
          ProcessingBuildingName::Sawmill,
        },
      },
    },

    {
      ProcessedMaterial::Flour,
      {
        {
          { RawMaterial::Wheat },
          ProcessingBuildingName::Mill,
        },
      },
    },

    {
      ProcessedMaterial::Bricks,
      {
        {
          { RawMaterial::Clay },
          ProcessingBuildingName::Kiln,
        },
      },
    },

    {
      ProcessedMaterial::Bricks,
      {
        {
          { RawMaterial::Clay },
          ProcessingBuildingName::Kiln,
        },
      },
    },

    {
      ProcessedMaterial::StoneBricks,
      {
        {
          { RawMaterial::Stone },
          ProcessingBuildingName::Stonemason,
        },
      },
    },

    {
      ProcessedMaterial::Pottery,
      {
        {
          { RawMaterial::Clay },
          ProcessingBuildingName::Kiln,
        },
      },
    },

    {
      ProcessedMaterial::Cloth,
      {
        {
          { RawMaterial::Wool },
          ProcessingBuildingName::Spinner,
        },
      },
    },

    {
      ProcessedMaterial::Linen,
      {
        {
          { RawMaterial::Flax },
          ProcessingBuildingName::Spinner,
        },
      },
    },

    {
      ProcessedMaterial::Leather,
      {
        {
          { RawMaterial::Hide },
          ProcessingBuildingName::Tanner,
        },
      },
    },

    {
      ProcessedMaterial::Silver,
      {
        {
          { RawMaterial::SilverOre },
          ProcessingBuildingName::Smith,
        },
      },
    },

    {
      ProcessedMaterial::Silver,
      {
        {
          { RawMaterial::SilverOre },
          ProcessingBuildingName::Smith,
        },
      },
    },

    {
      ProcessedMaterial::Gold,
      {
        {
          { RawMaterial::GoldOre },
          ProcessingBuildingName::Smith,
        },
      },
    },

    {
      ProcessedMaterial::Iron,
      {
        {
          { RawMaterial::IronOre },
          ProcessingBuildingName::Smith,
        },
      },
    },

    {
      ProcessedMaterial::Bronze,
      {
        {
          { RawMaterial::TinOre, RawMaterial::CopperOre },
          ProcessingBuildingName::Smith,
        },
      },
    },
};

inline std::map<Product, std::vector<ProductRecipe>> products = {
  {
    Product::OliveOil,
    {
      {
        { RawMaterial::Olives },
        { ProcessedMaterial::Pottery },
        ProductionBuildingName::OlivePress,
      },
    },
  },

  {
    Product::Swords,
    {
      {
        {},
        { ProcessedMaterial::Iron },
        ProductionBuildingName::SwordSmith,
      },
      {
        {},
        { ProcessedMaterial::Bronze },
        ProductionBuildingName::SwordSmith,
      },
    },
  },

  {
    Product::Spears,
    {
      {
        {},
        { ProcessedMaterial::Iron, ProcessedMaterial::Lumber },
        ProductionBuildingName::Poleturner,
      },
      {
        {},
        { ProcessedMaterial::Bronze, ProcessedMaterial::Lumber },
        ProductionBuildingName::Poleturner,
      },
    },
  },

  {
    Product::Shields,
    {
      {
        {},
        { ProcessedMaterial::Lumber },
        ProductionBuildingName::Armourer,
      },
      {
        {},
        { ProcessedMaterial::Lumber, ProcessedMaterial::Leather },
        ProductionBuildingName::Armourer,
      },
      {
        {},
        { ProcessedMaterial::Lumber, ProcessedMaterial::Bronze },
        ProductionBuildingName::Armourer,
      },
    },
  },

  {
    Product::Helmets,
    {
      {
        {},
        { ProcessedMaterial::Iron },
        ProductionBuildingName::Armourer,
      },
      {
        {},
        { ProcessedMaterial::Bronze },
        ProductionBuildingName::Armourer,
      },
    },
  },

  {
    Product::Armour,
    {
      {
        {},
        { ProcessedMaterial::Iron },
        ProductionBuildingName::Armourer,
      },
      {
        {},
        { ProcessedMaterial::Bronze },
        ProductionBuildingName::Armourer,
      },
      {
        {},
        { ProcessedMaterial::Leather },
        ProductionBuildingName::Armourer,
      },
      {
        {},
        { ProcessedMaterial::Linen },
        ProductionBuildingName::Armourer,
      },
    },
  },

  {
    Product::Jewelry,
    {
      {
        { RawMaterial::Gems },
        {},
        ProductionBuildingName::Jeweler,
      },

      {
        {},
        { ProcessedMaterial::Gold },
        ProductionBuildingName::Jeweler,
      },

      {
        {},
        { ProcessedMaterial::Gold },
        ProductionBuildingName::Jeweler,
      },
    },
  },
};

struct GatheringBuilding {
  GatheringBuildingName name;
  std::vector<RawMaterial> materials;

  void Init() {
    for ( auto mat_recipe: raw_materials ) {
      RawMaterial mat = mat_recipe.first;
      for ( auto recipe: mat_recipe.second ) {
        if ( recipe.building == name ) {
          materials.push_back( mat );
        }
      }
    }
  }
};

struct ProcessingBuilding {};

struct ProductionBuilding {};
