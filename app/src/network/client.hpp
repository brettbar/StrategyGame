/*
  0. Clicked Join Game
  1. 
 */

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

    void CheckForMessage() {
      if ( _server_conn != k_HSteamNetConnection_Invalid ) {
        SteamNetworkingMessage_t *msg;
        int r = SteamNetworkingSockets()->ReceiveMessagesOnConnection(
          _server_conn, &msg, 1
        );

        assert( r == 0 || r == 1 );

        if ( r == 1 ) {
          char *message = (char *) msg->GetData();

          nlohmann::json body = nlohmann::json::parse( message );

          if ( body["type"] == "player_connected" ) {
            auto player_id = body["data"]["player_id"];
            auto name = body["data"]["steam_name"];

            if ( !_peers.contains( player_id ) ) {
              _peers[player_id] = name;
            }
          }

          printf( "Received message: '%s'\n", message );
          msg->Release();
        }
      }
    }

    void Delete() {
      delete this;
    }

    std::vector<CSteamID> GetLobbyList() {
      std::vector<CSteamID> lobby_list = {};
      for ( uint32 i_lobby = 0; i_lobby < _lobby_list_arr; i_lobby++ ) {
        CSteamID steam_lobby_id =
          SteamMatchmaking()->GetLobbyByIndex( i_lobby );
        lobby_list.push_back( steam_lobby_id );
      }
      return lobby_list;
    }


    std::vector<std::string> GetConnectedUsers() {
      std::vector<std::string> list = {};

      for ( auto &[key, val]: _peers ) {
        list.push_back( key + ": " + val );
      }

      return list;
    }

    bool AttemptJoinLobby( CSteamID lobby_id ) {
      const char *lobby_name =
        SteamMatchmaking()->GetLobbyData( lobby_id, "name" );

      if ( lobby_name && lobby_name[0] ) {
        if ( strcmp( lobby_name, "Conquistador's lobby" ) == 0 ) {
          printf( "IClient >> Found %s, attempting join\n", lobby_name );

          SteamAPICall_t steam_api_call =
            SteamMatchmaking()->JoinLobby( lobby_id );

          result_lobby_entered.Set(
            steam_api_call, this, &IClient::OnLobbyEntered
          );

          return true;
        }
      }
      else {
        SteamMatchmaking()->RequestLobbyData( lobby_id );
      }

      return false;
    }

private:
    IClient( IClient const & ) = delete;
    void operator=( const IClient & ) = delete;


    HSteamNetConnection _server_conn;
    u32 _lobby_list_arr;

    std::map<std::string, std::string> _peers = {};

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
    _lobby_list_arr = cb->m_nLobbiesMatching;
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
      "Player %s joined lobby\n",
      SteamFriends()->GetPersonaName()
    );
#else
    snprintf(
      msg,
      sizeof( msg ),
      "Player %s joined lobby\n",
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
      case k_ESteamNetworkingConnectionState_Connected: {
        printf( "IClient >> Connected!...\n" );
        break;
      }
    }
  }

  inline void IClient::InitiateServerConnection( CSteamID owner_id ) {
    // TODO move to after game is started?
    // if ( Network::lobby_id.IsValid() ) {
    //   printf( "Initiating connection, can leave lobby now\n" );
    //   SteamMatchmaking()->LeaveLobby( Network::lobby_id );
    // }

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

    SendMessageOnConnection( _server_conn, "IClient >> Hello from IClient!!!" );

    // TODO make this only close when the game is started
    // SteamMatchmaking()->LeaveLobby( _lobby_id );
  }

};// namespace Network
