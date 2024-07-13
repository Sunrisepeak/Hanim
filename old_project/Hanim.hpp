// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/HAnim
//


#ifndef __HI_ANIM_HPP__HANIM
#define __HI_ANIM_HPP__HANIM

#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <variant>
#include <cmath>
#include <cassert>
#include <exception>

#define HANIM_VERIFY (0x68616e696d)
#define HANIM_PI 3.14159265358979323846   // pi

#define HANIM_CLONE(HAnimateType) \
virtual std::shared_ptr<HAnimate> __clone() { \
    auto anim = std::make_shared<HAnimateType>(); \
    *anim = *this; \
    return anim; \
}


namespace hanim {

// ----------------------------------------------- Type Define -------------------------------------------------------

struct HVec2 {
    float x, y;
};

static HVec2 operator+(HVec2 vec1, HVec2 vec2) {
    return {vec1.x + vec2.x, vec1.y + vec2.y};
}

struct HVec3 {
    float x, y, z;
};

struct HVec4 {
    float x, y, z, w;
};

struct IAFrame {
    std::vector<float> data;
    IAFrame(std::vector<float> vec) : data { vec } { }
    IAFrame(float x = 0) : data { x } { }
    IAFrame(std::initializer_list<float> &&ilist) : IAFrame{std::vector<float>(ilist.begin(), ilist.end())} { }

    IAFrame(HVec2 vec) : IAFrame({vec.x, vec.y}) { }
    IAFrame(HVec3 vec) : IAFrame({vec.x, vec.y, vec.z}) { }
    IAFrame(HVec4 vec) : IAFrame({vec.x, vec.y, vec.z, vec.w}) { }

    void rotation(float &x, float &y) const {
        // data [refSysX, refSysY, rotationAngle1/degree]
        /*
            90
            ^
            |
            +------> 0
        */
        std::vector<float> homoCoordinate1 {x - data[0], y - data[1], 1}, homoCoordinate2 {0, 0, 0};
        float radian = data[2] * HANIM_PI / 180;
        float sin = std::sin(radian);
        float cos = std::cos(radian);
        float rotationMatrix[3][3] = {
            {cos, -sin, data[0]},
            {sin, cos, data[1]},
            {0, 0, 1}
        };

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                homoCoordinate2[i] += homoCoordinate1[j] * rotationMatrix[i][j];
            }
        }

        x = homoCoordinate2[0] / homoCoordinate2[2];
        y = homoCoordinate2[1] / homoCoordinate2[2];
    }
};

struct FAFrame {
// type
    enum ColorType {
        RGB,
        RGBA,
    };
// constructor
    FAFrame(
        std::vector<unsigned char> data,
        unsigned int w, unsigned h,
        unsigned int _x = 0, unsigned _y = 0
    ) : x { _x }, y { _y }, width { w }, height { h } {
        dataPtr = std::make_shared<std::vector<unsigned char>>(data);
    }
// data
    unsigned int x, y;
    unsigned int width, height;
    std::shared_ptr<std::vector<unsigned char>> dataPtr;
};

using Frame = std::variant<IAFrame, FAFrame>;


// -----------------------------------------------Frameworks-------------------------------------------------------

class EasingCurve {
public:
    using EasingCurveFunc = float (*)(float);

    enum ECType {
        CUSTOM      = 0,
        LINEAR      = 1,
        // quad
        IN_QUAD     = 2,
        OUT_QUAD    = 3,
        IN_OUT_QUAD = 4,
        // CUBIC
        IN_CUBIC     = 5,
        OUT_CUBIC    = 6,
        IN_OUT_CUBIC = 7,
        // sin
        IN_SIN      = 20,
        OUT_SIN     = 21,
        IN_OUT_SIN  = 22,
        // elastic
        IN_ELASTIC      = 40,
        OUT_ELASTIC     = 41,
        IN_OUT_ELASTIC  = 42,
    };

public:
    EasingCurve(ECType ect = ECType::LINEAR, EasingCurveFunc ecf = nullptr) : __mECType { ect }, __mECF { ecf } {
        assert(__mECType != ECType::CUSTOM || __mECF != nullptr);
    }

