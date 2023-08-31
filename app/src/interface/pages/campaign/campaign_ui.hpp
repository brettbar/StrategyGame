#pragma once

#include "../../../shared/common.hpp"

#include "../../element.hpp"
#include "../../ui_utils.hpp"

#include "actor_context.hpp"
#include "settlement_context/settlement_context.hpp"
#include <raylib.h>


namespace UI
{
  inline sptr<Element> CreateCampaignUI()
  {
    return {
      GridPanel( "campaign_root", 3, 3 )
        .FixedSize( GetScreenWidth(), GetScreenHeight() )
        .SetChildren( {
          // GridPanelElement::Slot{
          //   { 1, 1, 2, 2 },
          //   CreateSettlementContextPanel(),
          // },
          // CreateActorContextPanel(),
        } ),
    };
  }

};// namespace UI
