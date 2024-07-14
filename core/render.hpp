#ifndef RENDER_HPP_HANIM
#define RENDER_HPP_HANIM

#include <vector>
#include <memory>
#include <functional>
#include <thread>
#include <chrono>

#include <gl_interface.h>

#include "core/framework/object.hpp"

namespace hanim {

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    gli_viewport(0, 0, width, height);
}

class Render {

    friend class HEngine;

    using BuffHandler = std::function<void (std::vector<unsigned char> &)>;
    using RenderHandler = std::function<void ()>;

public: // render
    static void start() {
        gli_clear(0, 0, 0, 1);
        //gli_coordinate();
    }

    static void render_cobject(/* const */ HObject &obj) {

        GraphicsDataGLI data;

        auto objs = obj.getObjs();

        // TODO: optimize - batch render
        for (auto obj : objs) {
            if (obj->get_opacity() == 0 && obj->get_fill_opacity() == 0) {
                continue;
            }

            auto dataPtr = obj->_get_data();
            data.vertexNums = dataPtr->points.size();
            data.vertices = (float *) dataPtr->points.data();
            bool needFilled = dataPtr->fillRgbs[3] > 0;

            // fill
            if (needFilled) {
                data.colors = &(dataPtr->fillRgbs[0]);
                data.mode.draw = GLI_TRIANGLE_FAN;
                data.mode.color = GLI_COL_ONE;
                data.filled = GLI_TRUE;

                gli_draw(&data);
            }

            // border
            data.colors = (float *) dataPtr->rgbs.data();
            data.thickness = dataPtr->thickness;

            // TODO: double draw for line - two points
            DrawMode mode = dataPtr->drawMode;
            data.mode.draw = mode != GLI_NONE ? mode : GLI_LINE_LOOP;
            data.mode.color = GLI_COL_MULTI;
            data.filled = GLI_FALSE;

            gli_draw(&data);
        }
    }

    static void read_buff(std::vector<unsigned char> &buff) {
        int width, height;
        glfwGetFramebufferSize(Instance().mWindow, &width, &height);
        buff.resize(width * height * 4);
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buff.data());
    }

    static void end() {
        if (Instance().mRenderHandler) {
            Instance().mRenderHandler();
        }
        if (Instance().mWindowVisible) {
            glfwSwapBuffers(Instance().mWindow);
        }
        glfwPollEvents();
        call_buff_handler();
    }

    static void call_buff_handler() {
        if (nullptr != Instance().mBuffHandler) {
            static std::vector<unsigned char> buff;
            read_buff(buff);
            Instance().mBuffHandler(buff);
        }
    }

public: // config

    static void set_buff_handler(BuffHandler bHandler) {
        Instance().mBuffHandler = bHandler;
    }

public:
    static void init(int w, int h, bool window) {

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        if (window) {
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        } else {
            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        }

        Instance().mWindow = glfwCreateWindow(w, h, "Hanim Engine", NULL, NULL);
        if (Instance().mWindow == NULL) {
            HONLY_LOGE("Failed to create GLFW window\n");
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(Instance().mWindow);
        glfwSetFramebufferSizeCallback(Instance().mWindow, framebuffer_size_callback);

        // gl interface backend init
        gli_backend_init((void *)glfwGetProcAddress);
        gli_viewport(0, 0, w, h);
        gli_camera_pos(0, 3, 0);
        gli_2d(5, 0, 0);

        Instance().mWindowVisible = window;
    }

    static void deinit() {
        if (Instance().mWindowVisible) {
            while (!glfwWindowShouldClose(Instance().mWindow)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                glfwPollEvents();
            }
        }
        gli_backend_deinit();
        glfwTerminate();
    }

private:
    bool mWindowVisible;
    GLFWwindow *mWindow;
    BuffHandler mBuffHandler;
    RenderHandler mRenderHandler;

    Render() : mWindowVisible { false }, mWindow { nullptr }, mBuffHandler { nullptr }  {
        mRenderHandler = nullptr;
    }
    Render(const Render &r) = delete;
    Render & operator=(const Render &r) = delete;

    static Render & Instance() {
        static Render r;
        return r;
    }
};

};

#endif