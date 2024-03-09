// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/HAnim
//

#ifndef __HANIM_DEMO_HPP__
#define __HANIM_DEMO_HPP__

// Hanim
#include <Hanim.hpp>
#include <ImGui.hanim.hpp>

namespace hanim {

static void loading_example() {
    HPlayWidget(1, hanim::Loading("Loading", {100, 100}, {200, 30}));

    HPlayWidget(
        2,
        hanim::Loading(
            "Hello Hanim", {100, 150}, {200, 30},
            60, ImVec4(1.f, 0, 0, 0.5f), ImVec4(0, 0.5f, 0, 0.8f)
        )
    );
}

static struct DemoManager {
    using DemoTitle = std::string;
    using DemoType = std::function<void ()>;

    DemoTitle currentDemo = "Loading";

    std::map<DemoTitle, DemoType> demoList {
        {"Loading", loading_example}
    };

    void render() {
        demoList[currentDemo]();
    }

} __gDemoManager;

static void DemoControlWindow() {
    ImGui::Begin("Demo Control");
    ImGui::End();
}

static void ShowDemoWindow() {
    static bool set_window_size = true;
    if (set_window_size) {
        ImGui::SetNextWindowSize(ImVec2(400, 300));
        set_window_size = false;
    }

    ImGui::Begin((std::string("Demo: ") + __gDemoManager.currentDemo).c_str());
        __gDemoManager.render();
    ImGui::End();
}

static void demo_render() {
    DemoControlWindow();
    ShowDemoWindow();
}

}
#endif