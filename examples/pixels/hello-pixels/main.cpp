#include <vector>

#include "hanim.h"

/*
base-on:
    date: 2024/7/15
    os: ubuntu22.04
    commit-id: e8959b2fc766735f4f0e0c752fe42f485b0b6fc2 +- 1

// Bug: DrawBorder for Rect when 30fps

video: https://github.com/user-attachments/assets/4a4d9aa0-8e95-4018-a899-1dfc7a0e0ba4

*/

using namespace hanim;

const static int THICKNESS_240P = 1;
const static int THICKNESS_480P = 2;
const static int THICKNESS_1080P = 4;

const static int THICKNESS = THICKNESS_480P; // THICKNESS_1080P;

static std::vector<std::vector<float>> ikArtistMap {
    { 8, 8, 8, 8, 8, 8, 8, 7, 6, 6, 8, 8, 8, 8, 8, 8 },
    { 8, 8, 8, 8, 8, 8, 8, 6, 5, 5, 8, 8, 8, 8, 8, 8 },
    { 8, 8, 8, 8, 8, 8, 8, 5, 7, 8, 8, 8, 8, 8, 8, 8 },
    { 8, 8, 8, 8, 8, 2, 9, 2, 5, 7, 8, 8, 8, 8, 8, 8 },
    { 8, 8, 8, 8, 2, 2, 2, 9, 2, 3, 6, 8, 8, 8, 8, 8 },
    { 8, 8, 8, 8, 4, 2, 2, 9, 9, 2, 5, 8, 8, 8, 8, 8 },
    { 8, 8, 8, 8, 6, 2, 2, 9, 2, 9, 3, 8, 8, 8, 8, 8 },
    { 8, 8, 8, 8, 8, 2, 2, 9, 2, 2, 9, 8, 8, 8, 8, 8 },

    { 8, 8, 8, 8, 8, 7, 6, 6, 6, 4, 8, 8, 8, 8, 8, 8 },
    { 8, 8, 8, 8, 8, 6, 6, 6, 6, 6, 8, 8, 8, 8, 8, 8 },
    { 8, 8, 8, 8, 7, 6, 6, 8, 6, 6, 7, 8, 8, 8, 8, 8 },
    { 8, 8, 8, 7, 6, 6, 8, 8, 8, 6, 6, 7, 8, 8, 8, 8 },
    { 8, 8, 8, 6, 6, 8, 8, 8, 8, 8, 6, 6, 8, 8, 8, 8 },
    { 8, 8, 8, 6, 6, 8, 8, 8, 8, 8, 6, 6, 8, 8, 8, 8 },
    { 8, 8, 8, 4, 3, 8, 8, 8, 8, 8, 3, 4, 8, 8, 8, 8 },
    { 8, 8, 2, 2, 2, 8, 8, 8, 8, 8, 2, 2, 2, 8, 8, 8 }
};

struct HelloPixels : public Scene {
    void start_anim() {
        auto rect = Rectangle()
            .scale(2)
            .shift({-3, 0, 0})
            .thickness(THICKNESS);
        auto circle = Circle()
            .thickness(THICKNESS)
            .shift({3, 0, 0})
            .stroke_color({0, 1, 0, 1})
            .opacity(0.5);
        play(DrawBorder(rect));
        play(Transform(rect, circle));
        play(Opacity(rect, 0));
    }

