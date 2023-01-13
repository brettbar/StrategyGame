#pragma once

#include "network.hpp"
#include "steam/isteammatchmaking.h"
#include "steam/isteamnetworkingsockets.h"
#include "steam/steamclientpublic.h"
#include "steam/steamnetworkingtypes.h"

namespace Network {


  class IClient {
public:
    static IClient *Client() {
      static IClient instance;
      return &instance;
    }
    void Init() {
      SteamAPICall_t steam_api_call = SteamMatchmaking()->RequestLobbyList();
      result_lobby_match_list.Set(
        steam_api_call, this, &IClient::OnLobbyMatchList
      );

      Network::is_host = false;
    }

    void CheckForMessages() {
      Network::CheckForMessages( _server_conn );
    }

    void Delete() {
      delete this;
    }

private:
    IClient( IClient const & ) = delete;
    void operator=( const IClient & ) = delete;

    CSteamID _player_ids[MAX_PLAYERS_PER_SERVER];
    CSteamID _server_id;
    // CSteamID _lobby_id;

    HSteamNetConnection _server_conn;

    void OnLobbyMatchList( LobbyMatchList_t *, bool );
    CCallResult<IClient, LobbyMatchList_t> result_lobby_match_list;

    void OnLobbyEntered( LobbyEnter_t *, bool );
    CCallResult<IClient, LobbyEnter_t> result_lobby_entered;

    STEAM_CALLBACK( IClient, OnLobbyChatMsg, LobbyChatMsg_t );
    STEAM_CALLBACK(
      IClient,
      OnNetConnectionStatusChanged,
      SteamNetConnectionStatusChangedCallback_t
    );

    void InitiateServerConnection( CSteamID );

    IClient() {}
    ~IClient() {
      printf( "~IClient()\n" );
      if ( _server_conn != k_HSteamNetConnection_Invalid )
        SteamNetworkingSockets()->CloseConnection(
          _server_conn, k_ESteamNetConnectionEnd_App_Min + 1, nullptr, false
        );

      _server_id = CSteamID();
      _server_conn = k_HSteamNetConnection_Invalid;
    }
  };

  inline IClient *Client() {
    return IClient::Client();
  }

  inline void IClient::OnLobbyChatMsg( LobbyChatMsg_t *param ) {
    char buf[4 * 1024];
    SteamMatchmaking()->GetLobbyChatEntry(
      param->m_ulSteamIDLobby,
      param->m_iChatID,
      nullptr,
      buf,
      sizeof( buf ),
      NULL
    );

    printf( "Lobby: %s\n", buf );
  }

  inline void
  IClient::OnLobbyMatchList( LobbyMatchList_t *cb, bool io_failure ) {
    for ( uint32 i_lobby = 0; i_lobby < cb->m_nLobbiesMatching; i_lobby++ ) {
      CSteamID steam_lobby_id = SteamMatchmaking()->GetLobbyByIndex( i_lobby );

      const char *lobby_name =
        SteamMatchmaking()->GetLobbyData( steam_lobby_id, "name" );

      if ( lobby_name && lobby_name[0] ) {
        if ( strcmp( lobby_name, "Conquistador's lobby" ) == 0 ) {
          printf( "IClient >> Found %s, attempting join\n", lobby_name );

          SteamAPICall_t steam_api_call =
            SteamMatchmaking()->JoinLobby( steam_lobby_id );

          result_lobby_entered.Set(
            steam_api_call, this, &IClient::OnLobbyEntered
          );
        }
      }
      else {
        SteamMatchmaking()->RequestLobbyData( steam_lobby_id );
      }
    }
  }

  inline void IClient::OnLobbyEntered( LobbyEnter_t *cb, bool io_failure ) {
    if ( cb->m_EChatRoomEnterResponse != k_EChatRoomEnterResponseSuccess ) {
      printf( "IClient >> Failed to enter lobby...\n" );
      return;
    }

    Network::lobby_id = cb->m_ulSteamIDLobby;

    char msg[4 * 1024];
#if _WINDLL
    sprintf_s(
      msg,
      sizeof( msg ),
      "Player %s joined as IClient\n",
      SteamFriends()->GetPersonaName()
    );
#else
    snprintf(
      msg,
      sizeof( msg ),
      "Player %s joined as IClient\n",
      SteamFriends()->GetPersonaName()
    );
#endif

    SteamMatchmaking()->SendLobbyChatMsg(
      Network::lobby_id, msg, sizeof( msg )
    );

    CSteamID owner_id = SteamMatchmaking()->GetLobbyOwner( Network::lobby_id );

    InitiateServerConnection( owner_id );
  }

  inline void IClient::OnNetConnectionStatusChanged(
    SteamNetConnectionStatusChangedCallback_t *cb
  ) {
    switch ( cb->m_info.m_eState ) {
      case k_ESteamNetworkingConnectionState_Connecting: {
        printf( "IClient >> Connecting?...\n" );
      } break;
    }
  }

  inline void IClient::InitiateServerConnection( CSteamID owner_id ) {
    if ( Network::lobby_id.IsValid() ) {
      printf( "Initiating connection, can leave lobby now\n" );
      SteamMatchmaking()->LeaveLobby( Network::lobby_id );
    }

    _server_id = owner_id;


    // This is useful for localhost testing on the same machine
    if ( LOCAL ) {
      SteamNetworkingIPAddr addr;
      addr.SetIPv6LocalHost( 10202 );
      printf( "IClient listening on addr: %d\n", addr.GetIPv4() );
      _server_conn =
        SteamNetworkingSockets()->ConnectByIPAddress( addr, 0, nullptr );
    }
    /// This is useful for real production, different machines
    else {
      SteamNetworkingIdentity identity;
      identity.SetSteamID( owner_id );
      _server_conn =
        SteamNetworkingSockets()->ConnectP2P( identity, 0, 0, nullptr );
    }

    if ( _server_conn == k_HSteamNetConnection_Invalid ) {
      SteamNetworkingSockets()->CloseConnection(
        _server_conn,
        k_ESteamNetConnectionEnd_App_Generic,
        "Failed to join, closing conn\n",
        false
      );
      return;
    }

    SendMessageToPeer( _server_conn, "IClient >> Hello from IClient!!!" );

    // TODO make this only close when the game is started
    SteamMatchmaking()->LeaveLobby( lobby_id );
  }

};// namespace Network
