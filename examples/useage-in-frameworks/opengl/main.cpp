// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/HAnim
//



#if defined(_WIN64) || defined(_WIN32)
#include <windows.h> // to avoid gl.h win-type issue
#endif

#include <GLFW/glfw3.h>

#include <Hanim.hpp>
#include <HanimAnimate.hpp>

#define HOBJECT_OPENGL
#include <HanimObject.hpp>

#ifdef HANIM_ENABLE_XRECORDER_OPENGL
#include <XRecorder/OpenGLRecorder.hpp>
#endif

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(300, 300, "HAnim: Demo", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    /*
      (0,0) set gl-matrix
        +---->
        |
        V
    */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 300, 300, 0, -1, 1);

    // hanim1: create animate
    auto bAnim = hanim::animate::BreathingAnim(0, 0, 255);
    bAnim.setFrameNums(200);

    auto button = hanim::object::opengl::Button();
    button.setPos(100, 100);
    button.setSize(100, 100);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        /*
        // hanim2: execute/play animation on HObject
        hanim::HEngine::PlayFrame(bAnim, hanim::HObject([](int type, const hanim::IAFrame &frame) {
            glColor3f(frame.data[0] / 255, frame.data[1] / 255, frame.data[2] / 255);

            glBegin(GL_TRIANGLES);
            glVertex2f(-0.5f, -0.5f);
            glVertex2f(0.5f, -0.5f);
            glVertex2f(0.0f, 0.5f);
            glEnd();

        }));
        */

        hanim::HEngine::PlayFrame(bAnim, button);

    #ifdef HANIM_ENABLE_XRECORDER_OPENGL
        static xrecorder::OpenGLRecorder<1920,1080> xr("hanim-demo-opengl");
        xr.captureFrameData();
        xr.saveToVideo();
        //xr.saveToImg();
    #endif

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}