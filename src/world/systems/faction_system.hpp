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

        auto colors = element.value().at( "colors" );
        auto culture = element.value().at( "culture" );
        auto government = element.value().at( "government" );
        auto mobility = element.value().at( "mobility" );

        Faction::Component faction = {
          .id = element.key(),
          .colors =
            {
              .primary = colors.at( "primary" ),
              .secondary = colors.at( "secondary" ),
            },
          .culture =
            {
              .adjective = culture.at( "adjective" ),
              .denonym = culture.at( "denonym" ),
              .settlement_names = culture.at( "settlement_names" ),
            },
          .government =
            {
              .start_gov = Faction::Component::Government::gov_from_str(government.at( "start_gov" )),
              .empire_name = government.at( "empire_name" ),
              .empire_leader_title = government.at( "empire_leader_title" ),
              .republic_name = government.at( "republic_name" ),
              .republic_leader_title = government.at( "republic_leader_title" ),
              .federation_name = government.at( "federation_name" ),
              .federation_leader_title =
                government.at( "federation_leader_title" ),
            },
          .mobility =
            {
              .type = mobility.at( "type" ),
            }
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
