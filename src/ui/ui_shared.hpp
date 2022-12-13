#pragma once

#include "components/element.hpp"
#include "components/panel.hpp"
#include "components/stack_panel.hpp"
#include "components/text_elements.hpp"
#include "components/texture_elements.hpp"

namespace UI {
// TOOD see if we can replace all these with template functions
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
