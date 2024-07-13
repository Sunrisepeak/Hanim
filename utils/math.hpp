#ifndef MATH_HPP_HANIM
#define MATH_HPP_HANIM

#include <third-party/third_party_header.hpp>

using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat4 = glm::mat4;

using Point = vec3;
using Color = vec4;
using Points = std::vector<Point>;
using Colors = std::vector<Color>; 

bool safe_divide(glm::vec3& vec, float divisor) {
    if (std::abs(divisor) < std::numeric_limits<float>::epsilon()) {
        HONLY_LOGW("Division by zero or near-zero value");
        return false;
    } else {
        vec = vec / divisor;
        return true;
    }
}

/*
struct vec3 {
    float x1, x2, x3;

    vec3 & operator+=(const vec3 &v) {
        x1 = x1 + v[0];
        x2 = x2 + v[1];
        x3 = x3 + v[2];
        return *this;
    }

    vec3 & operator-=(const vec3 &v) {
        x1 = x1 - v[0];
        x2 = x2 - v[1];
        x3 = x3 - v[2];
        return *this;
    }

    vec3 & operator*=(float scale) {
        x1 = x1 * scale;
        x2 = x2 * scale;
        x3 = x3 * scale;
        return *this;
    }

};

static vec3 operator+(const vec3 &v1, const vec3 &v2) {
    vec3 v;
    v[0] = v1[0] + v2[0];
    v[1] = v1[1] + v2[1];
    v[2] = v1[2] + v2[2];
    return v;
}

static vec3 operator*(const vec3 &v1, float scale) {
    vec3 v;
    v[0] = v1[0] * scale;
    v[1] = v1[1] * scale;
    v[2] = v1[2] * scale;
    return v;
}

struct vec4 {
    float x1, x2, x3, x4;
};
*/
#endif