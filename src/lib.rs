use pages::main_menu;
mod components;
mod pages;

static mut CONTENT: Vec<Box<dyn components::element::Draw>> = vec![];

fn init() {
    unsafe {
        CONTENT.push(main_menu::create());
        CONTENT[0].enable();
    }
}

fn draw_ui() {
    unsafe {
        for panel in CONTENT.iter_mut() {
            panel.draw();
        }
    }
}

#[cxx::bridge(namespace = "rustgui")]
mod ffi {
    extern "Rust" {
        fn init();
        fn draw_ui();
    }
}
