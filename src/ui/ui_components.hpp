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
  vec2 pos;
  f32 width;
  f32 height;
  Color background;
};

struct Panel {
  Axis children_axis;
  Align children_horiz_align;
  Align children_vert_align;
  std::vector<entt::entity> children;

  bool self_positioned = false;
  std::function<vec2()> update;
  inline void Update( Element &elem ) { elem.pos = this->update(); }
};


struct TextLabel {
  std::string text;
  i32 font_size;
  Color text_color;
};

struct TextButton : TextLabel {
  std::function<void()> action;
};

struct TextureLabel {
  hstr texture_key;
};

struct TextureButton : TextureLabel {
  std::function<void()> action;
};

};// namespace UI
