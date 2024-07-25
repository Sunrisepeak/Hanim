#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "hanim.h"

using namespace hanim;

struct MyScene : public hanim::Scene {
    virtual void timeline() override {
        auto rect = Rectangle()
            .fill_color({0, 1.0, 1.0, 0.2})
            .move_to({-2, 0, 0});
        auto circle = Circle({2, 0, 0}, 1).color({1.0, 1.0, 0, 1});

        play(DrawBorder(rect));
        play(Transform(rect, circle));
        play(Opacity(rect, 0));
    }
};

struct Rect {
    ImVec2 p1;
    ImVec2 p2;
    bool isDrawing;
};

ImVec2 pixelToNormalized(ImVec2 pixelPos) {
    float normalized_x = (pixelPos.x / 854) * 10.0f - 5.0f;
    float normalized_y = (pixelPos.y / 480) * 10.0f - 5.0f;
    return ImVec2(normalized_x, normalized_y);
}

int main() {
    hanim::HEngine::set_recorder(true);
    auto window = hanim::HEngine::get_render_window();

    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    Rect currentRect;
    bool isDrawing = false;
    auto myScene = MyScene();
    bool playAnim = false;
    int target_frame_number = 0;

    // config window render - imgui
    hanim::HEngine::set_render_handler(
        [&](){ //

            ImGuiIO& io = ImGui::GetIO();
            if (io.MouseDown[0]) {
                if (!isDrawing) {
                    // 开始绘制一个新的矩形
                    currentRect.p1 = io.MousePos;
                    currentRect.isDrawing = true;
                    isDrawing = true;
                }
                currentRect.p2 = io.MousePos;
            } else {
                if (isDrawing) {
                    // 添加矩形到场景
                    auto rect = Rectangle(
                        {pixelToNormalized(currentRect.p1).x, pixelToNormalized(currentRect.p1).y, 0},
                        {pixelToNormalized(currentRect.p2).x, pixelToNormalized(currentRect.p2).y, 0}
                    );
                    static int cnt = 0;
                    myScene.add(rect, cnt++);
                    currentRect.isDrawing = false;
                    isDrawing = false;
                }
            }

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("Hanim Control");
            {
                ImGui::Checkbox("Play Animation", &playAnim);
                ImGui::SliderInt("Target Frame", &target_frame_number, 0, myScene.get_frame_number());
            }
            ImGui::End();

            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
    );

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        if (playAnim) {
            HEngine::render(myScene);
        } else {
            HEngine::render(myScene, target_frame_number);
        } 
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}