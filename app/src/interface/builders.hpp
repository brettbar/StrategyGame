#pragma once

#include "element.hpp"

namespace UI
{

  //   class AbstractBuilder
  //   {
  // protected:
  //     Element &_element;
  //     explicit AbstractBuilder( Element &element ) : _element( element ) {}

  // public:
  //     operator Element() const
  //     {
  //       return std::move( _element );
  //     };
  //   };

  class GridPanelBuilder
  {
    sptr<Element> element;

public:
    GridPanelBuilder( str id, u32 cols, u32 rows )
        : element{ std::make_shared<Element>( Element{ Type::GridPanel, id } ) }
    {
      element->type = Type::GridPanel;
      element->id = id;
      element->num_cols = cols;
      element->num_rows = rows;
      for ( u32 i = 0; i < element->num_cols * element->num_rows; i++ )
      {
        element->grid.push_back( { 0, 0, 0, 0 } );
      }
    }

    operator sptr<Element>() const
    {
      return std::move( element );
    }

    GridPanelBuilder &FixedSize( u32 width, u32 height )
    {
      element->fixed_size = true;
      element->transform.width = width;
      element->transform.height = height;
      return *this;
    }

    GridPanelBuilder &Background( Color background )
    {
      element->background = background;
      return *this;
    }

    GridPanelBuilder &SetChild(
      list<u32> cols,
      list<u32> rows,
      sptr<Element> child
    )
    {
      // TODO LEFT OFF HERE.
      // Need to have some way to assocaited a child with the grid slots
      // So that when we go to reposition/resize the child we know
      // where to put it
      element->children.push_back( child );
      return *this;
    }
  };

  //   class GridPanelBuilder : public AbstractBuilder
  //   {
  //     uptr<Element> _element;

  // public:
  //     explicit GridPanelBuilder( std::string id, u32 num_cols, u32 num_rows )
  //         : AbstractBuilder{ _element }
  //     {
  //       _element.type = Type::GridPanel;
  //       _element.id = id;
  //       _element.num_cols = num_cols;
  //       _element.num_rows = num_rows;
  //       for ( u32 i = 0; i < num_cols * num_rows; i++ )
  //       {
  //         _element.grid.push_back( { 0, 0, 0, 0 } );
  //       }
  //     }

  //     GridPanelBuilder &On(
  //       InterfaceUpdate::ID update_id,
  //       std::function<void( Element &self, InterfaceUpdate::Update update )>
  //         update_fn
  //     )
  //     {
  //       _element.updates.emplace( update_id, update_fn );
  //       return *this;
  //     }

  //     GridPanelBuilder &StartDisabled()
  //     {
  //       _element.starts_disabled = true;
  //       return *this;
  //     }


  //     GridPanelBuilder &UpdateChildren(
  //       std::function<
  //         void( std::map<std::string, bool> &, std::vector<Element> & )>
  //         update_children
  //     )
  //     {
  //       // _element.update_children = update_children;
  //       return *this;
  //     }


  //     GridPanelBuilder &Margins( Margins margins )
  //     {
  //       _element.margins = margins;
  //       return *this;
  //     }
  //   };

  //   class DataPanelBuilder : public AbstractBuilder
  //   {
  //     Element _element;

  // public:
  //     explicit DataPanelBuilder( std::string id ) : AbstractBuilder{ _element }
  //     {
  //       _element.type = Type::DataPanel;
  //       _element.id = id;
  //       _element.curr_index = 0;
  //     }

  //     DataPanelBuilder &FixedSize( u32 width, u32 height )
  //     {
  //       _element.fixed_size = true;
  //       _element.transform.width = width;
  //       _element.transform.height = height;
  //       return *this;
  //     }

  //     DataPanelBuilder &DataPoints( std::map<std::string, Element> data_points )
  //     {
  //       _element.data_points = data_points;
  //       return *this;
  //     }

  //     DataPanelBuilder &Update( std::function<void( Element & )> update )
  //     {
  //       _element.self_update = update;
  //       return *this;
  //     }
  //   };

  //   class StackPanelBuilder : public AbstractBuilder
  //   {
  //     Element _element;

  // public:
  //     explicit StackPanelBuilder( std::string id ) : AbstractBuilder{ _element }
  //     {
  //       _element.type = Type::StackPanel;
  //       _element.id = id;
  //       _element.curr_index = 0;
  //     }

  //     StackPanelBuilder &FixedSize( u32 width, u32 height )
  //     {
  //       _element.fixed_size = true;
  //       _element.transform.width = width;
  //       _element.transform.height = height;
  //       return *this;
  //     }

  //     StackPanelBuilder &On(
  //       InterfaceUpdate::ID update_id,
  //       std::function<void( Element &self, InterfaceUpdate::Update update )>
  //         update_fn
  //     )
  //     {
  //       _element.updates.emplace( update_id, update_fn );
  //       return *this;
  //     }


  //     StackPanelBuilder &Background( Color background )
  //     {
  //       _element.background = background;
  //       return *this;
  //     }

