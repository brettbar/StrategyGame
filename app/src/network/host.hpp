/*
  0. Clicked Host Game
  1. Host() :: Host singleton is Created, Networking is setup using IP or P2P
  2. Init() :: Lobby is Created, MessageQueue begins processing messages
  3. OnLobbyCreated() :: Lobby is created and people can now connect
  4. Client attempts to connect, triggering SteamNetConnectionStatusChangedCallback_t,
  5. OnNetConnectionStatusChanged() :: connection is checks and accepts client connection, giving slot in _clients
  6. Once a client joins, all 
 */

#pragma once

#include "../shared/utils.hpp"
#include "network.hpp"
#include <chrono>
#include <isteammatchmaking.h>

#include "../signals/updates.hpp"

namespace Network
{

  namespace
  {
    struct ClientConnectionData
    {
      PeerData peer_data;
      HSteamNetConnection conn;
      long latest_timestamp;
      u32 ping_ms;

      ClientConnectionData()
      {
        peer_data.player_id = "";
        peer_data.active = false;
        conn = 0;
        latest_timestamp = TimestampMS();
        ping_ms = 1000;
      }
    };

    struct MessageQueue : entt::dispatcher
    {
      void Enqueue( const Message &msg )
      {
        this->enqueue( msg );
      }
    };
  };// namespace


  class IHost
  {
private:
    IHost( IHost const & ) = delete;
    void operator=( const IHost & ) = delete;

    CSteamID _server_id;

    // Used after lobby is done
    // ClientConnectionData _clients[MAX_PLAYERS_PER_SERVER];
    std::array<ClientConnectionData, MAX_PLAYERS_PER_SERVER> _clients;

    HSteamListenSocket _socket;
    // HSteamNetPollGroup _poll_grp;

    void OnLobbyCreated( LobbyCreated_t *, bool );
    CCallResult<IHost, LobbyCreated_t> result_lobby_created;

    STEAM_CALLBACK( IHost, OnLobbyChatMsg, LobbyChatMsg_t );

    STEAM_CALLBACK(
      IHost,
      OnNetConnectionStatusChanged,
      SteamNetConnectionStatusChangedCallback_t
    );

    IHost()
    {
      // zero the client connection data
      // memset( &_clients, 0, sizeof( _clients ) );
      _clients = {};


      // _poll_grp = SteamNetworkingSockets()->CreatePollGroup();

      // make this if debug
      if ( LOCAL )
        SetupUsingIP();
      else
        SetupUsingP2P();
    }

    ~IHost()
    {
      // Close connection
      printf( "~IHost()\n" );

      SteamNetworkingSockets()->CloseListenSocket( _socket );
      // SteamNetworkingSockets()->DestroyPollGroup( poll_grp );
    }

    // This is useful for localhost testing on the same machine
    void SetupUsingIP()
    {
      SteamNetworkingIPAddr addr = SteamNetworkingIPAddr();
      addr.SetIPv6LocalHost( 10202 );

      printf( "Hosting with addr: %d\n", addr.GetIPv4() );
      _socket =
        SteamNetworkingSockets()->CreateListenSocketIP( addr, 0, nullptr );
      assert( _socket != k_HSteamListenSocket_Invalid );
    }

    // This is useful for real production, different machines
    void SetupUsingP2P()
    {
      _socket =
        SteamNetworkingSockets()->CreateListenSocketP2P( 0, 0, nullptr );
      assert( _socket != k_HSteamListenSocket_Invalid );
    }


public:
    // TODO make private
    std::string _player_id = "player_0";

    MessageQueue _msg_queue;

    static IHost *Host()
    {
      static IHost instance;
      return &instance;
    }

    void Init()
    {
      Network::is_host = true;

      SteamAPICall_t steam_api_call = SteamMatchmaking()->CreateLobby(
        k_ELobbyTypePublic, MAX_PLAYERS_PER_SERVER
      );

      result_lobby_created.Set( steam_api_call, this, &IHost::OnLobbyCreated );

      _msg_queue = {};

      _msg_queue.sink<Message>().connect<&IHost::ProcessQueuedMessageSwitch>(
        this
      );
    }

    // TODO(rf) this is a bit clunky
    // This is for the case where the host selected his faction
    // before any players connected
    inline void SetHostFaction( std::string faction )
    {
      _clients[0].peer_data.faction = faction;
    }

    void Update()
    {
      CheckForMessages();
      EvaluateMessages();
    }

    void CheckForMessages()
    {
      for ( uint32 i = 0; i < MAX_PLAYERS_PER_SERVER; ++i )
      {
        if ( !_clients[i].peer_data.active || _clients[i].conn == k_HSteamNetConnection_Invalid )
          continue;

        SteamNetworkingMessage_t *msg;
        int r = SteamNetworkingSockets()->ReceiveMessagesOnConnection(
          _clients[i].conn, &msg, 1
        );

        assert( r == 0 || r == 1 );

        if ( r == 1 )
        {
          // printf( "Received message: '%s'\n", (char *) msg->GetData() );
          char *raw_message = (char *) msg->GetData();
          nlohmann::json message = nlohmann::json::parse( raw_message );
          MessageID message_id = message["message_id"];
          auto body = message["body"];


          _msg_queue.Enqueue( Message{
            message_id,
            body,
          } );

          msg->Release();
        }
      }
    }

