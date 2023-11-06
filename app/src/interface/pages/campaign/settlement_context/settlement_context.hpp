#pragma once

#include "../../../irongui/forge.hpp"
#include "construction_tab.hpp"
#include "military_tab.hpp"
#include "population_tab.hpp"


namespace UI {

  inline void SettlementContext( Settlement::Component *settlement ) {
    auto f = Iron::Forge();


    rect root_r = rect{ 0, 0, (f32) GetScreenWidth(), (f32) GetScreenHeight() };
    auto root_g = f.Grid( root_r, 4, 3 );

    auto context_g =
      f.Grid( root_g->Slots( 9, 10 ), 4, 4, Fade( BLACK, 0.5f ) );

    auto tabs_g = f.Grid( context_g->Col( 0 ), 1, 5 );

    bool pop_clicked = f.TextButton( tabs_g->Slot( 0 ), "Population", BLUE );
    bool constr_clicked =
      f.TextButton( tabs_g->Slot( 1 ), "Construction", YELLOW );
    bool resources_clicked =
      f.TextButton( tabs_g->Slot( 2 ), "Resources", GREEN );
    bool mil_clicked = f.TextButton( tabs_g->Slot( 3 ), "Military", RED );
    bool agents_clicked = f.TextButton( tabs_g->Slot( 4 ), "Agents", PURPLE );

    auto tabs = f.TabsVertical( 5 );

    switch ( tabs.tab ) {
      case 0:
        f.Grid( context_g->Cols( 1, 4 ), 4, 4, BLUE );
        break;
      case 1:
        f.Grid( context_g->Cols( 1, 4 ), 4, 4, YELLOW );
        break;
      case 2:
        f.Grid( context_g->Cols( 1, 4 ), 4, 4, GREEN );
        break;
      case 3:
        f.Grid( context_g->Cols( 1, 4 ), 4, 4, RED );
        break;
      case 4:
        f.Grid( context_g->Cols( 1, 4 ), 4, 4, PURPLE );
        break;
      default:
        f.Grid( context_g->Cols( 1, 4 ), 4, 4, BLUE );
        break;
    }


    // if ( pop_clicked )
    //   f.Grid( context_g->Cols( 1, 4 ), 4, 4, BLUE );
    // else if ( constr_clicked )
    //   f.Grid( context_g->Cols( 1, 4 ), 4, 4, YELLOW );
    // else if ( resources_clicked )
    //   f.Grid( context_g->Cols( 1, 4 ), 4, 4, GREEN );
    // else if ( mil_clicked )
    //   f.Grid( context_g->Cols( 1, 4 ), 4, 4, RED );
    // else if ( agents_clicked )
    //   f.Grid( context_g->Cols( 1, 4 ), 4, 4, PURPLE );
    // else
    //   f.Grid( context_g->Cols( 1, 4 ), 4, 4, BLUE );

    // content
    // f.TextLabel( context_g->Slot( 0 ), settlement->name, GREEN );
    // f.TextLabel(
    //   context_g->Slot( 1 ),
    //   Settlement::dev_names[settlement->development],
    //   GREEN
    // );
    // f.TextLabel(
    //   context_g->Slot( 2 ),
    //   std::to_string( settlement->population.current ).c_str(),
    //   GREEN
    // );


    f.Draw();
  }

  // inline sptr<Element> CreateSettlementContextPanel() {
  //   return GridPanel( "settlement_context", 4, 3 )
  //     .StartsDisabled()
  //     .Background( Fade( BLACK, 0.5 ) )
  //     .On(
  //       InterfaceUpdate::ID::SettlementContext,
  //       []( Element &self, InterfaceUpdate::Update update ) {
  //         printf(
  //           "InterfaceUpdate::ID::SettlementContext %d\n", update.condition
  //         );

  //         if ( update.condition )
  //           self.Enable();
  //         else
  //           self.Disable();
  //       }
  //     )
  //     .Children( {
  //       GridSlot(
  //         { 0, 0, 0, 2 },
  //         GridPanel( "settlement_context_tab_group", 1, 5 )
  //           .Background( BLUE )
  //           .Children( {
  //             GridSlot(
  //               { 0, 0, 0, 0 },
  //               TextureButton(
  //                 "settlement_context_tab_population",
  //                 GetTexture( "settlement_context_tab_population" ),
  //                 InterfaceEvent::ID::SettlementContextPopulationTab
  //               )
  //             ),
  //             GridSlot(
  //               { 0, 0, 1, 1 },
  //               TextureButton(
  //                 "settlement_context_tab_construction",
  //                 GetTexture( "settlement_context_tab_construction" ),
  //                 InterfaceEvent::ID::SettlementContextConstructionTab
  //               )
  //             ),
  //             GridSlot(
  //               { 0, 0, 2, 2 },
  //               TextureButton(
  //                 "settlement_context_tab_resources",
  //                 GetTexture( "settlement_context_tab_resources" ),
  //                 InterfaceEvent::ID::SettlementContextResourcesTab
  //               )
  //             ),
  //             GridSlot(
  //               { 0, 0, 3, 3 },
  //               TextureButton(
  //                 "settlement_context_tab_garrison",
  //                 GetTexture( "settlement_context_tab_garrison" ),
  //                 InterfaceEvent::ID::SettlementContextMilitaryTab
  //               )
  //             ),

  //             GridSlot(
  //               { 0, 0, 4, 4 },
  //               TextureButton(
  //                 "settlement_context_tab_culture",
  //                 GetTexture( "settlement_context_tab_culture" ),
  //                 InterfaceEvent::ID::SettlementContextMilitaryTab
  //               )
  //             ),
  //           } )
  //       ),

  //       GridSlot(
  //         { 1, 3, 0, 2 },
  //         StackPanel(
  //           "settlement_context_content",
  //           {
  //             CreateSettlementContextTabPopulation(),

  //             GridPanel( "resources_tab", 3, 3 )
  //               .Background( PURPLE )
  //               .Children( {
  //                 GridSlot(
  //                   { 1, 1, 1, 1 },
  //                   TextLabel( "resources_tab_content", "Resources Tab", 32 )
  //                 ),
  //               } ),

  //             CreateSettlementContextTabConstruction(),

  //             CreateSettlementContextTabMilitary(),
  //           },
  //           {
  //             InterfaceUpdate::ID::SettlementContextPopulationTab,
  //             InterfaceUpdate::ID::SettlementContextResourcesTab,
  //             InterfaceUpdate::ID::SettlementContextConstructionTab,
  //             InterfaceUpdate::ID::SettlementContextMilitaryTab,
  //           }
  //         )
  //       ),
  //     } );
  // }
}// namespace UI
