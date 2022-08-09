#pragma once

#include "../../common.hpp"
#include "../../components/actor.hpp"
#include "../../components/event.hpp"
#include "../../components/province.hpp"
#include "../../components/selected.hpp"
#include "../../renderer/fonts.hpp"
#include "../event_system.hpp"

#include "irongui/irongui.hpp"
#include "game_ui.hpp"
#include "main_menu_ui.hpp"

namespace GAME_UI {

inline void SelectListener( entt::registry &, entt::entity );
inline void DeSelectListener();
inline void Init( entt::registry & );
inline void Update( entt::registry & );
// inline void HandleFloatingPanel( IRONGUI::Panel &, Vector2 );

inline void SelectListener( entt::registry &reg, entt::entity entity ) {
  if ( reg.all_of<Province::Component>( entity ) ) {

    auto stats = ui_lookup.at( "settlement_stats" );
    IRONGUI::Element &elem = IRONGUI::reg.get<IRONGUI::Element>( stats );

    elem.enabled = true;

  } else if ( reg.all_of<Actor::Component>( entity ) ) {
    auto actor = reg.get<Actor::Component>( entity );
    printf( "Actor: %s \n", actor.name );

    entt::entity context_label = ui_lookup.at( "context_label" );
    IRONGUI::reg.get<IRONGUI::TextLabel>( context_label ).text = actor.name;
  }
}

inline void DeSelectListener() {
  auto stats = ui_lookup.at( "settlement_stats" );
  IRONGUI::Element &elem = IRONGUI::reg.get<IRONGUI::Element>( stats );
  elem.enabled = false;
}

inline void Init( entt::registry &reg ) {
  reg.on_construct<Selected::Component>().connect<&SelectListener>();
  reg.on_destroy<Selected::Component>().connect<&DeSelectListener>();
}

inline void Update( entt::registry &reg ) {
  entt::entity stats = ui_lookup.at( "settlement_stats" );
  IRONGUI::Element &elem = IRONGUI::reg.get<IRONGUI::Element>( stats );

  if ( elem.enabled ) {
    auto prov_entity =
      reg.view<Selected::Component, Province::Component>().front();

    if ( prov_entity == entt::null )
      return;


    auto &province = reg.get<Province::Component>( prov_entity );
    // printf( "Settlement: %s \n", province.settlement->name );

    entt::entity context_label = ui_lookup.at( "context_label" );
    IRONGUI::reg.get<IRONGUI::TextLabel>( context_label ).text =
      province.settlement.name;

    entt::entity settlement_stats = ui_lookup.at( "settlement_stats" );
    // printf(
    //   "current population %d\n",
    //   province.settlement->population.current );

    std::string text = "";
    text +=
      "Pop: " + std::to_string( province.settlement.population.current ) + "\n";
    text +=
      "BR: " + std::to_string( province.settlement.population.birthRate ) +
      "\n";
    text +=
      "DR: " + std::to_string( province.settlement.population.deathRate ) +
      "\n";
    text +=
      "GR: " + std::to_string( province.settlement.population.growthRate ) +
      "\n";

    IRONGUI::reg.get<IRONGUI::TextLabel>( settlement_stats ).text = text;
  }
}


// inline void HandleFloatingPanel( IRONGUI::Panel &panel, Vector2 mousePos ) {
//   // printf( "DoFloatingPanel: %d\n", panel.index );
//   panel.pos.x = panel.oldOffset.x + mousePos.x;
//   panel.pos.y = panel.oldOffset.y + mousePos.y;

//   if ( ( panel.pos.x + panel.dmns.x ) > GetScreenWidth() ) {
//     panel.pos.x = GetScreenWidth() - panel.dmns.x;
//   } else if ( ( panel.pos.x ) < 0 ) {
//     panel.pos.x = 0;
//   }

//   if ( ( panel.pos.y + panel.dmns.y ) > GetScreenHeight() ) {
//     panel.pos.y = GetScreenHeight() - panel.dmns.y;
//   } else if ( ( panel.pos.y ) < 0 ) {
//     panel.pos.y = 0;
//   }
// }
//  for ( IRONGUI::Panel &panel: floating ) {
//    if ( !panel.floating )
//      return;
//
//    bool inside = CheckCollisionPointRec(
//      GetMousePosition(),
//      IRONGUI::GetAbsoluteRectangle( panel.pos, panel.dmns ) );
//
//    if ( !overAnyElem )
//      overAnyElem = inside;
//
//    if ( IRONGUI::DoFloatingPanel(
//           context,
//           panel,
//           inside,
//           mouseWentUp,
//           mouseWentDown,
//           mouseHeldDown ) ) {
//      HandleFloatingPanel( panel, mousePos );
//    } else {
//      panel.oldOffset = {
//        panel.pos.x - mousePos.x,
//        panel.pos.y - mousePos.y,
//      };
//    }
//  }
};// namespace GAME_UI

//    for ( auto it = items_view.rbegin(); it != items_view.rend(); ++it ) {