    float operator()(float val, float base) {
        switch (__mECType)  {
            case ECType::CUSTOM: return __mECF(val / base) * base;
            // quad
            case ECType::IN_QUAD: return InQuad(val / base) * base;
            case ECType::OUT_QUAD: return OutQuad(val / base) * base;
            case ECType::IN_OUT_QUAD: return InOutQuad(val / base) * base;
            // cubic
            case ECType::IN_CUBIC: return InCubic(val / base) * base;
            case ECType::OUT_CUBIC: return OutCubic(val / base) * base;
            case ECType::IN_OUT_CUBIC: return InOutCubic(val / base) * base;
            // sin
            case ECType::IN_SIN: return InSin(val / base) * base;
            case ECType::OUT_SIN: return OutSin(val / base) * base;
            case ECType::IN_OUT_SIN: return InOutSin(val / base) * base;
            // elastic
            case ECType::IN_ELASTIC: return InElastic(val / base) * base;
            case ECType::OUT_ELASTIC: return OutElastic(val / base) * base;
            case ECType::IN_OUT_ELASTIC: return InOutElastic(val / base) * base;
            default: return val; // ECType::LINEAR
        }
    }

protected:

    // Quadratic
    float InQuad(float x) {
        return x * x;
    }

    float OutQuad(float x) {
        return -x * (x - 2);
    }

    float InOutQuad(float x) {
        if (x < 0.5) {
            return 0.5 * InQuad(x * 2);
        } else {
            return (0.5 * OutQuad(x * 2 - 1) + 0.5);
        }
    }

    // Cubic
    float InCubic(float x) {
        return x * x * x;
    }

    float OutCubic(float x) {
        return 1.0 - pow(1.0 - x, 3);
    }

    float InOutCubic(float x) {
        if (x < 0.5) {
            return 0.5 * InCubic(2.0 * x);
        } else {
            return 0.5 * OutCubic(2.0 * x - 1.0) + 0.5;
        }
    }

    // Sin
    float InSin(float x) {
        return 1.0 - std::cos(x * (HANIM_PI / 2));
    }

    float OutSin(float x) {
        return std::sin(x * (HANIM_PI / 2));
    }

    float InOutSin(float x) {
        if (x < 0.5) {
            return 0.5 * InSin(x * 2);
        } else {
            return 0.5 * OutSin(x * 2 - 1) + 0.5;
        }
    }

    // Elastic
    float InElastic(float x) {
        const double c4 = (2 * HANIM_PI) / 3;
        if (x == 0 || x == 1) {
            return x;
        } else {
            return -std::pow(2, 10 * x - 10) * std::sin((x * 10 - 10.75) * c4);
        }
    }

    float OutElastic(float x) {
        const double c4 = (2 * HANIM_PI) / 3;
        if (x == 0 || x == 1) {
            return x;
        } else {
            return std::pow(2, -10 * x) * std::sin((x * 10 - 0.75) * c4) + 1;
        }
    }

    float InOutElastic(float x) {
        if (x < 0.5) {
            return 0.5 * InElastic(x * 2);
        } else {
            return (0.5 * OutElastic(x * 2 - 1) + 0.5);
        }
    }

private:
    ECType __mECType;
    EasingCurveFunc __mECF;
};


template<typename HType>
static void __HEngineUnregister(HType *ptr);

class HAnimate { // Animate Tree

    HANIM_CLONE(HAnimate);

    friend class HEngine;
public:

    enum AType {
        ATREE = -1,
        CANIM = 0, // custom anim
        IANIM = 1, // interpolation anim
        FANIM = 2, // frame anime
    };

    enum class PlayType {
        Unknown,
        OneShot,
        Repeat,
        RT, // come/back
    };

    enum class Status {
        Stopped,
        Running,
        Paused,
        Finished
    };

    enum Direction {
        Forward = 1,
        Backward = -1
    };

    struct Config {
        PlayType playType = PlayType::OneShot;
        Direction direction = Direction::Forward;
        EasingCurve easingCurve = EasingCurve();
        unsigned int frameNums = 60;  
    };

public: // big-five
    HAnimate() :
        _mSubType { 0 },
        _mFrameNums { 120 },
        __mAutoPlayFlag { 0 },
        __mType { AType::ATREE },
        __mPlayType { PlayType::OneShot },
        __mStatus { Status::Stopped },
        __mCurrentFrames { 0 },
        __mDirection { Direction::Forward },
        __mEasingCurve {},
        __mAnimVec {},
        __mFrameTrackVec {} { /* body */ }

