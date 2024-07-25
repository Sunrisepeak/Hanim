#include <string>

#include "hanim.h"

using namespace hanim;

int gRunIndex = 0;
auto rect = Rectangle()
    .fill_color(HColor::ORANGE)
    .fill_opacity(0.5);

struct AutoRecordHelper {
    AutoRecordHelper(std::string name) {
        HEngine::recorder_file_name(std::to_string(gRunIndex++) + "-" + name, true);
        HEngine::recorder_start();
    }
    ~AutoRecordHelper() {
        HEngine::recorder_stop();
    }
};

static void fade_in() {
    AutoRecordHelper _ar("FadeIn");

    HEngine::play(FadeIn(rect));
    HEngine::wait(30);
}

static void shift() {
    AutoRecordHelper _ar("Shift");

    HEngine::play(Shift(rect));
    HEngine::wait(30);
}

static void move_to() {
    AutoRecordHelper _ar("MoveTo");

    auto target = Triangle().shift(HPos::RIGHT * 1.5);
    rect.move_to(HPos::ORIGIN);

    HEngine::add(target);
    HEngine::play(MoveTo(rect, target.get_center()));

    target.opacity(0);
    HEngine::wait(30);
}

static void rotate() {
    AutoRecordHelper _ar("Rotate");
    rect.move_to(HPos::ORIGIN);
    HEngine::play(Rotate(rect, 90), 40);
    HEngine::play(Rotate(rect, -90), 20);
    HEngine::wait(30);
}

static void scale() {
    AutoRecordHelper _ar("Scale");
    rect.move_to(HPos::ORIGIN);
    HEngine::play(Scale(rect, 2), 40);
    HEngine::play(Scale(rect, 0.5), 20);
    HEngine::wait(30);
}

static void color_update() {
    AutoRecordHelper _ar("ColorUpdate");
    HEngine::play(ColorUpdate(rect, HColor::BLUE, false, true));
    HEngine::wait(30);
}

static void opacity() {
    AutoRecordHelper _ar("Opacity");
    HEngine::play(Opacity(rect, 0, true, false), 30);
    HEngine::play(HAnimGroup(
        Opacity(rect, 0.5, true, false),
        Opacity(rect, 0.5, false, true)
    ), 30);
    HEngine::wait(30);
}

static void fade_out() {
    AutoRecordHelper _ar("FadeOut");
    HEngine::play(FadeOut(rect));
    HEngine::wait(30);
}


static void create() {
    AutoRecordHelper _ar("Create");
    rect.stroke_color(HColor::WHITE)
        .fill_color(HColor::GREEN)
        .fill_opacity(0.5);
    HEngine::play(Create(rect));
    HEngine::wait(30);
}

static void draw_border() {
    AutoRecordHelper _ar("DrawBorder");
    HEngine::play(DrawBorder(rect));
    HEngine::wait(30);
}

static void transform() {
    AutoRecordHelper _ar("Transform");
    auto point = Point().shift(HPos::UP * 1.5);
    auto line = Line().shift(HPos::LEFT * 1.5);
    auto triangle = Triangle().shift(HPos::DOWN * 1.5);
    auto rectCopy = rect.clone();
    auto circle = Circle().shift(HPos::RIGHT * 1.5);
    HEngine::play(Transform(rect, point));
    HEngine::play(Transform(rect, line));
    HEngine::play(Transform(rect, triangle));
    HEngine::play(Transform(rect, circle));
    HEngine::play(Transform(rect, rectCopy));
    HEngine::wait(30);
}

static void anim_start() {
    HEngine::default_config1();

    // base
    fade_in();
    shift();
    move_to();
    rotate();
    scale();
    color_update();
    opacity();
    fade_out();

    // create
    create();
    draw_border();
    transform();
}

int main() {
    anim_start();
    return 0;
}