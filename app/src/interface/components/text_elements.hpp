#pragma once

#include "element.hpp"

// TODO this might be a circular dependency issue
#include "../ui_manager.hpp"

namespace UI {

  struct TextLabel {
    Element elem;
    std::string text;
    i32 font_size;
    Color text_color;
    bool dynamic = false;
    // std::function<std::string()> update;

    Messages::Basic subscribed_message;

    std::string ID() {
      return elem.id;
    }

    void Resize() {
      const vec2 text_dims = MeasureTextEx(
        Global::font_cache[hstr{ "font_romulus" }]->font,
        text.c_str(),
        font_size,
        2.0f
      );

      elem.transform.width = text_dims.x;
      elem.transform.height = text_dims.y;
    }

    // void Update() {
    //   if ( elem.enabled && dynamic ) {
    //     if ( update ) {
    //       text = update();
    //     }
    //     else {
    //       printf( "ERROR :: Update not found for elem %s\n", elem.id.c_str() );
    //     }
    //   }
    // }

    // void Update( std::string updated_text ) {
    //   if ( elem.enabled ) {
    //     text = updated_text;
    //   }
    // }

    void CheckSubscribedMessages() {}


    void Draw() {
      DrawRectangleV(
        { elem.transform.x, elem.transform.y },
        { elem.transform.width, elem.transform.height },
        elem.background
      );

      DrawTextEx(
        Global::font_cache[hstr{ "font_romulus" }]->font,
        text.c_str(),
        {
          elem.transform.x,
          elem.transform.y,
        },
        font_size,
        2.0,
        text_color
      );
    }

    void Draw( Color override_background ) {
      DrawRectangleV(
        { elem.transform.x, elem.transform.y },
        { elem.transform.width, elem.transform.height },
        override_background
      );

      DrawTextEx(
        Global::font_cache[hstr{ "font_romulus" }]->font,
        text.c_str(),
        {
          elem.transform.x,
          elem.transform.y,
        },
        font_size,
        2.0,
        text_color
      );
    }

    TextLabel(
      std::string id,
      std::string text,
      i32 font_size,
      Color background,
      Color text_color,
      bool dynamic,
      Messages::Basic subscribed_message
    )
        : elem( Element( id, background, false, {}, {} ) ), text( text ),
          font_size( font_size ), text_color( text_color ), dynamic( dynamic ),
          subscribed_message( subscribed_message ) {}
  };

  struct TextButton : TextLabel {
    bool always_clickable = true;
    bool clickable = true;

    Events::Basic *on_click;

    void Draw() {
      if ( !clickable )
        TextLabel::Draw( Fade( BLACK, 0.5 ) );
      else
        TextLabel::Draw();
    }

    void Action() {
      Events::event_emitter.publish( *on_click );
    }

    TextButton(
      std::string id,
      std::string text,
      i32 font_size,
      Color background,
      Color text_color,
      bool dynamic,
      Messages::Basic subscribed_message
    )
        : TextLabel(
            id,
            text,
            font_size,
            background,
            text_color,
            dynamic,
            subscribed_message
          ),
          on_click( new Events::Basic{ id } ) {}

    ~TextButton() {
      delete on_click;
    }

    // TextButton(
    //   std::string id,
    //   std::string text,
    //   i32 font_size,
    //   Color background,
    //   Color text_color,
    //   bool dynamic,
    //   Messages::Basic subscribed_message,
    //   Events::Basic on_click
    // )
    //     : TextLabel(
    //         id,
    //         text,
    //         font_size,
    //         background,
    //         text_color,
    //         dynamic,
    //         subscribed_message
    //       ) {
    //   this->on_click = std::make_unique<Events::Basic>( on_click );
    // }
  };
};// namespace UI
