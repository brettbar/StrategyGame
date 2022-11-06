#pragma once

#include "buildings.hpp"

namespace Resource {

enum class RawMaterial {
  // Raw Materials
  Timber,
  Wheat,
  Olives,
  Fish,
  Clay,
  Wool,
  Flax,
  Hide,
  SilverOre,
  GoldOre,
  IronOre,
  TinOre,
  CopperOre,
};

enum class ProcessedMaterial {
  // Processed Materials
  Lumber,
  Flour,
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
  // Final Products
  OliveOil,
  Swords,
  Spears,
  Shields,
  Helmets,
  Armor,
  Bows,
};

// struct RefinedRecipe {
//   Buildings::Refining produced_in;
//   std::vector<RawMaterial> raw_mats_needed;
// };

// struct ProductRecipe {
//   Buildings::Production produced_in;
//   std::vector<RawMaterial> raw_mats_needed;
//   std::vector<RefinedMaterial> refined_mats_needed;
// };

/*
  Map<RawMaterial, Building[]> raw_recipes
*/


// inline std::map<RawMaterial, Buildings::Gathering> RAW_MAT_SOURCES = {
//   {
//     RawMaterial::Timber,
//     Buildings::Gathering::WoodCutter,
//   },
//   { RawMaterial::Wheat, Buildings::Gathering::WheatFarm },
//   { RawMaterial::Olives, Buildings::Gathering::OliveGrove },
//   { RawMaterial::Fish, Buildings::Gathering::FishingDock },
//   { RawMaterial::Clay, Buildings::Gathering::Mine },
//   { RawMaterial::Wool, Buildings::Gathering::SheepRanch },
//   { RawMaterial::Flax, Buildings::Gathering::FlaxFarm },
//   { RawMaterial::Hide, Buildings::Gathering::CowFarm },
//   { RawMaterial::SilverOre, {} },
//   { RawMaterial::GoldOre, {} },
//   { RawMaterial::IronOre, {} },
//   { RawMaterial::TinOre, {} },
//   { RawMaterial::CopperOre, {} },

// };

// inline std::map<RefinedMaterial, RefinedRecipe> REFINED_RECIPES = {
//   {
//     RefinedMaterial::Lumber,
//     { Buildings::Refining::SawMill, { RawMaterial::Timber } },
//   },
//   {
//     RefinedMaterial::Flour,
//     { Buildings::Refining::Mill, { RawMaterial::Wheat } },
//   },
//   {
//     RefinedMaterial::Pottery,
//     { Buildings::Refining::Ovens, { RawMaterial::Clay } },
//   },
//   {
//     RefinedMaterial::Cloth,
//     { Buildings::Refining::Clothier, { RawMaterial::Wool } },
//   },
//   {
//     RefinedMaterial::Linen,
//     { Buildings::Refining::Clothier, { RawMaterial::Flax } },
//   },
//   {
//     RefinedMaterial::Leather,
//     { Buildings::Refining::Tanner, { RawMaterial::Hide } },
//   },
//   {
//     RefinedMaterial::Silver,
//     { Buildings::Refining::Smelter, { RawMaterial::SilverOre } },
//   },
//   {
//     RefinedMaterial::Gold,
//     { Buildings::Refining::Smelter, { RawMaterial::GoldOre } },
//   },
//   {
//     RefinedMaterial::Iron,
//     { Buildings::Refining::Smelter, { RawMaterial::IronOre } },
//   },
//   {
//     RefinedMaterial::Bronze,
//     {
//       Buildings::Refining::Smelter,
//       {
//         RawMaterial::TinOre,
//         RawMaterial::CopperOre,
//       },
//     },
//   },
// };

// inline std::map<Product, ProductRecipe> PRODUCT_RECIPES = {};


};// namespace Resource
