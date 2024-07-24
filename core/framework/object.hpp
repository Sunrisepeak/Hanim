#ifndef OBJECT_HPP_FWK_HANIM
#define OBJECT_HPP_FWK_HANIM


#include <memory>
#include <vector>
#include <functional>

#include <gl_interface.h>

#include "utils/math.hpp"

namespace hanim {

class HAnimate;

struct HAABB {
    vec3 min;
    vec3 max;
};

class HObject { // HObject(data) is Ref Counter Type

    friend bool operator==(const HObject &obj1, const HObject &obj2);
    friend class Interpolator;
 
 private:
    // use RC to wrapper data
    struct ObjectData {
        bool componentMode;
        DrawMode drawMode; // GLI_NONE is off - default
        vec4 fillRgbs;
        float thickness;
        Points points;
        Colors rgbs;
        vec3 center;
        HAABB aabb;
        bool active;
        // only allow one layer, components isn't tree
        std::vector<HObject> components;

        ObjectData() :
            componentMode { false },
            drawMode { GLI_NONE },
            fillRgbs {0, 0, 0, 0},
            thickness { 1 },
            points {},
            rgbs {},
            center { 0 },
            aabb { vec3(0), vec3(0) },
            components {}
        {
            // nothing
        }
    };

public:

    HObject() : mMoveSemaFlag { true }, mData { std::make_shared<ObjectData>() } {

    }

    HObject(const HObject &obj) : HObject() {
        *mData = *(obj.mData);
    }

    HObject & operator=(const HObject &obj) {
        if (mData != obj.mData) {
            *mData = *(obj.mData);
        }
        return *this;
    }

    HObject(HObject &&obj)  {
        mMoveSemaFlag = true;
        if (obj.mMoveSemaFlag) {
            mData = obj.mData;
            obj.mData = std::make_shared<ObjectData>();
        } else {
            mData = std::make_shared<ObjectData>();
            *mData = *(obj.mData);
            HONLY_LOGI(
                "mMoveSemaFlag is false - create new obj and only copy(%p -> %p)",
                &obj, this
            );
        }
    }

    HObject & operator=(HObject &&obj) {
        if (mData != obj.mData) {
            // only obj1 = obj2;
            if (mMoveSemaFlag && obj.mMoveSemaFlag) { // move data
                mData = obj.mData;
                //mMoveSemaFlag = obj.mMoveSemaFlag;
                // reset
                obj.mData = std::make_shared<ObjectData>();
                //obj.mMoveSemaFlag = true;
            } else { // copy
                *mData = *(obj.mData);
                HONLY_LOGI(
                    "mMoveSemaFlag a(%s) = b(%s) - only copy(%p -> %p)",
                    mMoveSemaFlag ? "true" : "false",
                    obj.mMoveSemaFlag ? "true" : "false",
                    &obj, this
                );
            }
        }
        return *this;
    }


public: // object control

    HObject && move_sema(bool moveSema) {
        mMoveSemaFlag = moveSema;
        return std::move(*this);
    }

    // Note: only ref mData member of Obj, rather than Obj
    void ref(HObject &obj) {
        mData = obj.mData;
    }

    // TODO: const issue
    HObject create_ref() {
        HObject obj;
        obj.ref(*this);
        return obj; // RVO
    }

    HObject clone() {
        HObject objRef = *this;
        return objRef; // RVO
    }

    int ref_count() {
        return mData.use_count();
    }

public: // interface 
    // template
    virtual void generate_object_data() final {

        if (!is_components()) {
            object_points_init(mData->points, mData->rgbs);
            if (mData->points.size() != mData->rgbs.size()) {
                HONLY_LOGW("points.size isn't equal rgbs.size.resize from %ld to %ld",
                    mData->rgbs.size(),
                    mData->points.size()
                );
                mData->rgbs.resize(mData->points.size(), mData->rgbs.back());
            }
        }

        compute_aabb();

        mData->center = (mData->aabb.min + mData->aabb.max) / 2.0f;

        HONLY_LOGD("center: %f %f %f - aabb: %f %f %f - %f %f %f",
            mData->center[0], mData->center[1], mData->center[2],
            mData->aabb.min[0], mData->aabb.min[1], mData->aabb.min[2],
            mData->aabb.max[0], mData->aabb.max[1], mData->aabb.max[2]
        );
    }

