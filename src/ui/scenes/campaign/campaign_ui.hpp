#pragma once

#include "../../common.h"
#include "context/actor_context.hpp"
#include "context/settlement_context.hpp"
#include "minimap.hpp"
#include "overview_panel/overview_content.hpp"
#include "overview_panel/overview_tabs.hpp"
#include "overview_panel/top_bar.hpp"

namespace UI {

  inline Commands::Command settlement_context(entt::entity player_e) {
    auto settlement =
      Selection::System::GetSelectedComponent<Settlement::Component>();
    auto prov = Selection::System::GetSelectedComponent<Province::Component>();
    // entt::entity player_e = GetLocalPlayerE();
    auto faction = Global::world.get<Faction::Component>(player_e);

    // if ( !settlement || !prov ) {
    //   printf( "Got null prov/settlement??\n" );
    //   return nullptr;
    // }

    auto action = UI::settlement_context(settlement);
    switch (action) {
      case UI::Action_SettlementContext::None:
        break;
      case UI::Action_SettlementContext::SpawnColonist:
        return Commands::Command::spawn_colonist(
          player_e, Settlement::System::settlement_position(*prov)
        );
        break;
      case UI::Action_SettlementContext::SpawnArmy:
        return Commands::Command::spawn_army(
          player_e, Settlement::System::settlement_position(*prov)
        );
        break;
        // case UI::Action_SettlementContext::BuildFarm:
        //   PostCommand( Commands::Command::construct_building(
        //     Selection::GetSelectedEntity(), "farm"
        //   ) );
        //   break;
    }

    return Commands::Command::none();
  }
  inline Commands::Command actor_context() {
    auto actor = Selection::System::GetSelectedComponent<Actor::Component>();
    // entt::entity player_e = GetLocalPlayerE();

    // if ( !actor ) {
    //   printf( "Got null actor??\n" );
    //   return;
    // }

    // auto action = UI::ActorContext( actor );
    auto action = UI::actor_context(actor);

    switch (action) {
      case UI::Action_ActorContext::None:
        break;
      case UI::Action_ActorContext::ClaimProvince: {
        return Commands::Command::claim_province(
          Selection::System::GetSelectedEntity()
        );
      } break;
      case UI::Action_ActorContext::SpawnSettlement: {
        return Commands::Command::build_settlement(
          Selection::System::GetSelectedEntity()
        );
      } break;
    }

    return Commands::Command::none();
  }

  enum class Action_Campaign_t {
    None,
    Command,
    SelectBuilding
  };

  struct Action_Campaign {
    Action_Campaign_t type;
    Commands::Command cmd;
    Buildings::BuildingType building;
  };

  inline Action_Campaign campaign_ui(entt::entity player_e) {

    // @todo refactor this shit
    opt<Buildings::BuildingType> building_to_build = std::nullopt;
    Commands::Command cmd = Commands::Command::none();

    CLAY({
      .id = CLAY_ID("Campaign"),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_GROW(),
              .height = CLAY_SIZING_GROW(),
            },
          // .padding = { 2, 2, 2, 2 },
          .childAlignment =
            {
              .x = CLAY_ALIGN_X_LEFT,
              .y = CLAY_ALIGN_Y_TOP,
            },
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
    }) {
      CLAY({
        .id = CLAY_ID("Campaign::TopRow"),
        .layout =
          {
            .sizing =
              {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_PERCENT(0.6),
              },
            .childAlignment =
              {
                .x = CLAY_ALIGN_X_LEFT,
                .y = CLAY_ALIGN_Y_TOP,
              },
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
          },
      }) {
        tab = UI::overview_tabs();

        CLAY({
          .id = CLAY_ID("Campaign::TopRow::LeftCol"),
          .layout =
            {
              .sizing =
                {
                  .width = CLAY_SIZING_GROW(),
                  .height = CLAY_SIZING_GROW(),
                },
              // .childGap = 8,
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
        }) {
          UI::top_bar(1, 0);

          if (std::string(tab.chars) != "") {
            UI::OverviewAction action = UI::overview_content(tab);

            switch (action.type) {
              case UI::OverviewAction_t::None:
                break;
              case UI::OverviewAction_t::Construction:
                building_to_build = action.building;
                Map::Manager()->mode = Map::Mode::BuildPreview;
                break;
            }
          }
        }
      }

      CLAY({
        .id = CLAY_ID("Campaign::BottomRow"),
        .layout =
          {
            .sizing =
              {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_GROW(),
              },
            .childAlignment =
              {
                .x = CLAY_ALIGN_X_CENTER,
                .y = CLAY_ALIGN_Y_TOP,
              },
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
          },
      }) {
        CLAY({
          .id = CLAY_ID("Campaign::BottomRow::LeftCol"),
          .layout =
            {
              .sizing =
                {
                  .width = CLAY_SIZING_PERCENT(0.33),
                  .height = CLAY_SIZING_GROW(),
                },
              .childAlignment =
                {
                  .y = CLAY_ALIGN_Y_BOTTOM,
                },
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
        }) {
          if (Selection::System::
                Selected<Province::Component, Settlement::Component>()) {
            cmd = settlement_context(player_e);
          }

          if (Selection::System::Selected<Actor::Component>()) {
            cmd = actor_context();
          }
        }

        UI::spacer();

        CLAY({
          .id = CLAY_ID("Campaign::RightCol"),
          .layout =
            {
              .sizing =
                {.width = CLAY_SIZING_PERCENT(0.33),
                 .height = CLAY_SIZING_GROW()},
              .childAlignment =
                {
                  .x = CLAY_ALIGN_X_RIGHT,
                  .y = CLAY_ALIGN_Y_BOTTOM,
                },
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
        }) {

          switch (UI::minimap()) {
            case UI::Action_Minimap::Default:
              Map::Manager()->mode = Map::Mode::Default;
              break;
            case UI::Action_Minimap::Terrain:
              Map::Manager()->mode = Map::Mode::Terrain;
              break;
            case UI::Action_Minimap::Political:
              Map::Manager()->mode = Map::Mode::Political;
              break;
            case UI::Action_Minimap::Resources:
              Map::Manager()->mode = Map::Mode::Resources;
              break;
            default:
              break;
          }
        }
      }
    }

    if (cmd.type != Commands::Type::None) {
      return Action_Campaign{.type = Action_Campaign_t::Command, .cmd = cmd};
    } else if (building_to_build.has_value()) {
      return Action_Campaign{
        .type = Action_Campaign_t::SelectBuilding,
        .building = building_to_build.value()
      };
    }

    return Action_Campaign{.type = Action_Campaign_t::None};
  }
}// namespace UI
