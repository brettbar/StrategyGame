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

#include "../shared/utils.hpp"

#include "../signals/updates.hpp"

namespace Network
{
  class IClient
  {
private:
    IClient( IClient const & ) = delete;
    void operator=( const IClient & ) = delete;

    HSteamNetConnection _server_conn;
    u32 _lobby_list_arr;

    // std::map<std::string, PeerData> _peers = {};
    std::array<PeerData, Network::MAX_PLAYERS_PER_SERVER> _peers;

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

    IClient()
    {
      _peers = {};
    }
    ~IClient()
    {
      printf( "~IClient()\n" );
      if ( _server_conn != k_HSteamNetConnection_Invalid )
        SteamNetworkingSockets()->CloseConnection(
          _server_conn, k_ESteamNetConnectionEnd_App_Min + 1, nullptr, false
        );

      _server_conn = k_HSteamNetConnection_Invalid;
    }

public:
    // TODO make private
    std::string player_id;

    static IClient *Client()
    {
      static IClient instance;
      return &instance;
    }
    void Init()
    {
      SteamAPICall_t steam_api_call = SteamMatchmaking()->RequestLobbyList();
      result_lobby_match_list.Set(
        steam_api_call, this, &IClient::OnLobbyMatchList
      );

      Network::is_host = false;
    }

    void Update()
    {
      if ( _server_conn == k_HSteamNetConnection_Invalid )
        return;

      SteamNetworkingMessage_t *msg;
      int r = SteamNetworkingSockets()->ReceiveMessagesOnConnection(
        _server_conn, &msg, 1
      );

      assert( r == 0 || r == 1 );

      if ( r == 1 )
      {
        char *raw_message = (char *) msg->GetData();
        nlohmann::json message = nlohmann::json::parse( raw_message );
        MessageID message_id = message["message_id"];
        auto body = message["body"];

        HandleMessageSwitch( message_id, body );

        printf( "Received message: '%s'\n", raw_message );
        msg->Release();
      }
    }

    void HandleMessageSwitch(
      MessageID message_id,
      nlohmann::basic_json<> body
    )
    {
      switch ( message_id )
      {
        case MessageID::Ping:
          break;
        case MessageID::AssignedPlayerId:
        {
          player_id = body["new_player_id"];
          InterfaceUpdate::Text( InterfaceUpdate::ID::JoinLobby )
            .SetTarget( player_id + "_label" )
            .SetText( player_id )
            .build()
            .send();
          InterfaceUpdate::Clickable( InterfaceUpdate::ID::JoinLobby, true )
            .SetTarget( player_id + "_faction_selection" )
            .build()
            .send();
          InterfaceUpdate::Background( InterfaceUpdate::ID::JoinLobby, GREEN )
            .SetTarget( player_id + "_faction_selection" )
            .build()
            .send();
        }
        break;
        case MessageID::PlayerConnected:
        {
          auto data = body["data"];
          std::string player_id = data["player_id"];
          u64 steam_user_id_u64 = data["steam_user_id"];

          for ( u32 i = 0; i < MAX_PLAYERS_PER_SERVER; i++ )
          {
            if ( "player_" + std::to_string( i ) == player_id )
            {
              _peers[i] = PeerData{
                .player_id = player_id,
                .faction = "",
                .active = true,
                .readied_up = false,
                .steam_user_id = CSteamID( steam_user_id_u64 ),
              };

              InterfaceUpdate::Text( InterfaceUpdate::ID::JoinLobby )
                .SetTarget( player_id + "_label" )
                .SetText( player_id )
                .build()
                .send();
              InterfaceUpdate::Clickable( InterfaceUpdate::ID::JoinLobby, true )
                .SetTarget( player_id + "_faction_selection" )
                .build()
                .send();
              InterfaceUpdate::Background(
                InterfaceUpdate::ID::JoinLobby, PURPLE
              )
                .SetTarget( player_id + "_faction_selection" )
                .build()
                .send();
            }
          }
        }
        break;
        case MessageID::PlayerDisconnected:
          break;
        case MessageID::PlayerFactionSelect:
        {
          std::string player_id = body["player_id"];
          std::string faction = body["faction"];
          std::string target = player_id + "_select_faction";

          InterfaceUpdate::Text( InterfaceUpdate::ID::FactionSelected )
            .SetTarget( target )
            .SetText( faction )
            .build()
            .send();


          InterfaceUpdate::Background(
            InterfaceUpdate::ID::FactionSelected,
            GetPrimaryFactionColor( faction )
          )
            .SetTarget( target )
            .build()
            .send();
        }
        break;
        default:
          printf( "INVALID MESSAGE ID RECEIVED!!!\n" );
          break;
      }
    }

