// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/HAnim
//


#ifndef __ANIMATE_HPP__HANIM
#define __ANIMATE_HPP__HANIM

#include <Hanim.hpp>
// std
#include <memory>
#include <vector>

namespace hanim {
namespace animate {
class Focus : public ComposeAnim {
public:
    Focus(int frameNumbers = 60) {
        ComposeAnim::scale(1, 1.1)
            .setEasingCurve(hanim::EasingCurve::OUT_ELASTIC)
            .setFrameNums(frameNumbers);

        ComposeAnim::alpha(200, 255)
            .setEasingCurve(hanim::EasingCurve::ECType::IN_SIN)
            .setFrameNums(frameNumbers);
        
        this->setFrameNums(frameNumbers);
    }
};

class BreathingAnim : public HAnimate {
public:
    BreathingAnim(float r, float g, float b, float _a = 0.5) {

        addAnim(hanim::gradient(r * _a, g * _a, b * _a, r, g, b, 30))
            .setPlayType(PlayType::RT);

        setPlayType(PlayType::RT);
    }
};

struct FadeIn : public ComposeAnim {

    FadeIn(
        int x, int y,
        int frameNumbers = 60,
        std::function<HAnimate&& ()> pathAnimGenFunc = nullptr
    ) {
        if (pathAnimGenFunc) {
            addAnim(pathAnimGenFunc());
        } else {
            ComposeAnim::move(0, y, x, y)
                .setEasingCurve(hanim::EasingCurve::ECType::IN_CUBIC)
                .setFrameNums(frameNumbers);
        }

        ComposeAnim::alpha(0, 255)
            .setEasingCurve(hanim::EasingCurve::ECType::IN_CUBIC)
            .setFrameNums(frameNumbers);

        setFrameNums(frameNumbers);
    }

};

struct FadeOut : public ComposeAnim {

    FadeOut(
        int x, int y,
        int frameNumbers = 60,
        std::function<HAnimate&& ()> pathAnimGenFunc = nullptr
    ) {
        if (pathAnimGenFunc) {
            addAnim(pathAnimGenFunc());
        } else {
            ComposeAnim::move(x, y, x * 2, y)
                .setEasingCurve(hanim::EasingCurve::ECType::IN_CUBIC)
                .setFrameNums(frameNumbers);
        }

        ComposeAnim::alpha(255, 0)
            .setEasingCurve(hanim::EasingCurve::ECType::IN_CUBIC)
            .setFrameNums(frameNumbers);

        setFrameNums(frameNumbers);
    }

};

} // animate namespace end
} // hanim namespace end

#endif