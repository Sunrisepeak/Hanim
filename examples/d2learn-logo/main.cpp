#include <vector>

#include "hanim.h"


using namespace hanim;

const static int THICKNESS_240P = 1;
const static int THICKNESS_480P = 2;
const static int THICKNESS_1080P = 4;

const static int THICKNESS = THICKNESS_480P; // THICKNESS_1080P;

static std::vector<std::vector<bool>> d2learnLogoMap {
    {0,1,1,0,0,1,0,0}, // d
    {0,0,1,1,0,0,1,0}, // 2
    {0,1,1,0,1,1,0,0}, // l
    {0,1,1,0,0,1,0,1}, // e
    {0,1,1,0,0,0,0,1}, // a
    {0,1,1,1,0,0,1,0}, // r
    {0,1,1,0,1,1,1,0}  // n
};

struct D2LearnLogo : public Scene {
    virtual void timeline() override {
        auto logo = PixelPanel();
        logo.stroke_color(HColor::BLACK);
        logo.scale(2);
        //play(Create(logo));
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 8; j++) {
                if (d2learnLogoMap[i][j]) {
                    play(FillColor(logo[i][j], HColor::CYAN), 5);
                } else {
                    play(FadeOut(logo[i][j]), 5);
                }
            }
        }
        play(FillColor(logo[7][0], HColor::RED), 10);
        play(FillColor(logo[7][1], HColor::ORANGE), 10);
        play(FillColor(logo[7][2], HColor::YELLOW), 10);
        play(FillColor(logo[7][3], HColor::GREEN), 10);
        play(FillColor(logo[7][4], HColor::CYAN), 10);
        play(FillColor(logo[7][5], HColor::BLUE), 10);
        play(FillColor(logo[7][6], HColor::PURPLE), 10);
        play(FillColor(logo[7][7], HColor::WHITE), 10);
    }
};

int main() {
    hanim::HEngine::default_config1();
    //hanim::HEngine::set_window_size(320, 240);
    //hanim::HEngine::default_config2();
    hanim::HEngine::recorder_file_name("d2learn-logo");
    hanim::HEngine::render(D2LearnLogo());
    hanim::HEngine::save_frame_to_img("d2learn-logo.png");
    return 0;
}