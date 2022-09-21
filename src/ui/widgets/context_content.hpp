#pragma once
#include "../../components/province.hpp"
#include "../../renderer/textures.hpp"
#include "../ui_components.hpp"

namespace UI {

inline entt::entity ContextContent();
inline entt::entity ProvinceContext();
inline entt::entity ProvinceName();
inline entt::entity ProvincePopulation();

inline entt::entity ContextContent() {
  Element elem = {
    .id = "context_content",
    .type = Type::PANEL,
  };

  Panel panel = {
    .background = BLACK,
    .children_axis = Axis::ROW,
    .children_horiz_align = Align::START,
    .children_vert_align = Align::START,
    .children =
      {
        ProvinceContext(),
      },
  };

  return CreateElement( panel, elem );
}

inline entt::entity ProvinceContext() {
  Element elem = {
    .id = "province_context",
    .type = Type::PANEL,
  };

  Panel panel = {
    .background = BLACK,
    .children_axis = Axis::COLUMN,
    .children_horiz_align = Align::START,
    .children_vert_align = Align::START,
    .children =
      {
        ProvinceName(),
        ProvincePopulation(),
      },
  };

  return CreateElement( panel, elem );
}

inline entt::entity ProvinceName() {
  Element elem = {
    .id = "province_name",
    .type = Type::TEXT_LABEL,
    .margins = { 0, 0, 16, 16 },
  };

  TextLabel name = {
    .text = "SETTLEMENT_PLACEHOLDER",
    .font_size = 32,
    .text_color = WHITE,
    .background = BLACK,
  };


  return CreateElement( name, elem );
}

inline entt::entity ProvincePopulation() {
  Element elem = {
    .id = "province_population",
    .type = Type::TEXT_LABEL,
    .margins = { 0, 0, 16, 16 },
  };

  TextLabel name = {
    .text = "0",
    .font_size = 32,
    .text_color = WHITE,
    .background = BLACK,
  };

  return CreateElement( name, elem );
}

};// namespace UI
