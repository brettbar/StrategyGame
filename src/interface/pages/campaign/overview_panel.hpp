#include "../../irongui/forge.hpp"

namespace UI {
  inline void OverviewPanel() {
    auto f = Iron::Forge();

    rect root_r = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_g = f->Grid( root_r, 8, 4 );

    auto panel_g = f->Grid( root_g->Col( 0 ), 1, 3, BLACK );

    f->TextLabel( panel_g->Slot( 0 ), "Faction", RED );
  }
};// namespace UI
