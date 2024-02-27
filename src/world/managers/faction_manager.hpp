#pragma once

#include "../../shared/common.hpp"

#include "../components/faction.hpp"

namespace Faction {
  class Manager {
public:
    map<str, Faction::Component> factions = {};

    static Manager *Get() {
      static Manager instance;
      return &instance;
    }

private:
    Manager() {
      init();
    }
    ~Manager() {}
    Manager( Manager const & ) = delete;
    void operator=( const Manager & ) = delete;

    inline void init() {
      factions = {};

      std::filesystem::path cwd = std::filesystem::current_path();

      std::cout << "Current Working Directory: " << cwd.generic_string()
                << std::endl;

      std::ifstream f( "src/data/factions.json" );
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
                .start_gov =
                  Faction::gov_from_str( government.at( "start_gov" ) ),
                .empire_name = government.at( "empire_name" ),
                .empire_leader_title = government.at( "empire_leader_title" ),
                .republic_name = government.at( "republic_name" ),
                .republic_leader_title =
                  government.at( "republic_leader_title" ),
                .federation_name = government.at( "federation_name" ),
                .federation_leader_title =
                  government.at( "federation_leader_title" ),
              },
            .mobility = {
              .type = Faction::from_str( mobility.at( "type" ) ),
            } };

          factions.emplace( element.key(), faction );
        }
      }
      f.close();
    }
  };
};// namespace Faction