    HAnimate(AType aType, Config config) : HAnimate() {
        __mType = aType;
        __mPlayType = config.playType;
        __mDirection = config.direction;
        __mEasingCurve = config.easingCurve;
        _mFrameNums = config.frameNums;
    }

    HAnimate(const HAnimate &anim) { *this = anim; }
    HAnimate &operator=(const HAnimate &anim) {
        _mSubType = anim._mSubType;
        _mFrameNums = anim._mFrameNums;

        // HEngine data
        __mAutoPlayFlag = anim.__mAutoPlayFlag;
        __mType = anim.__mType;
        __mPlayType = anim.__mPlayType;
        __mStatus = anim.__mStatus;
        __mCurrentFrames = anim.__mCurrentFrames;
        __mDirection = anim.__mDirection;
        __mEasingCurve = anim.__mEasingCurve;

        // Anim Tree Data: need HANIM_CLONE to support -> __clone()
        for (auto ptr : anim.__mAnimVec) {
            __mAnimVec.push_back(ptr->__clone());
        }
        __mFrameTrackVec = anim.__mFrameTrackVec;

        return *this;
    }

    HAnimate(const HAnimate &&anim) { *this = std::move(anim); }
    HAnimate &operator=(HAnimate &&anim) {
        _mSubType = anim._mSubType; anim._mSubType = 0;
        _mFrameNums = anim._mFrameNums; anim._mFrameNums = 0;

        // HEngine data
        __mAutoPlayFlag = anim.__mAutoPlayFlag; anim.__mAutoPlayFlag = 0;
        __mType = anim.__mType; anim.__mType = AType::ATREE;
        __mPlayType = anim.__mPlayType; anim.__mPlayType = PlayType::OneShot;
        __mStatus = anim.__mStatus; anim.__mStatus = Status::Stopped;
        __mCurrentFrames = anim.__mCurrentFrames; anim.__mCurrentFrames = 0;
        __mDirection = anim.__mDirection; anim.__mDirection = Direction::Forward;
        __mEasingCurve = anim.__mEasingCurve; anim.__mEasingCurve = EasingCurve(); // TODO

        // Anim Tree Data
        __mAnimVec = std::move(anim.__mAnimVec);
        __mFrameTrackVec = std::move(anim.__mFrameTrackVec);

        return *this;
    }

    virtual ~HAnimate() {
        __mStatus = Status::Stopped;
        __HEngineUnregister<HAnimate>(this);
    }

public: // basic info
    // getter
    int getFrameNums() const { return _mFrameNums; }

    PlayType getPlayType() const { return __mPlayType; }

    int getCurrentFrame() const { return __mCurrentFrames; }

    // setter
    HAnimate & setFrameNums(int frames, bool syncScale = false) {
        if (syncScale) {
            float scale = frames * 1.f / _mFrameNums;
            for (int i = 0; i < __mAnimVec.size(); i++) {
                __mFrameTrackVec[i] *= scale;
                __mAnimVec[i]->setFrameNums(__mAnimVec[i]->_mFrameNums * scale, true);
            }
        }
        _mFrameNums = frames;
        return *this;
    }

    HAnimate & setEasingCurve(EasingCurve ec) {
        __mEasingCurve = ec;
        return *this;
    }

    HAnimate & setDirection(Direction direction) {
        __mDirection = direction;
        return *this;
    }

    HAnimate & setPlayType(PlayType pType) {
        __mPlayType = pType;
        return *this;
    }

    HAnimate & setCurrentFrame(int frameIndex) {

        assert(frameIndex >= 0);

        if (frameIndex < _mFrameNums) {
            __mCurrentFrames = frameIndex;
        } else {
            __mCurrentFrames = _mFrameNums;
        }

        for (int i = 0; i < __mAnimVec.size(); i++) {
            int subFrameIndex = 0;
            if (frameIndex > __mFrameTrackVec[i])
                subFrameIndex = frameIndex - __mFrameTrackVec[i];
            __mAnimVec[i]->setCurrentFrame(subFrameIndex);
        }

        return *this;
    }

public: // status control
    virtual void start(bool forceUpdate = false) {

        if (!forceUpdate && __mStatus == Status::Running) return;

        __mStatus = Status::Running;
        __mCurrentFrames = 0;

        if (__mType == AType::ATREE) {
            [ & ] {
                for (auto aPtr : this->__mAnimVec)
                    aPtr->start(forceUpdate);
            }();
        };
    }

