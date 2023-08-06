use std::ffi::CString;

use raylib_ffi::*;

fn draw() {
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

#[cxx::bridge(namespace = "rustgui")]
mod ffi {
    extern "Rust" {
        fn draw();
    }
}
