// @todo
// Add an explicity Eval step and Execute step
// Eval:
// - 0. (happens in game regardless of campaign) Process Events
// 1. Receive Messages
// 2. Receive Commands
// 3. Validate Messages
// 4. Validate Commands (like remove duplicates or something)

// Exec:
// 1. Update the campaign state with changes from Commands
// 2. Update UI with Messages

#pragma once


#include "network/client.hpp"
#include "network/host.hpp"
#include "network/network.hpp"
#include "shared/global.hpp"


#include "shared/common.hpp"


// #include "ui/common.h"
#include "ui/common.h"
#include "ui/scenes/campaign/campaign_ui.hpp"

#include "world/components/player_component.hpp"
#include "world/components/settlement_component.hpp"
#include "world/managers/map_manager.hpp"
#include "world/managers/settlement_manager.hpp"
#include "world/systems/ai_system.hpp"
#include "world/systems/animation_system.hpp"
#include "world/systems/commands_system.hpp"
#include "world/systems/movement_system.hpp"
#include "world/systems/player_system.hpp"
#include "world/systems/province_system.hpp"
#include "world/systems/resource_system.hpp"
#include "world/systems/selection_system.hpp"
#include "world/systems/settlement_system.hpp"

#include "renderer.hpp"

#include <optional>
#include <raylib.h>

// If you ever get strange compile time errors from cereal
// Its probably because you didnt include a type
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>

struct Campaign {
  Campaign(bool is_singleplayer) {
    _is_singleplayer = is_singleplayer;

    // Start();
  }

  // @todo delete this stuff
  Campaign(bool is_singleplayer, const char *) {
    _is_singleplayer = is_singleplayer;

    // TODO make take in file path
    // Load();
  }

  str GetLocalPlayerID();

  entt::entity GetLocalPlayerE();

  void common_start();
  void start(str);
  void save(str);
  void start_mp();
  void load(cstr);
  void UpdateOnFrame(f32 &, f32 &, f32 &);
  void Update60TPS();
  void Update1TPS();
  void PostCommand(Commands::Command);

  void Draw();

  void EvaluteCommands(const Commands::Command &);
  // void HandleSpawnRequest( const Commands::Command & );
  void HandleTimeChangeRequest(const Commands::Command &);

  // void ForwardEvent( const InterfaceEvent::Data & );
  void ConvertCommandRequest(str);


  private:
  bool _is_singleplayer = true;
  opt<Buildings::Building> _building_to_build = std::nullopt;
};

inline entt::entity Campaign::GetLocalPlayerE() {
  return Global::world.view<Player::LocalTag>().front();
}

inline str Campaign::GetLocalPlayerID() {
  if (_is_singleplayer || Network::is_host) {
    return "player_0";
  } else {
    return Network::Client()->_local_player_id;
  }
}


inline void Campaign::common_start() {
  Global::ClearRegistry();
  Renderer::Get()->Init();


  Map::Manager()->Init();
  Settlement::Manager()->on_start();
}

inline void Campaign::start(str player_faction) {
  common_start();

  Player::System::create_players_for_sp(player_faction);
  Province::System::Init();
  Resource::System::init();
  Actor::System::Init();
  AI::System::Start();

  Commands::Manager()->init();
  Commands::Manager()
    ->queue.sink<Commands::Command>()
    .connect<&Campaign::EvaluteCommands>(this);
}

inline void Campaign::start_mp() {
  common_start();

  if (Network::is_host) {
    Player::System::HostStartMultiplayer();
  } else {
    Player::System::ClientStartMultiplayer();
  }

  Province::System::Init();
  Resource::System::init();
  Actor::System::Init();
  AI::System::Start();

  Commands::Manager()->init();
  Commands::Manager()
    ->queue.sink<Commands::Command>()
    .connect<&Campaign::EvaluteCommands>(this);
}

