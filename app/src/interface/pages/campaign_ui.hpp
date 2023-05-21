#pragma once

#include "../../shared/common.hpp"

#include "../element.hpp"
#include "../ui_utils.hpp"


namespace UI
{

  inline std::vector<UI::Element> CreateCampaignUI()
  {
    return {
      Panel( "settlement_context_panel" )
        .SetAnchor( Anchor::BottomMid )
        .FixedSize( 800, 350 )
        .Background( Fade( BLACK, 0.5 ) )
        .On(
          InterfaceUpdate::ID::SettlementContext,
          []( Element &self, InterfaceUpdate::Update update ) {
            if ( update.condition )
              self.Enable();
            else
              self.Disable();
          }
        )
        .Children( {
          Panel( "settlement_context_tab_group" )
            .SetAxis( Axis::Column )
            .Background( BLUE )
            .Children( {
              TextureButton( "settlement_context_tab_population" )
                .SetEvent( InterfaceEvent::ID::SettlementContextPopulationTab ),
              TextureButton( "settlement_context_tab_resources" )
                .SetEvent( InterfaceEvent::ID::SettlementContextResourcesTab ),
              TextureButton( "settlement_context_tab_construction" )
                .SetEvent( InterfaceEvent::ID::SettlementContextConstructionTab
                ),
              TextureButton( "settlement_context_tab_culture" ),
              TextureButton( "settlement_context_tab_religion" ),
              TextureButton( "settlement_context_tab_garrison" ),
            } ),
          StackPanel( "settlement_context_content" )
            .Background( RED )
            .On(
              InterfaceUpdate::ID::SettlementContextPopulationTab,
              []( Element &self, InterfaceUpdate::Update update ) {
                self.SwitchChild( 0 );
              }
            )
            .On(
              InterfaceUpdate::ID::SettlementContextResourcesTab,
              []( Element &self, InterfaceUpdate::Update update ) {
                self.SwitchChild( 1 );
              }
            )
            .On(
              InterfaceUpdate::ID::SettlementContextConstructionTab,
              []( Element &self, InterfaceUpdate::Update update ) {
                self.SwitchChild( 2 );
              }
            )
            .Children( {
              Panel( "settlement_context_overview" )
                .Children( {
                  TextLabel( "settlement_name" )
                    .SetText( "Uninhabited", 26 )
                    .On(
                      InterfaceUpdate::ID::SettlementContext,
                      []( Element &self, InterfaceUpdate::Update update ) {
                        if ( update.condition )
                        {
                          self.UpdateText( update.json["name"] );
                        }
                      }
                    ),
                  TextLabel( "settlement_population" )
                    .SetText( "Uninhabited", 26 )
                    .On(
                      InterfaceUpdate::ID::SettlementContext,
                      []( Element &self, InterfaceUpdate::Update update ) {
                        if ( update.condition )
                        {
                          self.UpdateText( update.json["population"] );
                        }
                      }
                    ),
                  TextLabel( "settlement_development" )
                    .SetText( "Uninhabited", 26 )
                    .On(
                      InterfaceUpdate::ID::SettlementContext,
                      []( Element &self, InterfaceUpdate::Update update ) {
                        if ( update.condition )
                        {
                          self.UpdateText( update.json["development"] );
                        }
                      }
                    ),
                } ),
              Panel( "settlement_context_resources" )
                .Children( {
                  TextLabel( "settlement_resource_list" )
                    .SetText( "Resource List", 26 ),
                } ),
              Panel( "settlement_context_construction" )
                .SetAxis( Axis::Column )
                .Children( {
                  TextLabel( "buildings_label" ).SetText( "Buildings", 26 ),
                  TextButton( "build_farm" )
                    .SetText( "Build Farm", 26 )
                    .SetEvent( InterfaceEvent::Data{
                      InterfaceEvent::ID::SettlementContextConstructBuilding,
                      "build_farm",
                    } )
                    .Background( BLUE ),
                  Panel( "settlement_context_building_list" )
                    .SetAxis( Axis::Column )
                    .On(
                      InterfaceUpdate::ID::SettlementContextConstructBuilding,
                      []( Element &self, InterfaceUpdate::Update update ) {
                        self.children.push_back(
                          TextLabel( "farm" )
                            .SetText( update.update_txt, 24 )
                            .Background( GREEN )
                        );

                        self.children[self.children.size() - 1].Enable();
                      }
                    ),
                } ),
            } ),
        } ),
      Panel( "actor_context_panel" )
        .SetAnchor( Anchor::BottomMid )
        .Background( Fade( BLACK, 0.5 ) )
        .FixedSize( 800, 350 )
        .On(
          InterfaceUpdate::ID::ActorContext,
          []( Element &self, InterfaceUpdate::Update update ) {
            if ( update.condition )
              self.Enable();
            else
              self.Disable();
          }
        )
        .Children( {
          Panel( "actor_actions_panel" )
            .Children( {
              TextButton( "actor_spawn_settlement_button" )
                .Clickable( false )
                .Background( GREEN )
                .SetText( "Spawn?", 26 )
                .SetEvent( InterfaceEvent::Data(
                  InterfaceEvent::ID::ActorSpawnSettlment
                ) )
                .On(
                  InterfaceUpdate::ID::ActorCanSpawnSettlement,
                  []( Element &self, InterfaceUpdate::Update update ) {
                    self.UpdateClickable( update.condition );
                    if ( update.condition )
                      self.UpdateBackground( GREEN );
                    else
                      self.UpdateBackground( RED );
                  }
                ),
            } ),
        } ),
    };
  }

};// namespace UI
