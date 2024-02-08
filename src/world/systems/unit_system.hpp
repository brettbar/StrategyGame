#pragma once

#include "../../shared/common.hpp"
#include "../components/unit.hpp"

namespace UnitSystem {

inline std::map<std::string, Unit::Component> units = {};

inline void Init() {
    // units = {};
    //
    // std::filesystem::path cwd = std::filesystem::current_path();
    //
    // std::cout << "Current Working Directory: " << cwd.generic_string()
    //           << std::endl;
    //
    // std::ifstream f( "assets/data/units.json" );
    // {
    //
    //   nlohmann::json js = nlohmann::json::parse( f );
    //
    //   std::cout << js << std::endl;
    //   // std::cout << js["romans"] << std::endl;
    //   // std::cout << js["romans"]["adjectival"] << std::endl;
    //   for ( auto &element: js.items() ) {
    //     std::cout << element.key() << std::endl;
    //     std::cout << element.value() << std::endl;
    //
    //     Unit::Component unit = {
    //       .id = element.value().at("id"),
    //       .faction = element.key(),
    //       .tier = element.value().at("tier"),
    //     };
    //
    //     units.emplace( element.key(), unit );
    //   }
    // }
    // f.close();
    //
}

};
