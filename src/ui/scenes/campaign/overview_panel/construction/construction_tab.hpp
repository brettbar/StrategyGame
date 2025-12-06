#pragma once

#include "../../../../../shared/common.hpp"
#include "../../../../../shared/manager.hpp"
#include "../../../../../world/managers/map_manager.hpp"
#include "../../../../library/text_label.hpp"
#include <cstdint>
#include <optional>
#define CLAY_EXTEND_CONFIG_IMAGE hstr texture_id;
#include "../../../../../../data/buildings.hpp"
#include "../../../../common.h"
#include "construction_preview.hpp"

namespace UI {
  struct ConstructionTab : public Manager<ConstructionTab> {
private:
    bool _constructing = false;
    opt<Buildings::Building> _selected_building = std::nullopt;

    ConstructionTab() = default;
    friend class Manager<ConstructionTab>;

public:
    void exit() {
      if (_constructing)
        Map::Manager()->mode = Map::Mode::Default;

      _constructing = false;
      _selected_building = std::nullopt;
    }

    opt<Buildings::Building> construction_tab() {

      if (_constructing) {
        auto action = ConstructionPreview::Get()->construction_preview(
          _selected_building.value()
        );

        switch (action.type) {
          case UI::Action_ConstructionPreview_t::None:
            break;
          case UI::Action_ConstructionPreview_t::Back:
            exit();
            break;
          case UI::Action_ConstructionPreview_t::Build:
            _selected_building.value().current_recipe = action.recipe;
            return _selected_building;
        }
      } else {
        construction_browser();

        if (_selected_building != std::nullopt) {
          _constructing = true;
        }
      }

      return std::nullopt;
    }

    Clay_String building_category_str(Buildings::BuildingCategory category) {
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


    list<Buildings::Building> buildings = {
      {
        .type = Buildings::Type::Farm,
        .category = Buildings::BuildingCategory::Agricultural,
        .label = CLAY_STRING("Farm"),
        .path = "farm_icon.png",
      },
      {
        .type = Buildings::Type::LumberMill,
        .category = Buildings::BuildingCategory::Gathering,
        .label = CLAY_STRING("Lumber Mill"),
        .path = "lumber_mill_icon.png",
      },
      {
        .type = Buildings::Type::Mine,
        .category = Buildings::BuildingCategory::Gathering,
        .label = CLAY_STRING("Mine"),
        .path = "mine_icon.png",
      },
    };

    void show_building_column(
      Clay_ElementId id,
      list<Buildings::BuildingCategory> col
    ) {

      CLAY({
        .id = id,
        .layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM},
      }) {
        for (u32 i = 0; i < col.size(); i++) {
          text_label(building_category_str(col[i]), 12);

          CLAY({
            .layout = {.childGap = uint16_t(3 * UI_SCALE)},
          }) {
            for (u32 j = 0; j < buildings.size(); j++) {
              Buildings::Building building = buildings[j];

              if (building.category != col[i]) {
                continue;
              }

              if (texture_w_tooltip(
                    CLAY_STRING("ConstructionTab::BuildingIcon"),
                    building.label,
                    hstr{building.path.c_str()},
                    {32, 32},
                    j
                  )) {
                printf("Yep got it\n");
                _selected_building = building;
              }
            }
          }
        }
      }
    }

    void construction_browser() {
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
    }
  };

}// namespace UI
