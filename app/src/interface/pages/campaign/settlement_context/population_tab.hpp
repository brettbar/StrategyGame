#include "../../../builders.hpp"
#include "../../../element.hpp"


namespace UI
{
  inline sptr<Element> CreateSettlementContextTabPopulation()
  {
    return GridPanel( "population_tab", 3, 3 )
      .Background( BLUE )
      .Children( {
        Slot(
          { 0, 0, 0, 0 },
          TextLabel( "settlement_name", "Uninhabited", 26 )
            .On(
              InterfaceUpdate::ID::SettlementContext,
              []( Element &self, InterfaceUpdate::Update update ) {
                if ( update.condition )
                {
                  self.UpdateText( update.json["name"] );
                }
              }
            )
        ),
        Slot(
          { 1, 1, 0, 0 },
          TextLabel( "settlement_population", "Uninhabited", 26 )
            .On(
              InterfaceUpdate::ID::SettlementContext,
              []( Element &self, InterfaceUpdate::Update update ) {
                if ( update.condition )
                {
                  self.UpdateText( update.json["population"] );
                }
              }
            )
        ),
        Slot(
          { 2, 2, 0, 0 },
          TextLabel( "settlement_development", "Uninhabited", 26 )
            .On(
              InterfaceUpdate::ID::SettlementContext,
              []( Element &self, InterfaceUpdate::Update update ) {
                if ( update.condition )
                {
                  self.UpdateText( update.json["development"] );
                }
              }
            )
        ),
      } );
  }


};// namespace UI