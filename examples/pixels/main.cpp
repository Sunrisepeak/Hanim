#include "hanim.h"

using namespace hanim;

struct Pixels : public Scene {
    virtual void timeline() override {
        
    }
};

int main() {
    hanim::HEngine::default_config1();
    hanim::HEngine::render(Pixels());
    return 0;
}