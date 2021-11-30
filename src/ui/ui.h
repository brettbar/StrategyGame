#pragma once

#ifndef UI_H 
#define UI_H

#include "../common.h"

namespace UI {

struct UI_Element {
  u32 id;
  str type;
  bool debugOnly;
  Rectangle panel;
  Vector2 position;
  std::vector<UI_Element> children;
  Texture2D texture;
};

void Init(State &);
void Input(State &);
void Update(State &);
void Draw(State &);

void DrawTopBar(State &);
void DrawDebug(State &);

UI_Element createButton(Texture2D, Vector2, Color);

};


#endif
