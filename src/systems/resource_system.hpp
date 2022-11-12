#pragma once

#include "../data/resources.hpp"

#include <map>

namespace ResourceSystem {

inline std::map<RawMaterial, std::vector<RawMaterialRecipe>> raw_materials = {
  {
    RawMaterial::Timber,
    {
      {
        NaturalResource::Trees,
        GatheringBuilding::WoodCutter,
        { Biome::Forest },
      },
    },
  },

  {
    RawMaterial::Wheat,
    {
      {
        NaturalResource::Soil,
        GatheringBuilding::Farm,
        { Biome::Plains },
      },
    },
  },

  {
    RawMaterial::Olives,
    {
      {
        NaturalResource::Soil,
        GatheringBuilding::Farm,
        { Biome::Hills },
      },
    },
  },

  {
    RawMaterial::Fish,
    {
      {
        NaturalResource::Fish,
        GatheringBuilding::Fisherman,
        { Biome::Sea },// TODO this needs to be something next to the sea
      },
    },
  },

  {
    RawMaterial::Clay,
    {
      {
        NaturalResource::Clay,
        GatheringBuilding::Quarry,
        { Biome::Hills, Biome::Mountains },
      },
    },
  },

  {
    RawMaterial::Stone,
    {
      {
        NaturalResource::Stone,
        GatheringBuilding::Quarry,
        { Biome::Hills, Biome::Mountains },
      },
    },
  },

  {
    RawMaterial::Wool,
    {
      {
        NaturalResource::Sheep,
        GatheringBuilding::Shepherd,
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
        GatheringBuilding::Farm,
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
        GatheringBuilding::Tanner,
        { Biome::Plains },
      },
    },
  },

  {
    RawMaterial::SilverOre,
    {
      {
        NaturalResource::SilverVein,
        GatheringBuilding::Mine,
        { Biome::Mountains },
      },
    },
  },

  {
    RawMaterial::GoldOre,
    {
      {
        NaturalResource::GoldVein,
        GatheringBuilding::Mine,
        { Biome::Mountains },
      },
    },
  },

  {
    RawMaterial::IronOre,
    {
      {
        NaturalResource::IronVein,
        GatheringBuilding::Mine,
        { Biome::Mountains },
      },
    },
  },

  {
    RawMaterial::TinOre,
    {
      {
        NaturalResource::TinVein,
        GatheringBuilding::Mine,
        { Biome::Mountains },
      },
    },
  },

  {
    RawMaterial::CopperOre,
    {
      {
        NaturalResource::CopperVein,
        GatheringBuilding::Mine,
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
          ProcessingBuilding::Sawmill,
        },
      },
    },

    {
      ProcessedMaterial::Flour,
      {
        {
          { RawMaterial::Wheat },
          ProcessingBuilding::Mill,
        },
      },
    },

    {
      ProcessedMaterial::Bricks,
      {
        {
          { RawMaterial::Clay },
          ProcessingBuilding::Kiln,
        },
      },
    },

    {
      ProcessedMaterial::Bricks,
      {
        {
          { RawMaterial::Clay },
          ProcessingBuilding::Kiln,
        },
      },
    },

    {
      ProcessedMaterial::StoneBricks,
      {
        {
          { RawMaterial::Stone },
          ProcessingBuilding::Stonemason,
        },
      },
    },

    {
      ProcessedMaterial::Pottery,
      {
        {
          { RawMaterial::Clay },
          ProcessingBuilding::Kiln,
        },
      },
    },

    {
      ProcessedMaterial::Cloth,
      {
        {
          { RawMaterial::Wool },
          ProcessingBuilding::Spinner,
        },
      },
    },

    {
      ProcessedMaterial::Linen,
      {
        {
          { RawMaterial::Flax },
          ProcessingBuilding::Spinner,
        },
      },
    },

    {
      ProcessedMaterial::Leather,
      {
        {
          { RawMaterial::Hide },
          ProcessingBuilding::Tanner,
        },
      },
    },

    {
      ProcessedMaterial::Silver,
      {
        {
          { RawMaterial::SilverOre },
          ProcessingBuilding::Smith,
        },
      },
    },

    {
      ProcessedMaterial::Silver,
      {
        {
          { RawMaterial::SilverOre },
          ProcessingBuilding::Smith,
        },
      },
    },

    {
      ProcessedMaterial::Gold,
      {
        {
          { RawMaterial::GoldOre },
          ProcessingBuilding::Smith,
        },
      },
    },

    {
      ProcessedMaterial::Iron,
      {
        {
          { RawMaterial::IronOre },
          ProcessingBuilding::Smith,
        },
      },
    },

    {
      ProcessedMaterial::Bronze,
      {
        {
          { RawMaterial::TinOre, RawMaterial::CopperOre },
          ProcessingBuilding::Smith,
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
        ProductionBuilding::OlivePress,
      },
    },
  },

  {
    Product::Swords,
    {
      {
        {},
        { ProcessedMaterial::Iron },
        ProductionBuilding::SwordSmith,
      },
      {
        {},
        { ProcessedMaterial::Bronze },
        ProductionBuilding::SwordSmith,
      },
    },
  },

  {
    Product::Spears,
    {
      {
        {},
        { ProcessedMaterial::Iron, ProcessedMaterial::Lumber },
        ProductionBuilding::Poleturner,
      },
      {
        {},
        { ProcessedMaterial::Bronze, ProcessedMaterial::Lumber },
        ProductionBuilding::Poleturner,
      },
    },
  },

  {
    Product::Shields,
    {
      {
        {},
        { ProcessedMaterial::Lumber },
        ProductionBuilding::Armourer,
      },
      {
        {},
        { ProcessedMaterial::Lumber, ProcessedMaterial::Leather },
        ProductionBuilding::Armourer,
      },
      {
        {},
        { ProcessedMaterial::Lumber, ProcessedMaterial::Bronze },
        ProductionBuilding::Armourer,
      },
    },
  },

  {
    Product::Helmets,
    {
      {
        {},
        { ProcessedMaterial::Iron },
        ProductionBuilding::Armourer,
      },
      {
        {},
        { ProcessedMaterial::Bronze },
        ProductionBuilding::Armourer,
      },
    },
  },

  {
    Product::Armour,
    {
      {
        {},
        { ProcessedMaterial::Iron },
        ProductionBuilding::Armourer,
      },
      {
        {},
        { ProcessedMaterial::Bronze },
        ProductionBuilding::Armourer,
      },
      {
        {},
        { ProcessedMaterial::Leather },
        ProductionBuilding::Armourer,
      },
      {
        {},
        { ProcessedMaterial::Linen },
        ProductionBuilding::Armourer,
      },
    },
  },

  {
    Product::Jewelry,
    {
      {
        { RawMaterial::Gems },
        {},
        ProductionBuilding::Jeweler,
      },

      {
        {},
        { ProcessedMaterial::Gold },
        ProductionBuilding::Jeweler,
      },

      {
        {},
        { ProcessedMaterial::Gold },
        ProductionBuilding::Jeweler,
      },
    },
  },


};


// struct Really {
//   std::string resource;
//   std::string value;

//   template<class Archive>
//   void serialize( Archive &ar ) {
//     ar( CEREAL_NVP( resource ) );
//     CEREAL_NVP( value );
//   }
// };
// https://uscilab.github.io/cereal/serialization_archives.html
// https://stackoverflow.com/questions/64865338/cereal-unable-to-deserialize-a-json-string-unhandled-exception
// void epilogue( cereal::JSONInputArchive &, const Really & ) {}
// void prologue( cereal::JSONInputArchive &, const Really & ) {}
// inline void LoadData() {
//   std::ifstream file( "data/really.json" );
//   cereal::JSONInputArchive input{ file };

//   Really resources;
//   input( resources );

//   std::cout << "SUCCESS INPUT" << std::endl;
//   cereal::JSONOutputArchive pretty_print( std::cout );
//   pretty_print( resources );
// }

};// namespace ResourceSystem
