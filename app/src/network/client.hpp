#pragma once

#include "network.hpp"
#include "steam/steamclientpublic.h"
#include "steam/steamnetworkingtypes.h"

namespace Network {


struct Client {
  HSteamNetConnection conn;

  void OnLobbyMatchList( LobbyMatchList_t *, bool );
  CCallResult<Client, LobbyMatchList_t> result_lobby_match_list;

  void OnLobbyEntered( LobbyEnter_t *, bool );
  CCallResult<Client, LobbyEnter_t> result_lobby_entered;

  STEAM_CALLBACK( Client, OnLobbyChatMsg, LobbyChatMsg_t );

  ~Client() {
    printf( "~Client()\n" );
  }

  void Init() {
    SteamAPICall_t steam_api_call = SteamMatchmaking()->RequestLobbyList();
    result_lobby_match_list.Set(
      steam_api_call, this, &Client::OnLobbyMatchList
    );
  }

  void Join( CSteamID server_id ) {
    SteamNetworkingIdentity identity;
    identity.SetSteamID( server_id );

    conn = SteamNetworkingSockets()->ConnectP2P( identity, 0, 0, nullptr );
    assert( conn != k_HSteamNetConnection_Invalid );

    printf( "Client::Join()\n" );
    SendMessageToPeer( conn, "Client >> Hello from Client!!!\n" );
  }

  void CheckForMessages() {}
};

inline void Client::OnLobbyChatMsg( LobbyChatMsg_t *param ) {
  char buf[4 * 1024];
  SteamMatchmaking()->GetLobbyChatEntry(
    param->m_ulSteamIDLobby, param->m_iChatID, nullptr, buf, sizeof( buf ), NULL
  );

  printf( "Client >> ChatMsg found: %s\n", buf );
}

inline void Client::OnLobbyMatchList( LobbyMatchList_t *cb, bool io_failure ) {
  printf( "Client >> OnMatchLobbyList\n" );

  for ( uint32 i_lobby = 0; i_lobby < cb->m_nLobbiesMatching; i_lobby++ ) {
    CSteamID steam_lobby_id = SteamMatchmaking()->GetLobbyByIndex( i_lobby );

    const char *lobby_name =
      SteamMatchmaking()->GetLobbyData( steam_lobby_id, "name" );

    if ( lobby_name && lobby_name[0] ) {
      printf( "Client >> Lobby name found: %s\n", lobby_name );

      if ( strcmp( lobby_name, "Conquistador's lobby" ) == 0 ) {
        printf( "Client >> Found %s, attempting join\n", lobby_name );

        SteamAPICall_t steam_api_call =
          SteamMatchmaking()->JoinLobby( steam_lobby_id );

        result_lobby_entered.Set(
          steam_api_call, this, &Client::OnLobbyEntered
        );
      }
    }
    else {
      SteamMatchmaking()->RequestLobbyData( steam_lobby_id );
    }
  }
}

inline void Client::OnLobbyEntered( LobbyEnter_t *cb, bool io_failure ) {
  if ( cb->m_EChatRoomEnterResponse != k_EChatRoomEnterResponseSuccess ) {
    printf( "Client >> Failed to enter lobby...\n" );
    return;
  }

  printf( "Client >> Joined the lobby!\n" );

  char msg[4 * 1024];
  sprintf_s(
    msg,
    sizeof( msg ),
    "Player %s joined as Client\n",
    SteamFriends()->GetPersonaName()
  );

  SteamMatchmaking()->SendLobbyChatMsg(
    cb->m_ulSteamIDLobby, msg, sizeof( msg )
  );

  // TODO might need the servers id instaed of the lobby id
  Join( cb->m_ulSteamIDLobby );
}

};// namespace Network