    void pixel_logo_anim(HObject &hobj, int changeFrameNumber = 10, float x = 0, float y = 0){

        auto pixel = Rectangle()
            .shift({x, y, 0})
            .thickness(THICKNESS)
            .stroke_color({0, 1, 1, 1})
            .fill_color({0.2, 0.2, 0.2, 0.2});

        play(HAnimGroup(Transform(hobj, pixel)));

        wait(30);

        pixel = hobj.create_ref();

        std::vector<HObject> pixels(4, pixel);
        std::vector<vec3> pShifts {
            {-2, 0, 0},
            {0, -2, 0},
            {2, 0, 0},
            {0, 2, 0}
        };
        Colors pColors {
            {0.2, 0, 0, 1},
            {0, 0.2, 0, 1},
            {0, 0, 0.2, 1},
            {1, 1, 1, 0.2}
        };
        auto pixelOutAnim = HAnimGroup();
        for (int i = 0; i < 4; i++) {
            pixels[i].stroke_color({1, 1, 0, 1});
            pixelOutAnim.add(Transform(
                pixels[i],
                pixels[i].clone().shift(pShifts[i]).fill_color(pColors[i])
            ));
        }

        play(pixelOutAnim);

        auto box = Rectangle().stroke_color({1, 0, 0, 1}).thickness(THICKNESS);
        add(box);
        for (int i = 0; i < 4 * 4; i++) {
            int index = i % 4; // R0 G1 B2 A3
            box.move_to(pixels[index].get_center());
            auto pixelColor = pixel.get_fill_color();
            auto subPixelColor = pixels[index].get_fill_color();
            pixelColor[index] = subPixelColor[index] = subPixelColor[index] + 0.2;
            pixels[index].fill_color(subPixelColor);
            pixel.fill_color(pixelColor);
            wait(changeFrameNumber);
        }
        box.opacity(0);

        wait(30);

        auto pixelInAnim = HAnimGroup();
        for (auto &p : pixels) {
            pixelInAnim.add(Transform(p, pixel));
        }

        play(pixelInAnim);

        auto fadeOut = HAnimGroup(Opacity(pixel, 0));
        for (auto &p : pixels) {
            fadeOut.add(Opacity(p, 0));
        }
        play(fadeOut, 90);
    }

    void ik_art_anim(float time = 2 /* sec */) {
        // draw-ikArtist
        auto ikGrayPicture = HObject();
        for (int i = 0; i < 16 * 16; i++) {
            int x = i / 16;
            int y = i % 16;
            auto p = Rectangle().fill_color(Color(0.1) * ikArtistMap[x][y]);
            p.scale(0.25);
            p.move_to({x * 0.25 + 0.125, y * 0.25 + 0.125, 0});
            ikGrayPicture.add(p);
        }
        ikGrayPicture.move_to({0, 0, 0});
        ikGrayPicture.rotate(-90);
        ikGrayPicture.scale(1.4);
        play(DrawBorder(ikGrayPicture));

        auto basketball = Circle()
            .scale(0.5)
            .shift({2.25, 2.25, 0})
            .thickness(THICKNESS)
            .stroke_color({0, 0, 0.5, 0.8})
            .fill_color({0.87, 0.45, 0.13, 0.6});

        play(DrawBorder(basketball));

        simulate_free_fall(basketball, 2.25, -2.25, 60 * time);

        play(HAnimGroup(
            Transform(ikGrayPicture, basketball.clone().opacity(0)),
            Opacity(basketball, 0)
        ));

    }

