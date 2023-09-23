#pragma once

#include "../../../shared/common.hpp"

#include "../../builders.hpp"
#include "../../ui_utils.hpp"

#include "actor_context.hpp"
#include "settlement_context/settlement_context.hpp"
#include <raylib.h>


namespace UI
{
  inline sptr<Element> CreateCampaignUI()
  {
    return {
      GridPanel( "campaign_root", 4, 3 )
        .FixedSize( GetScreenWidth(), GetScreenHeight() )
        .Children( {
          GridPanelElement::Slot{
            { 1, 2, 2, 2 },
            CreateSettlementContextPanel(),
          },
          GridPanelElement::Slot{
            { 1, 2, 2, 2 },
            CreateActorContextPanel(),
          },
        } ),
    };
  }

};// namespace UI
