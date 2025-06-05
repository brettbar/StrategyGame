#pragma once

#include "../../../../../../data/buildings.hpp"
#include "../../../../library/resource_icon.hpp"
#include "../../../../library/text_button.hpp"
#include "../../../../library/text_label.hpp"
#include "../../../../library/texture_button.hpp"


namespace UI {

  // @todo this should be coming from data/ at somepoint
  struct Building {
    Buildings::BuildingType name;
    Buildings::BuildingCategory category;
    Clay_String label;
    str path;
  };

  enum class Action_ConstructionPreview {
    None,
    Back,
    Build
  };

  inline void construction_preview_top_row(Building building) {
    CLAY({
      .id = CLAY_ID("Construction::Preview::TopRow"),
      .layout = {.childGap = 8},
    }) {
      texture_button(
        CLAY_STRING("Construction::Preview::Back"),
        hstr{"back_button.png"},
        {15, 15}
      );

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
  }

  inline void construction_preview_middle_row(Building building) {

    list<Buildings::Recipe> recipes =
      Buildings::recipes_for_building(building.name);

    CLAY({
      .id = CLAY_ID("Construction::Preview::Desc"),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIXED(256 * UI_SCALE),
            },
        },
    }) {
      text_label(
        CLAY_STRING("Description of a farm. Description of a farm. "
                    "Description of a farm."),
        8
      );
    }


    CLAY({.id = CLAY_ID("Construction::Preview::MiddleRow")}) {
      CLAY() {
        text_label(CLAY_STRING("Produce:"), 12);

        for (const Buildings::Recipe &recipe: recipes) {

          CLAY() {
            printf(
              "Ok dude, what is going on here: %d\n",
              (int) recipe.outputs.size()
            );

            for (u32 i = 0; i < recipe.outputs.size(); i++) {
              Buildings::RecipeItem item = recipe.outputs[i];
              printf("resource %d\n", item.resource);


              texture_label(hstr{"arrow.png"}, {13, 13});

              resource_icon(item.resource, i);
            }
          }
        }
      }

      CLAY() {
        text_label(CLAY_STRING("Requires:"), 12);
      }
    }
  }

  inline void construction_preview_bottom_row() {
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
      text_button_small(
        CLAY_STRING("Construction::Build"), CLAY_STRING("Build"), 0
      );
    }
  }

  inline Action_ConstructionPreview construction_preview(Building building) {
    CLAY({
      .id = CLAY_ID("Construction::Preview"),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_GROW(),
              .height = CLAY_SIZING_GROW(),
            },
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
    }) {

      construction_preview_top_row(building);

      construction_preview_middle_row(building);

      UI::spacer();

      // construction_preview_bottom_row();
    }


    if (ButtonWasClicked(CLAY_STRING("Construction::Preview::Back"))) {
      return Action_ConstructionPreview::Back;
    }

    if (ButtonWasClicked(CLAY_STRING("Construction::Build"))) {
      return Action_ConstructionPreview::Build;
    }

    return Action_ConstructionPreview::None;
  }

}// namespace UI
