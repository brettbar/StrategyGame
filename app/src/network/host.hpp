#pragma once

#include "network.hpp"
#include "steam/steam_api_common.h"
#include "steam/steamnetworkingtypes.h"

namespace Network {

struct Host {
  // STEAM_CALLBACK(
  //   Host,
  //   OnHostNetConnectionStatusChanged,
  //   SteamNetConnectionStatusChangedCallback_t
  // );

  HSteamListenSocket socket;
  HSteamNetConnection conn;

  CSteamID steam_lobby_id;

  void OnLobbyCreated( LobbyCreated_t *, bool );
  CCallResult<Host, LobbyCreated_t> result_lobby_created;

  STEAM_CALLBACK( Host, OnLobbyChatMsg, LobbyChatMsg_t );

  //   STEAM_CALLBACK(
  //     Host,
  //     OnNetConnectionStatusChanged,
  //     SteamNetConnectionStatusChangedCallback_t
  //   );

  ~Host() {
    // Close connection
    printf( "~Host()\n" );
  }


  void Init() {
    SteamAPICall_t steam_api_call =
      SteamMatchmaking()->CreateLobby( k_ELobbyTypePublic, 2 );

    result_lobby_created.Set( steam_api_call, this, &Host::OnLobbyCreated );
  }

  void CheckForMessages() {
    if ( conn != k_HSteamNetConnection_Invalid ) {
      SteamNetworkingMessage_t *msg;
      int r =
        SteamNetworkingSockets()->ReceiveMessagesOnConnection( conn, &msg, 1 );

      assert( r == 0 || r == 1 );

      if ( r == 1 ) {
        printf( "Host >> Received message '%s'\n", (char *) msg->GetData() );
        msg->Release();

        SendMessageToPeer( conn, "Host >> Hey client, I got your message\n" );
      }
    }
  }
};


inline void Host::OnLobbyCreated( LobbyCreated_t *cb, bool io_failure ) {
  if ( cb->m_eResult == k_EResultOK ) {
    printf( "Lobby created successfully\n" );

    steam_lobby_id = cb->m_ulSteamIDLobby;

    char rgchLobbyName[256];
    sprintf_s( rgchLobbyName, "%s's lobby", SteamFriends()->GetPersonaName() );
    SteamMatchmaking()->SetLobbyData( steam_lobby_id, "name", rgchLobbyName );
    printf( "Hosting lobby %s\n", rgchLobbyName );

    // TODO Move to after a player is found
    socket = SteamNetworkingSockets()->CreateListenSocketP2P( 0, 0, nullptr );
    assert( socket != k_HSteamListenSocket_Invalid );
  }
  else {
    printf( "Failed to create lobby, lost connection to steam back-end\n" );
  }
}

inline void Host::OnLobbyChatMsg( LobbyChatMsg_t *param ) {
  char buf[4 * 1024];
  SteamMatchmaking()->GetLobbyChatEntry(
    param->m_ulSteamIDLobby, param->m_iChatID, nullptr, buf, sizeof( buf ), NULL
  );

  printf( "Host >> ChatMsg found: %s\n", buf );
}

};// namespace Network
