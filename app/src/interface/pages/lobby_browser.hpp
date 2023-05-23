#pragma once

#include "../../shared/common.hpp"


#include "../../network/client.hpp"
#include "../../network/host.hpp"
#include "../../network/network.hpp"

#include "../element.hpp"

#include "../ui_manager.hpp"

namespace UI
{
  inline std::vector<Element> CreateLobbyBrowser()
  {
    // TODO better way of making the id and label
    auto update_children = [](
                             std::map<std::string, bool> existing_ids,
                             std::vector<Element> &children
                           ) {
      for ( CSteamID lobby_id: Network::Client()->GetLobbyList() )
      {
        const char *lobby_name =
          SteamMatchmaking()->GetLobbyData( lobby_id, "name" );

        if ( lobby_name && lobby_name[0] && !existing_ids.contains( std::string( lobby_name ) ) )
        {
          std::cout << "Lobby Name: " << lobby_name << std::endl;

          std::string button_id = "lobby_entry_" + std::string( lobby_name );

          Element button_e =
            TextButton( lobby_name )
              .SetText( std::string( lobby_name ), 24 )
              .Background( GREEN )
              .SetEvent( InterfaceEvent::Data(
                InterfaceEvent::ID::JoinLobby, button_id, lobby_id
              ) );


          // TODO only clickable based on host/client checksum compat
          button_e.Enable();

          children.push_back( button_e );
        }
      }

      // TODO remove the buttons that dont have a lobby anymore
    };

    return {
      Panel( "lobby_browser" )
        .SetAxis( Axis::Column )
        .SetAnchor( Anchor::Centered )
        .UpdateChildren( update_children ),
    };
  }
};// namespace UI
