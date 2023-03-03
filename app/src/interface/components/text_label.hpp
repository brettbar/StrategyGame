#pragma once

#include "element.hpp"

// TODO this might be a circular dependency issue
#include "../ui_manager.hpp"

namespace UI
{
  struct TextLabel : Element
  {
    // Element elem;
    std::string text;
    i32 font_size;
    Color text_color;
    // TODO(rf) probably can remove all together
    bool dynamic = false;
    std::vector<InterfaceUpdate::ID> subscribed_messages;

    void Enable() override
    {
      Element::Enable();
      SubscribeToMessages();
    }

    void Disable() override
    {
      Element::Disable();
      UnsubscribeFromMessages();
    }


    void ReceiveMessage( const InterfaceUpdate::Data &msg )
    {
      for ( InterfaceUpdate::ID msg_id: subscribed_messages )
      {
        if ( msg_id == msg.update_id )
        {
          switch ( msg.type )
          {
            case InterfaceUpdate::EnabledUpdate:
              if ( msg.on )
                Enable();
              else
                Disable();
              break;
            case InterfaceUpdate::TextUpdate:
              text = msg.updated_text;
              break;
            case InterfaceUpdate::BackgroundUpdate:
              background = msg.updated_background;
              break;
            case InterfaceUpdate::TargetedTextUpdate:
              printf( "TextLabel::ReceiveMessage msg type: %d\n", msg.type );

              printf(
                "msg.target %s, id %s\n", msg.target.c_str(), id.c_str()
              );

              if ( msg.target == id )
              {
                text = msg.updated_text;
              }
              break;
            case InterfaceUpdate::TargetedBackgroundUpdate:
              if ( msg.target == id )
              {
                background = msg.updated_background;
              }
              break;
            default:
              printf( "Error :: Panel deos not support message type\n" );
              break;
          }
          break;
        }
        break;
      }
    }

    void Resize() override
    {
      const vec2 text_dims = MeasureTextEx(
        Global::font_cache[hstr{ "font_romulus" }]->font,
        text.c_str(),
        font_size,
        2.0f
      );

      transform.width = text_dims.x;
      transform.height = text_dims.y;
    }

    void SubscribeToMessages() override
    {
      InterfaceUpdate::dispatcher.sink<InterfaceUpdate::Data>()
        .connect<&TextLabel::ReceiveMessage>( this );
    }

    void UnsubscribeFromMessages() override
    {
      InterfaceUpdate::dispatcher.sink<InterfaceUpdate::Data>()
        .disconnect<&TextLabel::ReceiveMessage>( this );
    }

    void Draw() override
    {
      if ( !IsEnabled() )
        return;

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

    void Draw( Color override_background )
    {
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
          font_size( font_size ), text_color( text_color ), dynamic( false )
    {
    }

    TextLabel(
      std::string id,
      std::string text,
      i32 font_size,
      Color background,
      Color text_color,
      std::vector<InterfaceUpdate::ID> subscribed_messages
    )
        : Element( id, background, false, {}, {} ), text( text ),
          font_size( font_size ), text_color( text_color ), dynamic( true ),
          subscribed_messages( subscribed_messages )
    {
    }
  };
};// namespace UI
