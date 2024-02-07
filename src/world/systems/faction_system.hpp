#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"

#include "../components/faction.hpp"

namespace FactionSystem {
  inline std::map<std::string, Faction::Component> factions = {};

  inline std::map<std::string, Color> color_map = {
    { "red", RED },
    { "gold", GOLD },
    { "cyan", BLUE },
    { "green", GREEN },
    { "white", WHITE },
    { "purple", PURPLE },
    { "orange", ORANGE },
    { "pink", PINK },
    { "grey", GRAY },
  };

  inline void Init() {
    factions = {};

    std::filesystem::path cwd = std::filesystem::current_path();

    std::cout << "Current Working Directory: " << cwd.generic_string()
              << std::endl;

    std::ifstream f( "assets/data/factions.json" );
    {

      nlohmann::json js = nlohmann::json::parse( f );

      std::cout << js << std::endl;
      // std::cout << js["romans"] << std::endl;
      // std::cout << js["romans"]["adjectival"] << std::endl;
      for ( auto &element: js.items() ) {
        std::cout << element.key() << std::endl;
        std::cout << element.value() << std::endl;

        Faction::Component faction = {
          element.key(),
          element.value().at( "primary_color" ),
          element.value().at( "secondary_color" ),
          element.value().at( "adjectival" ),
          element.value().at( "denonym" ),
        };

        factions.emplace( element.key(), faction );
      }
    }
    f.close();

    // entt::entity ai_player = Global::world.create();
    // Global::world.emplace<Player::Component>(
    //   ai_player, ai_player, false, "celts"
    // );
    // Global::world.emplace<AI::Component>(
    //   ai_player, AI::Goal::DevelopSettlements
    // );
  }


  inline Texture2D villagar_texure_from_owner( entt::entity owner ) {
    std::string faction_id = Global::world.get<Faction::Component>( owner ).id;
    return Global::texture_cache[hstr{ ( faction_id + "_villager_texture" )
                                         .c_str() }]
      ->texture;
  }

  inline Texture2D hastati_texure_from_owner( entt::entity owner ) {
    std::string faction_id = Global::world.get<Faction::Component>( owner ).id;
    return Global::texture_cache[hstr{ ( faction_id + "_hastati_texture" )
                                         .c_str() }]
      ->texture;
  }
};// namespace FactionSystem