    virtual void pause() {
        __mStatus = Status::Paused;
    }

    virtual void resume() {
        if (__mStatus == Status::Paused)
            __mStatus = Status::Running;
    }

    virtual void stop() {
        __mStatus = Status::Stopped;
        __mCurrentFrames = 0;
    }

    virtual Status status() {
        return __mStatus;
    }
public: // Anim Tree OP
    template <typename T>
    HAnimate & addAnim(const T & anim, int frameTrackIndex = 0) {
        __mAnimVec.push_back(std::make_shared<T>(anim));
        __mFrameTrackVec.push_back(frameTrackIndex);
        return *(__mAnimVec.back());
    }

    void setFrameTrackIndex(int frameTrackIndex) {
        __mFrameTrackVec.back() = frameTrackIndex;
    }
protected: // interface
    virtual Frame _nextFrame(float fIndex) { return IAFrame(); } // TODO: balance Anim and Anim Node 
protected:
    int _mSubType;
    unsigned int _mFrameNums;
private: // HEngine data
    unsigned long long __mAutoPlayFlag;
    AType __mType;
    PlayType __mPlayType;
    Status __mStatus;
    int __mCurrentFrames;
    float __mDirection;
    /*
        __mEasingCurve(__mCurrentFrames) / real-frames
            ^
            |
            |
            |
            +------------> __mCurrentFrames
         (0, 0)
    */
    EasingCurve __mEasingCurve;

    // Anim Tree Data
    std::vector<std::shared_ptr<HAnimate>> __mAnimVec;
    std::vector<int> __mFrameTrackVec; // index for _mFrameNums(frame mainline)
};



class HObject {
    friend class HEngine;
public: // type
    using IAnimCallBack = std::function<void (int, const IAFrame &)>;
    using FAnimCallBack = std::function<void (int, const FAFrame &)>;
public: // contor
    HObject() : __mAutoPlayFlag { 0 }, __mIAnimCB { nullptr }, __mFAnimCB { nullptr } { }
    HObject(IAnimCallBack iacb) : HObject(iacb, nullptr) { }
    HObject(FAnimCallBack facb) : HObject(nullptr, facb) { }
    HObject(IAnimCallBack iacb, FAnimCallBack facb) : __mIAnimCB { iacb }, __mFAnimCB { facb } { }

    HObject(const HObject &hObj) { *this = hObj; }
    HObject & operator=(const HObject &hObj) {
        __mAutoPlayFlag = hObj.__mAutoPlayFlag;
        __mIAnimCB = hObj.__mIAnimCB;
        __mFAnimCB = hObj.__mFAnimCB;
        return *this;
    }

    HObject(HObject &&hObj) { *this = std::move(hObj); }
    HObject & operator=(HObject &&hObj) {
        __mAutoPlayFlag = hObj.__mAutoPlayFlag; hObj.__mAutoPlayFlag = 0;
        __mIAnimCB = std::move(hObj.__mIAnimCB);
        __mFAnimCB = std::move(hObj.__mFAnimCB);
        return *this;
    }

    virtual ~HObject() {
        __HEngineUnregister<HObject>(this);
    }

protected: // anim interface
    virtual void _interpolationHAnimate(int type, const IAFrame &frame) {
        if(__mIAnimCB) __mIAnimCB(type, frame);
    }

    virtual void _frameHAnimate(int type, const FAFrame &frame) {
        if(__mFAnimCB) __mFAnimCB(type, frame);
    }

    virtual void _render() { }

    virtual void _syncData() { }
private:
    unsigned long long __mAutoPlayFlag;
    IAnimCallBack __mIAnimCB; // for support tmp obj
    FAnimCallBack __mFAnimCB; // for support tmp obj
};


class HEngine {

/////////////////////////////////////////////// 

public:

    static void Play(HAnimate &anim, HObject &hObj) {
        anim.start();
        _register(&anim, &hObj);
    }

    static void setFrameRate(unsigned int frameRate) {
        if (frameRate > 0 && frameRate < 1000) {
            __Instance().__mFrameRate = frameRate;
            __Instance().__mDida.setInterval((1000 * 1000) / frameRate);
        }
    }

