// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/HAnim
//

// ffmpeg -i hanim-demo.xx.mkv -s 200x200 hanim-demo.xx.gif

#if defined(_WIN64) || defined(_WIN32)
#include <Windows.h> // to avoid gl.h type issue on window-platform
#endif

#include <GLFW/glfw3.h>

#include <anim-demo.hpp>

#include <XRecorder/OpenGLRecorder.hpp>

int main() {
    if (!glfwInit()) {
        return -1;
    }

    int w = 500, h = 500;

    GLFWwindow* window = glfwCreateWindow(w, h, "Hanim: Demo", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // support alpha
    glEnable( GL_BLEND );


    static xrecorder::OpenGLRecorder<500, 500, 30> xr("hanim-demo");

    while (true) {
        glClear(GL_COLOR_BUFFER_BIT);

        // anim demo
        //if (hanim::demo::move() == hanim::HAnimate::Status::Finished) break;
        //if (hanim::demo::scale() == hanim::HAnimate::Status::Finished) break;
        //if (hanim::demo::alpha() == hanim::HAnimate::Status::Finished) break;
        //if (hanim::demo::gradient() == hanim::HAnimate::Status::Finished) break;
        //if (hanim::demo::path() == hanim::HAnimate::Status::Finished) break;
        //if (hanim::demo::pathKF() == hanim::HAnimate::Status::Finished) break;
        //if (hanim::demo::rotation() == hanim::HAnimate::Status::Finished) break;

        // compose anim
        //if (hanim::demo::fadeIn() == hanim::HAnimate::Status::Finished) break;
        //if (hanim::demo::fadeOut() == hanim::HAnimate::Status::Finished) break;
        if (hanim::demo::focus() == hanim::HAnimate::Status::Finished) break;


        xr.captureFrameData();
        xr.saveToVideo();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}