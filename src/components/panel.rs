use crate::components::element::*;
use raylib_ffi::*;

pub struct Panel {
    elem: Element,
    children: Vec<Box<dyn Draw>>,
}

pub fn new(
    id: String,
    background: Color,
    transform: Rectangle,
    children: Vec<Box<dyn Draw>>,
) -> Box<Panel> {
    Box::new(Panel {
        elem: Element::new(id, background, transform),
        children,
    })
}

impl Draw for Panel {
    fn enable(&mut self) {
        self.elem.enable();
        for child in self.children.iter_mut() {
            child.enable();
        }
    }

    fn disable(&mut self) {
        for child in self.children.iter_mut() {
            child.disable();
        }
        self.elem.disable();
    }

    fn draw(&self) {
        if self.elem.enabled {
            self.elem.draw();
            for child in self.children.iter() {
                child.draw();
            }
        }
    }
}
