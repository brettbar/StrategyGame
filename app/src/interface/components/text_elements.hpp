#pragma once

#include "element.hpp"

// TODO this might be a circular dependency issue
#include "../ui_manager.hpp"
#include <memory>

namespace UI {

  struct TextLabel : Element {
    // Element elem;
    std::string text;
    i32 font_size;
    Color text_color;
    // TODO(rf) probably can remove all together
    bool dynamic = false;
    std::vector<Messages::ID> subscribed_messages;

    void Enable() override {
      Element::Enable();
      SubscribeToMessages();
    }

    void Disable() override {
      Element::Disable();
      UnsubscribeFromMessages();
    }


    void ReceiveUpdateText( const Messages::UpdateText &event ) {
      for ( Messages::ID msg_id: subscribed_messages ) {
        if ( msg_id == event.message_id ) {
          text = event.updated_text;
          break;
        }
      }
    }

    void ReceiveUpdateBackground( const Messages::UpdateBackground &event ) {
      for ( Messages::ID msg_id: subscribed_messages ) {
        if ( msg_id == event.message_id ) {
          background = event.updated_background;
          break;
        }
      }
    }

    void Resize() {
      const vec2 text_dims = MeasureTextEx(
        Global::font_cache[hstr{ "font_romulus" }]->font,
        text.c_str(),
        font_size,
        2.0f
      );

      transform.width = text_dims.x;
      transform.height = text_dims.y;
    }

    void SubscribeToMessages() {
      Messages::dispatcher.sink<Messages::UpdateText>()
        .connect<&TextLabel::ReceiveUpdateText>( this );

      Messages::dispatcher.sink<Messages::UpdateBackground>()
        .connect<&TextLabel::ReceiveUpdateBackground>( this );
    }

    void UnsubscribeFromMessages() {
      Messages::dispatcher.sink<Messages::UpdateText>()
        .disconnect<&TextLabel::ReceiveUpdateText>( this );

      Messages::dispatcher.sink<Messages::UpdateBackground>()
        .disconnect<&TextLabel::ReceiveUpdateBackground>( this );
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
        { transform.x, transform.y },
        { transform.width, transform.height },
        background
      );

      DrawTextEx(
        Global::font_cache[hstr{ "font_romulus" }]->font,
        text.c_str(),
        {
          transform.x,
          transform.y,
        },
        font_size,
        2.0,
        text_color
      );
    }

    void Draw( Color override_background ) {
      DrawRectangleV(
        { transform.x, transform.y },
        { transform.width, transform.height },
        override_background
      );

      DrawTextEx(
        Global::font_cache[hstr{ "font_romulus" }]->font,
        text.c_str(),
        {
          transform.x,
          transform.y,
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
        : Element( id, background, false, {}, {} ), text( text ),
          font_size( font_size ), text_color( text_color ), dynamic( false ) {}

    TextLabel(
      std::string id,
      std::string text,
      i32 font_size,
      Color background,
      Color text_color,
      std::vector<Messages::ID> subscribed_messages
    )
        : Element( id, background, false, {}, {} ), text( text ),
          font_size( font_size ), text_color( text_color ), dynamic( true ),
          subscribed_messages( subscribed_messages ) {}
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
      std::vector<Messages::ID> subscribed_messages
    )
        : TextLabel(
            id,
            text,
            font_size,
            background,
            text_color,
            subscribed_messages
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
      std::vector<Messages::ID> subscribed_messages,
      std::shared_ptr<Events::Basic> event
    )
        : TextLabel(
            id,
            text,
            font_size,
            background,
            text_color,
            subscribed_messages
          ),
          on_click( event ) {}

    ~TextButton() {
      on_click = nullptr;
    }
  };
};// namespace UI
