#include "../../irongui/forge.hpp"

#include "../../src/data/resources.hpp"


namespace UI {
  inline void resource_overview( Iron::IForge *f, rect panel_g );

  inline void OverviewPanel() {
    auto f = Iron::Forge();

    rect root_r = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_g = f->Grid( root_r, 8, 4 );

    auto panel_g = f->Grid( root_g->Col( 0 ), 1, 8, BLACK );

    f->TextLabel( panel_g->Slot( 0 ), "Faction", RED );

    resource_overview( f, panel_g->Rows( 1, 7 ) );
  }

  inline void resource_overview( Iron::IForge *f, rect panel_g ) {
    auto panel = f->Grid(panel_g, 1, 10);

    for ( u32 i = 0; i < Resources::raw.size(); i++) {
      rect slot = panel->Slot(i);

      auto row = f->Grid(slot, 2, 1);

      f->TextureLabel(row->Slot(0), hstr{(Resources::raw[i] + ".png").c_str()}, Color{0,0,0,0}, 3.0f);
      f->TextLabel(row->Slot(1), "0");
    }
  }
};// namespace UI
