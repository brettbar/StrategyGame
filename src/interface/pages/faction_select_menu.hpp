#pragma once

#include "../../shared/common.hpp"

#include "../../world/managers/faction_manager.hpp"
#include "../irongui/forge.hpp"


namespace UI {
  inline str DrawFactionSelectScreen() {
    auto f = Iron::Forge();
    rect root = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_grid = f->Grid( root, 1, 3 );

    auto fm = Faction::Manager::Get();
    u32 num_factions = fm->num_factions;

    auto grid = f->Grid( root_grid->Slot( 1 ), num_factions, 1 );

    for ( u32 i = 0; i < num_factions; i++ ) {
      str faction = fm->ids[i];

      bool faction_select =
        f->TextButton( grid->Slot( i ), faction, fm->primary_colors[faction] );

      if ( faction_select ) {
        return faction;
      }
    }

    return "";
  }


};// namespace UI
