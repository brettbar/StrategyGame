#pragma once

#ifndef UI_HPP
#define UI_HPP

#include "../common.hpp"

namespace UI {

enum Shape {
  Texture,
  Rect,
  Rect_Lines,
  Rect_Rounded,
};

struct Element {
  // Required
  u32 id;
  str name;
  Shape shape;
  bool debugOnly;
  Vector2 position;

  // Optional
  Rectangle panel;
  Color color;
  Texture2D texture;
  std::vector<Element> children;
};

void Init(State &);
void Input(State &);
void Update(State &);
void Draw(State &);

void DrawTopBar(State &);
void DrawElement(Element);

Element CreateButton(Texture2D, Vector2, Color);
Element CreateSideBar(State &);

// Debug Only
Element CreateDrawer(State &);

}; // namespace UI

#endif
