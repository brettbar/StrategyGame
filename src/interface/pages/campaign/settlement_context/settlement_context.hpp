#pragma once

#include "../../../../world/components/settlement.hpp"
#include "../../../irongui/forge.hpp"
#include "construction_tab.hpp"
#include "military_tab.hpp"
#include "population_tab.hpp"

namespace UI {
  enum class Action_SettlementContext {
    None,
    SpawnColonist,
    SpawnArmy,
    BuildFarm,
  };

  inline Action_SettlementContext SettlementContext(
    Settlement::Component *settlement
  ) {
    auto f = Iron::Forge();

    rect root_r = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_g = f->Grid( root_r, 4, 3 );

    auto context_g = f->Grid( root_g->Slots( 9, 10 ), 1, 5 );

    auto header_g = f->Grid( context_g->Row( 0 ), 3, 1, BLACK );
    f->TextLabel( header_g->Slot( 0 ), settlement->name, BLACK );
    f->TextLabel(
      header_g->Slot( 1 ), Settlement::dev_names[settlement->development], BLACK
    );
    f->TextLabel(
      header_g->Slot( 2 ),
      std::to_string( settlement->population.current ),
      BLACK
    );

    auto content_g = f->Grid( context_g->Rows( 1, 5 ), 5, 4 );


    auto tabs_g = f->Grid( content_g->Col( 0 ), 1, 5 );
    auto tab_btns = {
      f->TextButton( tabs_g->Slot( 0 ), "Population", BLUE ),
      f->TextButton( tabs_g->Slot( 1 ), "Construction", YELLOW ),
      f->TextButton( tabs_g->Slot( 2 ), "Resources", GREEN ),
      f->TextButton( tabs_g->Slot( 3 ), "Military", RED ),
      f->TextButton( tabs_g->Slot( 4 ), "Agents", PURPLE ),
    };

    auto tabs = f->Tabs( tab_btns );
    switch ( tabs->t.tabs->current_tab ) {
      case 0: {// Population

        auto population_g = f->Grid( content_g->Cols( 1, 5 ), 3, 1, BLUE );

        auto growth_g =
          f->Grid( population_g->Slot( 0 ), 1, 3, Color{ 0, 0, 100, 255 } );
        auto demographics_g =
          f->Grid( population_g->Slot( 1 ), 1, 3, Color{ 0, 0, 155, 255 } );
        auto public_order_g =
          f->Grid( population_g->Slot( 2 ), 1, 3, Color{ 0, 0, 250, 255 } );


      } break;
      case 1: {
        // Construction
        auto construction_g = f->Grid( content_g->Cols( 1, 5 ), 4, 4, YELLOW );
        auto build_farm =
          f->TextButton( construction_g->Slot( 0 ), "Farm", RED );

        if ( build_farm ) {
          return Action_SettlementContext::BuildFarm;
        }

      } break;
      case 2: {
        // Resources
        auto resources_g = f->Grid( content_g->Cols( 1, 5 ), 4, 4, GREEN );

        // @volatile
        u32 slot = 0;
        for ( const auto resource: settlement->raw_materials ) {
          if ( resource.second > 0 ) {
            f->TextureLabel(
              resources_g->Slot( slot ),
              hstr{
                Resources::GetRawMaterialName( resource.first ).c_str(),
              }
            );
          }
        }


      } break;
      case 3: {
        // Military
        auto military_g = f->Grid( content_g->Cols( 1, 5 ), 3, 1, RED );

        {
          auto recruitment_g =
            f->Grid( military_g->Slot( 0 ), 1, 3, Color{ 100, 0, 0, 255 } );

          f->TextLabel( recruitment_g->Slot( 0 ), "Recruitment", GREEN );

          auto spawn_hastati = f->TextureButton(
            recruitment_g->Slot( 1 ), "romans_hastati_overview", 2.0f, BLACK
          );

          if ( spawn_hastati ) {
            return Action_SettlementContext::SpawnArmy;
          }
        }


        auto training_g =
          f->Grid( military_g->Slot( 1 ), 1, 3, Color{ 155, 0, 0, 255 } );
        auto garrison_g =
          f->Grid( military_g->Slot( 2 ), 1, 3, Color{ 250, 0, 0, 255 } );

      } break;
      case 4: {
        // Agents
        auto agents_g = f->Grid( content_g->Cols( 1, 5 ), 4, 4, PURPLE );
        auto spawn_colonist = f->TextureButton(agents_g->Slot(0), "romans_colonist_overview", 3.0f, BLACK);
        auto spawn_army = f->TextButton( agents_g->Slot( 1 ), "Army", GREEN );

        if ( spawn_colonist) {
          return Action_SettlementContext::SpawnColonist;
        }

        if ( spawn_army ) {
          return Action_SettlementContext::SpawnArmy;
        }
      } break;
    }

    return Action_SettlementContext::None;
  }
}// namespace UI