inline void Campaign::save(str file_name) {
  printf("Saving to output\n");


  // I think only needed for human readability??
  std::ofstream jfile("./saves/" + file_name + ".json");
  {
    cereal::JSONOutputArchive output{jfile};

    entt::snapshot{Global::world}
      .get<Player::Component>(output)
      .get<Faction::Component>(output)
      .get<Actor::Component>(output)
      .get<Animated::Component>(output)
      .get<Stockpile::Component>(output)
      .get<Tile::Component>(output)
      .get<Province::Component>(output)
      .get<Settlement::Component>(output)
      .get<AI::Component>(output);
  }
  jfile.close();

  std::ofstream file("./saves/" + file_name + ".dat", std::ios::binary);
  {
    cereal::BinaryOutputArchive output{file};

    entt::snapshot{Global::world}
      .get<Player::Component>(output)
      .get<Faction::Component>(output)
      .get<Actor::Component>(output)
      .get<Animated::Component>(output)
      .get<Stockpile::Component>(output)
      .get<Tile::Component>(output)
      .get<Province::Component>(output)
      .get<Settlement::Component>(output)
      .get<AI::Component>(output);
  }
  file.close();
}

inline void Campaign::load(cstr file_path) {
  common_start();

  printf("Loading from file: %s \n", file_path);

  std::ifstream file(file_path, std::ios::binary);
  {
    cereal::BinaryInputArchive input{file};

    entt::snapshot_loader{Global::world}
      .get<Player::Component>(input)
      .get<Faction::Component>(input)
      .get<Actor::Component>(input)
      .get<Animated::Component>(input)
      .get<Stockpile::Component>(input)
      .get<Tile::Component>(input)
      .get<Province::Component>(input)
      .get<Settlement::Component>(input)
      .get<AI::Component>(input);
    // Sight::Component

    // printf( "%u\n", (int) Global::world.size() );
  }
  file.close();

  // Actor::System::Init();
  // AI::Start();

  Global::world.view<Settlement::Component>().each(
    [](Settlement::Component &settlement) {
      settlement.texture =
        LoadTextureFromImage(Settlement::Manager()->building_map.at("roman_m1")
        );
    }
  );
  Global::world.view<Player::Component>().each(
    [](entt::entity et, Player::Component &player) {
      printf("Player %s\n", player.player_id.c_str());
      if (player.player_id == "player_0" && player.is_human) {
        printf("Assigning!!!!!!!!!!!!!!!!!!!!!!$$$$$$$$$$$$$$$$$$$$$$$\n");
        Global::world.emplace<Player::LocalTag>(et);
      }
    }
  );

  Commands::Manager()->init();
  Commands::Manager()
    ->queue.sink<Commands::Command>()
    .connect<&Campaign::EvaluteCommands>(this);
}


