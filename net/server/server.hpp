// server.hpp
#pragma once

#pragma once

#include "../network.hpp"
#include <atomic>
#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <utility>


struct Server : public ISteamNetworkingSockets {
  std::shared_ptr<Network> network;
  SteamNetworkingIPAddr addr_server;

  std::atomic_flag thread_run = ATOMIC_FLAG_INIT;
  std::unique_ptr<std::thread> listening_thread;
  ISteamNetworkingSockets *network_interface;
  HSteamListenSocket listen_socket;

  struct Client_t {
    std::string nick;
  };

  std::map<HSteamNetConnection, Client_t> client_map;

  std::mutex message_queue_mutex;
  std::queue<std::pair<std::unique_ptr<flatbuffers::FlatBufferBuilder>, int>>
    message_queue;
  inline Server( std::shared_ptr<Network> net, uint16 port )
      : network( std::move( net ) ), addr_server(), listening_thread( nullptr ),
        network_interface( nullptr ), listen_socket() {
    addr_server.Clear();
    addr_server.m_port = port;

    // Select instance to use.  For now we'll always use the default.
    // But we could use SteamGameServerNetworkingSockets() on Steam.
    network_interface = SteamNetworkingSockets();
    listen_socket =
      network_interface->CreateListenSocketIP( addr_server, 0, nullptr );

    if ( listen_socket == k_HSteamListenSocket_Invalid ) {
      std::ostringstream error_message;
      error_message << "Failed to listen to port " << port;
      throw std::runtime_error( error_message.str() );
    }
    std::cout << "Server listening to port " << port << std::endl;
  }

  inline ~Server() {
    if ( listening_thread ) {
      this->close();
    }
  }

  inline void run() {
    if ( listening_thread ) {
      throw std::runtime_error( "Server already running" );
    }

    thread_run.test_and_set();
    listening_thread =
      std::make_unique<std::thread>( [this]() { threaded_run(); } );
  }

  inline void close() {}

  inline void join();

  inline void send_message(
    std::unique_ptr<flatbuffers::FlatBufferBuilder> message_buffer,
    int send_flag = k_nSteamNetworkingSend_Reliable );

  inline void threaded_run();

  inline void poll_incoming_messages();

  inline void poll_connection_state_changes();

  inline void poll_sending_message_queue();

  // Callback
  inline void OnSteamNetConnectionStatusChanged(
    SteamNetConnectionStatusChangedCallback_t *info ) override;

  inline void
  set_client_nick( HSteamNetConnection connection, const std::string &nick );

  inline void send_data_to_client(
    HSteamNetConnection connection,
    const void *data,
    unsigned int data_size,
    int send_flag = k_nSteamNetworkingSend_Reliable );

  inline void send_data_to_all_clients(
    const void *data,
    unsigned int data_size,
    int send_flag = k_nSteamNetworkingSend_Reliable,
    HSteamNetConnection except = k_HSteamNetConnection_Invalid );

  inline void send_string_to_client(
    HSteamNetConnection connection,
    const std::string &text );

  inline void send_string_to_all_clients(
    const std::string &text,
    HSteamNetConnection except = k_HSteamNetConnection_Invalid );
};
