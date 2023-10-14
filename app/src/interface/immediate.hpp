#include "../shared/common.hpp"

namespace Iron {

  inline void DrawSettlementContext() {
    rect rectangle = rect{};
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.width = 250;
    rectangle.height = 250;

    Color background = BLUE;

    {
      rect button = rect{};
      button.x = 0;
      button.y = 0;
      button.width = 50;
      button.height = 50;
    }

    DrawRectangleRec( rectangle, background );
  }

}// namespace Iron