// Runs inside game loop
inline void Campaign::UpdateOnFrame(f32 &dt, f32 &lag, f32 &oncelag) {
  f32 cameraSpeed = 500.0f;

  bool show_content = false;
  Clay_String tab = CLAY_STRING("");


  auto action = UI::campaign_ui(GetLocalPlayerE());
  switch (action.type) {
    case UI::Action_Campaign_t::None:
      break;
    case UI::Action_Campaign_t::Command:
      if (action.cmd.type != Commands::Type::None) {
        PostCommand(action.cmd);
      }
      break;
    case UI::Action_Campaign_t::SelectBuilding:
      _building_to_build = action.building;
      break;
  }

  Vector2 click_pos =
    GetScreenToWorld2D(GetMousePosition(), Global::state.camera);

  auto player_e =
    Global::world.view<Player::Component, Player::LocalTag>().front();

  if (player_e == entt::null) {
    std::cout << "ERROR"
              << " no local player was found" << '\n';
    return;
  }


  bool hovered = false;
  const u32 items = 4;
  const Clay_String foos[items] = {
    CLAY_STRING("OverviewPanel::Tabs"),
    CLAY_STRING("OverviewPanel::Content"),
    CLAY_STRING("SettlementContext"),
    CLAY_STRING("ActorContext")
  };
  for (u32 i = 0; i < items; i++) {
    Clay_ElementId id = Clay_GetElementId(foos[i]);
    hovered = hovered || Clay_PointerOver(id);
  }

  if (IsKeyPressed(KEY_SPACE)) {
    PostCommand(Commands::Command::time_change(player_e, "Player request Pause")
    );
  }

  if (IsKeyPressed(KEY_MINUS)) {
    PostCommand(
      Commands::Command::time_change(player_e, "Player request Slower")
    );
  }

  if (IsKeyPressed(KEY_EQUAL)) {
    PostCommand(
      Commands::Command::time_change(player_e, "Player request Faster")
    );
  }

  if (IsKeyPressed(KEY_V)) {
    PostCommand(Commands::Command::spawn_colonist(player_e, click_pos));
  }

  if (IsKeyPressed(KEY_P)) {
    Map::Manager()->mode = Map::Mode::Political;
  }

  if (IsKeyPressed(KEY_T)) {
    Map::Manager()->mode = Map::Mode::Terrain;
  }

  if (IsKeyDown(KEY_D))
    Global::state.camera.target.x +=
      dt * cameraSpeed / Global::state.camera.zoom;
  if (IsKeyDown(KEY_A))
    Global::state.camera.target.x -=
      dt * cameraSpeed / Global::state.camera.zoom;
  if (IsKeyDown(KEY_W))
    Global::state.camera.target.y -=
      dt * cameraSpeed / Global::state.camera.zoom;
  if (IsKeyDown(KEY_S))
    Global::state.camera.target.y +=
      dt * cameraSpeed / Global::state.camera.zoom;

  if (hovered) {
    return;
  }

  if (IsMouseButtonPressed(0)) {
    if (Map::Manager()->mode == Map::Mode::BuildPreview) {
      // if we click on a valid province, post a build command

      entt::entity local_player = GetLocalPlayerE();

      auto sc =
        Selection::System::CheckClickOnSettlement(local_player, click_pos);

      if (sc != entt::null) {
        Province::Component prov_component =
          Global::world.get<Province::Component>(sc);
        Settlement::Component settlement_component =
          Global::world.get<Settlement::Component>(sc);

        if (_building_to_build.has_value() &&
            Settlement::System::can_build_immediately(
              prov_component, settlement_component, _building_to_build.value()
            )) {
          PostCommand(Commands::Command::construct_building(
            sc, _building_to_build.value()
          ));
          _building_to_build = std::nullopt;
          Map::Manager()->mode = Map::Mode::Default;
        }
      }
    } else {
      Selection::System::UpdateSelection(click_pos, GetLocalPlayerID());
    }
  }

  if (IsMouseButtonPressed(1)) {
    auto selected_e =
      Global::world
        .view<Actor::Component, Animated::Component, Selected::Component>()
        .front();

    if (selected_e != entt::null) {
      std::cout << "Moving entity: " << EntityIdToString(selected_e) << '\n';
      PostCommand(Commands::Command::move(player_e, click_pos, selected_e));
    }
  }


  // Vector2 screenCenter = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  // Vector2 target = GetScreenToWorld2D(screenCenter, camera);
  // PrintVec2(target);

  // camera.offset = target;


  if (IsKeyDown(KEY_Z))
    Global::state.camera.zoom -= 0.05f;
  if (IsKeyDown(KEY_X))
    Global::state.camera.zoom += 0.05f;

  f32 mouseWheelDelta = GetMouseWheelMove();

  Global::state.camera.zoom += (mouseWheelDelta * 0.2f);
  if (Global::state.camera.zoom > 8.0f)
    Global::state.camera.zoom = 8.0f;
  else if (Global::state.camera.zoom < 0.08f)
    Global::state.camera.zoom = 0.08f;

  Global::state.camera.offset = {
    (f32) GetScreenWidth() / 2, (f32) GetScreenHeight() / 2
  };
}

