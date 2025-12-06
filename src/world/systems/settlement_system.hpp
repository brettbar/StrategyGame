#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"
#include "../../shared/textures.hpp"

#include "../managers/settlement_manager.hpp"

#include "../components/faction_component.hpp"
#include "../components/province_component.hpp"
#include "../components/settlement_component.hpp"
#include "../components/stockpile_component.hpp"

#include "selection_system.hpp"


namespace Settlement {

struct System {
  static void update_1tps() {
    auto settlements =
      Global::world.view<Province::Component, Settlement::Component>();

    for (entt::entity entity: settlements) {
      auto &settlement = settlements.get<Settlement::Component>(entity);

      update_settlement(settlement);
      update_resources(settlement);
    }
  }

  static void draw(TextureCache &cache, bool showOverlays) {
    auto settlements =
      Global::world.view<Province::Component, Settlement::Component>();

    for (auto entity: settlements) {
      auto &province = settlements.get<Province::Component>(entity);
      auto &settlement = settlements.get<Settlement::Component>(entity);

      DrawTextureV(settlement.texture, settlement_position(province), WHITE);
    }
  }

  static vec2f settlement_position(Province::Component province) {
    return vec2f{
      province.tile.top_left_corner_position.x + 24,
      province.tile.top_left_corner_position.y + 24,
    };
  }

  static void build_settlement(entt::entity colonist_e) {
    Actor::Component unit = Global::world.get<Actor::Component>(colonist_e);

    vec2f pos = unit.position;
    i32 closest_tile = DetermineTileIdFromPosition(pos);

    if (closest_tile == -1)
      return;

    for (auto entity: Global::world.view<Province::Component>()) {
      auto &prov = Global::world.get<Province::Component>(entity);

      // TODO pretty sure I am checking this twice, another time in the Actor colonist area
      if (prov.tile.id == closest_tile && prov.owner == unit.owner) {
        if (!Global::world.any_of<Settlement::Component>(entity)) {
          printf("spawning settlement\n");

          Faction::Component faction =
            Global::world.get<Faction::Component>(unit.owner);

          Settlement::Component settlement = {
            .name = faction.culture.settlement_names[0],
            .development = Settlement::Development::Village,
            .population =
              {
                .current = 200,
                .birthRate = 40,
                .deathRate = 10,
                .growthRate = (40.0f - 10.0f) / 200,
                .carryingCapacity = 1000,
              },
            .texture =
              LoadTextureFromImage(Manager()->building_map.at("roman_m1")),
          };

          Global::world.emplace<Settlement::Component>(entity, settlement);

          Stockpile::Component &stockpile =
            Global::world.get<Stockpile::Component>(unit.owner);

          // @todo actually check what resources we used to build the settlement
          stockpile.resources[Resources::Type::Bricks] -= 10;

          Global::world.destroy(colonist_e);
          return;
        }
      }
    }
  }

  // static void construct_building( std::string building_name ) {
  //   Settlement::Component &settlement =
  //     Global::world.get<Settlement::Component>( Selection::GetSelectedEntity()
  //     );
  //
  //   settlement.buildings.push_back( Buildings::Building{
  //     .name = Buildings::BuildingType::Farm,
  //     .type = Buildings::Type::Gathering,
  //     .name_str = building_name,
  //   } );
  // }

  static bool has_available_building_slots(Settlement::Component settlement) {
    return settlement.max_building_slots > settlement.buildings.size();
  }

  static bool can_build_immediately(
    Province::Component prov,
    Settlement::Component settlement,
    Buildings::Building building
  ) {
    // @todo will add additional criteria later
    return has_available_building_slots(settlement) &&
           in_eligible_biome(prov, settlement, building);
  }

  static bool can_build_with_changes_needed(Settlement::Component settlement) {
    // @todo
    return false;
  }

  static bool in_eligible_biome(
    Province::Component prov,
    Settlement::Component settlement,
    Buildings::Building building
  ) {
    auto prov_biome = prov.tile.biome;

    auto eligible_biomes =
      Buildings::eligible_biomes_for_building(building.type);

    for (const auto biome: eligible_biomes) {
      if (biome == prov_biome) {
        return true;
      }
    }

    return false;
  }

  static void construct_building(
    entt::entity settlement_e,
    Buildings::Building building
  ) {
    Settlement::Component &settlement =
      Global::world.get<Settlement::Component>(settlement_e);

    auto recipes = Buildings::recipes_for_building(building.type);


    // u32 index = settlement.[building.output];
    // u32 index = 0;

    // printf(
    //   "You want me to make %d, so I think that is index %d\n",
    //   building.current_recipe.outputs[0],
    //   index
    // );


    settlement.buildings.push_back(
      Buildings::Building{
        .type = building.type, .current_recipe = building.current_recipe
      }
    );
  }


