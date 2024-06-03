#include "../../irongui/forge.hpp"

namespace UI {
  inline void OverviewPanel() {
    auto f = Iron::Forge();

    rect root_r = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_g = f->Grid( root_r, 4, 4 );
  }
};// namespace UI
