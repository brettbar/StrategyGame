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
    return {
      DataPanel( "lobby_browser" )
        .Axis( Axis::Column )
        .Anchor( Anchor::Centered )
        .Update( []( Element &self ) {
          for ( CSteamID lobby_id: Network::Client()->GetLobbyList() )
          {
            const char *lobby_name =
              SteamMatchmaking()->GetLobbyData( lobby_id, "name" );

            if ( lobby_name && lobby_name[0] )
            {
              // std::cout << "Lobby Name: " << lobby_name << std::endl;

              std::string button_id =
                "lobby_entry_" + std::string( lobby_name );

              self.CreateElementForDatapoints(
                TextButton( lobby_name )
                  .Text( std::string( lobby_name ), 24 )
                  .Background( GREEN )
                  .SetEvent( InterfaceEvent::Data(
                    InterfaceEvent::ID::JoinLobby, button_id, lobby_id
                  ) )
              );
            }
          }
        } ),
    };
  }
};// namespace UI
