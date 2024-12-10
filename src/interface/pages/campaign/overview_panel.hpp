#include "../../irongui/elements.hpp"
#include "../../irongui/forge.hpp"

#include "../../src/data/resources.hpp"

namespace UI {
  inline void resource_overview( Iron::IForge *f, rect panel_g );

  inline void OverviewPanel( Iron::Element *root_g ) {
    auto f = Iron::Forge();

    auto panel_g = f->Grid( root_g->ColsByRows( 0, 1, 0, 6 ), 1, 6, BLACK );

    f->TextLabel( panel_g->Slot( 0 ), "Faction", GRAY );
    f->TextLabel( panel_g->Slot( 1 ), "Diplomacy", BLUE );
    f->TextLabel( panel_g->Slot( 2 ), "Resources", GREEN );
    f->TextLabel( panel_g->Slot( 3 ), "Construction", RED );
    f->TextLabel( panel_g->Slot( 4 ), "Recruitment", ORANGE );
    f->TextLabel( panel_g->Slot( 5 ), "Agents", PURPLE );

    // timeline
    auto time_grid = f->Grid( root_g->ColsByRows( 10, 12, 0, 1 ), 2, 1, BLACK );
    f->TextLabel( time_grid->Slot( 0 ), "Feb, 336 BC" );
    f->TextLabel( time_grid->Slot( 1 ), "<< < || > >>" );


    resource_overview( f, root_g->Cols( 1, 3 ) );
  }


  inline void resource_overview( Iron::IForge *f, rect panel_g ) {

    // auto panel = f->Grid( panel_g, 2, 2 + Resources::raw.size() );

    // f->TextButton( panel->Row( 0 ), "Exit", RED );

    // f->TextLabel( panel->Slot( 2 ), "Resource", BLACK, 1.0, 1.0, WHITE, 18 );
    // f->TextLabel( panel->Slot( 3 ), "Amount", BLACK, 1.0, 1.0, WHITE, 18 );

    // for ( u32 i = 1; i < Resources::raw.size(); i++ ) {
    //   auto row = f->Grid( panel->Row( i ), 2, 1 );

    //   f->TextureLabel(
    //     row->Slot( 0 ),
    //     hstr{ ( Resources::raw[i - 1] + ".png" ).c_str() },
    //     BLACK,
    //     3.0f,
    //     2.0f,
    //     WHITE
    //   );
    //   f->TextLabel( row->Slot( 1 ), "0", BLACK, 1.0, 1.0, WHITE );
    // }
  }
}// namespace UI
