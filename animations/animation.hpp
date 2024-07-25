#ifndef ANIMATION_HPP_HANIM
#define ANIMATION_HPP_HANIM

// base animations

#include "core/framework.hpp"

namespace hanim {

// TODO: optimize efficency, avoid all-prop check
//       use set/get ?
// don't parallel when use default-obj-interpolate
#define PropertyAnimateTemplate(Name, methed, PropType, defaultValue) \
class Name : public HAnimate { \
public: \
    Name(HObject &obj, PropType value = defaultValue) \
        : HAnimate(obj), mValue { value } { } \
    void preprocess() override { \
        mTargetHObject = mStartHObject = mRenderHObject; \
        mTargetHObject.methed(mValue); \
    } \
private: \
    PropType mValue; \
};

//PropertyAnimateTemplate(Shift, shift, vec3, vec3(1, 0, 0))
PropertyAnimateTemplate(Scale, scale, float, 2)
//PropertyAnimateTemplate(Rotate, rotate, float, 90)
//PropertyAnimateTemplate(Opacity, opacity, float, 1)
PropertyAnimateTemplate(Thickness, thickness, float, 1)
//PropertyAnimateTemplate(Color, color, vec4, vec4(1, 0, 0, 0.5))


class Shift : public HAnimate {
public:
    Shift(HObject &obj, vec3 value = vec3(1, 0, 0))
        : HAnimate(obj), mValue { value } { }
    void preprocess() override {
        mStartHObject.move_to(mRenderHObject.get_center());
        mTargetHObject.move_to(mRenderHObject.get_center() + mValue);
    }
    void process(int currentFrame) override {
        float alpha = 1.0 * currentFrame / mFrameNumber;
        auto target = Interpolator::value(
            mStartHObject.get_center(),
            mTargetHObject.get_center(),
            alpha
        );
        HONLY_LOGD("Shift: %f -> target(%f %f %f)", alpha, target[0], target[1], target[2]);
        mRenderHObject.move_to(target);
    }
private:
    vec3 mValue;
};

class Rotate : public HAnimate {
public:
    Rotate(HObject &obj, float value = 90)
        : HAnimate(obj), mValue { value } { mOldValue = 0; }
    void process(int currentFrame) override {
        float alpha = 1.0 * currentFrame / mFrameNumber;
        auto target = Interpolator::value(0.0f, mValue, alpha);
        auto rotateDelta = target - mOldValue;
        mRenderHObject.rotate(rotateDelta);
        mOldValue = target;
    }
private:
    float mValue, mOldValue;
};

class Opacity : public HParallelAnimate<Opacity> {
public:
    Opacity(HObject &obj, float value = 1.0, bool stroke = true, bool fill = true)
        : HParallelAnimate(obj, value, stroke, fill), mValue { value }
    {
        mStroke = stroke;
        mFill = fill;
    }

    Opacity(HObject &&obj, float value = 1.0, bool stroke = true, bool fill = true)
        : Opacity(obj, value, stroke, fill) { }

public: // interface impl
    void preprocess() override {
        mTargetHObject = mStartHObject = mRenderHObject;
        if (mStroke) mTargetHObject.stroke_opacity(mValue);
        if (mFill) mTargetHObject.fill_opacity(mValue);
    }
    void process(int currentFrame) override {
        float alpha = 1.0 * currentFrame / mFrameNumber;
        if (mStroke) {
            auto opacity = Interpolator::value(
                mStartHObject.get_stroke_opacity(),
                mTargetHObject.get_stroke_opacity(),
                alpha
            );
            mRenderHObject.stroke_opacity(opacity);
        }
        if (mFill) {
            auto opacity = Interpolator::value(
                mStartHObject.get_fill_opacity(),
                mTargetHObject.get_fill_opacity(),
                alpha
            );
            mRenderHObject.fill_opacity(opacity);
        }
    }
private:
    float mValue;
    bool mStroke, mFill;
};

class ColorUpdate : public HAnimate {
public:
    ColorUpdate(HObject &obj, Color value = {1, 1, 1, 1}, bool stroke = true, bool fill = true)
        : HAnimate(obj), mValue { value }
    {
        mStroke = stroke;
        mFill = fill;
    }

    void preprocess() override {
        mTargetHObject = mStartHObject = mRenderHObject;
        if (mStroke) mTargetHObject.stroke_color(mValue);
        if (mFill) mTargetHObject.fill_color(mValue);
    }
    void process(int currentFrame) override {
        float alpha = 1.0 * currentFrame / mFrameNumber;
        if (mStroke) {
            auto color = Interpolator::value(
                mStartHObject.get_stroke_color(),
                mTargetHObject.get_stroke_color(),
                alpha
            );
            mRenderHObject.stroke_color(color);
        }
        if (mFill) {
            auto color = Interpolator::value(
                mStartHObject.get_fill_color(),
                mTargetHObject.get_fill_color(),
                alpha
            );
            mRenderHObject.fill_color(color);
        }
    }
private:
    Color mValue;
    bool mStroke, mFill;
};

class FillColor : public HAnimate {
public:
    FillColor(HObject &obj, Color value = {1, 1, 1, 1})
        : HAnimate(obj), mValue { value } { }
    void preprocess() override {
        mTargetHObject = mStartHObject = mRenderHObject;
        mTargetHObject.fill_color(mValue);
    }
    void process(int currentFrame) override {
        float alpha = 1.0 * currentFrame / mFrameNumber;
        auto color = Interpolator::value(
            mStartHObject.get_fill_color(),
            mTargetHObject.get_fill_color(),
            alpha
        );
        mRenderHObject.fill_color(color);
    }
private:
    Color mValue;
};

class MoveTo : public HAnimate {
// support parallel
public:
    MoveTo(HObject &obj, vec3 value = vec3(0))
        : HAnimate(obj), mValue { value } { }
    void preprocess() override {
        mTargetHObject = mStartHObject = mRenderHObject;
        mTargetHObject.move_to(mValue);
    }
    void process(int currentFrame) override {
        float alpha = 1.0 * currentFrame / mFrameNumber;
        auto pos = Interpolator::value(
            mStartHObject.get_center(),
            mTargetHObject.get_center(),
            alpha
        );
        mRenderHObject.move_to(pos);
    }
private:
    vec3 mValue;
};

// -------------------

class FadeIn : public HAnimate {
public:
    FadeIn(HObject &obj) : HAnimate(obj) { }

public:
    void preprocess() override {
        mAnims = HAnimGroup(
            Opacity(mRenderHObject, mRenderHObject.get_fill_opacity(), false, true),
            Opacity(mRenderHObject, mRenderHObject.get_stroke_opacity(), true, false)
        );
        mAnims.set_frame_number(mFrameNumber);
        mRenderHObject.opacity(0);
        mAnims.begin();
    }
    void process(int currentFrame) override {
        mAnims.update(currentFrame);
    }
    void postprocess() override {
        mAnims.finish();
    }
private:
    HAnimGroup mAnims;
};

struct FadeOut : public Opacity {
    FadeOut(HObject &obj) : Opacity(obj, 0) { }
};

}

#endif