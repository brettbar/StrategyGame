use crate::components::{element::Draw, panel, text_label};
use raylib_ffi::*;

pub fn create() -> Box<dyn Draw> {
    panel::new(
        "main_menu".to_string(),
        globals::colors::BLUE,
        Rectangle {
            x: 0.,
            y: 0.,
            width: 50.,
            height: 50.,
        },
        vec![text_label::new(
            "test".to_string(),
            globals::colors::BLACK,
            Rectangle {
                x: 0.,
                y: 0.,
                width: 50.,
                height: 50.,
            },
            "test123".to_string(),
            24,
            globals::color::WHITE,
        )],
    )
}
