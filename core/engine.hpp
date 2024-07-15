#ifndef HENGINE_HPP_HANIM
#define HENGINE_HPP_HANIM

#include <string>
#include <thread>
#include <chrono>

#include <third-party/third_party_header.hpp>

// core
#include "core/scene.hpp"
#include "core/render.hpp"
#include "core/recorder.hpp"

namespace hanim {

class HEngine {

public: // im-mode

    template <typename AnimType>
    static void play(AnimType &&anim, int frameNumber = 60) {
        Instance().mScene.play(anim, frameNumber);
    }

public:
    static void render(Scene &scene) {
        Instance()._render(scene);
    }

    static void render(Scene &scene, int fNumber) {
        Render::start();
        scene.render_target_frame(fNumber);
        Render::end();
    }

    static void render(Scene &&scene) {
        Instance()._render(scene);
    }

    static void set_render_handler(Render::RenderHandler handler) {
        Render::Instance().mRenderHandler = handler;
    }

    static GLFWwindow * get_render_window() {
        Instance(); // engine init
        return Render::Instance().mWindow;
    }

public: // config

    static void default_config1() {
        _config().window = false;
        _config().wWidth = 854;
        _config().wHeight = 480;
        _config().recorder = true;
        _config().rFps = 60;
    }

    static void default_config2() {
        _config().window = false;
        _config().wWidth = 1920;
        _config().wHeight = 1080;
        _config().recorder = true;
        _config().rFps = 60;
    }

    static void default_config3() {
        default_config2();
        _config().rLossless = true;
    }

public: // recorder
    static void set_recorder(bool enable) {
        _config().recorder = enable;
    }

    static void recorder_fps(int fps) {
        _config().rFps = fps;
    }

    static void recorder_lossless(bool enable) {
        _config().rLossless = enable;
    }

    static void recorder_file_name(std::string name) {
        _config().rName = name;
    }

    static void recorder_repeat_write(int frameNumber = 60) {
        Instance().mRecorder.repeat_write(frameNumber);
    }

    static void save_frame_to_img(std::string name) {
        Instance().mRecorder.save_frame_to_img(name);
    }

public: // window
    static void set_window_size(int w, int h) {
        _config().wWidth = w;
        _config().wHeight = h;
    }

    static void set_window(bool enable) {
        _config().window = enable;
    }

private:
    static HEngine & Instance() {
        static HEngine engine;
        return engine;
    }

private:
    Scene mScene;
    Recorder mRecorder;

    HEngine() {
        HONLY_LOGP("init hengine...");
        _config().start_time = std::chrono::steady_clock::now();
        
        HONLY_LOGP("init render...");
        Render::init(_config().wWidth, _config().wHeight, _config().window);

        if (_config().recorder) {
            HONLY_LOGP("init recorder...");
            mRecorder.set_name(_config().rName)
                .set_fps(_config().rFps)
                .set_size(_config().wWidth, _config().wHeight)
                .set_lossless(_config().rLossless)
                .start();
            Render::set_buff_handler([&](auto buff){
                mRecorder.write(buff);
            });
        }
    }

    HEngine(const HEngine &) = delete;
    HEngine(HEngine &&) = delete;

    ~HEngine() {
        Render::deinit();
        if (_config().recorder) mRecorder.stop();
        const auto end = std::chrono::steady_clock::now();
        const std::chrono::duration<double> diff = end - _config().start_time;
        HONLY_LOGP("engine time: %ldms", std::chrono::duration_cast<std::chrono::milliseconds>(diff).count());
    }

private:
    int _render(Scene &scene) {
        scene.timeline();
        return 0;
    }

private:
    struct Config { // Global Config
        // window
        bool window;
        int wWidth, wHeight;
        bool recorder;
        int rFps;
        bool rLossless;
        std::string rName;
        std::chrono::time_point<std::chrono::steady_clock> start_time;
    };

    static Config & _config() {
        static Config config {
            true, 854, 480, // window
            false, 60, false, "hanim"// recorder
            // stime,
        };
        return config;
    }
};

}

#endif