    void main_timeline() {
        start_anim();

        wait();

        auto pixel = Rectangle()
            .thickness(THICKNESS)
            .stroke_color({0, 1, 1, 1})
            .fill_color({0.5, 0.5, 0.5, 1});

        play(Create(pixel));

        wait();

        // 1.Gray
        HObject grayVals;
        float x = -6; float grayVal = 0;
        for (int i = 0; i < 11; i++) {
            auto grayV = Rectangle();
            Color c = {grayVal, grayVal, grayVal, 1};
            grayV.stroke_color(c).fill_color(c);
            grayV.move_to({x, -2, 0});
            grayVals.add(grayV);
            grayVal += 0.1;
            x += 1;
        }
        play(Transform(pixel.clone().stroke_color({0, 0, 0, 0}), grayVals, true));

        wait();

        auto selectionBox = Rectangle().stroke_color({1, 1, 0, 1});
        selectionBox.thickness(THICKNESS);
        selectionBox.move_to(grayVals[5].get_center());

        play(HAnimGroup(
            FillColor(pixel, {0.8, 0.8, 0.8, 1}),
            MoveTo(selectionBox, grayVals[8].get_center())
        ));

        wait();

        play(HAnimGroup(
            FillColor(pixel, {0.2, 0.2, 0.2, 1}),
            MoveTo(selectionBox, grayVals[2].get_center())
        ));

        wait();

        // GrayPicture
        auto pixelGrid = Grid(4, 4, 0.25);
        pixelGrid.shift({4, 2, 0});
        play(Transform(pixel.clone(), pixelGrid, true));

        wait();

        // draw-ikArtist
        auto ikGrayPicture = HObject();
        for (int i = 0; i < 16 * 16; i++) {
            int x = i / 16;
            int y = i % 16;
            auto p = Rectangle().fill_color(Color(0.1) * ikArtistMap[x][y]);
            p.scale(0.25);
            p.move_to({x * 0.25 + 0.125, y * 0.25 + 0.125, 0});
            ikGrayPicture.add(p);
        }
        ikGrayPicture.move_to(pixelGrid.get_center());
        ikGrayPicture.rotate(-90);
        play(DrawBorder(ikGrayPicture));

        wait();

        pixelGrid.opacity(0);
        selectionBox.opacity(0);
        auto colorSlider = grayVals;
        ikGrayPicture.opacity(0);
        play(HAnimGroup(
            Transform(ikGrayPicture.clone().opacity(1), pixel, true),
            Transform(grayVals, pixel)
        ));
        grayVals.opacity(0);

        wait(); // TODO: trigger static render bug

        // 2.RGB
        pixel.fill_color({0.5, 0.5, 0.5, 1});
        auto pixels = HObject();
        std::vector<vec3> pShifts {
            {-2, 0, 0},
            {0, -2, 0},
            {2, 0, 0},
            {0, 2, 0}
        };
        Colors pColors {
            {0.2, 0, 0, 1},
            {0, 0.2, 0, 1},
            {0, 0, 0.2, 1},
            {1, 1, 1, 0.2}
        };
        for (int i = 0; i < 3; i++) {
            auto p = pixel.clone()
                .shift(pShifts[i])
                .fill_color(pColors[i])
                .stroke_color({0.5, 0.5, 0.5, 1});
            pixels.add(p);
        }

        play(Transform(pixel.clone(), pixels, true));

        wait();

        for (int i = 0; i < 11; i++) {
            Color c = {0, 0.1 * i, 0, 1};
            colorSlider[i].stroke_color({0, 0, 0, 0}).fill_color(c);
        }

        selectionBox = selectionBox.clone();
        selectionBox.move_to(colorSlider[2].get_center());
        selectionBox.stroke_color({1, 1, 0, 1});
        pixels[1].opacity(0);
        play(HAnimGroup(
            Transform(pixels[1].clone().opacity(1), colorSlider, true),
            Transform(pixels[1].clone().opacity(1), selectionBox, true)
        ));

        wait();

        for (int i = 2; i < 10; i++) {
            play(Shift(selectionBox, {1, 0, 0}), 10);
            color_sync(pixel, pixels, colorSlider, i, 1);
        }

        wait();

        for (int i = 9; i >= 7; i--) {
            play(Shift(selectionBox, {-1, 0, 0}), 10);
            color_sync(pixel, pixels, colorSlider, i, 1);
        }

        wait();

        pixels[1].opacity(1);
        selectionBox.opacity(0);

        // add Alpha
        auto p = pixel.clone()
                .shift(pShifts[3])
                .fill_color(pColors[3])
                .stroke_color({0.5, 0.5, 0.5, 1})
                .opacity(0);
        pixels.add(p);
        

        // R
        play(Rotate(pixels, 90));
        init_color_slider(colorSlider, 0);
        pixels[0].opacity(0);
        selectionBox = selectionBox.clone();
        selectionBox.move_to(colorSlider[2].get_center());
        selectionBox.stroke_color({1, 1, 0, 1});
        play(
            Transform(pixels[0].clone().opacity(1), selectionBox, true)
        );

        wait();

        for (int i = 2; i < 10; i++) {
            play(Shift(selectionBox, {1, 0, 0}), 10);
            color_sync(pixel, pixels, colorSlider, i, 0);
        }

        for (int i = 9; i >= 4; i--) {
            play(Shift(selectionBox, {-1, 0, 0}), 10);
            color_sync(pixel, pixels, colorSlider, i, 0);
        }

        wait();

        pixels[0].opacity(1);
        selectionBox.opacity(0);

        // B
        //play(Rotate(pixels, 180)); // TODO: fix roate bug
        play(Rotate(pixels, 90), 45);
        play(Rotate(pixels, 90), 45);
        init_color_slider(colorSlider, 2);
        pixels[2].opacity(0);
        selectionBox = selectionBox.clone();
        selectionBox.move_to(colorSlider[2].get_center());
        selectionBox.stroke_color({1, 1, 0, 1});
        play(
            Transform(pixels[2].clone().opacity(1), selectionBox, true)
        );

        wait();

        for (int i = 2; i < 10; i++) {
            play(Shift(selectionBox, {1, 0, 0}), 10);
            color_sync(pixel, pixels, colorSlider, i, 2);
        }

        for (int i = 9; i > 1; i--) {
            play(Shift(selectionBox, {-1, 0, 0}), 10);
            color_sync(pixel, pixels, colorSlider, i, 2);
        }

        wait();

        // A - Alpha

        auto tmpAlpha = pixels[3].clone();
        play(Transform(pixel.clone(), tmpAlpha.opacity(1), true));
        tmpAlpha.opacity(0);
        pixels[3].opacity(1);

        wait();

        pixels[2].opacity(1);
        selectionBox.opacity(0);

        play(Rotate(pixels, -90));
        init_color_slider(colorSlider, 3);
        pixels[3].opacity(0);
        selectionBox = selectionBox.clone();
        selectionBox.move_to(colorSlider[10].get_center());
        selectionBox.stroke_color({1, 1, 0, 1});
        play(
            Transform(pixels[3].clone().opacity(1), selectionBox, true)
        );

        wait();

        for (int i = 10; i > 0; i--) {
            play(MoveTo(selectionBox, colorSlider[i].get_center()), 10);
            color_sync(pixel, pixels, colorSlider, i, 3);
        }

        for (int i = 1; i < 5; i++) {
            play(MoveTo(selectionBox, colorSlider[i].get_center()), 10);
            color_sync(pixel, pixels, colorSlider, i, 3);
        }

        wait();

        tmpAlpha = pixels[3].clone();
        tmpAlpha.opacity(1);
        tmpAlpha.fill_color(colorSlider[4].get_fill_color());
        play(HAnimGroup(
            Transform(colorSlider, tmpAlpha),
            Transform(selectionBox, tmpAlpha)
        ));
        pixels[3] = tmpAlpha;
        tmpAlpha.opacity(0);
        selectionBox.opacity(0);
        colorSlider.opacity(0);

        wait();

        ikGrayPicture.move_to(pixel.get_center());
        ikGrayPicture.scale(1.4);
        play(Transform(pixels, ikGrayPicture.clone().opacity(1)));

        wait();

        auto basketball = Circle()
            .scale(0.5)
            .shift({2.25, 2.25, 0})
            .thickness(THICKNESS)
            .stroke_color({0, 0, 0.5, 0.8})
            .fill_color({0.87, 0.45, 0.13, 0.6});

        play(DrawBorder(basketball));

        wait();

        simulate_free_fall(basketball, 2.25, -2.25, 120);

        wait();

        play(HAnimGroup(
            Transform(pixels, basketball.clone().opacity(0)),
            Opacity(basketball, 0)
            //Opacity(pixels, 0), // error - TODO: components opacity issue
        ));

        wait();

        pixel_logo_anim(pixel);
    }

