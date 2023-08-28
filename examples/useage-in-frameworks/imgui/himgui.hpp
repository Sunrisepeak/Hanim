// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/HAnim
//


#ifndef __DEMO_IMGUI_HPP__HANIM
#define __DEMO_IMGUI_HPP__HANIM

#include <Hanim.hpp>
#include <HanimDemoComm.hpp>

#define HOBJECT_IMGUI
#include <HanimObject.hpp>

namespace hanim {
namespace demo {

auto gImguiScaleButton = hanim::object::imgui::Button("Scale Anim");
auto gImguiMoveButton = hanim::object::imgui::Button("Move Anim");
auto gImguiGradientButton = hanim::object::imgui::Button("RGB Anim");
auto gImguiPathButton = hanim::object::imgui::Button("Path-KeyFrame Anim");
auto gImguiFocusButton = hanim::object::imgui::Button("Focus Anim");

static void imgui() {

    ImGui::Begin("Hanim: Scale");
    hanim::HEngine::PlayFrame(gScale, gImguiScaleButton);
    ImGui::End();

    ImGui::Begin("Hanim: Move");
    hanim::HEngine::PlayFrame(gMove, gImguiMoveButton);
    ImGui::End();

    ImGui::Begin("Hanim: Gradient");
    gImguiGradientButton.setSize(100, 100);
    gImguiGradientButton.setPos(100, 100);
    hanim::HEngine::PlayFrame(gGradient, gImguiGradientButton);
    ImGui::End();

    ImGui::SetNextWindowSize(ImVec2(400, 300));
    ImGui::Begin("Hanim: Path Key-Frame");
    hanim::HEngine::PlayFrame(gPathKeyFrame, gImguiPathButton);
    ImGui::End();

    ImGui::SetNextWindowSize(ImVec2(500, 300));
    ImGui::Begin("Hanim: Path");
    hanim::HEngine::PlayFrame(gPath, hanim::HObject(
        [ & ](int type, const hanim::IAFrame &frame) {

            ImDrawList* drawList = ImGui::GetWindowDrawList();

            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 windowSize = ImGui::GetWindowSize();
            
            ImVec2 clipRectMin = windowPos;
            ImVec2 clipRectMax = ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y);

            drawList->PushClipRect(clipRectMin, clipRectMax, true);
            for (float i = 0; i < 400; i++) {
                drawList->AddCircleFilled(ImVec2(windowPos.x + i, windowPos.y + gPathFunc(i)), 1.0f, IM_COL32(0, 100, 255, 255));
            }
            int len = 50 * (400 - frame.data[0]) / 400;
            for (float i = frame.data[0] - len; i > 0 && i < frame.data[0]; i++) {
                drawList->AddCircleFilled(ImVec2(windowPos.x + i, windowPos.y + gPathFunc(i)), 1.0f + 0.06 * (len - frame.data[0] + i), IM_COL32(255, 0, 0, 50));
            }

            drawList->AddCircleFilled(ImVec2(windowPos.x + frame.data[0], windowPos.y + frame.data[1]), 4.0f, IM_COL32(0, 255, 0, 255));

            drawList->PopClipRect();

            switch (type) {
                case hanim::InterpolationAnim::PATH:
                    ImGui::SetCursorPosX({ frame.data[0] });
                    ImGui::SetCursorPosY({ frame.data[1] });
                    ImGui::Button("Path Anim");
                    break;
                default:
                    break;
            }
        }
    ));
    ImGui::End();

    ImGui::Begin("Hanim: Focus");
    hanim::HEngine::PlayFrame(
        gfocusHighlight,
        gImguiFocusButton,
        [&] {
            static bool hovered = false;

            if (gImguiFocusButton.status() == hanim::object::imgui::Button::Hovered) {
                if (hovered != true)
                    gfocusHighlight.start(true); // reset
                hovered = true;
                return hovered;
            }

            gImguiFocusButton.setSize(100, 100);
            gImguiFocusButton.setPos(100, 100);

            return hovered = false;
        }
    );
    ImGui::End();

    ImGui::Begin("Hanim: Rotation");
    hanim::HEngine::PlayFrame(gRotation, hanim::HObject(
        [ & ](int type, const hanim::IAFrame &frame) {
            if (type == hanim::InterpolationAnim::Rotation) {
                ImDrawList* drawList = ImGui::GetWindowDrawList();

                ImVec2 windowPos = ImGui::GetWindowPos();
                ImVec2 windowSize = ImGui::GetWindowSize();

                ImVec2 clipRectMin = windowPos;
                ImVec2 clipRectMax = ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y);
                drawList->PushClipRect(clipRectMin, clipRectMax, true);

                float x = 150, y = 50;
                frame.rotation(x, y);
                ImVec2 startPos {windowPos.x + frame.data[0], windowPos.y + frame.data[1]};
                ImVec2 endPos {windowPos.x + x, windowPos.y + y};

                drawList->AddLine(startPos, endPos, IM_COL32(0, 0, 255, 255));
                drawList->AddCircleFilled(startPos, 4.0f, IM_COL32(0, 255, 0, 255));
                drawList->AddCircleFilled(endPos, 4.0f, IM_COL32(255, 0, 0, 255));

                drawList->PopClipRect();
            }
        }
    ));
    ImGui::End();
}

}
}
#endif