#pragma once

#include "../../shared/common.hpp"

#include "../../network/client.hpp"
#include "../../network/host.hpp"
#include "../../network/network.hpp"
#include "../irongui/forge.hpp"

#include "../../world/managers/faction_manager.hpp"

namespace UI {
  enum class Action_MultiplayerLobby {
    None,
    HitBack,
    PickFaction,
    ToggleReady,
    HostStartedGame,
  };

  void HostView( Iron::IForge *, Iron::Element * );
  void ClientView( Iron::IForge *, Iron::Element * );

  inline Action_MultiplayerLobby MultiPlayerLobby() {
    auto f = Iron::Forge();
    rect root = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_grid = f->Grid( root, 1, 4 );

    u32 per_row = 4;
    u32 rows = Network::MAX_PLAYERS_PER_SERVER / per_row;

    auto top_g = f->Grid( root_grid->Slot( 0 ), 3, 1 );
    if ( f->TextButton( top_g->Slot( 0 ), "<- Back" ) ) {
      return Action_MultiplayerLobby::HitBack;
    }

    f->TextLabel( top_g->Slot( 1 ), "Multiplayer Lobby" );

    auto slots_g = f->Grid( root_grid->Rows( 1, 2 ), per_row, rows );

    auto buttons_g = f->Grid( root_grid->Row( 3 ), 3, 1 );

    if ( f->TextButton( buttons_g->Slot( 0 ), "Pick Faction", RED ) ) {
      return Action_MultiplayerLobby::PickFaction;
    }
    if ( f->TextButton( buttons_g->Slot( 1 ), "Ready Up", RED ) ) {
      return Action_MultiplayerLobby::ToggleReady;
    }

    if ( Network::is_host ) {
      bool all_players_ready = false;

      if ( Network::Host()->CheckIfAllPlayersReady() ) {
        if ( f->TextButton( buttons_g->Slot( 2 ), "Start Game", GREEN ) ) {
          return Action_MultiplayerLobby::HostStartedGame;
        }
      } else {
        f->TextLabel( buttons_g->Slot( 2 ), "Waiting on Clients", RED );
      }

      HostView( f, slots_g );
    } else {
      ClientView( f, slots_g );
    }

    return Action_MultiplayerLobby::None;
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

        if ( pd.faction == "" ) {
          f->TextLabel( slot_g->Slot( 1 ), "Picking Faction", RED );
        } else {
          f->TextLabel(
            slot_g->Slot( 1 ),
            pd.faction,
            Faction::Manager::Get()->primary_colors[pd.faction]
          );
        }

        if ( pd.readied_up ) {
          f->TextLabel( slot_g->Slot( 2 ), "Ready", GREEN );
        } else {
          f->TextLabel( slot_g->Slot( 2 ), "Not Ready", RED );
        }

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
        if ( peer.faction == "" ) {
          f->TextLabel( slot_g->Slot( 1 ), "Picking Faction", RED );
        } else {
          f->TextLabel(
            slot_g->Slot( 1 ),
            peer.faction,
            Faction::Manager::Get()->primary_colors[peer.faction]
          );
        }

        if ( peer.readied_up ) {
          f->TextLabel( slot_g->Slot( 2 ), "Ready", GREEN );
        } else {
          f->TextLabel( slot_g->Slot( 2 ), "Not Ready", RED );
        }

      } else {
        f->TextLabel( slots_g->Slot( i ), "Open Slot", GRAY );
      }
    }
  }

}// namespace UI