    virtual void object_points_init(Points &points, Colors &rgba) {
        // impl by sub-class
    }

    // obj-tree to obj-vector (only return pointNum > 0 obj)
    virtual std::vector</* const */ HObject *> getObjs() /* const */ {
        std::vector</* const */ HObject *> objs;
        if (mData->componentMode) {
            HONLY_LOGI("obj componets - %ld", mData->components.size());
            for (auto &obj : mData->components) {
                auto subObjs = obj.getObjs();
                if (subObjs.size() > 0) {
                    objs.reserve(objs.size() + subObjs.size());
                    objs.insert(
                        objs.end(),
                        subObjs.begin(),
                        subObjs.end()
                    );
                }
            }
        } else if (mData->points.size() > 0) {
                objs.push_back(this);
        }

        if (objs.size() == 0) {
            HONLY_LOGD("obj haven't any data(HAnimGroup Root?)");
        } else {
            HONLY_LOGD("objs.size - %ld", objs.size());
        }

        return objs;
    }

    // TODO: custom animate...
    virtual std::shared_ptr<HAnimate> animate(int tag) {
        return nullptr;
    }

public: // get

    vec3 get_center() const {
        return mData->center;
    }

    const HAABB & get_aabb() const {
        return mData->aabb;
    }

    DrawMode get_draw_mode() const {
        return mData->drawMode;
    }

    const Color & get_stroke_color() const {
        return mData->rgbs[0];
    }

    const Color & get_fill_color() const {
        return  mData->fillRgbs;
    }

    float get_stroke_opacity() const {
        return mData->rgbs[0][3];
    }

    float get_fill_opacity() const {
        return mData->fillRgbs[3];
    }

    float get_thickness() const {
        return  mData->thickness;
    }

    bool is_active() const {
        return mData->active;
    }

    bool is_components() const {
        return mData->componentMode;
    }

public: // status/mode setter

    HObject & draw_mode(DrawMode mode) {
        mData->drawMode = mode;
        return *this;
    }


public: // property setter

    HObject & move_to(const vec3 &target) {
        vec3 shiftValue = target - mData->center;
        shift(shiftValue);
        return *this;
    }

// only in-obj-comp（include root-node-obj
// Note: update root after sub-obj if SyncRoot is true
#define ComponentsForTemplate \
std::vector<decltype(mData)> datas; \
if (is_components()) { \
    auto objs = getObjs(); \
    datas.reserve(objs.size()); \
    for (auto objPtr : objs) datas.push_back(objPtr->mData); \
} \
datas.push_back(mData); \
for (auto &data : datas)

    HObject & shift(const vec3 &vec) {
        ComponentsForTemplate {
            for (vec3 &point : data->points) {
                point += vec;
            }
            data->center += vec;
            data->aabb.min += vec;
            data->aabb.max += vec;
        }
        return *this;
    }

    HObject & scale(float value) {
        if (value <= 0) {
            HONLY_LOGW("scale %lf <= 0 (reset to 0)", value);
            value = 0;
        }
        ComponentsForTemplate {
            for (auto &p : data->points) {
                point_center_helper(p, [&](auto &p) { p *= value; });
            }

            // TODO: verify - aabb
            point_center_helper(data->aabb.min, [value](auto &p) { p *= value; });
            point_center_helper(data->aabb.max, [value](auto &p) { p *= value; });

            if (data != mData) {
                data->center -= mData->center;
                data->center *= value;
                data->center += mData->center;
            }
        }
        return *this;
    }

