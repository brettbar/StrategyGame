#pragma once

#include "element.hpp"

// TODO this might be a circular dependency issue
#include "../ui_manager.hpp"
#include <memory>

namespace UI {

  struct TextLabel {
    Element elem;
    std::string text;
    i32 font_size;
    Color text_color;
    // TODO(rf) probably can remove all together
    bool dynamic = false;
    // std::function<std::string()> update;

    std::shared_ptr<Messages::Basic> subscribed_message;

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
      Color text_color
    )
        : elem( Element( id, background, false, {}, {} ) ), text( text ),
          font_size( font_size ), text_color( text_color ), dynamic( false ),
          subscribed_message( {} ) {}

    TextLabel(
      std::string id,
      std::string text,
      i32 font_size,
      Color background,
      Color text_color,
      std::shared_ptr<Messages::Basic> subscribed_message
    )
        : elem( Element( id, background, false, {}, {} ) ), text( text ),
          font_size( font_size ), text_color( text_color ), dynamic( true ),
          subscribed_message( subscribed_message ) {}
  };

  struct TextButton : TextLabel {
    bool always_clickable = true;
    bool clickable = true;

    std::shared_ptr<Events::Basic> on_click;

    void Draw() {
      if ( !clickable )
        TextLabel::Draw( Fade( BLACK, 0.5 ) );
      else
        TextLabel::Draw();
    }

    void FireEvent() {
      switch ( on_click->type ) {
        case Events::Type::Basic: {
          Events::event_emitter.publish( *on_click );
        } break;
        case Events::Type::ButtonClick: {
          std::shared_ptr<Events::ButtonClick> button_click =
            std::dynamic_pointer_cast<Events::ButtonClick>( on_click );

          if ( button_click ) {
            printf(
              "Sending button click! %s %s\n",
              button_click->origin_id.c_str(),
              button_click->msg.c_str()
            );
            Events::event_emitter.publish( *button_click );
          }
        } break;
        case Events::Type::JoinLobby: {
        } break;
      }
    }

    TextButton(
      std::string id,
      std::string text,
      i32 font_size,
      Color background,
      Color text_color
    )
        : TextLabel( id, text, font_size, background, text_color ),
          on_click( new Events::Basic{ id } ) {}

    TextButton(
      std::string id,
      std::string text,
      i32 font_size,
      Color background,
      Color text_color,
      std::shared_ptr<Messages::Basic> subscribed_message
    )
        : TextLabel(
            id,
            text,
            font_size,
            background,
            text_color,
            subscribed_message
          ),
          on_click( new Events::Basic{ id } ) {}

    TextButton(
      std::string id,
      std::string text,
      i32 font_size,
      Color background,
      Color text_color,
      std::shared_ptr<Events::Basic> event
    )
        : TextLabel( id, text, font_size, background, text_color ),
          on_click( event ) {}

    TextButton(
      std::string id,
      std::string text,
      i32 font_size,
      Color background,
      Color text_color,
      std::shared_ptr<Messages::Basic> subscribed_message,
      std::shared_ptr<Events::Basic> event
    )
        : TextLabel(
            id,
            text,
            font_size,
            background,
            text_color,
            subscribed_message
          ),
          on_click( event ) {}

    ~TextButton() {
      on_click = nullptr;
    }
  };
};// namespace UI
