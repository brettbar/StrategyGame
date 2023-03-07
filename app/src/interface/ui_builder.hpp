#pragma once

#include "ui_manager.hpp"


namespace UI
{
  struct Builder
  {
protected:
    Element _element;

public:
    Element build()
    {
      auto created = std::make_shared<Element>( _element );
      created->SubscribeToMessages();
      Manager()->lookup.insert_or_assign( _element.id, created );
      return _element;
    }

    virtual ~Builder() {}
  };

  struct Panel : Builder
  {
    Panel( std::string id )
    {
      _element.type = Type::PANEL;
      _element.id = id;
      _element.children_axis = Axis::Row;
      _element.children_horiz_align = Align::Start;
      _element.children_vert_align = Align::Start;
    }

    Panel &SetAnchor( Anchor anchor )
    {
      _element.anchor = anchor;
      return *this;
    }

    Panel &SetAxis( Axis axis )
    {
      _element.children_axis = axis;
      return *this;
    }

    Panel &ListensFor( std::vector<InterfaceUpdate::ID> updates )
    {
      _element.subscribed_updates = updates;
      return *this;
    }

    Panel &Children( std::vector<Element> children )
    {
      _element.children = children;
      return *this;
    }

    Panel &UpdateChildren(
      std::function<void( std::vector<Element> & )> update_children
    )
    {
      _element.update_children = update_children;
      return *this;
    }

    Panel &Background( Color background )
    {
      _element.background = background;
      return *this;
    }

    Panel &Margins( Margins margins )
    {
      _element.margins = margins;
      return *this;
    }
  };

  struct StackPanel : Builder
  {
    StackPanel( std::string id )
    {
      _element.type = Type::STACK_PANEL;
      _element.id = id;
      _element.curr_index = 0;
    }

    StackPanel &Background( Color background )
    {
      _element.background = background;
      return *this;
    }

    StackPanel &Children( std::vector<Element> children )
    {
      _element.children = children;
      return *this;
    }
  };

  struct TextLabel : Builder
  {
    TextLabel( std::string id )
    {
      _element.type = Type::TEXT_LABEL;
      _element.id = id;
    }

    TextLabel &Background( Color background )
    {
      _element.background = background;
      return *this;
    }

    TextLabel &SetText( std::string text, f32 font_size )
    {
      _element.text = text;
      _element.font_size = font_size;
      return *this;
    }

    TextLabel &SetText( std::string text, f32 font_size, Color text_color )
    {
      _element.text_color = text_color;
      return SetText( text, font_size );
    }

    TextLabel &ListensFor( std::vector<InterfaceUpdate::ID> updates )
    {
      _element.subscribed_updates = updates;
      return *this;
    }
  };

  struct TextButton : Builder
  {
    TextButton( std::string id )
    {
      _element.type = Type::TEXT_BUTTON;
      _element.id = id;
    }

    TextButton &Background( Color background )
    {
      _element.background = background;
      return *this;
    }

    TextButton &SetText( std::string text, f32 font_size )
    {
      _element.text = text;
      _element.font_size = font_size;
      return *this;
    }

    TextButton &SetText( std::string text, f32 font_size, Color text_color )
    {
      _element.text_color = text_color;
      return SetText( text, font_size );
    }

    TextButton &SetEvent( InterfaceEvent::Data event )
    {
      _element.on_click = std::make_shared<InterfaceEvent::Data>( event );
      return *this;
    }

    TextButton &ListensFor( std::vector<InterfaceUpdate::ID> updates )
    {
      _element.subscribed_updates = updates;
      return *this;
    }
  };


};// namespace UI
