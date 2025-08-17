#pragma once

#include "../../../../shared/utils.hpp"
#include "../../../../world/components/settlement_component.hpp"
#include "../../../common.h"
#include "../../../library/resource_icon.hpp"
#include "../../../library/text_button.hpp"
#include "../../../library/texture_button.hpp"

namespace UI {
  enum class Action_SettlementContext {
    None,
    Exit,
    SpawnColonist,
    SpawnArmy,
  };

  inline void buildings_list(list<Buildings::Building>);
  inline void resource_list(map<Resources::Type, u32>);

  inline u32 _tab_index = 0;

  inline Action_SettlementContext settlement_context(
    Settlement::Component *selected_settlement
  ) {
    Action_SettlementContext action = Action_SettlementContext::None;


    const u32 num_tabs = 3;
    Clay_String tabs[num_tabs] = {
      CLAY_STRING("Population"),
      CLAY_STRING("Resources"),
      CLAY_STRING("Buildings"),
    };

    CLAY({
      .id = CLAY_ID("SettlementContext"),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_GROW(),
              .height = CLAY_SIZING_GROW(),
            },
          .padding = CLAY_PADDING_ALL(uint16_t(8 * UI_SCALE)),
          .childGap = 4,
          .childAlignment = {.x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_TOP},
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
      .backgroundColor = COLOR_TRANSPARENT_BLACK,
      .border = border(COLOR_GREY),
    }) {
      CLAY({
        .id = CLAY_ID("SettlementContext::TopBar"),
        .layout =
          {
            .sizing =
              {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_FIT(15, 15 * UI_SCALE),
              },
            .childAlignment =
              {
                .x = CLAY_ALIGN_X_LEFT,
                .y = CLAY_ALIGN_Y_CENTER,
              },
          },
      }) {
        auto cs = cstr_to_cs(selected_settlement->name.c_str());
        text_label(cs, 16);

        CLAY({.id = CLAY_ID("SettlementContext::Tabs")}) {

          for (u32 i = 0; i < num_tabs; i++) {
            Clay_String tab = tabs[i];
            if (text_button_small(tab, tab)) {
              _tab_index = i;
            }
          }
        }

        spacer();

        if (texture_button(CLAY_STRING("Exit"), hstr{"exit.png"}, {15, 15})) {
          action = Action_SettlementContext::Exit;
        }
      }

      switch (_tab_index) {
        case 0: {
          auto dev_lvl = selected_settlement->development;
          const char *dev_lvl_str = Settlement::dev_str(dev_lvl);
          auto dev_cs = cstr_to_cs(dev_lvl_str);
          text_label(dev_cs, 12);

          u32 pop = selected_settlement->population.current;
          const char *pop_txt = u32_to_cstr(pop);
          auto pop_cs = cstr_to_cs(pop_txt);
          text_label(pop_cs, 12);
        } break;
        case 1: {
          resource_list(selected_settlement->resource_quantities);
        } break;
        case 2: {
          buildings_list(selected_settlement->buildings);
        } break;
      }
    }

    return action;
  }

  inline void resource_list(map<Resources::Type, u32> resources) {
    for (const auto &[resource, quantity]: resources) {
      // auto foo = resource.foo

      CLAY({}) {
        const char *resource = Resources::resource_cstr(resource);

        Clay_String cs = Clay_String{
          .length = static_cast<int32_t>(strlen(resource)),
          .chars = resource,
        };


        resource_icon_w_quantity(output.resource, output.quantity, cs, i);
      }
    }
  }

  inline void buildings_list(list<Buildings::Building> buildings) {
    for (u32 i = 0; i < buildings.size(); i++) {
      CLAY({
        .layout =
          {
            .sizing = {.width = CLAY_SIZING_GROW()},
            .childGap = uint16_t(2 * UI_SCALE),
            .childAlignment =
              {
                .x = CLAY_ALIGN_X_CENTER,
                .y = CLAY_ALIGN_Y_CENTER,
              },
          },
      }) {
        texture_label(hstr{"slot.png"}, {32, 32});

        texture_label(hstr{"arrow.png"}, {13, 13});

        const char *label = Buildings::building_name_cstr(buildings[i].type);

        Clay_String cs = Clay_String{
          .length = static_cast<int32_t>(strlen(label)),
          .chars = label,
        };

        texture_w_tooltip(
          CLAY_STRING("SettlementContext::BuildingIcon"),
          cs,
          Buildings::building_icon_path(buildings[i].type),
          {32, 32},
          i
        );

        texture_label(hstr{"arrow.png"}, {13, 13});

        auto recipes = Buildings::recipes_for_building(buildings[i].type);

        auto current_recipe = buildings[i].current_recipe;

        for (const auto &output: current_recipe.outputs) {
          const char *resource = Resources::resource_cstr(output.resource);

          Clay_String cs = Clay_String{
            .length = static_cast<int32_t>(strlen(resource)),
            .chars = resource,
          };


          resource_icon_w_quantity(output.resource, output.quantity, cs, i);
        }
      }
    }
  }
}// namespace UI
