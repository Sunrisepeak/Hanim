#ifndef PIXELS_HPP_HANIM
#define PIXELS_HPP_HANIM

#include "core/framework.hpp"
#include "objects/object.hpp"

namespace hanim {

class Pixel : public Rectangle {
public:
    Pixel(float x = 0, float y = 0, float size = 1) :
        Rectangle({x - size / 2, y - size / 2, 0}, {x + size / 2, y + size / 2, 0}) {

        }
};

class PixelPanel : public HObject {

private: //disable
    using HObject::size;

public:
    PixelPanel(float w = 2, float h = 2, float pixelSize = 0.25) {
        // init root node - TODO: optimize by default?
        mData->componentMode = true;
        mData->points.resize(1, vec3(1));
        mData->rgbs.resize(1, vec4(1));
        fill_color({1, 1, 1, 1});
        stroke_color({1, 0, 0, 1});

        // init component
        auto pixel = Pixel().stroke_color({1, 0, 0, 1})
            .fill_color(get_fill_color())
            .stroke_color(get_stroke_color())
            .scale(pixelSize / 1);
        float offset = pixelSize / 2;

        for (float y = 2; y > 0; y -= pixelSize) {
            auto pixels = HObject()
                .stroke_color(get_stroke_color())
                .fill_color(get_fill_color());
            for (float x = 0; x < 2; x += pixelSize) {
                pixels.add(pixel.move_to({x + offset, y - offset, 0}));
            }
            mData->components.push_back(pixels);
        }

        generate_object_data();

        move_to({0, 0, 0});

    }

public:
    int width() const {
        return mData->components.size();
    }

    int height() const {
        return mData->components[0].size();
    }
};

}

#endif