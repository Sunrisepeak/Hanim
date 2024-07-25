#ifndef HTYPE_HPP_FWK_HANIM
#define HTYPE_HPP_FWK_HANIM

#include "utils/math.hpp"

namespace hanim {

using HPoints = std::vector<vec3>;

constexpr float DEFAULT_THICKNESS = 1.5; // 480P

struct HColor {
    constexpr static Color RED     = {1, 0, 0, 1};
    constexpr static Color GREEN   = {0, 1, 0, 1};
    constexpr static Color BLUE    = {0, 0, 1, 1};
    constexpr static Color WHITE   = {1, 1, 1, 1};
    constexpr static Color BLACK   = {0, 0, 0, 1};
    constexpr static Color YELLOW  = {1, 1, 0, 1};
    constexpr static Color CYAN    = {0, 1, 1, 1};
    constexpr static Color MAGENTA = {1, 0, 1, 1};
    constexpr static Color GRAY    = {0.5f, 0.5f, 0.5f, 1};
    constexpr static Color ORANGE  = {1, 0.5f, 0, 1};
    constexpr static Color PURPLE  = {0.5f, 0, 0.5f, 1};
    constexpr static Color BROWN   = {0.6f, 0.3f, 0, 1};
    constexpr static Color PINK    = {1, 0.75f, 0.8f, 1};
};

struct HPos {
    constexpr static vec3 ORIGIN = {0, 0, 0};

    // 1D directions
    constexpr static vec3 RIGHT  = {1.0f, 0, 0};
    constexpr static vec3 LEFT   = {-1.0f, 0, 0};
    constexpr static vec3 UP     = {0, 1.0f, 0};
    constexpr static vec3 DOWN   = {0, -1.0f, 0};
    constexpr static vec3 FRONT  = {0, 0, 1.0f};
    constexpr static vec3 BACK   = {0, 0, -1.0f};

    // 2D diagonal directions
    constexpr static vec3 RU = {1.0f, 1.0f, 0};
    constexpr static vec3 LU = {-1.0f, 1.0f, 0};
    constexpr static vec3 LD = {-1.0f, -1.0f, 0};
    constexpr static vec3 RD = {1.0f, -1.0f, 0};
    constexpr static vec3 UF = {0, 1.0f, 1.0f};
    constexpr static vec3 DF = {0, -1.0f, 1.0f};
    constexpr static vec3 DB = {0, -1.0f, -1.0f};
    constexpr static vec3 UB = {0, 1.0f, -1.0f};

    constexpr static vec3 RF = {1.0f, 0, 1.0f};
    constexpr static vec3 RB = {1.0f, 0, -1.0f};
    constexpr static vec3 LF = {-1.0f, 0, 1.0f};
    constexpr static vec3 LB = {-1.0f, 0, -1.0f};

    // 3D diagonal directions
    constexpr static vec3 RUF = {1.0f, 1.0f, 1.0f};
    constexpr static vec3 RDF = {1.0f, -1.0f, 1.0f};
    constexpr static vec3 RDB = {1.0f, -1.0f, -1.0f};
    constexpr static vec3 RUB = {1.0f, 1.0f, -1.0f};
    constexpr static vec3 LUF = {-1.0f, 1.0f, 1.0f};
    constexpr static vec3 LDF = {-1.0f, -1.0f, 1.0f};
    constexpr static vec3 LDB = {-1.0f, -1.0f, -1.0f};
    constexpr static vec3 LUB = {-1.0f, 1.0f, -1.0f};
};

struct HAABB {
    vec3 min;
    vec3 max;
};

}

#endif