    HObject & rotate(float angle, vec3 axis = {0, 0, 1}) {
        ComponentsForTemplate {
            mat4 rotationMatrix = glm::rotate(mat4(1.0f), glm::radians(angle), axis);
            for (vec3 &point : data->points) point -= mData->center;
            for (vec3 &point : data->points) {
                vec4 homogenousPoint = vec4(point, 1.0f);
                vec4 rotatedPoint = rotationMatrix * homogenousPoint;
                point = vec3(rotatedPoint);
            }
            for (vec3 &point : data->points) point += mData->center;

            if (data != mData) {
                data->center -= mData->center;
                data->center = vec3(rotationMatrix * vec4(data->center, 1.0f));
                data->center += mData->center;
            }
        }
        compute_aabb(); // TODO: optimize?
        return *this;
    }

    HObject & color(vec4 col) {
        stroke_color(col);
        fill_color(col);
        return *this;
    }

    HObject & stroke_color(Color col) {
        ComponentsForTemplate {
            for (auto &rgba : data->rgbs) {
                rgba = col;
            }
        }
        return *this;
    }

    HObject & stroke_colors(Colors cols) {
        if (cols.size() == 0) {
            HONLY_LOGW("cols size is 0");
        } else {
            // TODO: optimize
            Colors tmpCols;
            ComponentsForTemplate {
                int pointNumber = data->points.size();
                tmpCols = cols;
                if (tmpCols.size() > pointNumber) {
                    tmpCols.resize(pointNumber);
                } else {
                    int i = 0;
                    tmpCols.reserve(pointNumber);
                    while (tmpCols.size() < pointNumber) {
                        tmpCols.push_back(tmpCols[i++]);
                    }
                }
                data->rgbs = std::move(tmpCols);
            }
        }
        return *this;
    }

    HObject & fill_color(const vec4 &col) {
        ComponentsForTemplate {
            data->fillRgbs = col;
        }
        return *this;
    }

    HObject & opacity(float value) {
        stroke_opacity(value);
        fill_opacity(value);
        return *this;
    }

    HObject & stroke_opacity(float value) {
        ComponentsForTemplate {
            for (auto &rgba : data->rgbs) {
                rgba[3] = value;
            }
        }
        return *this;
    }

    HObject & fill_opacity(float value) {
        ComponentsForTemplate {
            data->fillRgbs[3] = value;
        }
        return *this;
    }

    HObject & opacity_factor(float factor) {
        ComponentsForTemplate {
            for (auto &rgba : data->rgbs) {
                rgba[3] *= factor;
            }
            data->fillRgbs[3] *= factor;
        }
        return *this;
    }

    HObject & thickness(float value = 1.0) {
        ComponentsForTemplate {
            data->thickness = value;
        }
        return *this;
    }

    HObject & active(bool enable) {
        ComponentsForTemplate {
            data->active = enable;
        }
        return *this;
    }

public: // static member

    static bool align_checker(const HObject &obj1, const HObject &obj2) {
        if (obj1.mData->componentMode != obj2.mData->componentMode) {
            HONLY_LOGW("obj1.mData->componentMode != obj2.mData->componentMode");
            return false;
        }

        if (obj1.mData->components.size() != obj2.mData->components.size()) {
            HONLY_LOGW("obj1 components.size() != obj2 components.size()");
            return false;
        }

        int pSize1 = 0, pSize2 = 0;

        pSize1 = obj1.mData->points.size();
        pSize2 = obj2.mData->points.size();

        if (pSize1 != pSize2) {
            HONLY_LOGW("obj1 points.size() != obj2 points.size()");
            return false;
        }

        for (int i = 0; i < obj1.mData->components.size(); i++) {
            pSize1 += obj1.mData->components[i].mData->points.size();
            pSize2 += obj2.mData->components[i].mData->points.size();
            if (pSize1 != pSize2) {
                HONLY_LOGW("components[%d] obj1 points %d != obj2 points %d",
                    i,
                    pSize1, pSize2
                );
                return false;
            }
        }

        HONLY_LOGI("data is aglin : [ componentMode: %s, componets: %ld, points: %d ]",
            obj1.mData->componentMode ? "true" : "false",
            obj1.mData->components.size(),
            pSize1
        );

        return true;
    }