    void EvaluateMessages()
    {
      _msg_queue.update();
    }

    void ProcessQueuedMessageSwitch( const Message msg )
    {
      switch ( msg.message_id )
      {
        case ClientPingResponse:
        {
          // printf(
          //   "Got ping response from client! %s\n", msg.body.dump().c_str()
          // );
          auto player_id = msg.body["player_id"];
          u32 index = player_id_index[player_id];

          auto newest_timestamp = TimestampMS();

          long latency = newest_timestamp - _clients[index].latest_timestamp -
                         1000;// take out the 1 second of delay we baked in

          if ( latency < 0 )
            latency = 0;

          // printf( "latency: %ldms\n", latency );

          // TODO some condition on latency or if its been too long since last resp
          _clients[index].latest_timestamp = newest_timestamp;
        }
        break;
        case PlayerFactionSelect:
        {
          std::string player_id = msg.body["player_id"];
          std::string faction = msg.body["faction"];
          std::string target = player_id + "_select_faction";

          u32 index = player_id_index[player_id];
          _clients[index].peer_data.faction = faction;

          // Tell all the other clients about it
          for ( u32 i = 1; i < MAX_PLAYERS_PER_SERVER; i++ )
          {
            if ( !_clients[i].peer_data.active )
              continue;

            SendMessageOnConnection(
              _clients[i].conn,
              Message{
                MessageID::PlayerFactionSelect,
                nlohmann::json{
                  { "player_id", player_id },
                  { "faction", faction },
                } }
            );
          }

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
        default:
          break;
      }
    }

    void PingAllActiveClients()
    {
      for ( uint32 i = 1; i < MAX_PLAYERS_PER_SERVER; i++ )
      {
        if ( !_clients[i].peer_data.active )
          continue;

        auto timestamp =
          std::chrono::system_clock::now().time_since_epoch().count();

        SendMessageOnConnection(
          _clients[i].conn,
          Message{
            MessageID::HostPingRequest,
            std::to_string( timestamp ).c_str(),
          }
        );
      }
    }

    void SendMessageToAllClients( Message message )
    {

      // We start at 1 since host is always i=0
      for ( uint32 i = 1; i < MAX_PLAYERS_PER_SERVER; i++ )
      {
        if ( !_clients[i].peer_data.active )
          continue;

        SendMessageOnConnection( _clients[i].conn, message );
      }
    }


    void Delete()
    {
      delete this;
    }

    std::vector<Network::PeerData> GetConnectedUsers()
    {
      std::vector<PeerData> clients = {};

      for ( u32 i = 0; i < MAX_PLAYERS_PER_SERVER; i++ )
      {
        if ( _clients[i].peer_data.active )
        {
          clients.push_back( _clients[i].peer_data );
        }
      }

      return clients;
    }
  };

  inline void IHost::OnLobbyCreated( LobbyCreated_t *cb, bool io_failure )
  {
    if ( cb->m_eResult == k_EResultOK )
    {
      printf( "Lobby created successfully\n" );

      Network::lobby_id = cb->m_ulSteamIDLobby;

      char rgchLobbyName[256];
#if _WINDLL
      sprintf_s(
        rgchLobbyName, "%s's lobby", SteamFriends()->GetPersonaName()
      );
#else
      snprintf(
        rgchLobbyName,
        sizeof( rgchLobbyName ),
        "%s's lobby",
        SteamFriends()->GetPersonaName()
      );
#endif

      SteamMatchmaking()->SetLobbyData(
        Network::lobby_id, "name", rgchLobbyName
      );
      printf( "Hosting lobby %s\n", rgchLobbyName );

      // Initialize ourself as player_0 since we are the host
      _clients[0].peer_data.player_id = "player_0";
      _clients[0].peer_data.active = true;
      _clients[0].peer_data.steam_user_id =
        SteamMatchmaking()->GetLobbyOwner( Network::lobby_id );
    }
    else
    {
      printf( "Failed to create lobby, lost connection to steam back-end\n" );
    }
  }

  inline void IHost::OnLobbyChatMsg( LobbyChatMsg_t *param )
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

