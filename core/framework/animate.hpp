#ifndef ANIMATE_HPP_FWK_HANIM
#define ANIMATE_HPP_FWK_HANIM

#include <memory>

#include "core/framework/object.hpp"

namespace hanim {


class HAnimate {

public:
    HAnimate(HObject &obj) :
        mFrameNumber { 0 },
        mRenderHObject { },
        mStartHObject { },
        mTargetHObject { }
    {
        mRenderHObject.ref(obj);
    }

    // support temp-obj
    HAnimate(HObject &&obj) : HAnimate(obj) { }

    HAnimate(HAnimate &anim) : HAnimate(anim.mRenderHObject) {
        mStartHObject.ref(anim.mStartHObject);
        mTargetHObject.ref(anim.mTargetHObject);
        mFrameNumber = anim.mFrameNumber;
    }

    // disable copy assign
    HAnimate & operator=(HAnimate &) = delete;

    HAnimate(HAnimate &&anim) : HAnimate() {
        mFrameNumber = anim.mFrameNumber;
        mRenderHObject = anim.mRenderHObject.move_sema(true);
        mStartHObject = anim.mStartHObject.move_sema(true);
        mTargetHObject = anim.mTargetHObject.move_sema(true);

        anim.mFrameNumber = 0;
    }

    HAnimate & operator=(HAnimate &&anim) {
        if (this != &anim) {
            mFrameNumber = anim.mFrameNumber;
            mRenderHObject = anim.mRenderHObject.move_sema(true);
            mStartHObject = anim.mStartHObject.move_sema(true);
            mTargetHObject = anim.mTargetHObject.move_sema(true);
            anim.mFrameNumber = 0;
        }
        return *this;
    }


protected:
    HAnimate() : HAnimate(HObject()) { }

public:
    HAnimate & setFrameNumber(int frameNumber) {
        mFrameNumber = frameNumber;
        HONLY_LOGW("set frameNumber %d", frameNumber);
        return *this;
    }

    int getFrameNumber() const {
        HONLY_LOGI("animate frame number is %d", mFrameNumber);
        return mFrameNumber;
    }

public:

    // TODO: use weak_ptr
    virtual std::vector<HObject *> getAnimObjects() {
        return mRenderHObject.getObjs();
    }

public: // begin / update / finish - mode use in frameworks

    virtual HAnimate & begin(bool preprocessFlag = true) {
        mRenderHObject.move_sema(false);
        mRenderHObject.active(true);
        if(preprocessFlag) preprocess();
        return *this;
    }

    virtual HAnimate & update(int currentFrame, bool processFlag = true) {
        if(processFlag) process(currentFrame);
        return *this;
    }

    virtual HAnimate & finish(bool postprocessFlag = true) {
        if(postprocessFlag) postprocess();
        mRenderHObject.active(false); // TODO: parallel animate issue?
        mRenderHObject.move_sema(true);
        return *this;
    }

public: // preprocess / process / postprocess - mode use in user/lib/component

    virtual void preprocess() {
        // init obj
        // init status
    }

    virtual void process(int currentFrame) {
        float alpha = currentFrame * 1.0 / mFrameNumber;
        //HONLY_LOGI("alpha(%f)", alpha);
        //mMoveSemaFlag a(false) = b(true) - only copy(0x7ffe34442d10 -> 0x570a41f78b70)
        //mRenderHObject = Interpolator::Hobject(mStartHObject, mTargetHObject, alpha);
        auto obj = Interpolator::hobject(mStartHObject, mTargetHObject, alpha);
        mRenderHObject = obj;
    }

    virtual void postprocess() {
        // To constructor
        // 1.restore obj's ref-relationship
        // 2.restore status
        // this->~HAnimate();
        // HAnimate(obj)
    }

protected:

    int mFrameNumber;
    // Note: don't use clone to cover mRenderHObject
    //      it will lead to disconnect status/relation
    // Example:
    //     Rectangle rect(xxx)
    //     play(new DrawBorder(rect)); - rect1
    //          mRenderHObject = mRenderHObject.clone();
    //           or
    //          mRenderHObject = otherCObj.clone();
    //          mRenderHObject = otherCObj; ?
    //     play(new DrawBorder(rect)); - rect2
    // when use clone on mRenderHObject -> rect1 != rect2
    // 
    // TODO: optimize and avoid
    //       2024/6/14 - remove clone, and use as_ref
    HObject mRenderHObject;
    HObject mStartHObject, mTargetHObject;
};



// only allow rvalue Anim add to this HAnimGroup
class HAnimGroup final : public HAnimate {

public:

    HAnimGroup() : mAnimTreePtr(std::make_shared<AnimTree>(0)) { }

    // ref: template <typename AnimType> HAnimGroup(AnimType &&anim)
    // remove const
    HAnimGroup(/* const */ HAnimGroup &anim) : HAnimate(anim) {
        // build ref
        HONLY_LOGI("create a ref for anim-tree -ref(%ld)",
            anim.mAnimTreePtr.use_count() + 1
        );
        mAnimTreePtr = anim.mAnimTreePtr;
    }

    HAnimGroup & operator=(const HAnimGroup &) = delete;

    HAnimGroup(HAnimGroup &&animGroup) {
        mAnimTreePtr = std::move(animGroup.mAnimTreePtr);
        HAnimate::operator=(std::move(animGroup));
        //reset
        animGroup.mAnimTreePtr = std::make_shared<AnimTree>(0);
    }

