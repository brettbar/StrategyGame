use raylib_ffi::*;
use std::ffi::CString;

trait Draw {
    fn draw(&self);
}

struct Child {
    element: Element,
    // start_col: u32,
    // end_col: u32,
    // start_row: u32,
    // end_row: u32,
}

enum Element {
    Grid {
        rect: Rectangle,
        background: Color,
        cols: u32,
        rows: u32,
        children: Vec<Child>,
    },
    TextLabel(Vector2, &'static str),
}

static mut ROOT: Vec<Element> = vec![];

impl Element {
    fn draw(&self) {
        match self {
            Element::Grid {
                rect,
                background,
                cols,
                rows,
                children,
            } => unsafe {
                DrawRectangleRec(rect.to_owned(), background.to_owned());

                let child_width = rect.width / (cols.to_owned() as f32);
                let child_height = rect.height / (rows.to_owned() as f32);

                // Draw the Grid
                for c in 0..cols.to_owned() {
                    for r in 0..rows.to_owned() {
                        DrawRectangleRec(
                            Rectangle {
                                x: rect.x + (child_width * c as f32),
                                y: rect.y + (child_height * r as f32),
                                width: child_width,
                                height: child_height,
                            },
                            colors::GRAY,
                        );
                    }
                }

                for child in children.iter() {
                    child.element.draw();
                }
            },
            Element::TextLabel(vec2, txt) => unsafe {
                DrawText(
                    CString::new(txt.to_owned()).unwrap().into_raw(),
                    vec2.to_owned().x as i32,
                    vec2.to_owned().y as i32,
                    32,
                    colors::ORANGE,
                );
            },
        }
    }
}

fn init() {
    unsafe {
        ROOT.push(Element::Grid {
            rect: Rectangle {
                x: 150.,
                y: 150.,
                width: 300.,
                height: 600.,
            },
            background: colors::RED,
            cols: 1,
            rows: 6,
            children: vec![Child {
                element: Element::TextLabel(Vector2 { x: 150., y: 150. }, "Hello from Rust!!!"),
            }],
        });
    }
}

fn draw_ui() {
    unsafe {
        for panel in ROOT.iter() {
            panel.draw();
        }
    }
}

#[cxx::bridge(namespace = "irongui")]
mod ffi {
    extern "Rust" {
        fn init();
        fn draw_ui();
    }
}
