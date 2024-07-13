import glfw
import sys, os

sys.path.append(os.getcwd())

#print(sys.path)

from api.python.gl_interface import *

def main():
    if not glfw.init():
        return

    window = glfw.create_window(800, 600, "GLI-OPENGL-PY Demo", None, None)
    if not window:
        glfw.terminate()
        return

    glfw.make_context_current(window)

    gli_backend_init(glfw._glfw.glfwGetProcAddress)
    gli_viewport(0, 0, 800, 600)

    while not glfw.window_should_close(window):
        glfw.poll_events()

        gli_clear()

        gli_coordinate()

        p1 = PointGLI(0, 1)
        p2 = PointGLI(1, 0)

        gli_line(PointGLI(0, 0), PointGLI(1, 1))
        #gli_rectangle(p1, p2)
        gli_circle(PointGLI(0, 0), 2)

        glfw.swap_buffers(window)

    gli_backend_deinit()
    glfw.terminate()

if __name__ == "__main__":
    main()