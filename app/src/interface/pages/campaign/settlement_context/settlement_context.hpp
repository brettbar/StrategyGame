#pragma once

#include "construction_tab.hpp"
#include "military_tab.hpp"
#include "population_tab.hpp"


namespace UI {

  inline void DrawSettlementContext( Settlement::Component *settlement ) {

    auto f = Iron::Forge();

    rect r = rect{ 0, 0, 250, 250 };
    auto grid = f.IronGrid( r, 3, 3 );
    rect slot = grid->Slot( 4 );

    // bool okay_pressed = f.IronTextButton( slot, "Okay" );
    auto text_button = f.IronTextLabel( slot, "Okay" );

    // if ( okay_pressed ) {
    //   printf( "okay pressed\n" );
    // }


    {
      f.Draw();


      // if ( settlement ) {
      //   Iron::DrawTextButton( rectangle, settlement->name, BLUE );
      // } else {
      //   Iron::DrawTextButton( rectangle, "Uninhabited", BLUE );
      // }
    }
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
