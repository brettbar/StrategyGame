use crate::components::element::*;
use raylib_ffi::*;
use std::ffi::CString;

pub struct TextLabel {
    elem: Element,
    text: String,
    font_size: i32,
    text_color: Color,
}

pub fn new(
    id: String,
    background: Color,
    transform: Rectangle,
    text: String,
    font_size: i32,
    text_color: Color,
) -> Box<TextLabel> {
    Box::new(TextLabel {
        elem: Element::new(id, background, transform),
        text,
        font_size,
        text_color,
    })
}

impl Draw for TextLabel {
    fn enable(&mut self) {
        self.elem.enable();
    }

    fn disable(&mut self) {
        self.elem.disable();
    }

    fn draw(&self) {
        if self.elem.enabled {
            self.elem.draw();
            unsafe {
                DrawText(
                    CString::new(self.text.as_str()).unwrap().into_raw(),
                    self.elem.transform.x as i32,
                    self.elem.transform.y as i32,
                    self.font_size,
                    self.text_color,
                );
            }
        }
    }
}