// TODO: look at all of these and see if any belong in UpdateOnFrame
inline void Campaign::Update60TPS() {
  auto animated_actors =
    Global::world.view<Actor::Component, Animated::Component>();
  auto players = Global::world.view<Player::Component>();

  Movement::System::Update(animated_actors, Global::state.timeScale);
  Animation::System::Update(animated_actors, Global::state.timeScale);
  Commands::Manager()->poll();
  Player::System::Update(players);
  //  Terrain::UpdateFOW(reg);
}

inline void Campaign::Update1TPS() {
  Settlement::System::update_1tps();
  Resource::System::update_1tps();

  Global::state.day++;

  if (Global::state.month < 12)
    Global::state.month++;
  else {
    Global::state.year++;
    Global::state.month = 1;
  }
}

inline void Campaign::Draw() {
  // Renderer::Get()->draw_map(Map::Manager()->mode);

  auto rndr = Renderer::Get();
  rndr->draw_common();

  switch (Map::Manager()->mode) {
    case Map::Mode::Default:
      rndr->draw_default();
      break;
    case Map::Mode::BuildPreview://@todo
      rndr->draw_build_preview(GetLocalPlayerE(), _building_to_build.value());
      break;
    case Map::Mode::Terrain:
      break;
    case Map::Mode::Political:
      rndr->draw_political();
      break;
    case Map::Mode::Resources:
      rndr->draw_resources();
      break;
  }

  rndr->draw_after(Map::Manager()->mode);
}

// @todo retire this, I think?
inline void Campaign::ConvertCommandRequest(str cmd) {
  nlohmann::json body = nlohmann::json::parse(cmd);

  std::cout << "BODY" << body << '\n';
  // std::cout << "body " << body.dump() << '\n';

  str cmd_player_id = body["cmd_player"];
  entt::entity cmd_player_e = body["cmd_player_e"];
  Commands::Type cmd_type = body["cmd_type"];
  str cmd_msg = body["cmd_msg"];
  // @todo the coords being a float is gonna cause issues
  // since this is just movement to a tile, it can probably be ints,
  // or even just the tile coord or something
  f32 cmd_click_pos_x = body["cmd_pos.x"];
  f32 cmd_click_pos_y = body["cmd_pos.y"];
  vec2f click_pos = vec2f{cmd_click_pos_x, cmd_click_pos_y};
  entt::entity entity = body["entity"];


  // this switch is only necessary so that we dont incorrectly access
  // missing keys from the json for commands that dont need certain values
  // certainly protobufs or something would be better than json!
  switch (cmd_type) {
    case Commands::Type::Move: {
      Commands::Manager()->enqueue(
        Commands::Command::move(cmd_player_e, click_pos, entity)
      );
    } break;
    case Commands::Type::TimeChange: {
      Commands::Manager()->enqueue(
        Commands::Command::time_change(cmd_player_e, cmd_msg)
      );
    } break;
    case Commands::Type::ClaimProvince: {
      Commands::Manager()->enqueue(Commands::Command::claim_province(entity));
    } break;
    case Commands::Type::BuildSettlement: {
      Commands::Manager()->enqueue(Commands::Command::build_settlement(entity));
    } break;
    case Commands::Type::SpawnArmy: {
      Commands::Manager()->enqueue(
        Commands::Command::spawn_army(cmd_player_e, click_pos)
      );
    } break;
    case Commands::Type::SpawnColonist: {
      Commands::Manager()->enqueue(
        Commands::Command::spawn_colonist(cmd_player_e, click_pos)
      );
    } break;
  }

  // auto players = Global::world.view<Player::Component>();

  // for ( entt::entity player: players ) {
  //   Player::Component pc = Global::world.get<Player::Component>( player );

  //   std::cout << "pc.player_id: " << pc.player_id << '\n';
  //   std::cout << "cmd_player_id: " << cmd_player_id << '\n';

  //   if ( pc.player_id == cmd_player_id ) {
  //     // @todo Gonna need to redo converting net command requests to local again now that
  //     // I changed how commands work
  //     // auto cmd = Commands::Command{
  //     //   .type = cmd_type,
  //     //   .player_e = player,
  //     //   .msg = cmd_msg,
  //     //   .click_pos = { cmd_click_pos_x, cmd_click_pos_y },
  //     //   .entity = entity,
  //     // };

  //     // Commands::Manager()->enqueue( cmd );
  //   }
  // }
}


