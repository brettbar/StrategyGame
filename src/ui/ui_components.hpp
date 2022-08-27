#pragma once


#include "../common.hpp"

namespace UI {

inline std::map<std::string, entt::entity> ui_lookup;

inline f32 SCALE = 2.0f;

struct Context {
  entt::entity hot;
  entt::entity active;
};

enum class Type {
  BASE_PANEL,
  PANEL,
  TEXT_LABEL,
  TEXTURE_LABEL,
  TEXT_BUTTON,
  TEXTURE_BUTTON,
};

enum class Axis {
  ROW,
  COLUMN,
};

enum class Align {
  START,
  CENTER,
  END,
  SPACE_OUT,
};


struct Element {
  Type type;
  std::string id;
  bool enabled;
};

struct Panel {
  Rectangle transform;
  Color background;
  Axis children_axis;
  Align children_horiz_align;
  Align children_vert_align;
  std::vector<entt::entity> children;
};


struct BasePanel : Panel {
  vec2 original_size;
  std::function<void( BasePanel & )> update;
};

struct TextLabel {
  Rectangle transform;
  std::string text;
  i32 font_size;
  Color text_color;
  Color background;
};

struct TextButton : TextLabel {
  std::function<void()> action;
};

struct TextureLabel {
  Rectangle transform;
  Texture2D texture;
};

struct TextureButton : TextureLabel {
  std::function<void()> action;
};

inline Rectangle &GetTransform( entt::registry &reg, entt::entity entity ) {
  Element &elem = reg.get<Element>( entity );

  switch ( elem.type ) {
    case Type::BASE_PANEL: {
      return reg.get<BasePanel>( entity ).transform;
    }
    case Type::PANEL: {
      return reg.get<Panel>( entity ).transform;
    }
    case Type::TEXTURE_BUTTON: {
      return reg.get<TextureButton>( entity ).transform;
    }

    case Type::TEXT_BUTTON: {
      return reg.get<TextButton>( entity ).transform;
    }

    default:
      // NOTE: This is only here to suppress a warning about returning nothing
      // from a non-void function. In theory this should never get here as long as
      // the Type enum class matches the struct definitions in this file.
      // If we ever got here it means we assigned a UI::Type that didnt have a corresponding
      // struct, big no-no.
      return reg.get<Panel>( entity ).transform;
  }
}

};// namespace UI
