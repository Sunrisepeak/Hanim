#ifndef PIXELS_HPP_HANIM
#define PIXELS_HPP_HANIM

#include "core/framework.hpp"
#include "objects/object.hpp"

namespace hanim {

struct PixelPanel : HObject {

    PixelPanel(float w = 2, float h = 2, float pixelSize = 0.25) {
        mData->componentMode = true;

        auto pixel = Rectangle().stroke_color({1, 0, 0, 1})
            .fill_color({1, 1, 1, 1})
            .scale(pixelSize / 1);
        float offset = pixelSize / 2;

        for (float y = 0; y < 2; y += pixelSize) {
            auto pixels = HObject();
            for (float x = 0; x < 2; x += pixelSize) {
                pixels.add(pixel.move_to({x + offset, y + offset, 0}));
            }
            mData->components.push_back(pixels);
        }

        generate_object_data();

        move_to({0, 0, 0});

    }

};

}

#endif