  static bool player_has_settlement(entt::entity owner) {
    auto settlements =
      Global::world.view<Province::Component, Settlement::Component>();

    for (auto settlement_e: settlements) {
      auto prov = settlements.get<Province::Component>(settlement_e);
      if (prov.owner == owner) {
        return true;
      }
    }

    return false;
  }

  static u32 num_player_settlements(entt::entity owner) {
    auto settlements =
      Global::world.view<Province::Component, Settlement::Component>();

    u32 num = 0;

    for (auto settlement_e: settlements) {
      auto prov = settlements.get<Province::Component>(settlement_e);
      if (prov.owner == owner) {
        num++;
      }
    }

    return num;
  }

  static vec2f position_of_a_player_settlement(entt::entity owner) {

    auto settlements =
      Global::world.view<Province::Component, Settlement::Component>();


    for (auto settlement_e: settlements) {
      auto prov = settlements.get<Province::Component>(settlement_e);
      if (prov.owner == owner) {
        return settlement_position(prov);
      }
    }

    return {};
  }

  private:
  static void update_settlement(Settlement::Component &settlement) {
    bool needs_sprawl_update = update_population(settlement);

    // Update Sprawl
    if (!needs_sprawl_update)
      return;

    printf("Sprawl increase at %d\n", settlement.population.current);


    // TODO determine development from population
    settlement.development = Settlement::Development::Village;


    // Image base = LoadImageFromTexture( settlement.texture );


    // for ( u32 i = 0; i < ( settlement.population.current / 100 ); i++ ) {
    //   ImageDraw(
    //     &base,
    //     building_map.at( "roman_s3" ),
    //     { 0, 0, 16, 16 },
    //     { ( i * 8.0f ), 0, 16, 16 },
    //     WHITE );
    // }

    // TODO maybe expensive?
    settlement.texture =
      LoadTextureFromImage(Manager()->building_map.at("roman_m1"));
  }

  static void update_resources(Settlement::Component &settlement) {
    for (Buildings::Building &building: settlement.buildings) {
      auto recipes = Buildings::recipes_for_building(building.type);
      auto current_recipe = building.current_recipe;

      for (const auto &recipe_item: current_recipe.outputs) {
        settlement.resource_quantities[recipe_item.resource] =
          recipe_item.quantity;
      }
    }
  }

  static bool update_population(Settlement::Component &settlement) {
    Settlement::Population &pop = settlement.population;

    int before = settlement.population.current / 100;

    // if growing exponentially
    // P(t) = P0*e^(kt)

    // dP/dt = r * P ( 1 - P/K )
    // dP/dt = r * P ( K - P ) / K
    pop.growthRate = (pop.birthRate - pop.deathRate) / pop.current;
    f32 dP_over_dt = pop.growthRate * pop.current *
                     (pop.carryingCapacity - pop.current) /
                     pop.carryingCapacity;

    settlement.population.current += (i32) dP_over_dt;


    // // Find total number of digits - 1
    // int digits = (int) log10( before );

    // // Find first digit
    // before = (int) ( before / pow( 10, digits ) );

    // digits = (int) log10( settlement.population.current );
    // int after = (int) ( settlement.population.current / pow( 10, digits ) );
    int after = settlement.population.current / 100;

    // InterfaceUpdate::Update{
    //   .id = InterfaceUpdate::ID::SettlementPopUpdated,
    //   .condition = true,
    //   .json =
    //     {
    //       { "name", settlement.name },
    //       { "population", std::to_string( settlement.population.current ) },
    //       { "development", Settlement::dev_names.at( settlement.development ) },
    //     },
    // }
    //   .Send();

    if (before < after) {
      return true;
    }

    return false;
  }


  void TrainRegiment(UnitType type) {
    Settlement::Component &settlement =
      Global::world.get<Settlement::Component>(
        Selection::System::GetSelectedEntity()
      );

    settlement.garrison.push_back(
      Regiment{
        (u32) settlement.garrison.size(),
        Hastati,
        100,
      }
    );
  }


  Settlement::Component ReadSelectedComponent() {
    return Global::world.get<Settlement::Component>(
      Selection::System::GetSelectedEntity()
    );
  }


  std::vector<Buildings::Building> SelectedSettlementBuildingList() {
    Settlement::Component *settlement =
      Global::world.try_get<Settlement::Component>(
        Selection::System::GetSelectedEntity()
      );

    if (settlement == nullptr) {
      return {};
    }

    return settlement->buildings;
  }

  std::vector<Regiment> SelectedSettlementGarrisonList() {
    Settlement::Component *settlement =
      Global::world.try_get<Settlement::Component>(
        Selection::System::GetSelectedEntity()
      );

    if (settlement == nullptr) {
      return {};
    }

    return settlement->garrison;
  }
};


};// namespace Settlement
