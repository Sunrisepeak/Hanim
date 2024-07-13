#ifndef SCENE_H_HANIM
#define SCENE_H_HANIM

#include <vector>
#include <list>
#include <memory>

#include <third-party/third_party_header.hpp>

#include "core/render.hpp"

namespace hanim {
class Scene {

    friend class HEngine;

    struct StaticObj {
        int startFrameNumber;
        std::shared_ptr<hanim::HObject> objPtr;
    };

public:
    virtual void timeline() { /* impl by sub-class */  };

public:
    Scene() : mTimeline {0}, mStaticObjs{}, mCAnimates{} {
        mFrameBuffVec.resize(1);
    }

    void add(hanim::HObject &obj, int startFrameNumber = -1) {
        for (auto &sObj : mStaticObjs) {
            if (*(sObj.objPtr) == obj) {
                HONLY_LOGW("add CObject to static-obj failed");
                return;
            }
        }
        auto objPtr = std::make_shared<hanim::HObject>();
        objPtr->ref(obj);
        mStaticObjs.push_back(StaticObj{
            startFrameNumber < 0 ? mTimeline : startFrameNumber,
            objPtr
        });
    }

    int get_frame_number() const {
        return mTimeline;
    }

protected:
    template <typename AnimType>
    void play(AnimType &&anim, int frameNumber = 60) {

        // use compile-time if constexpr to reduce error info
        if constexpr (!(std::is_pointer<AnimType>::value)) {

            using AType = typename std::decay<AnimType>::type;

            static_assert(
                std::is_base_of<hanim::HAnimate, AType>::value,
                "[ canim-error: anim's type must be derived from CAnimate - play(...)]"
            );

            // avoid error: new cannot be applied to a reference type
            static_assert(
                !(std::is_reference<AType>::value),
                "AType cannot is reference"
            );

            hanim::HAnimate *animPtr = nullptr;

            if constexpr (std::is_rvalue_reference<AnimType&&>::value) {
                animPtr = new AType(std::move(anim));
            } else {
                animPtr = new AType(anim);
            }

            play_internal(animPtr, frameNumber);
        } else {
            static_assert(
                !(std::is_pointer<AnimType>::value),
                "[ canim-error: anim cannot is pointer - play(...)]"
            );
        }
    }

    int mTimeline;
    std::list<StaticObj> mStaticObjs; // static cobj
    std::vector<std::shared_ptr<hanim::HAnimate>> mCAnimates;
    std::vector<std::vector<HObject>> mFrameBuffVec; // TODO: optimize

protected:

    void play_internal(hanim::HAnimate *anim, int frameNumber = 60) {
        auto animPtr = std::shared_ptr<hanim::HAnimate>(anim);
        if (animPtr->getFrameNumber() == 0)
            animPtr->setFrameNumber(frameNumber);
        //HONLY_LOGI("%ld", animPtr->mRenderCObject.mData->points.size());

        render(animPtr);

        mCAnimates.push_back(animPtr);
        mTimeline += animPtr->getFrameNumber();
    }

    void render(std::shared_ptr<hanim::HAnimate> animPtr) {
        animPtr->begin();
        auto animObjs = animPtr->getObjects();
        for (int i = 1; i <= animPtr->getFrameNumber(); i++) {

            Render::start();

            static_render(mTimeline);

            // render dynamic obj
            animPtr->update(i); // update aniamte data
            for (auto objPtr : animObjs) {
                Render::render_cobject(*objPtr); // render
                mFrameBuffVec.back().push_back(*objPtr);
            }

            Render::end();
            mFrameBuffVec.push_back(std::vector<HObject>()); // add new null-frame
        }
        animPtr->finish();
        for (auto objPtr : animObjs) {
            add(*objPtr, mTimeline);
        }
    }

    void static_render(int currentFrameNumber) {
        // render static obj
        // TODO: optimize - use framebuff replace
        for (auto it = mStaticObjs.begin(); it != mStaticObjs.end();) {
            // TODO: remove second render obj
            if (it->startFrameNumber > mTimeline) break;
            if (it->objPtr->active()) {
                // use linked-list erase is O(1)
                it = mStaticObjs.erase(it);
            } else {
                Render::render_cobject(*(it->objPtr));
                mFrameBuffVec.back().push_back(*(it->objPtr));
                it++;
            }
        }
    }

    void render_target_frame(int frame) {
        if (frame > mFrameBuffVec.size() - 1) {
            frame = mFrameBuffVec.size() - 1;
        }
        for (auto &obj : mFrameBuffVec[frame]) {
            Render::render_cobject(obj);
        }
    }

};

}

#endif