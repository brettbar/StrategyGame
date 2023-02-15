#pragma once

#include "../shared/global.hpp"

#include "components/panel.hpp"
#include "components/text_elements.hpp"
#include "components/texture_elements.hpp"
#include <iostream>


namespace UI {

  template<typename T>
  inline bool Has( ptr<Element> entity ) {
    ptr<T> attempt = std::dynamic_pointer_cast<T>( entity );
    return attempt != nullptr;
  }

  template<typename T>
  inline std::shared_ptr<T> Get( ptr<Element> entity ) {
    std::shared_ptr<T> converted = std::dynamic_pointer_cast<T>( entity );
    assert( converted );
    return converted;
  }


  // inline bool IsInteractive( ptr<Element> entity ) {
  //   return Has<TextureButton>( entity ) || Has<TextButton>( entity );
  // }

  // inline bool IsClickable( ptr<Element> entity ) {
  //   if ( Has<TextureButton>( entity ) ) {
  //     return Get<TextureButton>( entity )->clickable;
  //   }

  //   if ( Has<TextButton>( entity ) ) {
  //     return Get<TextButton>( entity )->clickable;
  //   }

  //   return false;
  // }

  // inline void DoAction( ptr<Element> entity ) {
  //   if ( Has<TextButton>( entity ) ) {
  //     Get<TextButton>( entity )->FireEvent();
  //   }


  //   if ( Has<TextureButton>( entity ) ) {
  //     Get<TextureButton>( entity )->Action();
  //   }
  // }


  // inline void RecursiveToggle( ptr<Element> entity, bool on ) {
  //   if ( entity )
  //     entity->enabled = on;

  //   if ( Has<TextLabel>( entity ) ) {
  //     if ( on ) {
  //       Get<TextLabel>( entity )->SubscribeToMessages();
  //     }
  //     else {
  //       Get<TextLabel>( entity )->UnsubscribeFromMessages();
  //     }
  //   }

  //   if ( !Has<Panel>( entity ) && !Has<StackPanel>( entity ) )
  //     return;

  //   if ( Has<StackPanel>( entity ) ) {
  //     std::shared_ptr<StackPanel> stack_panel = Get<StackPanel>( entity );
  //     RecursiveToggle( stack_panel->children[stack_panel->curr_index], on );
  //   }
  //   else if ( Has<Panel>( entity ) ) {
  //     for ( ptr<Element> child: Get<Panel>( entity )->children ) {

  //       RecursiveToggle( child, on );
  //     }
  //   }
  // }

  inline void RecursiveDelete( ptr<Element> entity ) {
    if ( Has<StackPanel>( entity ) ) {
      std::shared_ptr<StackPanel> stack_panel = Get<StackPanel>( entity );
      RecursiveDelete( stack_panel->children[stack_panel->curr_index] );
    }
    else if ( Has<Panel>( entity ) ) {
      for ( ptr<Element> child: Get<Panel>( entity )->children ) {
        RecursiveDelete( child );
      }
    }
    else {
      Manager()->lookup.erase( entity->id );
      // Manager()->registry.destroy( entity );
      // TODO Missing delete?
    }
  }

  // TODO maybe should be put on StackPanel struct as method?
  inline void SwitchChild( StackPanel &sp, u32 index ) {
    // RecursiveToggle( sp.children[sp.curr_index], false );
    // sp.curr_index = index;
    // RecursiveToggle( sp.children[sp.curr_index], true );
  }


};// namespace UI