    static unsigned int getFrameRate() {
        return __Instance().__mFrameRate;
    }


public: // TODO: optimize reg/unreg from one frame to one anim-frame
    static void _register(HAnimate *animPtr, HObject *hObjPtr) {
        __Instance().__mDida.aopRegister({animPtr, hObjPtr});
    }

    static void _unregister(HAnimate *ptr) {
        if (ptr->__mAutoPlayFlag != HANIM_VERIFY)
            __Instance().__mDida.aopUnregister(ptr);
    }

    static void _unregister(HObject *ptr) {
        if (ptr->__mAutoPlayFlag != HANIM_VERIFY)
            __Instance().__mDida.aopUnregister(ptr);
    }

private:
    class __Dida {
    public:
        struct AOPair {
            HAnimate *anim;
            HObject *obj;
        };
    public:
        __Dida(unsigned int microSec) : __mExit { false }, __mTriggerInterval { microSec }, __mUpdateMutex{}, __mAOVec{} {
            __mAnimDriverThread = std::move(std::thread(
                [&] {
                    while (!(this->__mExit)) {
                        {
                            std::lock_guard<std::mutex> _al(__mUpdateMutex);
                            for (int i = 0; i < this->__mAOVec.size();) {
                                try {
                                    //bool objInvalid = !(__mAOVec[i].anim->__mAutoPlayFlag && __mAOVec[i].obj->__mAutoPlayFlag);
                                    if (__mAOVec[i].anim->status() == HAnimate::Status::Finished) {
                                        __mAOVec[i].anim->__mAutoPlayFlag = __mAOVec[i].obj->__mAutoPlayFlag = 0;
                                        __mAOVec.erase(__mAOVec.begin() + i);
                                        continue;
                                    } else {
                                        PlayFrame(*(__mAOVec[i].anim), *(__mAOVec[i].obj));
                                    }

                                } catch(std::exception &e) {
                                    std::cout << "exception: " << e.what() << std::endl;
                                }
                                
                                i++;
                            }
                        }
                        // TODO: optimize by time compute
                        std::this_thread::sleep_for(std::chrono::microseconds(__mTriggerInterval));
                    }
                }
            ));
        }

        ~__Dida() {
            __mExit = true;
            if (__mAnimDriverThread.joinable())
                __mAnimDriverThread.join();
        }

    public:
        void setInterval(unsigned int microSec) {
            __mTriggerInterval = microSec;
        }

        void aopRegister(AOPair &&aop) {

            assert(aop.anim->__mAutoPlayFlag != HANIM_VERIFY && aop.obj->__mAutoPlayFlag != HANIM_VERIFY);

            aop.anim->__mAutoPlayFlag = aop.obj->__mAutoPlayFlag = HANIM_VERIFY;

            std::lock_guard<std::mutex> _al(__mUpdateMutex);
            __mAOVec.emplace_back(std::forward<AOPair>(aop));

        }

        void aopUnregister(void *objPtr) {
            std::lock_guard<std::mutex> _al(__mUpdateMutex);
            for (int i = 0; i < __mAOVec.size(); i++) {
                if (__mAOVec[i].anim == objPtr || __mAOVec[i].obj == objPtr) {
                    __mAOVec[i].anim->__mAutoPlayFlag = __mAOVec[i].obj->__mAutoPlayFlag = 0;
                    __mAOVec.erase(__mAOVec.begin() + i);
                    break;
                }
            }
        }

    private:
        bool __mExit;
        unsigned int __mTriggerInterval; // micro-sec
        std::mutex __mUpdateMutex;
        std::thread __mAnimDriverThread;
        std::vector<AOPair> __mAOVec; // TODO: use dlist
    };

private:
    unsigned int __mFrameRate;
    __Dida __mDida; // Anim Driver/Ticker/Timer

/////////////////////////////////////////////// 

public:
    // Animate Manager
    class AManager {
    public:
        static void setAnimateId(unsigned int id) {
            __Instance().__mCurrentId = id;
        }

        static void resetAnimateId() {
            __Instance().__mCurrentId = 0;
        }

