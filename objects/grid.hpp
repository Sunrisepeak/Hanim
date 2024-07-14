#ifndef GRID_HPP_HANIM
#define GRID_HPP_HANIM

#include "core/framework.hpp"
#include "objects/object.hpp"

namespace hanim {

struct Grid : hanim::HObject {

    Grid(float w = 2, float h = 2, float interval = 0.5) {
        mData->componentMode = true;

        w = w / 2; h = h / 2;

        add(Line({-w, 0, 0}, {w, 0, 0}).color({1, 0, 0, 1}));
        add(Line({0, -h, 0}, {0, h, 0}).color({0, 1, 0, 1}));

        // x
        for (float i = interval; i <= w; i += interval) {
            add(Line({i, -h, 0}, {i, h, 0}).color({1, 1, 0, 0.5}));
            add(Line({-i, -h, 0}, {-i, h, 0}).color({1, 1, 0, 0.5}));
        }

        for (float i = interval; i <= h; i += interval) {
            add(Line({-w, i, 0}, {w, i, 0}).color({1, 1, 0, 0.5}));
            add(Line({-w, -i, 0}, {w, -i, 0}).color({1, 1, 0, 0.5}));
        }

        generate_object_data();
    }

};

}

#endif