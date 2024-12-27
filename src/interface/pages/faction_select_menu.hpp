#pragma once

#include "../../shared/common.hpp"

#include "../../world/managers/actor_manager.hpp"
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
      auto faction_g = f->Grid( grid->Slot( i ), 1, 2 );

      str faction = fm->ids[i];

      bool faction_select = f->TextButton(
        faction_g->Slot( 0 ), faction, fm->primary_colors[faction]
      );

      auto actor = Actor::Manager::Get()->actor_from_faction_roster(
        Actor::Type::Colonist, faction
      );

      bool faction_character = f->TextureButton(
        faction_g->Slot( 1 ), hstr{ ( actor.sprite_id + "_overview" ).c_str() }
      );

      if ( faction_select || faction_character ) {
        return faction;
      }
    }

    return "";
  }


};// namespace UI
