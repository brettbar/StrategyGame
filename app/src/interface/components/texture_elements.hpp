#pragma once

#include "element.hpp"

// TODO this might be a circular dependency issue
#include "../ui_manager.hpp"

namespace UI {

  struct TextureLabel : Element {
    Texture2D texture;

    void Resize() {
      transform.width = texture.width * UI::SCALE;
      transform.height = texture.height * UI::SCALE;
    }

    void Draw() {
      DrawTextureEx( texture, { transform.x, transform.y }, 0.0, SCALE, WHITE );
    }

    TextureLabel( std::string id )
        : Element( id, WHITE, false, {}, {} ),
          texture( Global::texture_cache[hstr{ id.c_str() }]->texture ) {
      transform.x = texture.width * UI::SCALE;
      transform.y = texture.height * UI::SCALE;
    }
  };

  struct TextureButton : TextureLabel {
    bool clickable = false;
    bool always_clickable = false;
    // std::function<void()> action;

    void Update() {
      // if ( label.elem.enabled && !always_clickable )
      //   clickable = clickable_lookup.at( label.elem.id )();
    }

    void Action() {
      // action();
      // Events::event_emitter.publish(
      //     Events::ButtonClick{ label.elem.id } );
    }

    TextureButton( std::string id ) : TextureLabel( id ) {}
  };


};// namespace UI
