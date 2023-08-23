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
class FocusAnim : public ComposeAnim {
public:
    FocusAnim(int frameNumbers = 60) {
        ComposeAnim::scale(1, 1.1)
            .setEasingCurve(hanim::EasingCurve::OUT_ELASTIC)
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

} // animate namespace end
} // hanim namespace end

#endif