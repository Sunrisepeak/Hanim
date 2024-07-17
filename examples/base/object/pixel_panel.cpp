#include "hanim.h"

using namespace hanim;

struct PixelPanelExample : public Scene {
    virtual void timeline() override {
        play(DrawBorder(PixelPanel()));
        play(DrawBorder(Circle()));
    }
};

int main() {
    hanim::HEngine::default_config1();
    hanim::HEngine::render(PixelPanelExample());
    return 0;
}