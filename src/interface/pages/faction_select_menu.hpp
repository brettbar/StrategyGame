#pragma once

#include "../../shared/common.hpp"

#include "../../data/factions.hpp"
#include "../irongui/forge.hpp"

namespace UI {
  inline str DrawFactionSelectScreen() {
    auto f = Iron::Forge();
    rect root = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_grid = f->Grid( root, 1, 3 );

    auto grid = f->Grid( root_grid->Slot( 1 ), Factions::num_factions, 1 );

    for ( u32 i = 0; i < Factions::num_factions; i++ ) {
      bool faction_select = f->TextButton(
        grid->Slot( i ), Factions::factions[i], Factions::colors[i]
      );

      if ( faction_select ) {
        return Factions::factions[i];
      }
    }

    return "";
  }


};// namespace UI
