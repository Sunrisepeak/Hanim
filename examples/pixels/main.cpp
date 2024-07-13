#include "hanim.h"

using namespace hanim;

struct PixelsScene : public Scene {
    virtual void timeline() override {
        auto pixel = Rectangle()
            .thickness(2)
            .color({1, 1, 0, 1})
            .fill_color({0.5, 0.5, 0.5, 1});
        play(Create(pixel));
        auto pixelR = pixel.clone();
        auto pixelG = pixel.clone();
        auto pixelB = pixel.clone();
        play(HAnimGroup(
            Transform(
                pixelR,
                pixelR.clone().shift({-2, -2, 0}).fill_color({0.5, 0, 0, 1})
            ),
            Transform(
                pixelG,
                pixelG.clone().shift({0, -2, 0}).fill_color({0, 0.5, 0, 1})
            ),
            Transform(
                pixelB,
                pixelB.clone().shift({2, -2, 0}).fill_color({0, 0, 0.5, 1})
            )
        ));
    }
};

int main() {
    hanim::HEngine::default_config1();
    hanim::HEngine::render(PixelsScene());
    return 0;
}