        template <typename AnimateType, typename... Args>
        static std::weak_ptr<AnimateType> registerAnimate(Args&&... args) {
            unsigned int id = __Instance().__mCurrentId;

            assert(id != 0);

            if (__Instance().__mAnimates.find(id) == __Instance().__mAnimates.end()) {
                //printf("create Animate: %d\n", id);
                __Instance().__mAnimates[id] = std::make_shared<AnimateType>(args...);
            } else {
                //printf("Animate already created: %d\n", id);
            }

            return std::dynamic_pointer_cast<AnimateType>(__Instance().__mAnimates[id]);
        }

    private:
        unsigned int __mCurrentId;
        std::map<unsigned int, std::shared_ptr<HAnimate>> __mAnimates;

        AManager() : __mCurrentId { 0 } { }
        AManager(AManager &) = delete;
        AManager(AManager &&) = delete;

        static AManager & __Instance() {
            static AManager am;
            return am;
        }
    };

public: // base-play interface for user

    template <typename HObjectCallback>
    static void PlayFrame(unsigned int id, HAnimate &anim, HObjectCallback cb) {
        PlayFrame(id, anim, HObject(cb));
    }

    static void PlayFrame(unsigned int id, HAnimate &anim, HObject &hObj) {
        AManager::setAnimateId(id);

    }

    static void PlayFrame(HAnimate &anim, HObject::IAnimCallBack iacb) {
        HEngine::PlayFrame(anim, HObject(iacb));
    }

    static void PlayFrame(HAnimate &anim, HObject::FAnimCallBack facb) {
        HEngine::PlayFrame(anim, HObject(facb));
    }

    static void PlayFrame(HAnimate &anim, HObject &&hObj) { PlayFrame(anim, hObj); }
    static void PlayFrame(HAnimate &anim, HObject &hObj) {
        __Instance().__PlayFrame(anim, hObj, true);
    }

private:
    HEngine() : __mFrameRate { 60 }, __mDida { (1000 * 1000) / __mFrameRate } { }
    HEngine(const HEngine&) = delete;
    HEngine & operator=(const HEngine&) = delete;

    static HEngine & __Instance() {
        static HEngine ae;
        return ae;
    }

    // play direct
    // anim status
    void __statusSync(HAnimate &anim) {
        if (anim.__mType == HAnimate::ATREE) {
            int cnt = 0;
            for (auto nodePtr : anim.__mAnimVec) {
                nodePtr->__mDirection = anim.__mDirection;
                nodePtr->__mStatus = anim.__mStatus;
                __statusSync(*nodePtr);
            }
        }
    }

    void __PlayFrame(HAnimate &anim, HObject &hObj, bool render = true) {
        float frameIndex = 0;
        int subType =  anim._mSubType;

        if (anim.__mStatus == HAnimate::Status::Running) {
            if (anim.__mCurrentFrames == 0) {
                anim.__mDirection = HAnimate::Direction::Forward;
                __Instance().__statusSync(anim);
            } else if (anim.__mCurrentFrames == anim._mFrameNums + 1) {
                anim.__mCurrentFrames--;
                if (anim.__mDirection == HAnimate::Direction::Forward)
                    subType = -1; // anim  end
            }

            frameIndex = anim.__mEasingCurve(anim.__mCurrentFrames, anim._mFrameNums);

            auto frame = anim.__mType == HAnimate::AType::ATREE ? Frame() : anim._nextFrame(frameIndex);

            switch (anim.__mType) {
                case HAnimate::AType::IANIM:
                    hObj._interpolationHAnimate(subType, std::get<IAFrame>(frame));
                    break;
                case HAnimate::AType::FANIM:
                    hObj._frameHAnimate(subType, std::get<FAFrame>(frame));
                    break;
                case HAnimate::AType::ATREE:
                    printf("HHHH\n");
                    for (int i = 0; i < anim.__mAnimVec.size(); i++) {
                        if (anim.__mCurrentFrames >= anim.__mFrameTrackVec[i]) {
                            if (
                                anim.__mAnimVec[i]->__mPlayType == HAnimate::PlayType::OneShot &&
                                anim.__mFrameTrackVec[i] + anim.__mAnimVec[i]->_mFrameNums < anim.__mCurrentFrames
                            ) continue;

                            __PlayFrame(*(anim.__mAnimVec[i]), hObj, false);
                        }
                    }
                default: break;
            }

            if (subType == -1) { // anim end
                if (anim.__mPlayType == HAnimate::PlayType::RT) {
                    anim.__mDirection = HAnimate::Direction::Backward;
                    __Instance().__statusSync(anim);
                } else if (anim.__mPlayType == HAnimate::PlayType::Repeat) {
                    anim.start(true);
                    //TODO: __mCurrentFrames = -1 (0-frame play issue)
                } else if (anim.__mPlayType == HAnimate::PlayType::OneShot) {
                    anim.__mStatus = HAnimate::Status::Finished;
                    //end when anim.__mCurrentFrames = anim.__mFramesNUmbers + 1
                }
            }

            anim.__mCurrentFrames += anim.__mDirection;

        } else {
            if (hObj.__mIAnimCB) {
                hObj._interpolationHAnimate(-1 /*subType*/, std::get<IAFrame>(anim._nextFrame(anim._mFrameNums)));
            }

            if (hObj.__mFAnimCB) {
                hObj._frameHAnimate(-1 /*subType*/, std::get<FAFrame>(anim._nextFrame(anim._mFrameNums)));
            }
        }

        hObj._syncData(); // sync anim data

        if (render)
            hObj._render(); // render anim data

    }
};

