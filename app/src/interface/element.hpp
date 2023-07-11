#pragma once

#include "../shared/global.hpp"
#include "../shared/utils.hpp"
#include "../signals/events.hpp"
#include "../signals/updates.hpp"
#include "ui_shared.hpp"

namespace UI
{
  enum class Type : u32
  {
    Panel,
    DataPanel,
    StackPanel,
    TextLabel,
    TextButton,
    TextureLabel,
    TextureButton,
  };

  enum class Anchor
  {
    Free,
    TopLeft,
    TopMid,
    TopRight,
    MidLeft,
    Centered,
    MidRight,
    BottomLeft,
    BottomMid,
    BottomRight,
  };

  enum class Axis
  {
    Row,
    Column,
  };

  enum class Align
  {
    Start,
    SpaceBetween,
    Stretch,
  };


  struct Margins
  {
    u32 left;
    u32 right;
    u32 top;
    u32 bottom;
  };

  class Element
  {
    // Element() = delete;
    Element() = default;
    Element( std::string id ) : id( id ){};
    // ~Element() {
    //   // TODO Remove id from lookup
    // }

public:
    Type type;
    std::string id = "INVALID";
    bool enabled = false;
    bool starts_disabled = false;
    bool fixed_size = false;
    Color background = BLACK;
    rect transform = rect{ 0, 0, 0, 0 };
    Margins margins = Margins{ 0, 0, 0, 0 };

    // Panel
    Anchor anchor = Anchor::Free;// Might apply to others as well?
    Axis children_axis = Axis::Row;
    Align children_horiz_align;
    Align children_vert_align;
    std::vector<Element> children = {};

    //DataPanel
    std::map<std::string, Element> data_points = {};
    std::function<void( Element & )> self_update = {};

    std::vector<InterfaceUpdate::ID> subscribed_updates = {};
    std::map<
      InterfaceUpdate::ID,
      std::function<void( Element &, InterfaceUpdate::Update )>>
      updates = {};

    // StackPanel
    // TODO this should probably be made private
    u32 curr_index = 0;

    // TextLabel
    std::string text = "INVALID";
    i32 font_size = 14;
    Color text_color = WHITE;
    // TODO(rf) probably can remove all together
    bool dynamic = false;

    // TextButton
    bool clickable = true;
    ptr<InterfaceEvent::Data> on_click = nullptr;

    // TextureLabel
    Texture2D texture = Texture2D();

    void Enable();
    void Register();
    void Disable();
    void Destroy();
    // TODO should we take in to account ui scale
    void Resize();
    void Reposition();
    void LayoutChild( Element &, f32, f32 &, f32 & );
    void UpdateText( std::string );
    void UpdateBackground( Color );
    void UpdateClickable( bool new_clickable );
    void ExecuteInterfaceUpdate( const InterfaceUpdate::Update & );
    void Update();

    friend class PanelBuilder;
    void PanelEnable();
    void PanelRegister();
    void PanelDisable();
    void PanelResize();

    friend class DataPanelBuilder;
    void DataPanelEnable();
    void DataPanelRegister();
    void DataPanelDisable();
    void DataPanelResize();

    friend class StackPanelBuilder;
    void StackPanelEnable();
    void StackPanelRegister();
    void StackPanelDisable();
    void StackPanelSwitchChild( u32 );
    void StackPanelResize();

    friend class TextLabelBuilder;
    friend class TextButtonBuilder;
    friend class TextureLabelBuilder;
    friend class TextureButtonBuilder;

    void Draw();
    void FireEvent();

    void CreateElementForDatapoints( Element );
  };

};// namespace UI
