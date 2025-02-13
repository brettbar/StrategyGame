#pragma once

#include "../../irongui/forge.hpp"
#include <raylib.h>


namespace UI {
  enum class Action_MapModeChange {
    None,
    Default,
    Terrain,
    Political,
    Resources,
  };

  inline Action_MapModeChange Minimap( Iron::Element *root_g ) {
    auto f = Iron::Forge();

    auto minimap_g = f->Grid(
      root_g->ColsByRows( 9, 12, 9, 12 ), 4, 4, Color{ 0, 0, 0, 125 }
    );

    auto map_mode_g = f->Grid( minimap_g->Row( 3 ), 4, 1, GREEN );

    bool switch_default =
      f->TextButton( map_mode_g->Slot( 0 ), "Default", BLUE );
    bool switch_terrain =
      f->TextButton( map_mode_g->Slot( 1 ), "Terrain", GREEN );
    bool switch_political =
      f->TextButton( map_mode_g->Slot( 2 ), "Political", RED );
    bool switch_resources =
      f->TextButton( map_mode_g->Slot( 3 ), "Resources", ORANGE );

    if ( switch_default )
      return Action_MapModeChange::Default;
    else if ( switch_terrain )
      return Action_MapModeChange::Terrain;
    else if ( switch_political )
      return Action_MapModeChange::Political;
    else if ( switch_resources )
      return Action_MapModeChange::Resources;

    return Action_MapModeChange::None;
  }
}// namespace UI