    static void align_data(HObject &obj1, HObject &obj2) {
        if (obj1.mData->componentMode || obj2.mData->componentMode) {
            align_components(obj1, obj2);
            for (int i = 0; i < obj1.mData->components.size(); i++) {
                align_points(obj1.mData->components[i], obj2.mData->components[i]);
            }
        } else {
            align_points(obj1, obj2);
        }
    }

    static void align_points(HObject &obj1, HObject &obj2) {
        int size1 = obj1.mData->points.size();
        int size2 = obj2.mData->points.size();
        if (obj1.mData->points.size() == obj2.mData->points.size()) {
            HONLY_LOGI("obj1 == obj2 about points size - do nothing");
        } else if (size1 > size2) {
            align_data_style1(obj2, size1);
        } else {
            align_data_style1(obj1, size2);
        }
    }

    static void align_components(HObject &obj1, HObject &obj2) {
        // TODO: optimize
        HONLY_LOGI("start");
        if ((obj1.mData->componentMode == false && obj2.mData->componentMode == false)) {
            HONLY_LOGW("obj1 and obj2 aren't components");
        } else if (obj1.mData->componentMode && obj2.mData->componentMode) {
            // TODO: size1 == 0 ? - size2 / size1
            int size1 = obj1.mData->components.size();
            int size2 = obj2.mData->components.size();
            if (size1 < size2) {
                int dupNumber = size2 / size1;
                obj1.mData->components.reserve(size2);
                while (--dupNumber) {
                    for (int i = 0; i < size1; i++) {
                        obj1.mData->components.push_back(obj1.mData->components[i]);
                    }
                }
                for (int i = 0; i < size1 && size2 != obj1.mData->components.size(); i++) {
                    obj1.mData->components.push_back(obj1.mData->components[i]);
                }
            } else if (size1 > size2) {
                obj2.mData->components.reserve(size2);
                int dupNumber = size1 / size2;
                    while (--dupNumber) {
                        for (int i = 0; i < size2; i++) {
                            obj2.mData->components.push_back(obj2.mData->components[i]);
                        }
                    }
                for (int i = 0; i < size2 && size2 != obj2.mData->components.size(); i++) {
                    obj2.mData->components.push_back(obj2.mData->components[i]);
                }
            }
        } else if (!obj1.mData->componentMode) {
            int size = obj2.mData->components.size();
            if (size == 0) {
                HONLY_LOGE("obj2.mData->componets.size() is 0");
            }
            obj1.covert_to_components(size);
        } else if (!obj2.mData->componentMode) {
            int size = obj1.mData->components.size();
            if (size == 0) {
                HONLY_LOGE("obj1.mData->componets.size() is 0");
            }
            obj2.covert_to_components(size);
        }
    }

private:

    void compute_aabb() {
        if (!is_components()) {
            if (mData->points.size() > 0) {
                try_to_update_aabb(HAABB{mData->points[0], mData->points[0]}, true);
            }
            for (auto i = 1; i < mData->points.size(); i++) {
                try_to_update_aabb(HAABB{mData->points[i], mData->points[i]});
            }
        } else {

            if (mData->components.size() > 0) {
                mData->components[0].compute_aabb();
                try_to_update_aabb(mData->components[0].get_aabb());
            }

            for (int i = 1; i < mData->components.size(); i++) {
                // TODO: when mData->components[i] is null object
                // use aabb enable flag ? or use (0, 0, 0)?
                mData->components[i].compute_aabb();
                try_to_update_aabb(mData->components[i].get_aabb());
            }
        }
    }

