extern crate glfw;
extern crate gli_rs;
use glfw::{Action, Context, Key};
use gli_rs::{PointGLI, VectorGLI, ColorGLI /* GLI_POSITION, GLI_COLORS , GLI_NORMAL */ };


fn main() {
    let mut glfw = glfw::init(glfw::FAIL_ON_ERRORS).unwrap();

    let (mut window, events) = glfw.create_window(800, 600, "GLI-OPENGL-RUST Demo", glfw::WindowMode::Windowed)
        .expect("Failed to create GLFW window.");

    window.set_key_polling(true);
    window.make_current();

    //gl::load(|e| glfw.get_proc_address_raw(e) as *const std::os::raw::c_void);

    unsafe { gli_rs::gli_backend_init(glfw::ffi::glfwGetProcAddress as *mut ::std::os::raw::c_void) };

    unsafe {
        gli_rs::gli_viewport(0, 0, 800, 600);
        gli_rs::gli_2d(5.0, 0.0, 0.0);
    }

    while !window.should_close() {
        glfw.poll_events();
        for (_, event) in glfw::flush_messages(&events) {
            handle_window_event(&mut window, event);
        }

        unsafe {
            gli_rs::gli_clear(1.0, 1.0, 1.0, 1.0);
            gli_rs::gli_coordinate();
            /* TODO: global var symbol issue
            gli_rs::gli_circle(
                PointGLI {x: 0.0, y: 0.0, z: 0.0}, 2.0, GLI_POSITION.FRONT,
                GLI_COLORS.RED, 36, 2.0
            );
            */
            gli_rs::gli_circle(
                PointGLI {x: 0.0, y: 0.0, z: 0.0}, 2.0,
                VectorGLI {x: 0.0, y: 0.0, z: -1.0},
                ColorGLI {r: 1.0, g: 0.0, b: 0.0, a: 1.0 }, 36, 2.0
            );
        }

        window.swap_buffers();
    }
}

fn handle_window_event(window: &mut glfw::Window, event: glfw::WindowEvent) {
    match event {
        glfw::WindowEvent::Key(Key::Escape, _, Action::Press, _) => {
            window.set_should_close(true)
        }
        _ => {}
    }
}