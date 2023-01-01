#pragma once

#include "network.hpp"
#include "steam/isteammatchmaking.h"
#include "steam/isteamnetworkingsockets.h"
#include "steam/steamclientpublic.h"
#include "steam/steamnetworkingtypes.h"

namespace Network {


struct Client {
  CSteamID _player_ids[MAX_PLAYERS_PER_SERVER];
  CSteamID _server_id;
  CSteamID _lobby_id;
  HSteamNetConnection _server_conn;

  SteamNetworkingIdentity remote;

  void OnLobbyMatchList( LobbyMatchList_t *, bool );
  CCallResult<Client, LobbyMatchList_t> result_lobby_match_list;

  void OnLobbyEntered( LobbyEnter_t *, bool );
  CCallResult<Client, LobbyEnter_t> result_lobby_entered;

  STEAM_CALLBACK( Client, OnLobbyChatMsg, LobbyChatMsg_t );
  STEAM_CALLBACK(
    Client,
    OnNetConnectionStatusChanged,
    SteamNetConnectionStatusChangedCallback_t
  );

  void InitiateServerConnection( CSteamID );

  ~Client() {
    printf( "~Client()\n" );
    if ( _server_conn != k_HSteamNetConnection_Invalid )
      SteamNetworkingSockets()->CloseConnection(
        _server_conn, k_ESteamNetConnectionEnd_App_Min + 1, nullptr, false
      );

    _server_id = CSteamID();
    _server_conn = k_HSteamNetConnection_Invalid;
  }

  void Init() {
    SteamAPICall_t steam_api_call = SteamMatchmaking()->RequestLobbyList();
    result_lobby_match_list.Set(
      steam_api_call, this, &Client::OnLobbyMatchList
    );
  }

  void CheckForMessages() {}
};

inline void Client::OnLobbyChatMsg( LobbyChatMsg_t *param ) {
  char buf[4 * 1024];
  SteamMatchmaking()->GetLobbyChatEntry(
    param->m_ulSteamIDLobby, param->m_iChatID, nullptr, buf, sizeof( buf ), NULL
  );

  printf( "Lobby: %s\n", buf );
}

inline void Client::OnLobbyMatchList( LobbyMatchList_t *cb, bool io_failure ) {
  for ( uint32 i_lobby = 0; i_lobby < cb->m_nLobbiesMatching; i_lobby++ ) {
    CSteamID steam_lobby_id = SteamMatchmaking()->GetLobbyByIndex( i_lobby );

    const char *lobby_name =
      SteamMatchmaking()->GetLobbyData( steam_lobby_id, "name" );

    if ( lobby_name && lobby_name[0] ) {
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

  _lobby_id = cb->m_ulSteamIDLobby;

  char msg[4 * 1024];
  sprintf_s(
    msg,
    sizeof( msg ),
    "Player %s joined as Client\n",
    SteamFriends()->GetPersonaName()
  );

  SteamMatchmaking()->SendLobbyChatMsg( _lobby_id, msg, sizeof( msg ) );

  CSteamID owner_id = SteamMatchmaking()->GetLobbyOwner( _lobby_id );

  InitiateServerConnection( owner_id );
}

inline void Client::OnNetConnectionStatusChanged(
  SteamNetConnectionStatusChangedCallback_t *cb
) {
  printf( "Client >> !!!!!!!OnNetConnectionStatusChanged\n" );
  printf( "current state is %d\n", cb->m_info.m_eState );

  switch ( cb->m_info.m_eState ) {
    case k_ESteamNetworkingConnectionState_Connecting: {
      printf( "Client >> Connecting?...\n" );
    } break;
  }

  // SteamNetworkingSockets()->AcceptConnection( cb->m_hConn );
}

inline void Client::InitiateServerConnection( CSteamID owner_id ) {
  if ( _lobby_id.IsValid() ) {
    SteamMatchmaking()->LeaveLobby( _lobby_id );
  }

  _server_id = owner_id;


  // This is useful for localhost testing on the same machine
  SteamNetworkingIPAddr addr;
  addr.SetIPv6LocalHost( 10202 );
  printf( "Client listening on addr: %d\n", addr.GetIPv4() );
  _server_conn =
    SteamNetworkingSockets()->ConnectByIPAddress( addr, 0, nullptr );
  if ( _server_conn == k_HSteamNetConnection_Invalid ) {
    SteamNetworkingSockets()->CloseConnection(
      _server_conn,
      k_ESteamNetConnectionEnd_App_Generic,
      "Failed to join, closing conn\n",
      false
    );
  }


  /// This is useful for real production, different machines
  // SteamNetworkingIdentity identity;
  // identity.SetSteamID( owner_id );
  // _server_conn =
  //   SteamNetworkingSockets()->ConnectP2P( identity, 0, 0, nullptr );
  // assert( _server_conn != k_HSteamNetConnection_Invalid );

  SendMessageToPeer( _server_conn, "Client >> Hello from Client!!!\n" );
}

};// namespace Network
