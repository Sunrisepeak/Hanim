#ifndef INTERPOLATE_HPP_FWK_HANIM
#define INTERPOLATE_HPP_FWK_HANIM

#include "core/framework/object.hpp"

namespace hanim {

struct Interpolator {
    template <typename T>
    static T vector(const T &data1, const T &data2, float alpha) {
        T data = data1;
        int size = data1.size() > data2.size() ? data2.size() : data1.size();
        for (int i = 0; i < size; i++) {
            data[i] = Interpolator::value(data1[i], data2[i], alpha);
        }
        return data;
    }

    template <typename T>
    static T value(const T &data1, const T &data2, float alpha) {
        T data = data1;
        if (data1 != data2) {
            data = data1 + (data2 - data1) * alpha;
        }
        return data;
    }

    static HObject hobject(const HObject &obj1, const HObject &obj2, float alpha) {
        HObject obj;
        HObject::align_checker(obj1, obj2);
        if (obj1.is_components() || obj2.is_components()) {
            obj = obj1;
            for (int i = 0; i < obj1.mData->components.size(); i++) {
                obj.mData->components[i] = Interpolator::hobject(
                    obj1.mData->components[i], obj2.mData->components[i],
                    alpha
                );
            }
        } else {
            obj.mData->points = Interpolator::vector(
                obj1.mData->points, obj2.mData->points,
                alpha
            );
            obj.mData->rgbs = Interpolator::vector(
                obj1.mData->rgbs, obj2.mData->rgbs,
                alpha
            );
            obj.mData->fillRgbs = Interpolator::value(
                obj1.mData->fillRgbs, obj2.mData->fillRgbs,
                alpha
            );
            obj.mData->center = Interpolator::value(
                obj1.mData->center, obj2.mData->center,
                alpha
            );
            obj.mData->thickness = Interpolator::value(
                obj1.mData->thickness, obj2.mData->thickness,
                alpha
            );
        }
        return obj;
    }
};

} // hanim

#endif