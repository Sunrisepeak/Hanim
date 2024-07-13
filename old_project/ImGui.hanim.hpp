// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/HAnim
//

#ifndef __IMGUI_HANIM_HPP__
#define __IMGUI_HANIM_HPP__

#include <Hanim.hpp>
#include <imgui.h>

#define HPlayWidget(id, AnimatedWidget) \
{ \
    hanim::Begin(id); \
    AnimatedWidget; \
    hanim::End(); \
}

#define ToHVec2(vec2) hanim::HVec2{vec2.x, vec2.y}
#define ToImVec2(vec2) ImVec2{vec2.x, vec2.y}

namespace hanim {

// ImGui DrawList Wrapper
class Canvas {
public:
    void addRectFilled(const HVec2 &p_min, const HVec2 &p_max, ImU32 col, float rounding = 0, ImDrawFlags flags = 0) {
        ImVec2 min = ToImVec2((__mWindowPos + p_min));
        ImVec2 max = ToImVec2((__mWindowPos + p_max));
        __mDrawList->AddRectFilled(
            min, max,
            col, rounding, flags
        );
    }

public:
    Canvas() {
        __mDrawList = ImGui::GetWindowDrawList();
        __mWindowPos = ToHVec2(ImGui::GetWindowPos());

        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 clipRectMin = ImGui::GetWindowPos();
        ImVec2 clipRectMax = ImVec2(clipRectMin.x + windowSize.x, clipRectMin.y + windowSize.y);

        __mDrawList->PushClipRect(clipRectMin, clipRectMax, true);
    }

    Canvas(const Canvas &) = delete;
    Canvas & operator=(const Canvas &) = delete; 

    ~Canvas() {
        __mDrawList->PopClipRect();
    }

private:
    ImDrawList *__mDrawList;
    HVec2 __mWindowPos;
};

static void Begin(unsigned int id) {
    HEngine::AManager::setAnimateId(id);
}

static void End() {
    HEngine::AManager::resetAnimateId();
}

static void Loading(
    const char *text, HVec2 pos, HVec2 wh,
    unsigned int frameNums = 120,
    ImVec4 color = ImVec4(0.9, 0.9, 0.9, 0.5),
    ImVec4 background = ImVec4(0.2, 0.6, 0.9, 1)
) {

    // draw bg
    Canvas().addRectFilled(pos, pos + wh, ImColor(background));

    // draw text
    ImVec2 textSize = ImGui::CalcTextSize(text);
    auto textPos = pos + HVec2{(wh.x - textSize.x) / 2, (wh.y - textSize.y) / 2};
    ImGui::SetCursorPosX(textPos.x);
    ImGui::SetCursorPosY(textPos.y);
    ImGui::Text(text);

    // draw animated block
    float blockWidth = wh.x * 0.2;
    auto anim = HEngine::AManager::registerAnimate<hanim::Move>(
        pos, pos + HVec2{wh.x - blockWidth, 0},
        HAnimate::Config {
            .playType = hanim::HAnimate::PlayType::RT,
            .frameNums = frameNums
        }
    );

    if (auto animPtr = anim.lock()) {
        HEngine::PlayFrame(
            *animPtr,
            [ & ](int type, const hanim::IAFrame &frame) {
                auto pos = HVec2{frame.data[0], frame.data[1]};
                Canvas().addRectFilled(pos, pos + HVec2{blockWidth, wh.y}, ImColor(color));
            }
        );
    }

}

} // namespace hanim

#endif