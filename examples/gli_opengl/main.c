
#include <stdio.h>
#include <GLFW/glfw3.h>

#include "gl_interface.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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

    GLFWwindow* window = glfwCreateWindow(800, 600, "Line Drawing", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // gl interface backend init
    gli_backend_init(glfwGetProcAddress);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        struct PointGLI p1, p2;
        static int i = 0;

        p1.x = p1.y = -0.5f;
        p1.z = 0;

        p2.x = p2.y = 0.5f;
        p2.z = 0;

        gli_rectange_2d(p1, p2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    gli_backend_deinit();

    glfwTerminate();

    return 0;
}