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

    GLFWwindow* window = glfwCreateWindow(800, 600, "GLI Demos - GaoKao Fly", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // gl interface backend init
    gli_backend_init((void *)glfwGetProcAddress);
    gli_viewport(0, 0, 800, 600);
    gli_camera_pos(0, 3, 0);

    ColorGLI fly_col1 = {0.58f, 0.0f, 0.83f, 1.0f}; // violet
    ColorGLI fly_col2 = {1.0f, 0.5f, 0.31f, 1.0f}; // coral
    ColorGLI fly_col3 = {0.6f, 1.0f, 0.6f, 1.0f}; // mintGreen

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        gli_clear(0, 0, 0, 1);

        gli_camera_rotation(2);
        gli_camera_update();

        gli_coordinate();
        gli_space();

        // 高
        gli_point(GLI_POSITION.UF, GLI_COLORS.RED, 10);

        gli_line(gli_pos_obj(-0.65, 0.9, 1), gli_pos_obj(0.65, 0.9, 1), GLI_COLORS.RED, 10);
        gli_rectangle(gli_pos_obj(-0.4, 0.75, 1), gli_pos_obj(0.4, 0.4, 1), GLI_COLORS.RED, 10);

        PointGLI line_4[] = {
            gli_pos_obj(-0.8, -1, 1), gli_pos_obj(-0.8, 0.2, 1),
            gli_pos_obj(0.8, 0.2, 1), gli_pos_obj(0.8, -1, 1),
        };
        gli_line_strip(line_4, 4, GLI_COLORS.RED, 10);

        gli_rectangle(gli_pos_obj(-0.6, 0, 1), gli_pos_obj(0.6, -0.8, 1), GLI_COLORS.RED, 10);

        // 考
        gli_line(gli_pos_obj(1, 1, 0), gli_pos_obj(1, 0.3, 0), GLI_COLORS.GREEN, 10);
        gli_line(gli_pos_obj(1, 0.7, 0.3), gli_pos_obj(1, 0.7, -0.3), GLI_COLORS.GREEN, 10);
        gli_line(gli_pos_obj(1, 0.3, 0.7), gli_pos_obj(1, 0.3, -0.7), GLI_COLORS.GREEN, 10);

        gli_line(
            gli_pos_obj(1, 0.7, -0.7),
            gli_pos_obj(1, -0.1, 0.6),
            GLI_COLORS.GREEN, 10
        );

        gli_line(gli_pos_obj(1, -0.2, 0.2), gli_pos_obj(1, -0.2, -0.5), GLI_COLORS.GREEN, 10);

        PointGLI kao_line_4[] = {
            gli_pos_obj(1, 0, 0.2), gli_pos_obj(1, -0.5, 0.2),
            gli_pos_obj(1, -0.5, -0.5), gli_pos_obj(1, -1, -0.5),
        };
        gli_line_strip(kao_line_4, 4, GLI_COLORS.GREEN, 10);

        gli_line(gli_pos_obj(1, -0.8, 0.2), gli_pos_obj(1, -0.8, -0.5), GLI_COLORS.GREEN, 10);

        // 加
        gli_line(gli_pos_obj(0.5, 1, -1), gli_pos_obj(0.6, -1, -1), GLI_COLORS.BLUE, 10);

        PointGLI jia_line_3[] = {
            gli_pos_obj(1, 0.2, -1), gli_pos_obj(0, 0.2, -1),
            gli_pos_obj(0.1, -1, -1),
        };
        gli_line_strip(jia_line_3, 3, GLI_COLORS.BLUE, 10);

        gli_rectangle(
            gli_pos_obj(-0.3, 0, -1), gli_pos_obj(-0.7, -0.8, -1),
            GLI_COLORS.BLUE, 10
        );

        // 油
        gli_line(GLI_POSITION.LUB, gli_pos_obj(-1, 0.7, -0.7), fly_col1, 10);
        gli_line(gli_pos_obj(-1, 0, -1), gli_pos_obj(-1, 0, -0.7), fly_col2, 10);
        gli_line(GLI_POSITION.LDB, gli_pos_obj(-1, -0.7, -0.7), fly_col3, 10);

        gli_rectangle_base(
            GLI_POSITION.LEFT, GLI_POSITION.LDF,
            GLI_COLORS.RED, GLI_COLORS.GREEN, GLI_COLORS.BLUE, GLI_COLORS.PURPLE,
            0, 10
        );

        gli_line(
            gli_pos_obj(-1, 0.5, 0.5),
            gli_pos_obj(-1, -1, 0.5),
            GLI_COLORS.BLACK, 10
        );

        gli_line(
            gli_pos_obj(-1, -0.5, 0),
            gli_pos_obj(-1, -0.5, 1),
            GLI_COLORS.WHITE, 10
        );

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    gli_backend_deinit();

    glfwTerminate();

    return 0;
}