    HAnimGroup & operator=(HAnimGroup &&animGroup) {
        if (mAnimTreePtr != animGroup.mAnimTreePtr) {
            mAnimTreePtr = std::move(animGroup.mAnimTreePtr);
            HAnimate::operator=(std::move(animGroup));
            // reset
            animGroup.mAnimTreePtr = std::make_shared<AnimTree>(0);
        }
        return *this;
    }

public:
    template <typename AnimType>
    HAnimGroup(AnimType &&anim) : HAnimGroup() {
        static_assert(
            std::is_rvalue_reference<AnimType&&>::value,
            "anim must be rvalue"
        );
        add(std::forward<AnimType>(anim));
    }

    template <typename AnimType, typename... AnimTypes>
    HAnimGroup(AnimType &&anim, AnimTypes&&... anims) : HAnimGroup() {
        /* work ?
            add(std::forward<AnimType&&>(anim));
        */
        add(std::forward<AnimType>(anim));
        add(std::forward<AnimTypes>(anims)...);
    }


public:

    template <typename AnimType>
    void add(AnimType &&anim, int startFrame = 0) {
                // use compile-time if constexpr to reduce error info
        if constexpr (!(std::is_pointer<AnimType>::value)) {

            using AType = typename std::decay<AnimType>::type;

            static_assert(
                std::is_base_of<HAnimate, AType>::value,
                "[ canim-error: anim's type must be derived from HAnimate - HAnimGroup(...)]"
            );

            // avoid error: new cannot be applied to a reference type
            static_assert(
                !(std::is_reference<AType>::value),
                "AType cannot is reference"
            );

            HAnimate *animPtr = nullptr;

            if constexpr (std::is_rvalue_reference<AnimType&&>::value) {
                animPtr = new AType(std::move(anim));
            } else {
                animPtr = new AType(anim);
            }

            add(animPtr, startFrame);

        } else {
            static_assert(
                !(std::is_pointer<AnimType>::value),
                "[ canim-error: anim cannot is pointer - HAnimGroup(...)]"
            );
        }
    }

    template <typename AnimType, typename... AnimTypes>
    void add(AnimType &&anim, AnimTypes&&... anims) {
        add(std::forward<AnimType>(anim));
        add(std::forward<AnimTypes>(anims)...);
    }

protected:
    void add(HAnimate *anim, int startFrame = 0) {
        mAnimTreePtr->push_back({std::unique_ptr<HAnimate>(anim), startFrame});
        mFrameNumber = std::max(mFrameNumber, startFrame + anim->getFrameNumber());
    }

public:
    virtual std::vector<HObject *> getAnimObjects() override {
        std::vector<HObject *> objs;
        for (auto &node : *mAnimTreePtr) {
            auto subObjs = node.animPtr->getAnimObjects();
            objs.reserve(objs.size() + subObjs.size());
            for (HObject * objPtr : subObjs) {
                objs.push_back(objPtr);
            }
        }
        HONLY_LOGI("%ld", objs.size());
        return objs;
    }

public: 
// need use begin/update/finish mode to update status
    virtual void preprocess() final {
        for (auto &node : *mAnimTreePtr) {
            if (node.animPtr->getFrameNumber() == 0) {
                HONLY_LOGD("startFrame %d, mFrameNumber 0 -> %d",
                    node.startFrame,
                    mFrameNumber - node.startFrame
                );
                node.animPtr->setFrameNumber(mFrameNumber - node.startFrame);
            }
            node.animPtr->begin();
        }
    }

    virtual void process(int currentFrame) final {
        for (auto &node : *mAnimTreePtr) {
            if (currentFrame > node.startFrame)
                node.animPtr->update(currentFrame - node.startFrame);
        }
    }

    virtual void postprocess() final {
        for (auto &node : *mAnimTreePtr) {
            node.animPtr->finish();
        }
    }

    struct AnimTreeNode {
        std::unique_ptr<HAnimate> animPtr;
        int startFrame;
    };

    using AnimTree = std::vector<AnimTreeNode>;
    std::shared_ptr<AnimTree> mAnimTreePtr;

public:
    bool empty() const {
        return mAnimTreePtr->empty();
    }

};


template <typename AnimType, bool AutoParallel = true>
class HParallelAnimate : public HAnimate {
public:
    HParallelAnimate(HObject &obj) : HAnimate(obj), mAnimTree() {
        if (AutoParallel) apply_to_all();
    }

    template <typename... Args>
    HParallelAnimate(HObject &obj, Args... args) : HAnimate(obj) {
        if (AutoParallel) apply_to_all(args...);
    }

public:
    virtual HAnimate & begin(bool preprocessFlag = true) final {
        HAnimate::begin(false);
        if (!mAnimTree.empty()) {
            mAnimTree.setFrameNumber(mFrameNumber);
            mAnimTree.begin();
        } else {
            if (preprocessFlag) preprocess();
        }
        return *this;
    }

    virtual HAnimate & update(int currentFrame, bool processFlag = true) final {
        HAnimate::update(currentFrame, false);
        if (!mAnimTree.empty()) {
            mAnimTree.update(currentFrame);
        } else {
            if (processFlag) process(currentFrame);
        }
        return *this;
    }

    virtual HAnimate & finish(bool postprocessFlag = true) final {
        if (!mAnimTree.empty()) {
            mAnimTree.finish();
        } else {
            if (postprocessFlag) postprocess();
        }
        HAnimate::finish(false);
        return *this;
    }

public:

    template <typename... Args>
    void apply_to_all(Args... args) {
        // stop recursive contruct for AnimType
        if (mRenderHObject.is_components()) {
            auto objs = mRenderHObject.getObjs();
            for (auto objPtr : objs) {
                mAnimTree.add(AnimType(objPtr->create_ref(), args...));
            }
        }
    }

private:
    HAnimGroup mAnimTree; // only one layer by components
};


} // hanim

#endif