    void try_to_update_aabb(const HAABB &aabb, bool force = false) {
        if (force) {
            mData->aabb = aabb;
        } else {
            for (int i = 0; i < 3; i++) {
                if (aabb.min[i] < mData->aabb.min[i]) {
                    mData->aabb.min[i] = aabb.min[i];
                }
                if (aabb.max[i] > mData->aabb.max[i]) {
                    mData->aabb.max[i] = aabb.max[i];
                }
            }
        }
    }

    using PointsHandler = std::function<void (vec3 &)>;
    void point_center_helper(vec3 &points, PointsHandler pHandler) {
        points -= mData->center;
        pHandler(points);
        points += mData->center;
    }

private: // static
    // obj1.size() < size
    static void align_data_style1(HObject &obj, int size) {
        int oldSize = obj.mData->points.size();
        obj.opacity_factor(1.0 / (size / oldSize));
        obj.mData->points.reserve(size);
        obj.mData->rgbs.reserve(size);
        for (int i = 0; i < size - oldSize; i++) {
            obj.mData->points.push_back(obj.mData->points[i % oldSize]);
            obj.mData->rgbs.push_back(obj.mData->rgbs[i % oldSize]);
        }
    }

    static void align_data_style2(HObject &obj, int size) {
        // TODO: optimize compute
        auto oldPoints = obj.mData->points;
        int oldSize = oldPoints.size();
        obj.mData->points.resize(size, obj.mData->points.back());
        obj.mData->rgbs.resize(size, obj.mData->rgbs.back());
        int cnt = size / oldSize;
        for (int i = 0; i < oldSize; i++) {
            for (int j = 0; j < cnt; j++) {
                obj.mData->points[i * cnt + j] = oldPoints[i];
                //obj.mData->rgbs[i * cnt + j] = oldRgbs[i];
            }
        }
        //HONLY_LOGI("oldsize(%d) size(%d) cnt(%d)", oldSize, size, cnt);
    }

public: // component
    // TODO: dont to modify subObj + const ? - center re-compute?
    // center compute issue?
    //    1. auto update
    //    2. lazy compute
    //    3. explicity compute
    HObject & operator[](int index) {
        if (false == mData->componentMode) {
            return *this;
        }
        if (index < 0) {
            index += mData->components.size();
        }
        if (index < 0 || index >= mData->components.size()) {
            HONLY_LOGE("index out range - %d(%ld)",
                index,
                mData->components.size()
            );
        }
        return mData->components[index];
    }

    // only one interface for create components
    HObject & add(HObject obj) {
        if (mData->points.size() == 0) {
            *this = obj;
        } else {
            if (false == mData->componentMode) {
                this->covert_to_components();
            }
            mData->components.push_back(obj);
            try_to_update_aabb(obj.get_aabb());
            mData->center = (mData->aabb.min + mData->aabb.max) / 2.0f;
        }
        return *this;
    }

    void covert_to_components(int extendNum = 1) {
        if (!mData->componentMode) {
            if (mData->points.size() > 0) {
                mData->components.resize(
                    extendNum, *this
                );
            }
            mData->componentMode = true;
            HONLY_LOGD("set componentMode -> true");
        }
    }

    int size() const {
        return is_components() ? mData->components.size() : 1;
    }

protected:
    // only use in move-sema
    // control:
    //      obj.move_sema(bool)
    // Example:
    //     HObject func() { HObject obj; ... return obj;}
    //     obj = func(); the will triggle move-sema
    bool mMoveSemaFlag;
    std::shared_ptr<ObjectData> mData;

public:

    // TODO: optimize
    std::shared_ptr<ObjectData> _get_data() {
        return mData;
    }
};

// Note: addr rather than value
bool operator==(const HObject &obj1, const HObject &obj2) {
    return obj1.mData == obj2.mData;
}

bool operator!=(const HObject &obj1, const HObject &obj2) {
    return !(operator==(obj1, obj2));
}

} // hanim

#endif