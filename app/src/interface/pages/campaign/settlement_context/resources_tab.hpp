#include "../../../builders.hpp"
#include "../../../element.hpp"


namespace UI
{

  inline sptr<Element> CreateSettlementContextTabResources()
  {
    return GridPanel( "settlement_context_resources", 1, 2 )
      .Children( {
        Slot(
          { 0, 0, 0, 0 },
          TextLabel( "settlement_resource_list_label", "Resource List", 26 )
        ),
        // DataPanel( "settlement_resource_list" )
        //   .Axis( Axis::Column )
        //   .Update( []( Element &self ) {
        //     Settlement::Component selected_settlement =
        //       SettlementSystem::ReadSelectedComponent();


        //     for ( auto [resource, count]: selected_settlement.raw_materials )
        //     {
        //       auto id =
        //         Resources::GetRawMaterialName( resource ) + "_data_point";

        //       self.CreateElementForDatapoints(

        //         Panel(
        //           Resources::GetRawMaterialName( resource ) + "_data_point"
        //         )
        //           .Children( {
        //             TextureLabel(
        //               Resources::GetRawMaterialName( resource ) +
        //               "_data_point_label"
        //             )
        //               .Texture( GetRawMaterialName( resource ) + ".png" ),
        //             TextLabel(
        //               Resources::GetRawMaterialName( resource ) +
        //               "_data_point_value"
        //             )
        //               .Text( std::to_string( count ), 24 )
        //               .Background( BLACK ),
        //           } )
        //       );
        //     }
        //   } ),
      } );
  }
};// namespace UI