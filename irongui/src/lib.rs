use raylib_ffi::*;
use std::ffi::CString;

enum Element {
    Grid {
        // rect: Rectangle,
        background: Color,
        cols: u32,
        rows: u32,
    },
    TextLabel,
}

fn draw(element: Element) {
    match element {
        Element::Grid {
            background,
            cols,
            rows,
        } => {
            println!("Its a grid!");
        }
        Element::TextLabel => {
            println!("Its a text label!");
        }
    }
}

static mut CONTENT: Vec<Box<Element>> = vec![];

fn init() {
    let foo = Element::Grid {
        background: colors::RED,
        cols: 1,
        rows: 6,
    };
}

fn draw_ui() {
    unsafe {
        DrawText(
            CString::new("Hello From Rust!!").unwrap().into_raw(),
            150,
            150,
            32,
            colors::ORANGE,
        );
    }
}

#[cxx::bridge(namespace = "irongui")]
mod ffi {
    extern "Rust" {
        fn draw_ui();
    }
}
