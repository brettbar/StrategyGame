use raylib_ffi::*;

struct Margins {
    l: u32,
    r: u32,
    t: u32,
    b: u32,
}

pub trait Draw {
    fn enable(&mut self);
    fn disable(&mut self);
    fn draw(&self);
}

pub struct Element {
    id: String,
    pub enabled: bool,
    background: Color,
    pub transform: Rectangle,
    margins: Margins,
}
impl Element {
    pub fn new(id: String, background: Color, transform: Rectangle) -> Self {
        Element {
            id,
            enabled: false,
            background,
            transform,
            margins: Margins {
                l: 0,
                r: 0,
                t: 0,
                b: 0,
            },
        }
    }
}

impl Draw for Element {
    fn enable(&mut self) {
        self.enabled = true
    }

    fn disable(&mut self) {
        self.enabled = false
    }

    fn draw(&self) {
        if self.enabled {
            unsafe {
                DrawRectangleRec(self.transform, self.background);
            }
        }
    }
}
