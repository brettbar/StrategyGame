#pragma once

#include "../../../../world/systems/settlement_system.hpp"
#include "../../../builders.hpp"
#include "../../../element.hpp"
#include "../../../ui_utils.hpp"


namespace UI
{
  inline Element RecruitmentWindow()
  {
    return Panel( "recruitment_window" )
      .Children( {
        TextButton( "train_hastati" )
          .Text( "Train", 24 )
          .Background( GREEN )
          .SetEvent( InterfaceEvent::ID::SettlementContextTrainHastati ),
      } );
  }

  inline Element TrainingWindow()
  {
    return Panel( "training_window" )
      .Children( {
        TextLabel( "training" ).Text( "Training", 24 ),
      } );
  }


  inline Element GarrisonWindow()
  {
    return DataPanel( "settlement_garrison" ).Update( []( Element &self ) {
      std::vector<Regiment> regiments =
        SettlementSystem::SelectedSettlementGarrisonList();

      for ( auto regiment: regiments )
      {
        std::string panel_id = "regiment_" + std::to_string( regiment.id );

        self.CreateElementForDatapoints(
          Panel( panel_id )
            .Axis( Axis::Column )
            .Children( {
              TextureLabel( panel_id + "_unit_type" )
                .Texture( "romans_hastati_texture_overview" ),
              TextLabel( panel_id + "_unit_count" )
                .Text( std::to_string( regiment.number ), 24 ),
            } )
        );
      }
    } );
  }


  inline Element MilitaryTab()
  {
    return Panel( "settlement_context_military" )
      .FixedSize( 1200, 24 )
      .AlignHoriz( Align::SpaceBetween )
      .Children( {
        RecruitmentWindow(),
        TrainingWindow(),
        GarrisonWindow(),
      } );
  }


}// namespace UI