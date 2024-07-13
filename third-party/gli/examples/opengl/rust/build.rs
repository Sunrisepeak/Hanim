use std::env;

fn main() {
    println!("cargo:rustc-link-search=native=../../../build/linux/x86_64/release");
    println!("cargo:rustc-env=LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../../build/linux/x86_64/release");
    if let Ok(ld_library_path) = env::var("LD_LIBRARY_PATH") {
        println!("LD_LIBRARY_PATH is set to: {}", ld_library_path);
    } else {
        println!("LD_LIBRARY_PATH is not set");
    }
}