fn rustdraw() {
    println!("Hello from rust!");
}

#[cxx::bridge(namespace = "irongui")]
mod ffi {
    extern "Rust" {
        fn rustdraw();
    }
}
