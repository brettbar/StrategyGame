#pragma once

#include "../../../../../../data/buildings.hpp"
#include "../../../../common.h"
#include "../../../../library/resource_icon.hpp"
#include "../../../../library/text_button.hpp"
#include "../../../../library/text_label.hpp"
#include "../../../../library/texture_button.hpp"


namespace UI {

  enum class Action_ConstructionPreview_t {
    None,
    Back,
    Build
  };

  struct Action_ConstructionPreview {
    Action_ConstructionPreview_t type;
    Buildings::Recipe recipe;
  };

  inline opt<Buildings::Recipe> selected_recipe = std::nullopt;

  inline bool construction_preview_top_row(Buildings::Building building) {
    bool hit_back = false;

    CLAY({
      .id = CLAY_ID("Construction::Preview::TopRow"),
      .layout = {.childGap = 8},
    }) {

      if (texture_button(
            CLAY_STRING("Construction::Preview::Back"),
            hstr{"back_button.png"},
            {15, 15}
          )) {
        hit_back = true;
        selected_recipe = std::nullopt;
      }

      CLAY({
        .layout =
          {
            .childGap = 8,
            .childAlignment =
              {
                .y = CLAY_ALIGN_Y_CENTER,
              },
          },
      }) {

        texture_label(hstr{building.path.c_str()}, {32, 32});

        text_label(building.label, 16);
      }
    }

    return hit_back;
  }

  inline void construction_preview_middle_row(Buildings::Building building) {
    list<Buildings::Recipe> recipes =
      Buildings::recipes_for_building(building.type);

    CLAY({
      .id = CLAY_ID("Construction::Preview::Desc"),
    }) {
      text_label(
        CLAY_STRING("Description of a farm. Description of a farm. "
                    "Description of a farm."),
        8
      );
    }

    CLAY({
      .id = CLAY_ID("Construction::Preview::MiddleRow"),
      .layout =
        {
          .childGap = uint16_t(16 * UI_SCALE),
        },
    }) {
      CLAY({
        .layout =
          {
            .childGap = uint16_t(4 * UI_SCALE),
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          },
        .clip = {.vertical = true, .childOffset = Clay_GetScrollOffset()},
      }) {
        text_label(CLAY_STRING("Produce:"), 12);

        // for (const Buildings::Recipe &recipe: recipes) {
        for (u32 i = 0; i < recipes.size(); i++) {
          auto recipe = recipes[i];
          CLAY({
            .layout =
              {
                .childGap = uint16_t(8 * UI_SCALE),
                .childAlignment = {.y = CLAY_ALIGN_Y_CENTER},
              },
          }) {
            for (u32 j = 0; j < recipe.outputs.size(); j++) {
              Buildings::RecipeItem item = recipe.outputs[j];

              texture_label(hstr{"arrow.png"}, {13, 13});

              resource_icon(item.resource, j);
            }

            if (!selected_recipe.has_value()) {
              selected_recipe = recipe;
            }

            str building_name = Buildings::building_name_str(building.type);
            str label = building_name + "::Checkbox";

            Clay_String cs = Clay_String{
              .length = static_cast<int32_t>(strlen(label.c_str())),
              .chars = label.c_str(),
            };

            hstr icon = hstr{"checkbox_empty.png"};

            if (selected_recipe == recipe) {
              icon = hstr{"checkbox_checked.png"};
            } else {
              icon = hstr{"checkbox_empty.png"};
            }

            if (texture_button(cs, icon, {15, 15}, i)) {
              selected_recipe = recipe;
            }
          }
        }
      }


      CLAY({.layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
        text_label(CLAY_STRING("Requires:"), 12);
      }
    }
  }

  inline bool construction_preview_bottom_row() {
    bool clicked_build = false;

    CLAY({
      .id = CLAY_ID("Construction::Preview::BottomRow"),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_GROW(),
            },
          .childAlignment =
            {
              .x = CLAY_ALIGN_X_RIGHT,
            },
        },
    }) {
      clicked_build = text_button_small(
        CLAY_STRING("Construction::Build"), CLAY_STRING("Build"), 0
      );
    }

    return clicked_build;
  }

  inline Action_ConstructionPreview construction_preview(
    Buildings::Building building
  ) {
    Action_ConstructionPreview action = {Action_ConstructionPreview_t::None};

    CLAY({
      .id = CLAY_ID("Construction::Preview"),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_GROW(),
              .height = CLAY_SIZING_GROW(),
            },
          .childGap = uint16_t(4 * UI_SCALE),
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
    }) {

      if (construction_preview_top_row(building)) {
        selected_recipe = std::nullopt;
        action = {Action_ConstructionPreview_t::Back};
      }

      construction_preview_middle_row(building);

      UI::spacer();

      if (construction_preview_bottom_row()) {
        action = {
          .type = Action_ConstructionPreview_t::Build,
          .recipe = selected_recipe.value()
        };
      }
    }


    // if (ButtonWasClicked(CLAY_STRING("Construction::Preview::Back"))) {
    //   return Action_ConstructionPreview::Back;
    // }

    // if (ButtonWasClicked(CLAY_STRING("Construction::Build"))) {
    //   return Action_ConstructionPreview::Build;
    // }

    return action;
  }


}// namespace UI
