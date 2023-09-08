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

constexpr int DEFAULT_FRAME_NUM = 60;

struct Focus : public ComposeAnim {
    Focus() {
        ComposeAnim::scale(1, 1.1)
            .setEasingCurve(hanim::EasingCurve::OUT_ELASTIC)
            .setFrameNums(DEFAULT_FRAME_NUM);

        ComposeAnim::alpha(200, 255)
            .setEasingCurve(hanim::EasingCurve::ECType::IN_SIN)
            .setFrameNums(DEFAULT_FRAME_NUM);

        this->setFrameNums(DEFAULT_FRAME_NUM);
    }
};

struct BreathingAnim : public HAnimate {
    BreathingAnim(float r, float g, float b) {
        addAnim(hanim::gradient(r, g, b, r, g, b, DEFAULT_FRAME_NUM));

        addAnim(hanim::alpha(0, 255, DEFAULT_FRAME_NUM / 2), 0);
        addAnim(hanim::alpha(255, 0, DEFAULT_FRAME_NUM / 2));
        setFrameTrackIndex( DEFAULT_FRAME_NUM / 2);

        setPlayType(PlayType::RT);
        setFrameNums(DEFAULT_FRAME_NUM);

        start();
    }
};

struct FadeIn : public ComposeAnim {
    FadeIn(int x1, int y1, int x2, int y2) {
        ComposeAnim::move(x1, y1, x2, y2)
            .setEasingCurve(hanim::EasingCurve::ECType::IN_QUAD)
            .setFrameNums(DEFAULT_FRAME_NUM);

        ComposeAnim::alpha(0, 255)
            .setEasingCurve(hanim::EasingCurve::ECType::IN_CUBIC)
            .setFrameNums(DEFAULT_FRAME_NUM);

        setFrameNums(DEFAULT_FRAME_NUM);
    }
};

struct FadeOut : public ComposeAnim {
    FadeOut(int x1, int y1, int x2, int y2) {
        ComposeAnim::move(x1, y1, x2, y2)
            .setEasingCurve(hanim::EasingCurve::ECType::OUT_QUAD)
            .setFrameNums(DEFAULT_FRAME_NUM);

        ComposeAnim::alpha(255, 0)
            .setEasingCurve(hanim::EasingCurve::ECType::OUT_CUBIC)
            .setFrameNums(DEFAULT_FRAME_NUM);

        setFrameNums(DEFAULT_FRAME_NUM);
    }
};

} // animate namespace end
} // hanim namespace end

#endif