#pragma once

#include "../../shared/common.hpp"

#include "../irongui/forge.hpp"

namespace UI {
  inline str DrawFactionSelectScreen() {

    const int num_factions = 7;

    // TODO read this in from a dedicated faction.hpp or something like that
    const str factions[num_factions] = {
      "romans",
      "greeks",
      "celts",
      "punics",
      "persians",
      "scythians",
      "germans",
    };
    // TODO read this in from a dedicated faction.hpp or something like that
    const Color colors[num_factions] = {
      RED,
      BLUE,
      GREEN,
      PURPLE,
      ORANGE,
      PINK,
      GRAY,
    };

    auto f = Iron::Forge();
    rect root = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_grid = f->Grid( root, 1, 3 );

    auto grid = f->Grid( root_grid->Slot( 1 ), num_factions, 1 );

    for ( u32 i = 0; i < num_factions; i++ ) {
      bool faction_select =
        f->TextButton( grid->Slot( i ), factions[i], colors[i] );

      if ( faction_select ) {
        return factions[i];
      }
    }

    return "";
  }


};// namespace UI