    void Delete()
    {
      delete this;
    }

    std::vector<CSteamID> GetLobbyList()
    {
      std::vector<CSteamID> lobby_list = {};
      for ( uint32 i_lobby = 0; i_lobby < _lobby_list_arr; i_lobby++ )
      {
        CSteamID steam_lobby_id =
          SteamMatchmaking()->GetLobbyByIndex( i_lobby );
        lobby_list.push_back( steam_lobby_id );
      }
      return lobby_list;
    }


    std::vector<PeerData> GetConnectedUsers()
    {
      std::vector<PeerData> list = {};

      for ( auto &peer: _peers )
      {
        list.push_back( peer );
      }

      return list;
    }

    bool AttemptJoinLobby( CSteamID lobby_id )
    {
      const char *lobby_name =
        SteamMatchmaking()->GetLobbyData( lobby_id, "name" );

      if ( lobby_name && lobby_name[0] )
      {
        if ( strcmp( lobby_name, "Conquistador's lobby" ) == 0 )
        {
          printf( "IClient >> Found %s, attempting join\n", lobby_name );

          SteamAPICall_t steam_api_call =
            SteamMatchmaking()->JoinLobby( lobby_id );

          result_lobby_entered.Set(
            steam_api_call, this, &IClient::OnLobbyEntered
          );

          return true;
        }
      }
      else
      {
        SteamMatchmaking()->RequestLobbyData( lobby_id );
      }

      return false;
    }

    void SendMessageToHost( Message message )
    {
      SendMessageOnConnection( _server_conn, message );
    }
  };

  inline IClient *Client()
  {
    return IClient::Client();
  }

  inline void IClient::OnLobbyChatMsg( LobbyChatMsg_t *param )
  {
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

  inline void IClient::OnLobbyMatchList( LobbyMatchList_t *cb, bool io_failure )
  {
    _lobby_list_arr = cb->m_nLobbiesMatching;
  }

  inline void IClient::OnLobbyEntered( LobbyEnter_t *cb, bool io_failure )
  {
    if ( cb->m_EChatRoomEnterResponse != k_EChatRoomEnterResponseSuccess )
    {
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
  )
  {
    switch ( cb->m_info.m_eState )
    {
      case k_ESteamNetworkingConnectionState_Connecting:
      {
        printf( "IClient >> Connecting?...\n" );
      }
      break;
      case k_ESteamNetworkingConnectionState_Connected:
      {
        printf( "IClient >> Connected!...\n" );
        break;
      }
    }
  }

  inline void IClient::InitiateServerConnection( CSteamID owner_id )
  {
    // TODO move to after game is started?
    // if ( Network::lobby_id.IsValid() ) {
    //   printf( "Initiating connection, can leave lobby now\n" );
    //   SteamMatchmaking()->LeaveLobby( Network::lobby_id );
    // }

    // This is useful for localhost testing on the same machine
    if ( LOCAL )
    {
      SteamNetworkingIPAddr addr;
      addr.SetIPv6LocalHost( 10202 );
      printf( "IClient listening on addr: %d\n", addr.GetIPv4() );
      _server_conn =
        SteamNetworkingSockets()->ConnectByIPAddress( addr, 0, nullptr );
    }
    /// This is useful for real production, different machines
    else
    {
      SteamNetworkingIdentity identity;
      identity.SetSteamID( owner_id );
      _server_conn =
        SteamNetworkingSockets()->ConnectP2P( identity, 0, 0, nullptr );
    }

    if ( _server_conn == k_HSteamNetConnection_Invalid )
    {
      SteamNetworkingSockets()->CloseConnection(
        _server_conn,
        k_ESteamNetConnectionEnd_App_Generic,
        "Failed to join, closing conn\n",
        false
      );
      return;
    }

    SendMessageOnConnection(
      _server_conn,
      Message{ MessageID::InitiateContact, "IClient >> Hello from IClient!!!" }
    );

    // TODO make this only close when the game is started
    // SteamMatchmaking()->LeaveLobby( _lobby_id );
  }


};// namespace Network
