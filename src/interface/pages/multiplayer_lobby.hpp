#pragma once

#include "../../shared/common.hpp"

#include "../../network/client.hpp"
#include "../../network/host.hpp"
#include "../../network/network.hpp"
#include "../irongui/forge.hpp"

namespace UI {
  void HostView( Iron::IForge *, Iron::Element * );
  void ClientView( Iron::IForge *, Iron::Element * );

  inline void MultiPlayerLobby() {
    auto f = Iron::Forge();
    rect root = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_grid = f->Grid( root, 1, 3 );

    u32 per_row = 4;
    u32 rows = Network::MAX_PLAYERS_PER_SERVER / per_row;

    auto slots_g = f->Grid( root_grid->Row( 1 ), per_row, rows );

    f->TextLabel( root_grid->Slot( 0 ), "Multiplayer Lobby", BLUE );

    if ( Network::is_host ) {
      HostView( f, slots_g );
    } else {
      ClientView( f, slots_g );
    }
  }

  inline void HostView( Iron::IForge *f, Iron::Element *slots_g ) {
    for ( u32 i = 0; i < Network::MAX_PLAYERS_PER_SERVER; i++ ) {
      auto pd = Network::Host()->ReadClientPeerData( i );
      if ( pd.active ) {
        auto slot_g = f->Grid( slots_g->Slot( i ), 1, 3 );

        auto name =
          std::string( SteamFriends()->GetFriendPersonaName( pd.steam_user_id )
          );

        if ( pd.player_id == "player_0" )
          name = name + " (Me)";

        f->TextLabel( slot_g->Slot( 0 ), name, RED );
        f->TextLabel( slot_g->Slot( 1 ), "Picking Faction", RED );
        f->TextLabel( slot_g->Slot( 2 ), "Not Ready", RED );
      } else {
        f->TextLabel( slots_g->Slot( i ), "Open Slot", GRAY );
      }
    }
  }

  inline void ClientView( Iron::IForge *f, Iron::Element *slots_g ) {
    f->TextLabel( slots_g->Slot( 0 ), "Im bob client" );

    for ( u32 i = 0; i < Network::MAX_PLAYERS_PER_SERVER; i++ ) {
      auto peer = Network::Client()->_peers[i];

      if ( peer.active ) {
        auto slot_g = f->Grid( slots_g->Slot( i ), 1, 3 );

        auto name =
          std::string( SteamFriends()->GetFriendPersonaName( peer.steam_user_id
          ) );

        if ( peer.player_id == Network::Client()->_local_player_id )
          name = name + " (Me)";

        f->TextLabel( slot_g->Slot( 0 ), name, RED );
        f->TextLabel( slot_g->Slot( 1 ), "Picking Faction", RED );
        f->TextLabel( slot_g->Slot( 2 ), "Not Ready", RED );
      } else {
        f->TextLabel( slots_g->Slot( i ), "Open Slot", GRAY );
      }
    }
  }

}// namespace UI