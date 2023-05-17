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
              TextureButton( "settlement_context_tab_population" ),
              TextureButton( "settlement_context_tab_resources" ),
              TextureButton( "settlement_context_tab_culture" ),
              TextureButton( "settlement_context_tab_religion" ),
              TextureButton( "settlement_context_tab_construction" ),
              TextureButton( "settlement_context_tab_garrison" ),
            } ),
          StackPanel( "settlement_context_content" )
            .Background( RED )
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
            } ),
        } ),
      Panel( "actor_context_panel" )
        .SetAnchor( Anchor::BottomMid )
        .Background( Fade( BLACK, 0.5 ) )
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
