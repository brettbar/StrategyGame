#pragma once

#include "../../network/client.hpp"
#include "../../shared/common.hpp"
#include "../irongui/forge.hpp"


namespace UI {

  inline str LobbyBrowser() {
    auto f = Iron::Forge();
    rect root_r = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_g = f->Grid( root_r, 3, 3 );

    // @todo add scrolling
    u32 lobbies_shown_at_once = 10;
    auto lobby_g = f->Grid( root_g->Slot( 4 ), 1, lobbies_shown_at_once );

    u32 i = 0;
    for ( CSteamID lobby_id: Network::Client()->GetLobbyList() ) {
      const char *lobby_name =
        SteamMatchmaking()->GetLobbyData( lobby_id, "name" );

      if ( lobby_name && lobby_name[0] ) {
        str button_id = "lobby_entry_" + str( lobby_name );
        if ( f->TextButton( lobby_g->Slot( i++ ), str( lobby_name ) ) ) {
          return lobby_name;
        }
      }
    }
    return "";
  }

  // inline std::vector<Element> CreateLobbyBrowser()
  // {
  //   return {
  //     DataPanel( "lobby_browser" )
  //       .Axis( Axis::Column )
  //       .Anchor( Anchor::Centered )
  //       .Update( []( Element &self ) {
  //         for ( CSteamID lobby_id: Network::Client()->GetLobbyList() )
  //         {
  //           const char *lobby_name =
  //             SteamMatchmaking()->GetLobbyData( lobby_id, "name" );

  //           if ( lobby_name && lobby_name[0] )
  //           {
  //             // std::cout << "Lobby Name: " << lobby_name << std::endl;

  //             std::string button_id =
  //               "lobby_entry_" + std::string( lobby_name );

  //             self.CreateElementForDatapoints(
  //               TextButton( lobby_name )
  //                 .Text( std::string( lobby_name ), 24 )
  //                 .Background( GREEN )
  //                 .SetEvent( InterfaceEvent::Data(
  //                   InterfaceEvent::ID::JoinLobby, button_id, lobby_id
  //                 ) )
  //             );
  //           }
  //         }
  //       } ),
  //   };
  // }
};// namespace UI
