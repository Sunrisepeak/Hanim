#include <string>

#include "hanim.h"

using namespace hanim;

std::string gObjName = "Null";
int gRunIndex = 0;

static void create(hanim::HObject &obj) {
    hanim::HEngine::recorder_file_name(gObjName + std::to_string(gRunIndex++) +  "-Create", true);
    hanim::HEngine::recorder_start();
    hanim::HEngine::play(hanim::Create(obj));
    hanim::HEngine::recorder_stop();
}

static void draw_border(hanim::HObject &obj) {
    hanim::HEngine::recorder_file_name(gObjName + std::to_string(gRunIndex++) +  "-DrawBorder", true);
    hanim::HEngine::recorder_start();
    hanim::HEngine::play(hanim::DrawBorder(obj));
    hanim::HEngine::recorder_stop();
}

static void shift(hanim::HObject &obj) {
    hanim::HEngine::recorder_file_name(gObjName + std::to_string(gRunIndex++) +  "-Shift", true);
    hanim::HEngine::recorder_start();
    hanim::HEngine::play(hanim::Shift(obj));
    hanim::HEngine::recorder_stop();
}

static void move_to_and_rotate(hanim::HObject &obj) {
    hanim::HEngine::recorder_file_name(gObjName + std::to_string(gRunIndex++) +  "-MoveTo-Rotate", true);
    hanim::HEngine::recorder_start();
    hanim::HEngine::play(hanim::HAnimGroup(
        hanim::MoveTo(obj, {0, 0, 0}),
        hanim::Rotate(obj, -90)
    ));
    hanim::HEngine::recorder_stop();
}

static void rotate(hanim::HObject &obj) {
    hanim::HEngine::recorder_file_name(gObjName + std::to_string(gRunIndex++) +  "-Rotate", true);
    hanim::HEngine::recorder_start();
    hanim::HEngine::play(hanim::Rotate(obj));
    hanim::HEngine::recorder_stop();
}

static void scale(hanim::HObject &obj) {
    hanim::HEngine::recorder_file_name(gObjName + std::to_string(gRunIndex++) +  "-Scale", true);
    hanim::HEngine::recorder_start();
    hanim::HEngine::play(hanim::Scale(obj, 2), 30);
    hanim::HEngine::play(hanim::Scale(obj, 0.5), 30);
    hanim::HEngine::recorder_stop();
}

static void color_update(hanim::HObject &obj) {
    hanim::HEngine::recorder_file_name(gObjName + std::to_string(gRunIndex++) +  "-ColorUpdate", true);
    hanim::HEngine::recorder_start();
    hanim::HEngine::play(hanim::ColorUpdate(obj, {1, 1, 1, 1}, true, false), 30);
    hanim::HEngine::play(hanim::ColorUpdate(obj, {1, 0, 0, 0.5}, false), 30);
    hanim::HEngine::recorder_stop();
}

static void opacity_and_rotate(hanim::HObject &obj) {
    hanim::HEngine::recorder_file_name(gObjName + std::to_string(gRunIndex++) +  "-Opacity-Rotate", true);
    hanim::HEngine::recorder_start();
    hanim::HEngine::play(hanim::HAnimGroup(
        hanim::Opacity(obj, 0, true, false),
        hanim::Rotate(obj, 90)
    ), 30);
    hanim::HEngine::play(hanim::HAnimGroup(
        hanim::Opacity(obj, 0),
        hanim::Rotate(obj, 90)
    ), 30);
    hanim::HEngine::recorder_stop();
}

static void anim_start(hanim::HObject &obj) {
    hanim::HEngine::default_config1();
    create(obj);
    draw_border(obj);
    shift(obj);
    move_to_and_rotate(obj);
    rotate(obj);
    scale(obj);
    color_update(obj);
    opacity_and_rotate(obj);
}

static void anim_start(hanim::HObject &&obj) {
    anim_start(obj);
}

int main() {
    gObjName = "PixelPanel"; anim_start(hanim::PixelPanel());
    return 0;
}