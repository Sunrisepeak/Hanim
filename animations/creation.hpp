#ifndef CREATE_HPP_HANIM
#define CREATE_HPP_HANIM

#include "core/framework.hpp"

namespace hanim {

class Create : public hanim::HParallelAnimate<Create> {

public:
    Create(hanim::HObject &obj) : HParallelAnimate(obj) { }
    Create(hanim::HObject &&obj) : Create(obj) { }

public:

    void preprocess() {
        mStartHObject = mTargetHObject = mRenderHObject;
        int pointNumber = mStartHObject._get_data()->points.size();
        mStartHObject._get_data()->points.clear();
        mStartHObject._get_data()->points.resize(pointNumber, mRenderHObject._get_data()->center);
        mRenderHObject.fill_opacity(0);
    }

    void process(int currentFrame) {
        float alpha = currentFrame * 1.0 / mFrameNumber;
        std::vector<vec3> points;
        float alphaPart = mTargetHObject.get_fill_color()[3] > 0 ? 0.4 : 1;
        if (alpha <= alphaPart) {
            points = Interpolator::vector(
                mStartHObject._get_data()->points,
                mTargetHObject._get_data()->points,
                alpha * 1 / alphaPart
            );
            mRenderHObject._get_data()->points = std::move(points);
        } else {
            int frameOffset = mFrameNumber * alphaPart;
            Opacity(mRenderHObject, mTargetHObject.get_fill_opacity(), false, true)
                .setFrameNumber(mFrameNumber - frameOffset)
                .begin()
                .update(currentFrame - frameOffset)
                .finish();
        }
    }

};

class DrawBorder : public hanim::HParallelAnimate<DrawBorder> {

public:
    DrawBorder(hanim::HObject &obj) : HParallelAnimate(obj) { }
    DrawBorder(hanim::HObject &&obj) : DrawBorder(obj) { }

public:

    void preprocess() {
        // add end point for GLI_LINE_STRIP mode
        // TODO: for line
        //if (mRenderHObject._get_data()->points.size() > 2) {
            mRenderHObject._get_data()->points.push_back(mRenderHObject._get_data()->points[0]);
            mRenderHObject._get_data()->rgbs.push_back(mRenderHObject._get_data()->rgbs[0]);
        //}
        mTargetHObject = mRenderHObject;
        //mRenderHObject = mRenderHObject; TODO: This is a Bug
        mRenderHObject.draw_mode(GLI_LINE_STRIP);
        mRenderHObject._get_data()->points.clear();
        mRenderHObject._get_data()->points.resize(2, mTargetHObject._get_data()->points[0]);
        HONLY_LOGD("DrawBorder init done");
    }

    void process(int currentFrame) {
        float alpha = currentFrame * 1.0 / mFrameNumber;
        // TODO: optimize
        // 0 -> n - 1 segment, n + 1 points
        int segmentNum = mTargetHObject._get_data()->points.size() - 1;
        int currentSegmentIndex = mRenderHObject._get_data()->points.size() - 1;
        int completedSegmentNum = currentSegmentIndex - 1;
        float offset = completedSegmentNum * 1.0 / segmentNum;

        float segmentAlpha = (alpha - offset) * segmentNum;

        // TODO: accurate issue, workaround by segmentNum compare
        if (segmentAlpha > 1.0f && currentSegmentIndex < segmentNum) { // enter next segment
            mRenderHObject._get_data()->points.push_back(mRenderHObject._get_data()->points.back());
            segmentAlpha -= 1;
        }

        int targetPointIndex = mRenderHObject._get_data()->points.size() - 1;
        auto newPoint = Interpolator::value(
            mTargetHObject._get_data()->points[targetPointIndex - 1],
            mTargetHObject._get_data()->points[targetPointIndex],
            segmentAlpha // [0 ~ 1] for every segment
        );
        mRenderHObject._get_data()->points.back() = newPoint; // update
    }

    void postprocess() override {
        mRenderHObject._get_data()->points.pop_back();
        mRenderHObject._get_data()->rgbs.pop_back();
        //mTargetHObject = mRenderHObject; // release target memory (RF-auto)
        mRenderHObject.draw_mode(GLI_NONE);
    }

};

}

#endif