#pragma once


#include "../common.hpp"

namespace UI {

struct Context {
  entt::entity hot;
  entt::entity active;
};

enum class Type {
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
};

struct Panel {
  Rectangle transform;
  Color background;
  Axis children_axis;
  Align children_horiz_align;
  Align children_vert_align;
  std::vector<entt::entity> children;

  bool self_positioned = false;
  std::function<vec2()> update;
  inline void Update() {
    vec2 new_pos = this->update();
    this->transform.x = new_pos.x;
    this->transform.y = new_pos.y;
  }
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
  hstr texture_key;
};

struct TextureButton : TextureLabel {
  std::function<void()> action;
};

inline Rectangle &GetTransform( entt::registry &reg, entt::entity entity ) {
  Element &elem = reg.get<Element>( entity );

  switch ( elem.type ) {
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
