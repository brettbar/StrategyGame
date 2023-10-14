#include "../shared/common.hpp"

namespace UI {

  inline void DrawSettlementContext() {
    rect rectangle = rect{};
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.width = 250;
    rectangle.height = 250;

    Color background = BLUE;

    DrawRectangleRec( rectangle, background );
  }

}// namespace UI
