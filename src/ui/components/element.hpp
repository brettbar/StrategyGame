#pragma once

#include "../../global.hpp"
#include "../ui_common.hpp"

namespace UI {

struct Element {
  std::string id;
  Type type;
  Color background;
  bool enabled;
  rect transform;
  Margins margins;

  Element(
    std::string id,
    Type type,
    Color background,
    bool enabled,
    rect transform,
    Margins margins
  )
      : id( id ), type( type ), background( background ), enabled( enabled ),
        transform( transform ), margins( margins ) {}
};


};// namespace UI
