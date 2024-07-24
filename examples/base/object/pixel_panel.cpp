#include "hanim.h"

using namespace hanim;

struct PixelPanelExample : public Scene {
    virtual void timeline() override {
        auto pPanel = PixelPanel();
        play(Create(pPanel));
        play(FillColor(pPanel, {0, 0, 1, 0.5}));

        for (int i = 0; i < pPanel.width(); i++) {
            for (int j = 0; j < pPanel.height(); j++) {
                if ((i + j) % 2 == 0) {
                    play(Opacity(pPanel[i][j], 0), 5);
                }
            }
        }

        play(Rotate(pPanel));
    }
};

int main() {
    hanim::HEngine::default_config1();
    hanim::HEngine::render(PixelPanelExample());
    return 0;
}