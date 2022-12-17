#pragma once

#include "../global.hpp"

#include "components/panel.hpp"
#include "components/text_elements.hpp"
#include "components/texture_elements.hpp"
#include <iostream>


namespace UI {

template<typename T>
inline bool Has( entt::entity entity ) {
  return Global::local.all_of<T>( entity );
}

template<typename T>
inline T &Get( entt::entity entity ) {
  assert( Has<T>( entity ) );
  auto &got = Global::local.get<T>( entity );
  return got;
}

inline Type GetType( entt::entity entity ) {
  if ( Has<Panel>( entity ) ) {
    return Get<Panel>( entity ).elem.type;
  }

  if ( Has<StackPanel>( entity ) ) {
    return Get<StackPanel>( entity ).elem.type;
  }

  if ( Has<TextLabel>( entity ) ) {
    return Get<TextLabel>( entity ).elem.type;
  }

  if ( Has<TextButton>( entity ) ) {
    return Get<TextButton>( entity ).label.elem.type;
  }

  if ( Has<TextureLabel>( entity ) ) {
    return Get<TextureLabel>( entity ).elem.type;
  }

  if ( Has<TextureButton>( entity ) ) {
    return Get<TextureButton>( entity ).label.elem.type;
  }

  return Type::INVALID_TYPE;
}

inline rect &GetTransform( entt::entity entity ) {
  if ( Has<Panel>( entity ) ) {
    return Get<Panel>( entity ).elem.transform;
  }
  if ( Has<StackPanel>( entity ) ) {
    return Get<StackPanel>( entity ).elem.transform;
  }
  if ( Has<TextLabel>( entity ) ) {
    return Get<TextLabel>( entity ).elem.transform;
  }
  if ( Has<TextButton>( entity ) ) {
    return Get<TextButton>( entity ).label.elem.transform;
  }
  if ( Has<TextureLabel>( entity ) ) {
    return Get<TextureLabel>( entity ).elem.transform;
  }
  if ( Has<TextureButton>( entity ) ) {
    return Get<TextureButton>( entity ).label.elem.transform;
  }

  std::cout << "GetTransform() Invalid Transform!!" << std::endl;
  assert( false );
}

inline void UpdateElem( entt::entity entity ) {
  if ( Has<TextLabel>( entity ) ) {
    Get<TextLabel>( entity ).Update();
    return;
  }
  if ( Has<TextButton>( entity ) ) {
    Get<TextButton>( entity ).Update();
    return;
  }
}

inline void ToggleElem( entt::entity entity, bool on ) {
  if ( Has<Panel>( entity ) ) {
    Get<Panel>( entity ).elem.enabled = on;
    return;
  }

  if ( Has<StackPanel>( entity ) ) {
    Get<StackPanel>( entity ).elem.enabled = on;
    return;
  }

  if ( Has<TextLabel>( entity ) ) {
    Get<TextLabel>( entity ).elem.enabled = on;
    return;
  }

  if ( Has<TextButton>( entity ) ) {
    Get<TextButton>( entity ).label.elem.enabled = on;
    return;
  }

  if ( Has<TextureLabel>( entity ) ) {
    Get<TextureLabel>( entity ).elem.enabled = on;
    return;
  }

  if ( Has<TextureButton>( entity ) ) {
    Get<TextureButton>( entity ).label.elem.enabled = on;
    return;
  }
}


inline void RecursiveToggle( entt::entity entity, bool on ) {
  ToggleElem( entity, on );

  if ( !Has<Panel>( entity ) && !Has<StackPanel>( entity ) )
    return;

  if ( Has<StackPanel>( entity ) ) {
    StackPanel &stack_panel = Get<StackPanel>( entity );
    RecursiveToggle( stack_panel.children[stack_panel.curr_index], on );
  }
  else if ( Has<Panel>( entity ) ) {
    for ( entt::entity child: Global::local.get<Panel>( entity ).children ) {
      RecursiveToggle( child, on );
    }
  }
}

inline void SwitchChild( StackPanel &sp, u32 index ) {
  RecursiveToggle( sp.children[sp.curr_index], false );
  sp.curr_index = index;
  RecursiveToggle( sp.children[sp.curr_index], true );
}

inline bool IsEnabled( entt::entity entity ) {
  if ( Has<Panel>( entity ) ) {
    return Get<Panel>( entity ).elem.enabled;
  }
  if ( Has<StackPanel>( entity ) ) {
    return Get<StackPanel>( entity ).elem.enabled;
  }

  if ( Has<TextLabel>( entity ) ) {
    return Get<TextLabel>( entity ).elem.enabled;
  }

  if ( Has<TextButton>( entity ) ) {
    return Get<TextButton>( entity ).label.elem.enabled;
  }

  if ( Has<TextureLabel>( entity ) ) {
    return Get<TextureLabel>( entity ).elem.enabled;
  }

  if ( Has<TextureButton>( entity ) ) {
    return Get<TextureButton>( entity ).label.elem.enabled;
  }

  return false;
}

inline std::string GetId( entt::entity entity ) {
  if ( Has<Panel>( entity ) ) {
    return Get<Panel>( entity ).elem.id;
  }

  if ( Has<StackPanel>( entity ) ) {
    return Get<StackPanel>( entity ).elem.id;
  }

  if ( Has<TextLabel>( entity ) ) {
    return Get<TextLabel>( entity ).elem.id;
  }

  if ( Has<TextButton>( entity ) ) {
    return Get<TextButton>( entity ).label.elem.id;
  }

  if ( Has<TextureLabel>( entity ) ) {
    return Get<TextureLabel>( entity ).elem.id;
  }

  if ( Has<TextureButton>( entity ) ) {
    return Get<TextureButton>( entity ).label.elem.id;
  }

  return "INVALID_ID";
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

};// namespace UI
