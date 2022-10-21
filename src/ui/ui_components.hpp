#pragma once


#include "../common.hpp"

namespace UI {

inline entt::registry ui_reg;
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

struct Margins {
  u32 left;
  u32 right;
  u32 top;
  u32 bottom;
};


struct Element {
  std::string id;
  Type type;
  bool enabled;
  rect transform;
  Margins margins;
};

struct Panel {
  Color background;
  Axis children_axis;
  Align children_horiz_align;
  Align children_vert_align;
  std::vector<entt::entity> children;
};

struct BasePanel : Panel {
  vec2 original_size;
  std::function<void( Element &, BasePanel & )> update;
};

struct TextLabel {
  std::string text;
  i32 font_size;
  Color text_color;
  Color background;
};

struct TextButton : TextLabel {
  bool clickable;
  std::function<void()> action;
  std::function<bool()> check_clickable;

  void Action() {
    if ( clickable )
      action();
  }

  void Update() { clickable = check_clickable(); }
};

struct TextureLabel {
  Texture2D texture;
};

struct TextureButton : TextureLabel {
  bool clickable;
  std::function<void()> action;
  std::function<bool()> check_clickable;

  void Action() {
    if ( clickable )
      action();
  }

  void Update() { clickable = check_clickable(); }
};

template<typename T>
inline entt::entity CreateElement( T component, Element elem ) {
  entt::entity entity = ui_reg.create();

  ui_reg.emplace<Element>( entity, elem );
  ui_reg.emplace<T>( entity, component );

  ui_lookup.insert_or_assign( elem.id, entity );

  return entity;
}

};// namespace UI
