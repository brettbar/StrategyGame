use crate::components::{element::Draw, panel, text_label};
use raylib_ffi::*;

use raylib_ffi::Color;

const BLACK: Color = Color {
    r: 0,
    g: 0,
    b: 0,
    a: 255,
};
const WHITE: Color = Color {
    r: 255,
    g: 255,
    b: 255,
    a: 255,
};
const BLUE: Color = Color {
    r: 0,
    g: 0,
    b: 255,
    a: 255,
};

pub fn create() -> Box<dyn Draw> {
    panel::new(
        "main_menu".to_string(),
        BLUE,
        Rectangle {
            x: 0.,
            y: 0.,
            width: 50.,
            height: 50.,
        },
        vec![text_label::new(
            "test".to_string(),
            BLACK,
            Rectangle {
                x: 0.,
                y: 0.,
                width: 50.,
                height: 50.,
            },
            "test123".to_string(),
            24,
            WHITE,
        )],
    )
}
