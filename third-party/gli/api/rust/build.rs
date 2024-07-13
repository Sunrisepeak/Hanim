//use std::env;
use std::path::PathBuf;

fn main() {

    // enable gli api symbol for interface
    let gli_macro_define = "-DGLI_BUILD_LIB";
    let header = "../../gl_interface.h";

    // Tell cargo to look for shared libraries in the specified directory
    //let current_dir = env::current_dir().expect("Failed to get current directory");
    //println!("cargo:rustc-link-search={}../../build/linux/x86_64/release/", current_dir.display());

    // Tell cargo to tell rustc to link the system bzip2
    // shared library.
    println!("cargo:rustc-link-lib=gli-opengl");

    println!("cargo:rerun-if-changed={}", header);

    // The bindgen::Builder is the main entry point
    // to bindgen, and lets you build up options for
    // the resulting bindings.
    let bindings = bindgen::Builder::default()
        // The input header we would like to generate
        // bindings for.
        .header(header)
        .clang_arg(gli_macro_define) // control api gen
        // Tell cargo to invalidate the built crate whenever any of the
        // included header files changed.
        .parse_callbacks(Box::new(bindgen::CargoCallbacks))
        // Finish the builder and generate the bindings.
        .generate()
        // Unwrap the Result and panic on failure.
        .expect("Unable to generate bindings");

    let out_path = PathBuf::from("src");
    bindings
        .write_to_file(out_path.join("lib.rs"))
        .expect("Couldn't write bindings!");
}
