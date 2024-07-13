#include "hanim.h"

using namespace hanim;

struct HiAnimation : public Scene {
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

int main() {
    hanim::HEngine::render(HiAnimation());
    return 0;
}