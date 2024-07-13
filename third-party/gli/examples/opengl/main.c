#include <stdio.h>
#include <math.h>

#include <GLFW/glfw3.h>

#include "gl_interface.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    gli_viewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "GLI-OpenGL-C Demo", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // gl interface backend init
    gli_backend_init(glfwGetProcAddress);
    gli_viewport(0, 0, 800, 600);

    gli_camera_pos(0, 5, 5);
    gli_camera_update();

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        gli_clear(0.2, 0.1, 0.3, 0.5);

        gli_camera_rotation(2);

        gli_coordinate();
        gli_space();

        //gli_point(GLI_POSITION.ORIGIN, GLI_COLORS.ORANGE, 5);
        //gli_line(GLI_POSITION.ORIGIN, GLI_POSITION.RUF, GLI_COLORS.WHITE, 1);
        //gli_rectangle(gli_pos_obj(0, 0, 1), gli_pos_obj(1, 1, 0), GLI_COLORS.PURPLE, 1);
/*
        gli_triangle_base(
            GLI_POSITION.FRONT, GLI_COLORS.RED,
            GLI_POSITION.RIGHT, GLI_COLORS.GREEN,
            GLI_POSITION.UP, GLI_COLORS.BLUE,
            GLI_TRUE
        );

        gli_triangle_filled(
            GLI_POSITION.BACK, GLI_POSITION.LEFT, GLI_POSITION.DOWN,
            GLI_COLORS.BLUE
        );

        gli_rectangle_filled(
            GLI_POSITION.FRONT, GLI_POSITION.RIGHT,
            GLI_COLORS.BLUE
        );

        gli_rectangle_filled(
            GLI_POSITION.FRONT, GLI_POSITION.UP,
            GLI_COLORS.GREEN
        );

        gli_rectangle_filled(
            GLI_POSITION.RIGHT, GLI_POSITION.UP,
            GLI_COLORS.RED
        );


        gli_circle(GLI_POSITION.UP, 1, GLI_NORMAL.RU, GLI_COLORS.RED, 36, 1);
        gli_circle_filled(GLI_POSITION.ORIGIN, 1, GLI_NORMAL.RU, GLI_COLORS.GREEN, 8);

*/
        PointGLI controlPos[4] = {
            gli_pos_obj(-1, 0, 0),
            gli_pos_obj(-0.5, 0.5, 1),
            gli_pos_obj(0.5, 0.5, -1),
            gli_pos_obj(1, 0, 0)
        };

        gli_bezier_curve(controlPos, 4, GLI_COLORS.CYAN, 36, 3);

        GLI_2D(3, 0, 0, {
            gli_rectangle(
                gli_pos_obj(1, 1, 0), gli_pos_obj(2, 2, 0),
                GLI_COLORS.ORANGE, 3
            );
        });

        gli_rectangle(
            gli_pos_obj(1, 1, 0), gli_pos_obj(2, 2, 0),
            GLI_COLORS.ORANGE, 3
        );

        GLI_2D(3, 0, 0, {
            gli_line(
                gli_pos_obj(1, 1, 0), gli_pos_obj(2, 2, 0),
                GLI_COLORS.ORANGE, 3
            );
        });

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    gli_backend_deinit();

    glfwTerminate();

    return 0;
}