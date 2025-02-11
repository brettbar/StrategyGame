#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"
#include "../../shared/textures.hpp"

#include "../managers/settlement_manager.hpp"

#include "../components/faction_component.hpp"
#include "../components/province_component.hpp"
#include "../components/settlement_component.hpp"

#include "selection_system.hpp"


namespace Settlement {

  class System {

public:
    static void update_1tps() {
      auto settlements =
        Global::world.view<Province::Component, Settlement::Component>();

      for ( entt::entity entity: settlements ) {
        auto &settlement = Global::world.get<Settlement::Component>( entity );

        update_settlement( settlement );
        update_resources( settlement );
      }
    }

    static void draw( TextureCache &cache, bool showOverlays ) {
      auto settlements =
        Global::world.view<Province::Component, Settlement::Component>();

      for ( auto entity: settlements ) {
        auto &province = settlements.get<Province::Component>( entity );
        auto &settlement = settlements.get<Settlement::Component>( entity );

        // str idString = std::to_string(tile.id);
        // const char *idText = idString.c_str();
        // if (debug)
        //   DrawText(idText, tile.position.x + 16.0, tile.position.y + 16.0,
        //   14
        //            WHITE);

        // str ownerString = std::to_string(tile.owner);
        // const char *ownertext = ownerString.c_str();
        // DrawText(ownertext, tile.position.x + 48.0, tile.position.y + 16.0,
        // 14,
        //          BLUE);

        //       str coordString = std::to_string((u32)tile.coord.x) + "," +
        //                         std::to_string((u32)tile.coord.y);
        //       const char *coordText = coordString.c_str();
        ////       if (.debug)
        //         DrawText(coordText, tile.position.x + 16.0, tile.position.y + 16.0, 14,
        //                  BLUE);

        // str popString = std::to_string(tile.population);
        // const char *text = popString.c_str();
        // if (debug)
        //   DrawText(text, tile.position.x + 16.0, tile.position.y + 32.0,
        //   14,
        //            RED);

        // if ( prov.owner <= -1 || settlement.population.current <= 0 )
        //   continue;


        // // DrawRectangleRec({provPos.x + 50,
        // //                   provPos.y + 86, 128, 64},
        // //                  Fade(WHITE, 0.8f));
        // // DrawSingleBorder(tile);

        // // Draw Settlement
        DrawTextureV(
          settlement.texture, settlement_position( province ), WHITE
        );
      }
    }

    static vec2f settlement_position( Province::Component province ) {
      return vec2f{
        province.tile->position.x + 24,
        province.tile->position.y + 24,
      };
    }

    static void spawn_settlement( entt::entity colonist_e ) {
      Actor::Component unit = Global::world.get<Actor::Component>( colonist_e );

      vec2f pos = unit.position;
      i32 closest_tile = DetermineTileIdFromPosition( pos );

      if ( closest_tile == -1 )
        return;

      for ( auto entity: Global::world.view<Province::Component>() ) {
        auto &prov = Global::world.get<Province::Component>( entity );

        // TODO pretty sure I am checking this twice, another time in the Actor colonist area
        if ( prov.tile->id == closest_tile && prov.tile->owner == unit.owner ) {
          if ( !Global::world.any_of<Settlement::Component>( entity ) ) {
            printf( "spawning settlement\n" );

            Faction::Component faction =
              Global::world.get<Faction::Component>( unit.owner );

            Settlement::Component settlement = {
              .name = faction.culture.settlement_names[0],
              .development = Settlement::Development::Village,
              .population =
                {
                  .current = 200,
                  .birthRate = 40,
                  .deathRate = 10,
                  .growthRate = ( 40.0f - 10.0f ) / 200,
                  .carryingCapacity = 1000,
                },
              .texture =
                LoadTextureFromImage( Manager()->building_map.at( "roman_m1" )
                ),
            };

            Global::world.emplace<Settlement::Component>( entity, settlement );

            Global::world.destroy( colonist_e );
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
    //     .name = Buildings::BuildingName::Farm,
    //     .type = Buildings::Type::Gathering,
    //     .name_str = building_name,
    //   } );
    // }

    static void construct_building(
      entt::entity settlement_e,
      Buildings::BuildingName building,
      std::string building_name
    ) {
      Settlement::Component &settlement =
        Global::world.get<Settlement::Component>( settlement_e );

      settlement.buildings.push_back( Buildings::Building{
        .name = building,
        .name_str = building_name,
      } );
    }


    static bool player_has_settlement( entt::entity owner ) {
      auto settlements =
        Global::world.view<Province::Component, Settlement::Component>();

      for ( auto settlement_e: settlements ) {
        auto prov = settlements.get<Province::Component>( settlement_e );
        if ( prov.tile->owner == owner ) {
          // printf( "AI HAS A SETTLEMENT!!!!!!!!!!!!!!!!!!\n" );
          return true;
        }
      }

      return false;
    }

private:
    static void update_settlement( Settlement::Component &settlement ) {
      bool needs_sprawl_update = update_population( settlement );

      // Update Sprawl
      if ( !needs_sprawl_update )
        return;

      printf( "Sprawl increase at %d\n", settlement.population.current );


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
        LoadTextureFromImage( Manager()->building_map.at( "roman_m1" ) );
    }

    static void update_resources( Settlement::Component &settlement ) {
      for ( Buildings::Building &building: settlement.buildings ) {
        auto recipes = Buildings::recipes_for_building( building.name );
        auto current_recipe = recipes[building.current_recipe];

        for ( const auto &recipe_item: current_recipe.outputs ) {
          settlement.resources[recipe_item.resource] = recipe_item.quantity;
        }
      }
    }

    static bool update_population( Settlement::Component &settlement ) {
      Settlement::Population &pop = settlement.population;

      int before = settlement.population.current / 100;

      // if growing exponentially
      // P(t) = P0*e^(kt)

      // dP/dt = r * P ( 1 - P/K )
      // dP/dt = r * P ( K - P ) / K
      pop.growthRate = ( pop.birthRate - pop.deathRate ) / pop.current;
      f32 dP_over_dt = pop.growthRate * pop.current *
                       ( pop.carryingCapacity - pop.current ) /
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

      if ( before < after ) {
        return true;
      }

      return false;
    }


    void TrainRegiment( UnitType type ) {
      Settlement::Component &settlement =
        Global::world.get<Settlement::Component>( Selection::GetSelectedEntity()
        );

      settlement.garrison.push_back( Regiment{
        (u32) settlement.garrison.size(),
        Hastati,
        100,
      } );
    }


    Settlement::Component ReadSelectedComponent() {
      return Global::world.get<Settlement::Component>(
        Selection::GetSelectedEntity()
      );
    }


    std::vector<Buildings::Building> SelectedSettlementBuildingList() {
      Settlement::Component *settlement =
        Global::world.try_get<Settlement::Component>(
          Selection::GetSelectedEntity()
        );

      if ( settlement == nullptr ) {
        return {};
      }

      return settlement->buildings;
    }

    std::vector<Regiment> SelectedSettlementGarrisonList() {
      Settlement::Component *settlement =
        Global::world.try_get<Settlement::Component>(
          Selection::GetSelectedEntity()
        );

      if ( settlement == nullptr ) {
        return {};
      }

      return settlement->garrison;
    }
  };


};// namespace Settlement
