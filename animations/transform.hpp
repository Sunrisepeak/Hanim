#ifndef TRANSFORM_HPP_HANIM
#define TRANSFORM_HPP_HANIM

#include "core/framework.hpp"

namespace hanim {

class Transform : public hanim::HAnimate {
public:
    Transform(hanim::HObject &obj, hanim::HObject &target, bool useTargetObj = false) : HAnimate(obj) {
        mTarget.ref(target);
        mUseTargetObj = useTargetObj;
    }

    Transform(hanim::HObject &&obj, hanim::HObject &target, bool useTargetObj = false) : Transform(obj, target, useTargetObj) {

    }

public:
    virtual void preprocess() override {
        // use a copy/clone, avoid to change target
        mTargetHObject = mTarget;
        mStartHObject = mRenderHObject;

        int oldTargetSize = mTargetHObject._get_data()->points.size();
        int oldStartSize = mStartHObject._get_data()->points.size();

        hanim::HObject::align_data(mStartHObject, mTargetHObject);
        int targetSize = mTargetHObject._get_data()->points.size();
        int startSize = mStartHObject._get_data()->points.size();
        HONLY_LOGD("begin");
    }

    virtual void process(int currentFrame) override {
        /* realtime update target version
            float alpha = currentFrame * 1.0 / mFrameNumber;
            mTargetHObject = mTarget.clone();
            HObject::align_data(mRenderHObject, mTargetHObject);
            auto obj = Interpolator::Hobject(mStartHObject, mTargetHObject, alpha);
            *(mRenderHObject._get_data()) = *(obj._get_data());
        */
        HAnimate::process(currentFrame);
    }

    virtual void postprocess() override {
        if (mUseTargetObj) {
            mRenderHObject.ref(mTarget);
        } else {
            mTargetHObject = std::move(mTarget);
            mRenderHObject = mTargetHObject; // copy
        }
    }

protected:
    bool mUseTargetObj;
    hanim::HObject mTarget;
};

}

#endif