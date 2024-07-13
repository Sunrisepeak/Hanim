#ifndef OBJECT_HPP_HANIM
#define OBJECT_HPP_HANIM

#include "core/framework.hpp"

namespace hanim {

struct Line : hanim::HObject {
    Line(vec3 pos1, vec3 pos2) : mPos1 { pos1 }, mPos2 { pos2 } {
        generate_object_data();
    }

    virtual void object_points_init(Points &points, Colors &colors) override {
        points.push_back(mPos1);
        points.push_back(mPos2);
        colors.resize(2, {1, 0, 0, 1});
    }

private:
    vec3 mPos1, mPos2;
};

struct Triangle : hanim::HObject {
    Triangle(vec3 pos1, vec3 pos2, vec3 pos3)
        : mPos1 { pos1 }, mPos2 { pos2 }, mPos3 { pos3 } {
        generate_object_data();
    }

    virtual void object_points_init(Points &points, Colors &colors) override {
        points.resize(3);
        points[0] = mPos1;
        points[1] = mPos2;
        points[2] = mPos3;
        colors = {
            {1, 0, 0, 1},
            {0, 1, 0, 1},
            {0, 0, 1, 1}
        };
    }

private:
    vec3 mPos1, mPos2, mPos3;
};

struct Rectangle : hanim::HObject {
    Rectangle(vec3 p1 = {-0.5, -0.5, 0}, vec3 p2 = {0.5, 0.5, 0}) : mPos1 { p1 }, mPos2 { p2 } {
        generate_object_data();
    }

    virtual void object_points_init(Points &points, Colors &colors) override {
        points.resize(4);
        colors.resize(4, {1, 0, 0, 1});
        points[0] = {mPos2[0], mPos2[1], 0};
        points[1] = {mPos1[0], mPos2[1], 0};
        points[2] = {mPos1[0], mPos1[1], 0};
        points[3] = {mPos2[0], mPos1[1], 0};
    }

private:
    vec3 mPos1, mPos2;
};

struct Circle : hanim::HObject {
    Circle(vec3 center = {0, 0, 0}, float radius = 1.0, int segments = 32)
        : mCenter { center }, mRadius { radius }, mSegments { segments }
    {
        generate_object_data();
    }

    virtual void object_points_init(Points &points, Colors &colors) override {
        points.resize(mSegments);
        colors.resize(mSegments, {1, 0, 0, 1});

        for (int i = 0; i < mSegments; ++i) {
            float theta = 2.0f * M_PI * float(i) / float(mSegments);
            float x = mRadius * cosf(theta);
            float y = mRadius * sinf(theta);
            points[i] = {mCenter[0] + x, mCenter[1] + y, mCenter[2]};
        }
    }

private:
    vec3 mCenter;
    float mRadius;
    int mSegments;
};

}

#endif