  //     StackPanelBuilder &Children( std::vector<Element> children )
  //     {
  //       _element.children = children;
  //       return *this;
  //     }
  //   };

  //   class TextLabelBuilder : public AbstractBuilder
  //   {
  //     Element _element;

  // public:
  //     explicit TextLabelBuilder( std::string id ) : AbstractBuilder{ _element }
  //     {
  //       _element.type = Type::TextLabel;
  //       _element.id = id;
  //     }

  //     TextLabelBuilder &On(
  //       InterfaceUpdate::ID update_id,
  //       std::function<void( Element &self, InterfaceUpdate::Update update )>
  //         update_fn
  //     )
  //     {
  //       _element.updates.emplace( update_id, update_fn );
  //       return *this;
  //     }

  //     TextLabelBuilder &Background( Color background )
  //     {
  //       _element.background = background;
  //       return *this;
  //     }

  //     TextLabelBuilder &Text( std::string text, f32 font_size )
  //     {
  //       _element.text = text;
  //       _element.font_size = font_size;
  //       return *this;
  //     }

  //     TextLabelBuilder &Text( std::string text, f32 font_size, Color text_color )
  //     {
  //       _element.text_color = text_color;
  //       return Text( text, font_size );
  //     }

  //     TextLabelBuilder &Margins( Margins margins )
  //     {
  //       _element.margins = margins;
  //       return *this;
  //     }
  //   };

  //   class TextButtonBuilder : public AbstractBuilder
  //   {
  //     Element _element;

  // public:
  //     explicit TextButtonBuilder( std::string id ) : AbstractBuilder{ _element }
  //     {
  //       _element.type = Type::TextButton;
  //       _element.id = id;
  //     }

  //     TextButtonBuilder &On(
  //       InterfaceUpdate::ID update_id,
  //       std::function<void( Element &self, InterfaceUpdate::Update update )>
  //         update_fn
  //     )
  //     {
  //       _element.updates.emplace( update_id, update_fn );
  //       return *this;
  //     }

  //     TextButtonBuilder &StartDisabled()
  //     {
  //       _element.starts_disabled = true;
  //       return *this;
  //     }

  //     TextButtonBuilder &Background( Color background )
  //     {
  //       _element.background = background;
  //       return *this;
  //     }

  //     TextButtonBuilder &Text( std::string text, f32 font_size )
  //     {
  //       _element.text = text;
  //       _element.font_size = font_size;
  //       return *this;
  //     }

  //     TextButtonBuilder &Text( std::string text, f32 font_size, Color text_color )
  //     {
  //       _element.text_color = text_color;
  //       return Text( text, font_size );
  //     }

  //     TextButtonBuilder &SetEvent( InterfaceEvent::Data event )
  //     {
  //       _element.on_click = std::make_shared<InterfaceEvent::Data>( event );
  //       return *this;
  //     }


  //     TextButtonBuilder &Clickable( bool clickable )
  //     {
  //       _element.clickable = clickable;
  //       return *this;
  //     }
  //   };

  //   class TextureLabelBuilder : public AbstractBuilder
  //   {
  //     Element _element;

  // public:
  //     explicit TextureLabelBuilder( std::string id ) : AbstractBuilder{ _element }
  //     {
  //       _element.type = Type::TextureButton;
  //       _element.id = id;
  //     }

  //     TextureLabelBuilder &Texture( std::string texture_id )
  //     {
  //       _element.texture =
  //         Global::texture_cache[hstr{ texture_id.c_str() }]->texture;
  //       return *this;
  //     }
  //   };

  //   class TextureButtonBuilder : public AbstractBuilder
  //   {
  //     Element _element;

  // public:
  //     explicit TextureButtonBuilder( std::string id )
  //         : AbstractBuilder{ _element }
  //     {
  //       _element.type = Type::TextureButton;
  //       _element.id = id;
  //       _element.texture = Global::texture_cache[hstr{ id.c_str() }]->texture;
  //     }

  //     TextureButtonBuilder &SetEvent( InterfaceEvent::Data event )
  //     {
  //       _element.on_click = std::make_shared<InterfaceEvent::Data>( event );
  //       return *this;
  //     }
  //   };


  inline GridPanelBuilder GridPanel(
    std::string id,
    u32 num_cols,
    u32 num_rows
  )
  {
    return GridPanelBuilder{ id, num_cols, num_rows };
  }

  // inline DataPanelBuilder DataPanel( std::string id )
  // {
  //   return DataPanelBuilder{ id };
  // }

  // inline StackPanelBuilder StackPanel( std::string id )
  // {
  //   return StackPanelBuilder{ id };
  // }

  // inline TextLabelBuilder TextLabel( std::string id )
  // {
  //   return TextLabelBuilder{ id };
  // }

  // inline TextButtonBuilder TextButton( std::string id )
  // {
  //   return TextButtonBuilder{ id };
  // }

  // inline TextureLabelBuilder TextureLabel( std::string id )
  // {
  //   return TextureLabelBuilder{ id };
  // }

  // inline TextureButtonBuilder TextureButton( std::string id )
  // {
  //   return TextureButtonBuilder{ id };
  // }

};// namespace UI