inline void Campaign::PostCommand(Commands::Command cmd) {
  if (_is_singleplayer) {
    Commands::Manager()->enqueue(cmd);
  } else {
    auto body = nlohmann::json{
      {"cmd_player", GetLocalPlayerID()},
      {"cmd_player_e", cmd.player_e},
      {"cmd_type", cmd.type},
      {"cmd_msg", cmd.msg},
      {"cmd_pos.x", cmd.click_pos.x},
      {"cmd_pos.y", cmd.click_pos.y},
      {"entity", cmd.entity},
    };

    if (Network::is_host) {
      Network::Host()->SendMessageToAllActiveClients(Network::Message{
        Network::MessageID::Command,
        body,
      });
      Commands::Manager()->enqueue(cmd);
    } else {
      Network::Client()->SendMessageToHost(Network::Message{
        Network::MessageID::Command,
        body,
      });
    }
  }
}


// @todo This should be split to explicit Evaluate and Execute steps.
inline void Campaign::EvaluteCommands(const Commands::Command &cmd) {
  switch (cmd.type) {
    case Commands::Type::BuildSettlement: {
      Settlement::System::spawn_settlement(cmd.entity);
      return;
    }
    case Commands::Type::ClaimProvince: {
      Province::System::colonist_claim_province(cmd.entity);
      return;
    }
    case Commands::Type::TimeChange: {
      HandleTimeChangeRequest(cmd);
      return;
    }
    case Commands::Type::SpawnColonist: {
      Actor::System::spawn_colonist(cmd.player_e, cmd.click_pos);
      return;
    }
    case Commands::Type::SpawnArmy: {
      Actor::System::spawn_army(cmd.player_e, cmd.click_pos);
      return;
    }
    case Commands::Type::Move: {
      Movement::System::SetDestinations(cmd.entity, cmd.click_pos);
      return;
    }
    case Commands::Type::ConstructBuilding: {
      Settlement::System::construct_building(cmd.entity, cmd.building);
      return;
    };
  }
}


inline void Campaign::HandleTimeChangeRequest(const Commands::Command &cmd) {
  if (cmd.msg == "Player request Pause") {
    std::cout << cmd.msg << std::endl;

    if (Global::state.timeScale > 0.0f) {
      Global::state.prevTimeScale = Global::state.timeScale;
      Global::state.timeScale = 0.0f;
    } else if (Global::state.timeScale == 0.0f) {
      Global::state.timeScale = Global::state.prevTimeScale;
    }

    return;
  }

  if (cmd.msg == "Player request Slower") {
    Global::state.timeScale -= 0.5f;
    if (Global::state.timeScale < 0.0f)
      Global::state.timeScale = 0.0f;

    if (Global::state.timeScale == 0.0f && Global::state.prevTimeScale > 0.5f) {
      Global::state.prevTimeScale -= 0.5f;
      Global::state.timeScale = Global::state.prevTimeScale;
    }

    return;
  }

  if (cmd.msg == "Player request Faster") {
    Global::state.timeScale += 0.5f;
    if (Global::state.timeScale > 1.5f)
      Global::state.timeScale = 1.5f;

    return;
  }
}