template<typename HType>
static void __HEngineUnregister(HType *ptr) {
    HEngine::_unregister(ptr);
}


// -----------------------------------------------Animate-------------------------------------------------------


class InterpolationAnim : public HAnimate {

    HANIM_CLONE(InterpolationAnim);

public:
    using PathFunc = std::function<float (float)>;

    enum class Var { N, X, Y };

    enum IAType {
        CUSTOM   = 0,
        MOVE     = 1,
        SCALE      = 11,
        SCALE_1    = SCALE,
        SCALE_2    = 12,
        ALPHA    = 20,
        GRADIENT = 30,
        Rotation = 40,
        PATH     = 50,
    };

public:
    InterpolationAnim(IAType subType = IAType::MOVE, Config config = Config()) :
        HAnimate(AType::IANIM, config),
        _mVarFlag { Var::N },
        _mStartFrame(0),
        _mEndFrame(0),
        _mPathFunc { [](float x) { return x; } },
        _mKeyFrameVec(1, IAFrame({1,2,3}))
    {
        _mSubType = subType;
    }

    InterpolationAnim(const InterpolationAnim &iAnim) { *this = iAnim; }
    InterpolationAnim & operator=(const InterpolationAnim &iAnim) {
        HAnimate::operator=(iAnim);

        _mVarFlag = iAnim._mVarFlag;
        _mStartFrame = iAnim._mStartFrame;
        _mEndFrame = iAnim._mEndFrame;
        _mPathFunc = iAnim._mPathFunc;
        _mKeyFrameVec = iAnim._mKeyFrameVec;

        return *this;
    }

    InterpolationAnim(InterpolationAnim &&iAnim) { *this = std::move(iAnim); }
    InterpolationAnim & operator=(InterpolationAnim &&iAnim) {
        HAnimate::operator=(std::forward<InterpolationAnim>(iAnim));

        _mVarFlag = iAnim._mVarFlag; iAnim._mVarFlag = Var::N;
        _mStartFrame = std::move(iAnim._mStartFrame);
        _mEndFrame = std::move(iAnim._mEndFrame);
        _mPathFunc = std::move(iAnim._mPathFunc);
        _mKeyFrameVec = std::move(iAnim._mKeyFrameVec);

        return *this;
    }

public:
    void setStartFrame(const IAFrame &startFrame) { _mStartFrame = startFrame; }
    void setEndFrame(const IAFrame &endFrame) { _mEndFrame = endFrame; }

public:
    void addPathKeyFrame(IAFrame frame) {
        _mKeyFrameVec.push_back(frame);
    }

    template <Var VarFlag>
    void setInterval(float start, float end) {
        _mStartFrame.data[0] = start;
        _mEndFrame.data[0] = end;
        _mVarFlag = VarFlag;
        _mKeyFrameVec.resize(1, IAFrame());
    }

    void setPathFunc(PathFunc pFunc) {
        _mPathFunc = pFunc;
    }

protected:

