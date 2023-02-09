#pragma once

#include "element.hpp"

// TODO this might be a circular dependency issue
#include "../ui_manager.hpp"

namespace UI {

  struct TextureLabel {
    Element elem;
    Texture2D texture;

    std::string ID() {
      return elem.id;
    }

    void Resize() {
      elem.transform.width = texture.width * UI::SCALE;
      elem.transform.height = texture.height * UI::SCALE;
    }

    void Draw() {
      DrawTextureEx(
        texture, { elem.transform.x, elem.transform.y }, 0.0, SCALE, WHITE
      );
    }

    TextureLabel( std::string id )
        : elem( Element( id, WHITE, false, {}, {} ) ),
          texture( Global::texture_cache[hstr{ id.c_str() }]->texture ) {
      elem.transform.x = texture.width * UI::SCALE;
      elem.transform.y = texture.height * UI::SCALE;
    }
  };

  struct TextureButton {
    TextureLabel label;
    bool clickable = false;
    bool always_clickable = false;
    // std::function<void()> action;

    std::string ID() {
      return label.ID();
    }

    void Draw() {
      label.Draw();
    }

    void Update() {
      // if ( label.elem.enabled && !always_clickable )
      //   clickable = clickable_lookup.at( label.elem.id )();
    }

    void Action() {
      // action();
      // Events::event_emitter.publish(
      //     Events::ButtonClick{ label.elem.id } );
    }

    TextureButton( std::string id ) : label( TextureLabel( id ) ) {}
  };


};// namespace UI
