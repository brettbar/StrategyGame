#pragma once

#include "../../../../../../data/buildings.hpp"
#include "../../../../../shared/manager.hpp"
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

  struct ConstructionPreview : public Manager<ConstructionPreview> {

private:
    ConstructionPreview() = default;
    friend class Manager<ConstructionPreview>;

    i32 _selected_recipe = -1;
    i32 _selected_construction_materials = -1;

    bool construction_preview_top_row(Buildings::Building building) {
      bool hit_back = false;

      CLAY({
        .id = CLAY_ID("ConstructionPreview::TopRow"),
        .layout = {.childGap = 8},
      }) {

        if (texture_button(
              CLAY_STRING("ConstructionPreview::Back"),
              hstr{"back_button.png"},
              {15, 15}
            )) {
          hit_back = true;
          exit();
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
          texture_w_tooltip(
            CLAY_STRING("ConstructionPreview::BuildingIcon"),
            building.label,
            Buildings::building_icon_path(building.type),
            {32, 32}
          );

          text_label(building.label, 16);
        }
      }

      return hit_back;
    }

    void construction_preview_middle_row(Buildings::Building building) {
      CLAY({
        .id = CLAY_ID("ConstructionPreview::Desc"),
      }) {
        text_label(
          CLAY_STRING("Description of a farm. Description of a farm. "
                      "Description of a farm."),
          8
        );
      }

      CLAY({
        .id = CLAY_ID("ConstructionPreview::MiddleRow"),
        .layout =
          {
            .childGap = uint16_t(16 * UI_SCALE),
          },
      }) {

        CLAY({
          .layout =
            {
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
        }) {
          text_label(CLAY_STRING("Produce:"), 12);

          CLAY({
            .layout =
              {
                .childGap = uint16_t(4 * UI_SCALE),
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
              },
            .clip = {.vertical = true, .childOffset = Clay_GetScrollOffset()},
          }) {
            list<Buildings::Recipe> recipes =
              Buildings::recipes_for_building(building.type);

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

                  const char *resource =
                    Resources::resource_cstr(item.resource);

                  Clay_String cs = Clay_String{
                    .length = static_cast<int32_t>(strlen(resource)),
                    .chars = resource,
                  };

                  resource_icon(item.resource, cs, j);
                }

                if (_selected_recipe == -1) {
                  _selected_recipe = 0;
                }

                str building_name =
                  Buildings::building_name_cstr(building.type);
                str label = building_name + "::Checkbox";

                Clay_String cs = Clay_String{
                  .length = static_cast<int32_t>(strlen(label.c_str())),
                  .chars = label.c_str(),
                };

                hstr icon = hstr{"checkbox_empty.png"};

                if (_selected_recipe == i) {
                  icon = hstr{"checkbox_checked.png"};
                } else {
                  icon = hstr{"checkbox_empty.png"};
                }

                if (texture_button(cs, icon, {15, 15}, i)) {
                  _selected_recipe = i;
                }
              }
            }
          }
        }


        CLAY({.layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
          text_label(CLAY_STRING("Requires:"), 12);

          CLAY({
            .layout =
              {
                .childGap = uint16_t(4 * UI_SCALE),
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
              },
            .clip = {.vertical = true, .childOffset = Clay_GetScrollOffset()},
          }) {
            auto construction_recipes =
              Buildings::construction_recipes(building.type);

            for (u32 i = 0; i < construction_recipes.size(); i++) {
              auto recipe = construction_recipes[i];

              CLAY({
                .layout =
                  {
                    .childGap = uint16_t(8 * UI_SCALE),
                    .childAlignment = {.y = CLAY_ALIGN_Y_CENTER},
                  },
              }) {

                for (u32 j = 0; j < recipe.inputs.size(); j++) {
                  Buildings::RecipeItem item = recipe.inputs[j];

                  const char *resource =
                    Resources::resource_cstr(item.resource);
                  Clay_String cs = Clay_String{
                    .length = static_cast<int32_t>(strlen(resource)),
                    .chars = resource,
                  };

                  resource_icon_w_quantity(item.resource, item.quantity, cs, j);
                }

                if (_selected_construction_materials == -1) {
                  _selected_construction_materials = 0;
                }

                hstr icon = hstr{"checkbox_empty.png"};

                if (_selected_construction_materials == i) {
                  icon = hstr{"checkbox_checked.png"};
                } else {
                  icon = hstr{"checkbox_empty.png"};
                }

                if (texture_button(
                      CLAY_STRING("ConstructionPreview::ConstructionMaterials"),
                      icon,
                      {15, 15},
                      i
                    )) {
                  _selected_construction_materials = i;
                }
              }
            }
          }
        }
      }
    }

    bool construction_preview_bottom_row() {
      bool clicked_build = false;

      CLAY({
        .id = CLAY_ID("ConstructionPreview::BottomRow"),
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

public:
    void exit() {
      _selected_recipe = -1;
      _selected_construction_materials = -1;
    }

    Action_ConstructionPreview construction_preview(Buildings::Building building
    ) {
      Action_ConstructionPreview action = {Action_ConstructionPreview_t::None};

      CLAY({
        .id = CLAY_ID("ConstructionPreview"),
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
          _selected_recipe = -1;
          _selected_construction_materials = -1;
          action = {Action_ConstructionPreview_t::Back};
        }

        construction_preview_middle_row(building);

        UI::spacer();

        if (construction_preview_bottom_row()) {
          action = {
            .type = Action_ConstructionPreview_t::Build,
            .recipe =
              Buildings::recipes_for_building(building.type)[_selected_recipe]
          };
        }
      }


      return action;
    }
  };


}// namespace UI
