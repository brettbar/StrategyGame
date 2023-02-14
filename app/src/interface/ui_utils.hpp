#pragma once

#include "../shared/global.hpp"

#include "components/panel.hpp"
#include "components/text_elements.hpp"
#include "components/texture_elements.hpp"
#include <iostream>


// TODO(rf) many of these should be replaced with template !!!!!!!!!!

namespace UI {

  template<typename T>
  inline bool Has( entt::entity entity ) {
    return Manager()->registry.all_of<T>( entity );
  }

  // TODO Check assert
  template<typename T>
  inline T &Get( entt::entity entity ) {
    assert( Has<T>( entity ) );
    auto &got = Manager()->registry.get<T>( entity );
    return got;
  }

  inline std::string GetId( entt::entity entity ) {
    if ( Has<Panel>( entity ) ) {
      return Get<Panel>( entity ).id;
    }

    if ( Has<StackPanel>( entity ) ) {
      return Get<StackPanel>( entity ).id;
    }

    if ( Has<TextButton>( entity ) ) {
      return Get<TextButton>( entity ).id;
    }

    if ( Has<TextLabel>( entity ) ) {
      return Get<TextLabel>( entity ).id;
    }

    if ( Has<TextureLabel>( entity ) ) {
      return Get<TextureLabel>( entity ).id;
    }

    if ( Has<TextureButton>( entity ) ) {
      return Get<TextureButton>( entity ).id;
    }

    return "INVALID_ID";
  }

  inline rect &GetTransform( entt::entity entity ) {
    if ( Has<Panel>( entity ) ) {
      return Get<Panel>( entity ).transform;
    }
    if ( Has<StackPanel>( entity ) ) {
      return Get<StackPanel>( entity ).transform;
    }
    if ( Has<TextButton>( entity ) ) {
      return Get<TextButton>( entity ).transform;
    }
    if ( Has<TextLabel>( entity ) ) {
      return Get<TextLabel>( entity ).transform;
    }
    if ( Has<TextureLabel>( entity ) ) {
      return Get<TextureLabel>( entity ).transform;
    }
    if ( Has<TextureButton>( entity ) ) {
      return Get<TextureButton>( entity ).transform;
    }

    std::cout << "GetTransform() Invalid Transform!!" << std::endl;
    assert( false );
  }

  inline Margins &GetMargins( entt::entity entity ) {
    if ( Has<Panel>( entity ) ) {
      return Get<Panel>( entity ).margins;
    }
    if ( Has<StackPanel>( entity ) ) {
      return Get<StackPanel>( entity ).margins;
    }
    if ( Has<TextButton>( entity ) ) {
      return Get<TextButton>( entity ).margins;
    }
    if ( Has<TextLabel>( entity ) ) {
      return Get<TextLabel>( entity ).margins;
    }
    if ( Has<TextureLabel>( entity ) ) {
      return Get<TextureLabel>( entity ).margins;
    }
    if ( Has<TextureButton>( entity ) ) {
      return Get<TextureButton>( entity ).margins;
    }

    std::cout << "GetTransform() Invalid Transform!!" << std::endl;
    assert( false );
  }

  inline void UpdateElem( entt::entity entity ) {
    if ( Has<TextButton>( entity ) ) {
      // Get<TextButton>( entity ).Update();
      return;
    }
    if ( Has<TextLabel>( entity ) ) {
      // Get<TextLabel>( entity ).Update();
      return;
    }
  }

  inline void ToggleElem( entt::entity entity, bool on ) {
    if ( Has<Panel>( entity ) ) {
      Get<Panel>( entity ).enabled = on;
      return;
    }

    if ( Has<StackPanel>( entity ) ) {
      Get<StackPanel>( entity ).enabled = on;
      return;
    }

    if ( Has<TextButton>( entity ) ) {
      Get<TextButton>( entity ).enabled = on;
      return;
    }

    if ( Has<TextLabel>( entity ) ) {
      Get<TextLabel>( entity ).enabled = on;
      return;
    }

    if ( Has<TextureLabel>( entity ) ) {
      Get<TextureLabel>( entity ).enabled = on;
      return;
    }

    if ( Has<TextureButton>( entity ) ) {
      Get<TextureButton>( entity ).enabled = on;
      return;
    }
  }


  inline void RecursiveToggle( entt::entity entity, bool on ) {
    ToggleElem( entity, on );

    if ( Has<TextLabel>( entity ) ) {
      if ( on ) {
        Get<TextLabel>( entity ).SubscribeToMessages();
      }
      else {
        Get<TextLabel>( entity ).UnsubscribeFromMessages();
      }
    }

    if ( !Has<Panel>( entity ) && !Has<StackPanel>( entity ) )
      return;

    if ( Has<StackPanel>( entity ) ) {
      StackPanel &stack_panel = Get<StackPanel>( entity );
      RecursiveToggle( stack_panel.children[stack_panel.curr_index], on );
    }
    else if ( Has<Panel>( entity ) ) {
      for ( entt::entity child:
            Manager()->registry.get<Panel>( entity ).children ) {
        RecursiveToggle( child, on );
      }
    }
  }

  inline void RecursiveDelete( entt::entity entity ) {
    if ( Has<StackPanel>( entity ) ) {
      StackPanel &stack_panel = Get<StackPanel>( entity );
      RecursiveDelete( stack_panel.children[stack_panel.curr_index] );
    }
    else if ( Has<Panel>( entity ) ) {
      for ( entt::entity child:
            Manager()->registry.get<Panel>( entity ).children ) {
        RecursiveDelete( child );
      }
    }
    else {
      Manager()->lookup.erase( GetId( entity ) );
      Manager()->registry.destroy( entity );
    }
  }

  // TODO maybe should be put on StackPanel struct as method?
  inline void SwitchChild( StackPanel &sp, u32 index ) {
    RecursiveToggle( sp.children[sp.curr_index], false );
    sp.curr_index = index;
    RecursiveToggle( sp.children[sp.curr_index], true );
  }

  inline bool IsEnabled( entt::entity entity ) {
    if ( Has<Panel>( entity ) ) {
      return Get<Panel>( entity ).enabled;
    }

    if ( Has<StackPanel>( entity ) ) {
      return Get<StackPanel>( entity ).enabled;
    }

    if ( Has<TextButton>( entity ) ) {
      return Get<TextButton>( entity ).enabled;
    }

    if ( Has<TextLabel>( entity ) ) {
      return Get<TextLabel>( entity ).enabled;
    }

    if ( Has<TextureLabel>( entity ) ) {
      return Get<TextureLabel>( entity ).enabled;
    }

    if ( Has<TextureButton>( entity ) ) {
      return Get<TextureButton>( entity ).enabled;
    }

    return false;
  }


  inline bool IsInteractive( entt::entity entity ) {
    return Has<TextureButton>( entity ) || Has<TextButton>( entity );
  }

  inline bool IsClickable( entt::entity entity ) {
    if ( Has<TextureButton>( entity ) ) {
      return Get<TextureButton>( entity ).clickable;
    }

    if ( Has<TextButton>( entity ) ) {
      return Get<TextButton>( entity ).clickable;
    }

    return false;
  }

  inline void DoAction( entt::entity entity ) {
    if ( Has<TextButton>( entity ) ) {

      auto &button = Get<TextButton>( entity );
      button.FireEvent();
    }


    if ( Has<TextureButton>( entity ) ) {
      auto &button = Get<TextureButton>( entity );
      button.Action();
    }
  }

};// namespace UI
