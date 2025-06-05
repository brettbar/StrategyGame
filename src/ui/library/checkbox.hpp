#pragma once

#include "texture_button.hpp"

namespace UI {
  inline void checkbox(bool checked) {
    if (checked) {
      texture_label(hstr{"checkbox_checked.png"}, {15, 15});
    } else {
      texture_label(hstr{"checkbox_empty.png"}, {15, 15});
    }
  }
}// namespace UI
