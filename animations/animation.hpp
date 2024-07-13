#ifndef ANIMATION_HPP_HANIM
#define ANIMATION_HPP_HANIM

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
PropertyAnimateTemplate(Rotate, rotate, float, 90)
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

class Opacity : public HAnimate {
public:
    Opacity(HObject &obj, float value = 1.0)
        : HAnimate(obj), mValue { value } { }
    void preprocess() override {
        mTargetHObject = mStartHObject = mRenderHObject;
        mStartHObject.opacity(mValue);
    }
    void process(int currentFrame) override {
        float alpha = 1.0 * currentFrame / mFrameNumber;
        auto targetOpacity = Interpolator::value(
            mStartHObject.get_opacity(),
            mTargetHObject.get_opacity(),
            alpha
        );
        auto targetFillOpacity = Interpolator::value(
            mStartHObject.get_fill_opacity(),
            mTargetHObject.get_fill_opacity(),
            alpha
        );
        mRenderHObject.opacity(targetOpacity, false);
        mRenderHObject.fill_opacity(targetFillOpacity);
    }
private:
    float mValue;
};

}

#endif