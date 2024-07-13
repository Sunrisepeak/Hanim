#ifndef CAMERA_H_HANIM
#define CAMERA_H_HANIM

#include "core/framework.hpp"

namespace hanim {

struct Camera2D {

    struct Config {
        float x, y;
        float size;
    };

    static void target(float x, float y) {
        config().x = x;
        config().y = y;
        HONLY_LOGD("camera2d: %f %f %f", config().size, x, y);
        gli_2d(config().size, x, y);
    }

    static void size(float size) {
        config().size = size;
        HONLY_LOGD("camera2d: %f %f %f", config().size, config().x, config().y);
        gli_2d(size, config().x, config().y);
    }

    static Config & config() {
        static Config c { 0, 0, 5 };
        return c;
    }
};

};

#endif