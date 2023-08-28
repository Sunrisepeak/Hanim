// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/HAnim
//


#ifndef __HANIM_OBJECT_IMGUI_HPP__HANIM
#define __HANIM_OBJECT_IMGUI_HPP__HANIM

#include <Hanim.hpp>

#include <imgui.h>

namespace hanim {
namespace object {
namespace imgui {

class Button : public hanim::HObjectTemplate {
public:
    enum Status {
        SNULL,
        Hovered,
        Active,
    };

public:
    Button(std::string name) : HObjectTemplate(), __mName { name }, __mStatus { SNULL }  {
        //_mR = _mG = _mB = 255 / 255.f;
    }

public:
    Status status() const {
        return __mStatus;
    }

protected: // interface impl
    void _render() override {
        ImGui::SetCursorPosX(_mX);
        ImGui::SetCursorPosY(_mY);

        bool colorConfig = (_mR >= 0 && _mG >= 0 && _mR >= 0);
        if (colorConfig) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(_mR / 255, _mG / 255, _mB / 255, 0.3));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(_mR / 255, _mG / 255, _mB / 255, 0.6));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(_mR / 255, _mG / 255, _mB / 255, 0.9));
        }

        if (_mW > 0 && _mH > 0) ImGui::Button(__mName.c_str(), {_mW, _mH});
        else ImGui::Button(__mName.c_str());

        if (colorConfig) ImGui::PopStyleColor(3);

        if (ImGui::IsItemHovered())
            __mStatus = Hovered;
        else
            __mStatus = SNULL;
    }

private:
    std::string __mName;
    Status __mStatus;
};

} // imgui
} // object
} // hanim

#endif