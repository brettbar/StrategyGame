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
    GridPanel,
    DataPanel,
    StackPanel,
    TextLabel,
    TextButton,
    TextureLabel,
    TextureButton,
  };

  struct Margins
  {
    u32 left;
    u32 right;
    u32 top;
    u32 bottom;
  };

  struct Element
  {
    // Element() = delete;
    // Element() = default;
    // ~Element() {
    //   // TODO Remove id from lookup
    // }
    Element( Type type, str id ) : type( type ), id( id ) {}

    Type type;
    str id = "INVALID";
    bool enabled = false;
    bool starts_disabled = false;
    Color background = BLACK;
    rect transform = rect{ 0, 0, 0, 0 };
    Margins margins = Margins{ 0, 0, 0, 0 };

    // Panel
    bool fixed_size = false;
    u32 num_cols = 0;
    u32 num_rows = 0;
    list<rect> grid = {};
    list<sptr<Element>> children = {};

    //DataPanel
    std::map<std::string, Element> data_points = {};
    std::function<void( Element & )> self_update = {};

    list<InterfaceUpdate::ID> subscribed_updates = {};
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
    sptr<InterfaceEvent::Data> on_click = nullptr;

    // TextureLabel
    Texture2D texture = Texture2D();

    // High Level
    void Enable();
    void Register();
    void Disable();
    void Destroy();
    // TODO should we take in to account ui scale
    void ResizeRecursive();
    void RepositionRecursive();
    void Draw();
    void UpdateText( std::string );
    void UpdateBackground( Color );
    void UpdateClickable( bool new_clickable );
    void ExecuteInterfaceUpdate( const InterfaceUpdate::Update & );
    void Update();
    void FireEvent();

    friend class GridPanelBuilder;
    void PanelEnable();
    void PanelRegister();
    void PanelDisable();
    void PanelResize();
    void PanelReposition();
    void PanelDraw();
    void PanelExecuteInterfaceUpdate( const InterfaceUpdate::Update & );

    // friend class DataPanelBuilder;
    // void DataPanelEnable();
    // void DataPanelRegister();
    // void DataPanelDisable();
    // void DataPanelResize();
    // void DataPanelReposition();
    // void DataPanelDraw();
    // void DataPanelExecuteInterfaceUpdate( const InterfaceUpdate::Update & );
    // void CreateElementForDatapoints( Element );

    // friend class StackPanelBuilder;
    // void StackPanelEnable();
    // void StackPanelRegister();
    // void StackPanelDisable();
    // void StackPanelSwitchChild( u32 );
    // void StackPanelResize();
    // void StackPanelReposition();
    // void StackPanelDraw();
    // void StackPanelExecuteInterfaceUpdate( const InterfaceUpdate::Update & );

    // friend class TextLabelBuilder;
    // friend class TextButtonBuilder;
    // friend class TextureLabelBuilder;
    // friend class TextureButtonBuilder;
  };

};// namespace UI
