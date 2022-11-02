use std::{env, sync::mpsc};
use steamworks::*;

fn main() {
    let args: Vec<String> = env::args().collect();

    let (client, single) = Client::init().unwrap();

    let matchmaking = client.matchmaking();
    let networking = client.networking();

    let choice = &args[1];

    match choice.as_str() {
        "host" => run_host(matchmaking, single),
        "client" => run_client(matchmaking, single),
        _ => (),
    }
}

fn run_host(matchmaking: Matchmaking<ClientManager>, single: SingleClient) {
    println!("We are the host");

    matchmaking.create_lobby(LobbyType::FriendsOnly, 8, move |lobby| {
        println!("CB");
        match lobby {
            Ok(lobby) => {
                println!("Created lobby {:?}", lobby);
            }
            Err(_) => println!("error"),
        };
    });

    loop {
        single.run_callbacks();
    }
}

fn run_client(matchmaking: Matchmaking<ClientManager>, single: SingleClient) {
    println!("We are the client");

    let lobby: Result<u64, _> = "25559040".parse();
    matchmaking.join_lobby(
        LobbyId::from_raw(lobby.unwrap()),
        move |result| match result {
            Ok(_) => println!("Joined lobby!"),
            Err(_) => println!("Error joining lobby"),
        },
    );

    matchmaking.request_lobby_list(move |lobbies| {
        println!("{:?}", lobbies);
    });

    loop {
        single.run_callbacks();
    }
}
