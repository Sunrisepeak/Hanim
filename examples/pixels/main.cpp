#include <vector>

#include "hanim.h"

using namespace hanim;

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

struct PixelsScene : public Scene {
    void start_anim() {
        auto rect = Rectangle()
            .shift({-3, 0, 0})
            .thickness(2)
            .scale(2);
        auto circle = Circle()
            .thickness(2)
            .shift({3, 0, 0})
            .color({0, 1, 0, 1})
            .opacity(0.5);
        play(DrawBorder(rect));
        play(Transform(rect, circle));
        play(Opacity(rect, 0));
    }

    void pixel_logo_anim(HObject &hobj){

        auto pixel = Rectangle()
            .thickness(2)
            .color({0, 1, 1, 1})
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
            pixels[i].color({1, 1, 0, 1});
            pixelOutAnim.add(Transform(
                pixels[i],
                pixels[i].clone().shift(pShifts[i]).fill_color(pColors[i])
            ));
        }

        play(pixelOutAnim);

        auto box = Rectangle().color({1, 0, 0, 1}).thickness(2);
        add(box);
        for (int i = 0; i < 4 * 4; i++) {
            int index = i % 4; // R0 G1 B2 A3
            box.move_to(pixels[index].get_center());
            auto pixelColor = pixel.get_fill_color();
            auto subPixelColor = pixels[index].get_fill_color();
            pixelColor[index] = subPixelColor[index] = subPixelColor[index] + 0.2;
            pixels[index].fill_color(subPixelColor);
            pixel.fill_color(pixelColor);
            wait(8);
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
        play(fadeOut);
    }

    virtual void timeline() override {

        start_anim();

        wait();

        auto pixel = Rectangle()
            .thickness(2)
            .color({0, 1, 1, 1})
            .fill_color({0.5, 0.5, 0.5, 1});

        play(Create(pixel));

        wait();

        // 1.Gray
        HObject grayVals;
        float x = -6; float grayVal = 0;
        for (int i = 0; i < 11; i++) {
            auto grayV = Rectangle();
            Color c = {grayVal, grayVal, grayVal, 1};
            grayV.color(c).fill_color(c);
            grayV.move_to({x, -2, 0});
            grayVals.add(grayV);
            grayVal += 0.1;
            x += 1;
        }
        play(Transform(pixel.clone().color({0, 0, 0, 0}), grayVals, true));

        wait();

        auto selectionBox = Rectangle().color({1, 1, 0, 1});
        selectionBox.thickness(2);
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

        auto pixelGrid = Grid(4, 4, 0.25);
        pixelGrid.shift({4, 2, 0});
        play(Transform(pixel.clone(), pixelGrid, true));

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
        play(HAnimGroup(
            Transform(ikGrayPicture, pixel),
            Transform(grayVals, pixel)
        ));
        ikGrayPicture.opacity(0);
        grayVals.opacity(0);

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
                .color({0.5, 0.5, 0.5, 1});
            pixels.add(p);
        }

        play(Transform(pixel.clone(), pixels, true));

        for (int i = 0; i < 11; i++) {
            Color c = {0, 0.1 * i, 0, 1};
            colorSlider[i].color({0, 0, 0, 0}).fill_color(c);
        }

        selectionBox = selectionBox.clone();
        selectionBox.move_to(colorSlider[2].get_center());
        selectionBox.color({1, 1, 0, 1});
        pixels[1].opacity(0);
        play(HAnimGroup(
            Transform(pixels[1].clone().opacity(1), colorSlider, true),
            Transform(pixels[1].clone().opacity(1), selectionBox, true)
        ));

        for (int i = 2; i < 10; i++) {
            play(Shift(selectionBox, {1, 0, 0}), 10);
            color_sync(pixel, pixels, colorSlider, i, 1);
        }

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
                .color({0.5, 0.5, 0.5, 1})
                .opacity(0);
        pixels.add(p);
        

        // R
        play(Rotate(pixels, 90));
        init_color_slider(colorSlider, 0);
        pixels[0].opacity(0);
        selectionBox = selectionBox.clone();
        selectionBox.move_to(colorSlider[2].get_center());
        selectionBox.color({1, 1, 0, 1});
        play(
            Transform(pixels[0].clone().opacity(1), selectionBox, true)
        );

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
        selectionBox.color({1, 1, 0, 1});
        play(
            Transform(pixels[2].clone().opacity(1), selectionBox, true)
        );

        for (int i = 2; i < 10; i++) {
            play(Shift(selectionBox, {1, 0, 0}), 10);
            color_sync(pixel, pixels, colorSlider, i, 2);
        }

        for (int i = 9; i > 1; i--) {
            play(Shift(selectionBox, {-1, 0, 0}), 10);
            color_sync(pixel, pixels, colorSlider, i, 2);
        }

        wait();

        //pixel_logo_anim(circle);
    }

    void color_sync(HObject &pixel, HObject &pixels, HObject &colorSlider, int index, int channel) {
        auto subColor = pixels[channel].get_fill_color();
        auto color = pixel.get_fill_color();
        color[channel] = subColor[channel] = colorSlider[index].get_fill_color()[channel];
        pixels[channel].fill_color(subColor);
        pixel.fill_color(color);
    }

    void init_color_slider(HObject &colorSlider, int channel) {
        Color color {0, 0, 0, 1};
        for (int i = 0; i < 11; i++) {
            color[channel] = 0.1 * i;
            colorSlider[i].fill_color(color);
        }
    }

};

int main() {
    hanim::HEngine::default_config1();
    hanim::HEngine::render(PixelsScene());
    return 0;
}