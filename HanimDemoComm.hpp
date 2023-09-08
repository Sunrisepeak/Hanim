// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/HAnim
//


#ifndef __DEMO_COMM_HPP__HANIM
#define __DEMO_COMM_HPP__HANIM

#include <Hanim.hpp>
#include <HanimAnimate.hpp>

namespace hanim {
namespace demo {

constexpr int ANIM_DEMO_FRAMES = 300;

auto gScale = hanim::scale(100, 100, 280, 280, ANIM_DEMO_FRAMES);
auto gMove = hanim::move(100, 50, 100, 200, ANIM_DEMO_FRAMES);

auto gPathFunc = [](float x) { return 150 + 50 * std::sin(x / 20); };
auto gPath = hanim::path<hanim::InterpolationAnim::Var::X>(0, 400, gPathFunc, ANIM_DEMO_FRAMES);

auto gPathKeyFrame = hanim::path({}, ANIM_DEMO_FRAMES);
auto gGradient = hanim::gradient(0, 50,200, 0, 200, 20, ANIM_DEMO_FRAMES);
auto gfocusHighlight = hanim::animate::Focus();
auto gRotation = hanim::rotation(150, 150, 0, 360, ANIM_DEMO_FRAMES);

static void init() {

    gScale.setPlayType(hanim::HAnimate::PlayType::RT);

    gMove.setPlayType(hanim::HAnimate::PlayType::Repeat);
    gMove.setEasingCurve(hanim::EasingCurve(hanim::EasingCurve::OUT_ELASTIC));

    gPath.setPlayType(hanim::HAnimate::PlayType::Repeat);
    gPath.setEasingCurve(hanim::EasingCurve::OUT_SIN);

    gPathKeyFrame.addPathKeyFrame(hanim::IAFrame({50, 50}));
    gPathKeyFrame.addPathKeyFrame(hanim::IAFrame({50, 200}));
    gPathKeyFrame.addPathKeyFrame(hanim::IAFrame({200, 200}));
    gPathKeyFrame.addPathKeyFrame(hanim::IAFrame({50, 50}));
    gPathKeyFrame.setPlayType(hanim::HAnimate::PlayType::RT);

    gGradient.setPlayType(hanim::HAnimate::PlayType::RT);

    gRotation.setPlayType(hanim::HAnimate::PlayType::Repeat);
}

}
}

#endif