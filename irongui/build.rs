fn main() {
    cxx_build::bridge("src/lib.rs")
        .file("src/bridge.cc")
        .flag_if_supported("-std=c++17")
        .compile("irongui");

    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-changed=src/bridge.cc");
    println!("cargo:rerun-if-changed=include/bridge.h");
}
