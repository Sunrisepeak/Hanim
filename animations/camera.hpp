#ifndef ANIMATION_CAMERA_HPP_HANIM
#define ANIMATION_CAMERA_HPP_HANIM

#include "core/framework.hpp"
#include "core/camera.hpp"

namespace hanim {

class CameraMoveTo : public hanim::HAnimate {

public:
    CameraMoveTo(float x, float y) : mX2 { x }, mY2 { y } { }

    void preprocess() override {
        mX1 = Camera2D::config().x;
        mY1 = Camera2D::config().y;
    }

    void process(int currentFrame) override {
        float alpha = 1.0 * currentFrame / mFrameNumber;
        float x = Interpolator::value(mX1, mX2, alpha);
        float y = Interpolator::value(mY1, mY2, alpha);
        Camera2D::target(x, y);
    }

private:
    float mX1, mY1;
    float mX2, mY2;
};

class CameraScale : public hanim::HAnimate {

public:
    CameraScale(float scale = 2.0) : mScale { scale } { }

    void preprocess() override {
        mSize1 = Camera2D::config().size;
        mSize2 = mSize1 / mScale;
    }

    void process(int currentFrame) override {
        float alpha = 1.0 * currentFrame / mFrameNumber;
        float size = Interpolator::value(mSize1, mSize2, alpha);
        Camera2D::size(size);
    }

private:
    float mSize1, mSize2, mScale;
};

};

#endif