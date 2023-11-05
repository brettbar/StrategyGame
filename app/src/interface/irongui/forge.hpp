#pragma once

#include "../../shared/common.hpp"
#include "elements.hpp"

namespace Iron {

  struct Forge {
    list<Element *> queue;

    Forge() {}
    ~Forge() {
      queue.clear();
    }

    Element *Grid( rect t, u32 c, u32 r, Color color = { 0, 0, 0, 0 } ) {
      auto e = new Element();
      e->type = Type_t::Grid;
      e->id = queue.size();
      e->background = BLUE;
      e->transform = t;
      e->background = color;
      e->t.grid = new IGrid( c, r );
      queue.push_back( e );
      return e;
    }

    Element *TextLabel( rect t, str txt, Color c ) {
      auto e = new Element();
      e->type = Type_t::TextLabel;
      e->id = queue.size();
      e->background = c;
      e->transform = t;
      e->t.text_label = new ITextLabel( txt );
      queue.push_back( e );
      return e;
    }

    bool TextButton( rect t, str txt, Color c ) {
      auto e = TextLabel( t, txt, c );
      e->interactable = true;
      return State()->context.active == e->id;
    }

    inline void Draw() {
      for ( u32 i = 0; i < queue.size(); i++ ) {
        // State()->CheckInteract( *queue[i] );
        queue[i]->Draw();
      }
    }
  };

};// namespace Iron