    virtual Frame _nextFrame(float frameIndex) override {
        if (_mSubType != IAType::PATH) {
            _mKeyFrameVec[0] = _mStartFrame;
            for (int i = 0; i < _mKeyFrameVec[0].data.size(); i++) {
                float delta = (_mEndFrame.data[i] - _mStartFrame.data[i]) / _mFrameNums;
                _mKeyFrameVec[0].data[i] = _mStartFrame.data[i] + delta * frameIndex;
            }
        } else {
            if (_mVarFlag == Var::N) {
                //  0 --- 1 --- 2 --- 3 --- 4  -> 4 key-frame
                // [N]    |     |     |     |  -> 3 frame data
                float realFrameIndex = frameIndex / _mFrameNums * (_mKeyFrameVec.size() - 1 - 1) + 1;
                float offset = realFrameIndex - static_cast<int>(realFrameIndex);
                float interpolationNums = static_cast<float>(_mFrameNums) / (_mKeyFrameVec.size() - 1);

                _mKeyFrameVec[0].data[0] = _mKeyFrameVec[realFrameIndex].data[0];
                _mKeyFrameVec[0].data[1] = _mKeyFrameVec[realFrameIndex].data[1];

                if (
                    offset > 0 /*.01 */ &&
                    interpolationNums >= 1.5 &&
                    realFrameIndex < _mKeyFrameVec.size() - 1
                ) {
                    float nextX = _mKeyFrameVec[realFrameIndex + 1].data[0];
                    float nextY = _mKeyFrameVec[realFrameIndex + 1].data[1];
                    float deltaX = (nextX - _mKeyFrameVec[realFrameIndex].data[0]) * offset;
                    float deltaY = (nextY - _mKeyFrameVec[realFrameIndex].data[1]) * offset;
                    _mKeyFrameVec[0].data[0] += deltaX;
                    _mKeyFrameVec[0].data[1] += deltaY;
                }
            } else {
                _mKeyFrameVec[0].data[0] = _mStartFrame.data[0] + (_mEndFrame.data[0] - _mStartFrame.data[0]) / _mFrameNums * frameIndex;
                _mKeyFrameVec[0].data[1] = _mPathFunc(_mKeyFrameVec[0].data[0]);

                if (_mVarFlag == Var::Y) std::swap(_mKeyFrameVec[0].data[0], _mKeyFrameVec[0].data[1]);
            }
        }

        return _mKeyFrameVec[0];
    }

protected:
    Var _mVarFlag;
    IAFrame _mStartFrame, _mEndFrame; // [_mStartFrame, _mEndFrame)
    PathFunc _mPathFunc;
    std::vector<IAFrame> _mKeyFrameVec;
};

struct Move : public InterpolationAnim {
    Move(HVec2 posStart, HVec2 posEnd, Config config = Config()) :
        InterpolationAnim(InterpolationAnim::IAType::MOVE, config)
    {
        // config
        this->setStartFrame(posStart);
        this->setEndFrame(posEnd);
        this->start();

    }
};

struct Scale : public InterpolationAnim {
    Scale(HVec2 whStart, HVec2 whEnd, Config config = Config()) :
        InterpolationAnim(InterpolationAnim::IAType::SCALE, config)
    {
        // config
        this->setStartFrame(whStart);
        this->setEndFrame(whEnd);
        this->start();
    }
};

struct Opacity : public InterpolationAnim {
    Opacity(float alpha1, float alpha2, Config config = Config()) :
         InterpolationAnim(InterpolationAnim::IAType::ALPHA, config)
    {
        // config
        this->setStartFrame(alpha1);
        this->setEndFrame(alpha2);
        this->start();
    }
};

struct Gradient : public InterpolationAnim {
    Gradient(HVec3 rgbStart, HVec3 rgbEnd, Config config = Config()) :
        InterpolationAnim(InterpolationAnim::IAType::GRADIENT, config)
    {
        // config
        this->setStartFrame(rgbStart);
        this->setEndFrame(rgbEnd);
        this->start();
    }

    Gradient(HVec4 rgbaStart, HVec4 rgbaEnd, Config config = Config()) :
        InterpolationAnim(InterpolationAnim::IAType::GRADIENT, config)
    {
        // config
        this->setStartFrame(rgbaStart);
        this->setEndFrame(rgbaEnd);
        this->start();
    }

};

// TODO: FrameAnim
class FrameAnim;

}

#endif