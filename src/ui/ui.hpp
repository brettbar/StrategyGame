#pragma once

#ifndef UI_HPP
#define UI_HPP

#include "../common.hpp"
#include "../state.hpp"
#include "../resource.hpp"
#include "../systems/provinces.hpp"

namespace UI {

enum Style {
  RECT_FILLED,
  RECT_LINES,
  RECT_ROUNDED,
};

enum LayoutStyle {
  HORIZONTAL,
  VERTICAL,
  GRID,
};

struct Element {
  u32 id;
  str name;
  bool debug;
  bool stateful;
  Vector2 position;
};

struct Layout {
  u32 width;
  u32 height;
  LayoutStyle style;
};

struct PanelSolid : Element {
  Style style;
  Color color;
  Layout layout;
  std::vector<Element> children;
};

struct PanelTexture : Element {
  Texture texture;
  Layout layout;
  std::vector<Element> children;
};

struct TextButton : Element {
  Style style;
  Color color;
  str text;
};

struct ImageButton : Element {
  Texture2D texture;
};

void Create(State &, entt::registry &, TextureCache &);
void Update(State &, entt::registry &);
void Draw(State &, entt::registry &);
str FormatDate(u32, u32);


}; // namespace UI

#endif