    virtual void timeline() override {
        //start_anim();
        main_timeline();
        //ik_art_anim(120);
    }

    void color_sync(HObject &pixel, HObject &pixels, HObject &colorSlider, int index, int channel) {
        auto subColor = pixels[channel].get_fill_color();
        auto color = pixel.get_fill_color();
        color[channel] = subColor[channel] = colorSlider[index].get_fill_color()[channel];
        if (channel != 3) pixels[channel].fill_color(subColor);
        pixel.fill_color(color);
    }

    void init_color_slider(HObject &colorSlider, int channel) {
        Color color;
        if (channel == 3) {
            color = Color{1, 1, 1, 0};
        } else {
            color = Color{0, 0, 0, 1};
        }
        for (int i = 0; i < 11; i++) {
            color[channel] = 0.1 * i;
            colorSlider[i].fill_color(color);
        }
    }

    void simulate_free_fall(HObject &obj, float objY, float horizontalY, int totalFrameNumber) {
        float positionY = objY;
        float velocity = 0;
        float gravity = 0.98, restitution = 0.2;

        float restitutionDelta = 0.025 * totalFrameNumber / 120;

        restitution += restitutionDelta > 0.5 ? 0.5 : restitutionDelta;

        int f = 0, deltaF = 10;
        float dt = deltaF / 20.0;

        while (f <= totalFrameNumber) {
            positionY += velocity * dt;
            velocity -= gravity * dt;

            // 如果位置低于地面，进行反弹
            if (positionY < horizontalY) {
                positionY = horizontalY;
                velocity *= -restitution;
            }

            auto pos = obj.get_center();
            pos[1] = positionY;

            play(MoveTo(obj, pos), deltaF);
            f += deltaF;
        }
    }
};

int main() {
    hanim::HEngine::default_config1();
    //hanim::HEngine::set_window_size(320, 240);
    //hanim::HEngine::default_config2();
    hanim::HEngine::recorder_file_name("hello-pixels");
    hanim::HEngine::render(HelloPixels());
    return 0;
}