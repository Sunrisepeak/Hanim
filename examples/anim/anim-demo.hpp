// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/Hanim
//


#ifndef __ANIM_DEMO_HPP__HANIM
#define __ANIM_DEMO_HPP__HANIM

#include <cmath>

#include <Hanim.hpp>
#include <HanimDemoComm.hpp>

#define HOBJECT_OPENGL
#include <HanimObject.hpp>

namespace hanim {
namespace demo {

static hanim::HAnimate::Status move() {
    static auto move = hanim::move(50, 200, 350, 200, 30);
    static auto hobj = hanim::object::opengl::Button();

    hanim::HEngine::PlayFrame(move, hobj);

    return move.status();
}

static hanim::HAnimate::Status scale() {
    static auto scale = hanim::scale(100, 100, 450, 450, 30);
    static auto hobj = hanim::object::opengl::Button();

    hanim::HEngine::PlayFrame(scale, hobj);

    return scale.status();
}

static hanim::HAnimate::Status alpha() {
    static auto alpha = hanim::alpha(0, 255, 30);
    static auto hobj = hanim::object::opengl::Button();
    static bool init = true;

    if (init) {
        hobj.setPos(100, 100);
        hobj.setSize(300, 300);
        init = false;
    }

    hanim::HEngine::PlayFrame(alpha, hobj);

    return alpha.status();
}

static hanim::HAnimate::Status gradient() {
    static auto gradient = hanim::gradient(255, 0, 0, 0, 0, 255, 30);
    static auto hobj = hanim::object::opengl::Button();
    static bool init = true;

    if (init) {
        hobj.setPos(100, 100);
        hobj.setSize(300, 300);
        init = false;
    }

    hanim::HEngine::PlayFrame(gradient, hobj);

    return gradient.status();
}

static hanim::HAnimate::Status rotation() {
    static auto rotation = hanim::rotation(250, 250, 0, 360, 30);
    static auto hobj1 = hanim::object::opengl::Button();
    static auto hobj2 = hanim::object::opengl::Button();
    static bool init = true;

    if (init) {
        hobj1.setPos(100, 100);
        hobj1.setSize(300, 300);
        hobj1.setColor(255, 0, 0);
        hobj1.setAlpha(50);

        hobj2.setPos(100, 100);
        hobj2.setSize(50, 50);
        init = false;
    }

    hanim::HEngine::PlayFrame(hobj1); // only render
    hanim::HEngine::PlayFrame(rotation, hobj2);

    return rotation.status();
}

static hanim::HAnimate::Status path() {
    static auto path = hanim::path<hanim::InterpolationAnim::Var::X>(
        0, 360, // x from 0 to 360
        [](float x) {
            float radian = x * HANIM_PI / 180;
            return 200 + 100 * std::sin(radian * 2);
        },
        30
    );
    static auto hobj = hanim::object::opengl::Button();

    hanim::HEngine::PlayFrame(path, hobj);

    return path.status();
}

static hanim::HAnimate::Status pathKF() {
    static auto path = hanim::path(
        { // path key frame
            {0, 0},
            {100, 200},
            {300, 300},
            {300, 400},
            {500, 400}
        },
        30
    );
    static auto hobj = hanim::object::opengl::Button();

    hanim::HEngine::PlayFrame(path, hobj);

    return path.status();
}

/* compose anim */

static hanim::HAnimate::Status fadeIn() {
    static auto fadeIn = hanim::animate::FadeIn(0, 200, 300, 200);
    static auto hobj = hanim::object::opengl::Button();

    hanim::HEngine::PlayFrame(fadeIn, hobj);

    return fadeIn.status();
}

static hanim::HAnimate::Status fadeOut() {
    static auto fadeOut = hanim::animate::FadeOut(200, 200, 400, 200);
    static auto hobj = hanim::object::opengl::Button();

    hanim::HEngine::PlayFrame(fadeOut, hobj);

    return fadeOut.status();
}

static hanim::HAnimate::Status focus() {
    static auto focus = hanim::animate::Focus();
    static auto hobj = hanim::object::opengl::Button();
    static bool init = true;

    if (init) {
        hobj.setPos(150, 150);
        hobj.setSize(200, 200);
        hobj.setAlpha(100);
        init = false;
    }

    hanim::HEngine::PlayFrame(focus, hobj);

    return focus.status();
}

static hanim::HAnimate::Status image() {

    static auto anim = hanim::FrameAnim();
    static auto hobj = hanim::object::opengl::ImageView<500, 500>();
    static bool init = true;

    if (init) {
        auto imageGenerator = [](int bufferWidth, int bufferHeight) {
            std::vector<unsigned char> buffer(bufferWidth * bufferHeight * 4, 0);

            static unsigned char rgba[] = { 0, 0, 0, 0 };

            for (int y = 0; y < bufferHeight; ++y) {
                for (int x = 0; x < bufferWidth; ++x) {
                    int pixelIndex = (y * bufferWidth + x) * 4;
                    buffer[pixelIndex + 0] = rgba[0];
                    buffer[pixelIndex + 1] = rgba[1];
                    buffer[pixelIndex + 2] = rgba[2];
                    buffer[pixelIndex + 3] = rgba[3];
                }
                rgba[0] += 1; rgba[1] += 2; rgba[2] += 3; rgba[3] += 4;
            }

            return buffer;
        };

        for (int i = 0; i < 500; i++) {
            hanim::FAFrame frame(
                imageGenerator(500, 500),
                500, 500
            );
            anim.addFAFrame(frame);
        }

        anim.start();
        anim.setFrameNums(500);

        init = false;
    }

    hanim::HEngine::PlayFrame(anim, hobj);

    return anim.status();

}

}
}

#endif