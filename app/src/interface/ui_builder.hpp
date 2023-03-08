#pragma once

#include "ui_manager.hpp"


namespace UI
{
  //   struct Builder
  //   {
  // protected:
  // public:
  //     Element build()
  //     {
  //       // auto created = std::make_shared<Element>( _element );
  //       // Manager()->lookup.insert_or_assign( _element.id, created );
  //       return _element;
  //     }

  //     virtual ~Builder() {}
  //   };

  class Panel
  {
    Element _element;

public:
    Panel( std::string id )
    {
      _element = Element();
      _element.type = Type::Panel;
      _element.id = id;
      _element.children_axis = Axis::Row;
      _element.children_horiz_align = Align::Start;
      _element.children_vert_align = Align::Start;
      // _element.SubscribeToUpdates();
    }

    Element build()
    {
      return _element;
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

  class StackPanel
  {
    Element _element;

public:
    StackPanel( std::string id )
    {
      _element = Element();
      _element.type = Type::StackPanel;
      _element.id = id;
      _element.curr_index = 0;
      // _element.SubscribeToUpdates();
    }

    Element build()
    {
      return _element;
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

  struct TextLabel
  {
    Element _element;

public:
    TextLabel( std::string id )
    {
      _element = Element();
      _element.type = Type::TextLabel;
      _element.id = id;
      // _element.SubscribeToUpdates();
    }

    Element build()
    {
      return _element;
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

  struct TextButton
  {
    Element _element;

public:
    TextButton( std::string id )
    {
      _element = Element();
      _element.type = Type::TextButton;
      _element.id = id;
      // _element.SubscribeToUpdates();
    }

    Element build()
    {
      return _element;
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

    TextButton &Clickable( bool clickable )
    {
      _element.clickable = clickable;
      return *this;
    }
  };


};// namespace UI
