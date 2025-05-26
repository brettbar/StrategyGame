
#include "../../../../shared/common.hpp"
#include "../../../library/text_button.hpp"
#include "../../../library/text_label.hpp"
#include "../../../library/texture_button.hpp"
#include <cstdint>
#include <optional>
#define CLAY_EXTEND_CONFIG_IMAGE hstr texture_id;
#include "../../../../../data/buildings.hpp"
#include "../../../common.h"

namespace UI {
  inline bool _constructing = false;
  inline i32 _selected_building = -1;

  inline Clay_String building_category_str(Buildings::BuildingCategory category
  ) {
    switch (category) {
      case Buildings::BuildingCategory::Agricultural:
        return CLAY_STRING("Agricultural");
      case Buildings::BuildingCategory::Gathering:
        return CLAY_STRING("Gathering");
      case Buildings::BuildingCategory::Refinement:
        return CLAY_STRING("Refinement");
      case Buildings::BuildingCategory::Production:
        return CLAY_STRING("Production");
      case Buildings::BuildingCategory::Diplomatic:
        return CLAY_STRING("Diplomatic");
      case Buildings::BuildingCategory::Martial:
        return CLAY_STRING("Martial");
      case Buildings::BuildingCategory::Naval:
        return CLAY_STRING("Naval");
      case Buildings::BuildingCategory::Civic:
        return CLAY_STRING("Civic");
      case Buildings::BuildingCategory::Religious:
        return CLAY_STRING("Religious");
    }
  }


  // @todo this should be coming from data/ at somepoint
  struct Building {
    Buildings::BuildingType name;
    Buildings::BuildingCategory category;
    Clay_String label;
    str path;
  };

  inline list<Building> buildings = {
    {
      .name = Buildings::BuildingType::Farm,
      .category = Buildings::BuildingCategory::Agricultural,
      .label = CLAY_STRING("Farm"),
      .path = "farm_icon.png",
    },
    {
      .name = Buildings::BuildingType::LumberMill,
      .category = Buildings::BuildingCategory::Gathering,
      .label = CLAY_STRING("Lumber Mill"),
      .path = "lumber_mill_icon.png",
    },
    {
      .name = Buildings::BuildingType::Mine,
      .category = Buildings::BuildingCategory::Gathering,
      .label = CLAY_STRING("Mine"),
      .path = "mine_icon.png",
    },
  };

  inline void show_building_column(
    Clay_ElementId id,
    list<Buildings::BuildingCategory> col
  ) {
    CLAY({
      .id = id,
      .layout =
        {
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
    }) {
      for (u32 i = 0; i < col.size(); i++) {

        text_label(building_category_str(col[i]), 8);

        CLAY({
          .layout = {.childGap = uint16_t(3 * UI_SCALE)},
        }) {
          for (u32 j = 0; j < buildings.size(); j++) {
            Building building = buildings[j];

            if (building.category != col[i]) {
              continue;
            }

            texture_w_tooltip(
              CLAY_IDI("BuildingIcon", j),
              CLAY_IDI("BuildingIcon::Tooltip", j),
              building.label,
              hstr{building.path.c_str()},
              {32, 32}
            );
          }
        }
      }
    }
  }

  inline u32 construction_browser() {
    list<Buildings::BuildingCategory> left_col = {
      Buildings::BuildingCategory::Agricultural,
      Buildings::BuildingCategory::Gathering,
      Buildings::BuildingCategory::Refinement,
      Buildings::BuildingCategory::Production,
      Buildings::BuildingCategory::Diplomatic,
    };

    list<Buildings::BuildingCategory> right_col = {
      Buildings::BuildingCategory::Martial,
      Buildings::BuildingCategory::Naval,
      Buildings::BuildingCategory::Civic,
      Buildings::BuildingCategory::Religious,
    };

    CLAY({.layout = {.childGap = uint16_t(12 * UI_SCALE)}}) {
      show_building_column(CLAY_ID("Construction::LeftCol"), left_col);
      show_building_column(CLAY_ID("Construction::RightCol"), right_col);
    }


    for (u32 i = 0; i < buildings.size(); i++) {
      Building building = buildings[i];

      if (ButtonWasClicked(CLAY_STRING("BuildingIcon"), i)) {
        return i;
      }
    }

    return -1;
  }

  enum class Action_ConstructionPreview {
    None,
    Back,
    Build
  };

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
          16
        );
      }
      CLAY(CLAY_ID("Construction::Preview::MiddleRow")) {}
      UI::spacer();
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


    if (ButtonWasClicked(CLAY_STRING("Construction::Preview::Back"))) {
      return Action_ConstructionPreview::Back;
    }

    if (ButtonWasClicked(CLAY_STRING("Construction::Build"))) {
      return Action_ConstructionPreview::Build;
    }

    return Action_ConstructionPreview::None;
  }

  inline opt<Buildings::BuildingType> construction_tab() {

    if (_constructing) {
      Building building = buildings[_selected_building];

      auto action = construction_preview(building);

      // _constructing = false;
      // _selected_building = -1;
      switch (action) {
        case UI::Action_ConstructionPreview::None:
          break;
        case UI::Action_ConstructionPreview::Back:
          _constructing = false;
          break;
        case UI::Action_ConstructionPreview::Build:
          return building.name;
      }
    } else {
      _selected_building = construction_browser();

      if (_selected_building > -1) {
        _constructing = true;
      }
    }

    return std::nullopt;
  }
}// namespace UI
