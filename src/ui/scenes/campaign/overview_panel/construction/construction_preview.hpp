#pragma once

#include "../../../../../../data/buildings.hpp"
#include "../../../../common.h"
#include "../../../../library/checkbox.hpp"
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

  struct ConstructionPreview {
    opt<Resources::Type> selected_resource = std::nullopt;

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

          for (const Buildings::Recipe &recipe: recipes) {

            CLAY({
              .layout =
                {
                  .childGap = uint16_t(8 * UI_SCALE),
                  .childAlignment =
                    {
                      .y = CLAY_ALIGN_Y_CENTER,
                    },

                },
            }) {
              for (u32 i = 0; i < recipe.outputs.size(); i++) {
                Buildings::RecipeItem item = recipe.outputs[i];

                texture_label(hstr{"arrow.png"}, {13, 13});

                resource_icon(item.resource, i);

                if (!selected_resource.has_value()) {
                  selected_resource = item.resource;
                }

                str resource_name = Resources::ResourceStr(item.resource);
                str building_name = Buildings::building_name_str(building.name);
                str wtf = building_name + "::Checkbox::" + resource_name;

                Clay_String cs = str_to_cs(wtf);

                if (selected_resource == item.resource) {
                  texture_button(cs, hstr{"checkbox_checked.png"}, {15, 15});
                } else {
                  texture_button(cs, hstr{"checkbox_empty.png"}, {15, 15});
                }
              }
            }
          }
        }


        CLAY({.layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
          text_label(CLAY_STRING("Requires:"), 12);
        }
      }

      for (const Buildings::Recipe &recipe: recipes) {
        for (u32 i = 0; i < recipe.outputs.size(); i++) {
          Buildings::RecipeItem item = recipe.outputs[i];

          str resource_name = Resources::ResourceStr(item.resource);
          str building_name = Buildings::building_name_str(building.name);
          str wtf = building_name + "::Checkbox::" + resource_name;
          Clay_String cs = str_to_cs(wtf);

          if (ButtonWasClicked(cs)) {
            selected_resource = item.resource;
            printf("YOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"
                   "OOOOOO\n");
          }
        }
      }
      //
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
            .childGap = uint16_t(4 * UI_SCALE),
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          },
      }) {

        construction_preview_top_row(building);

        construction_preview_middle_row(building);

        UI::spacer();

        construction_preview_bottom_row();
      }


      if (ButtonWasClicked(CLAY_STRING("Construction::Preview::Back"))) {
        return Action_ConstructionPreview::Back;
      }

      if (ButtonWasClicked(CLAY_STRING("Construction::Build"))) {
        return Action_ConstructionPreview::Build;
      }

      return Action_ConstructionPreview::None;
    }
  };


}// namespace UI