  inline void IHost::OnNetConnectionStatusChanged(
    SteamNetConnectionStatusChangedCallback_t *cb
  )
  {
    SteamNetConnectionInfo_t info = cb->m_info;
    ESteamNetworkingConnectionState old_state = cb->m_eOldState;

    if ( _socket == k_HSteamListenSocket_Invalid )
    {
      printf( "Invalid socket, bailing\n" );
      return;
    }

    if ( info.m_hListenSocket != _socket )
    {
      printf( "Socket doesn't match?, bailing\n" );
      return;
    }


    // Check if a client has connected
    if ( info.m_eState == k_ESteamNetworkingConnectionState_Connecting && old_state == k_ESteamNetworkingConnectionState_None )
    {
      printf(
        "Accepting client connection %s\n", info.m_szConnectionDescription
      );

      // Connection from a new client
      // Search for an available slot
      for ( int i = 1; i < MAX_PLAYERS_PER_SERVER; ++i )
      {
        if ( _clients[i].peer_data.active )
          continue;

        EResult res = SteamNetworkingSockets()->AcceptConnection( cb->m_hConn );

        if ( res != k_EResultOK )
        {
          char msg[256];
#if _WINDLL
          sprintf_s( msg, "AcceptConnection returned %d", res );
#else
          snprintf( msg, sizeof( msg ), "AcceptConnection returned %d", res );
#endif

          printf( "%s\n", msg );

          SteamNetworkingSockets()->CloseConnection(
            cb->m_hConn,
            k_ESteamNetConnectionEnd_AppException_Generic,
            "Failed to accept connection",
            false
          );
          return;
        }

        // We found a slot, accepted the connection,
        // and are ready to set data for the client
        _clients[i].peer_data.player_id = "player_" + std::to_string( i );
        _clients[i].peer_data.steam_user_id =
          info.m_identityRemote.GetSteamID();
        _clients[i].peer_data.active = true;
        _clients[i].conn = cb->m_hConn;

        // Tell the new client which player id they are
        nlohmann::json new_player_id_body = {};
        new_player_id_body["new_player_id"] = _clients[i].peer_data.player_id;
        SendMessageOnConnection(
          _clients[i].conn,
          Message{ MessageID::AssignedPlayerId, new_player_id_body }
        );

        auto player_id = _clients[i].peer_data.player_id;
        InterfaceUpdate::Text( InterfaceUpdate::ID::JoinLobby )
          .SetTarget( player_id + "_label" )
          .SetText( player_id )
          .build()
          .send();
        InterfaceUpdate::Background( InterfaceUpdate::ID::JoinLobby, PURPLE )
          .SetTarget( player_id + "_faction_selection" )
          .build()
          .send();

        // Tell the new client about all the current clients
        for ( auto &client: _clients )
        {
          if ( !client.peer_data.active )
            continue;

          nlohmann::json body = {};
          body["data"] = {
            { "is_host", ( client.peer_data.player_id == "player_0" ) },
            { "player_id", client.peer_data.player_id },
            { "faction", client.peer_data.faction },
            {
              "steam_name",
              std::string( SteamFriends()->GetFriendPersonaName(
                client.peer_data.steam_user_id
              ) ),
            },
            {
              "steam_user_id",
              client.peer_data.steam_user_id.ConvertToUint64(),
            },
          };

          SendMessageOnConnection(
            _clients[i].conn, Message{ MessageID::PlayerConnected, body }
          );
        }


        // Tell all the current clients about the new client
        nlohmann::json body = {};
        body["data"] = {
          { "is_host", false },
          { "player_id", _clients[i].peer_data.player_id },
          { "faction", _clients[i].peer_data.faction },
          {
            "steam_name",
            std::string( SteamFriends()->GetFriendPersonaName(
              _clients[i].peer_data.steam_user_id
            ) ),
          },
          {
            "steam_user_id",
            _clients[i].peer_data.steam_user_id.ConvertToUint64(),
          },
        };

        // We start at 1 since the host already knows
        for ( uint32 j = 1; j < MAX_PLAYERS_PER_SERVER; j++ )
        {
          if ( !_clients[i].peer_data.active || i == j )// we don't need to tell ourselves
            continue;

          SendMessageOnConnection(
            _clients[j].conn, Message{ MessageID::PlayerConnected, body }
          );
        }


        // TODO make this only close when the game is started
        // SteamMatchmaking()->LeaveLobby( lobby_id );

        return;
      }

      printf( "Rejecting connection; server full\n" );
      SteamNetworkingSockets()->CloseConnection(
        cb->m_hConn,
        k_ESteamNetConnectionEnd_AppException_Generic,
        "Server full!",
        false
      );
    }
    // check if client has disconnected
    else if ( ( old_state == k_ESteamNetworkingConnectionState_Connecting || old_state == k_ESteamNetworkingConnectionState_Connected ) && info.m_eState == k_ESteamNetworkingConnectionState_ClosedByPeer )
    {
      for ( uint32 i = 1; i < MAX_PLAYERS_PER_SERVER; ++i )
      {
        if ( !_clients[i].peer_data.active )
          continue;

        if ( _clients[i].peer_data.steam_user_id == info.m_identityRemote.GetSteamID() )
        {
          printf( "Disconnected dropped user\n" );

          SteamNetworkingSockets()->CloseConnection(
            _clients[i].conn,
            k_ESteamNetConnectionEnd_App_Min + 1,
            nullptr,
            false
          );

          _clients[i].peer_data.active = false;
          _clients[i].peer_data.readied_up = false;
          _clients[i].peer_data.player_id = "";
          _clients[i].peer_data.faction = "";
          _clients[i].peer_data.steam_user_id = CSteamID();
        }
      }
    }
  }


  inline IHost *Host()
  {
    return IHost::Host();
  }


};// namespace Network
