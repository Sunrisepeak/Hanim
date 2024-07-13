// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2024 - present  Sunrisepeak, speakshen@163.com
//
// ProjectLinks: https://github.com/Sunrisepeak/gl-interface
//

#ifndef GL_INTERFACE_H_GLI
#define GL_INTERFACE_H_GLI

#ifdef GLI_BUILD_LIB
#define GLI_API
#else
#define GLI_API static inline
#endif

#define GLI_API_V

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define GLI_TRUE 1
#define GLI_FALSE 0

#ifndef bool
#define BoolGLI int
#else
#define BoolGLI bool
#endif

#define GLI_NORMAL GLI_POSITION

#ifdef __cplusplus
#define GLI_IF_CPP(expr) expr
extern "C" {
#else
#define GLI_IF_CPP(expr)
#endif

enum DrawMode {
    GLI_NONE = -1,
    GLI_POINTS = 0,
    GLI_LINES = 1,
    GLI_LINE_STRIP = 2,
    GLI_LINE_LOOP = 3,
    GLI_TRIANGLES = 4,
    GLI_TRIANGLE_FAN = 5,
};

enum ColorMode {
    GLI_COL_ONE = 1,
    GLI_COL_MULTI = 2,
};

/*
        y
       ^
       |
       |
       +------> x
      /
     / 
    L  
    z    
*/
typedef struct PointGLI {
    float x, y, z;
} PointGLI;

typedef PointGLI VectorGLI;

typedef struct ColorGLI {
    float r, g, b, a;
} ColorGLI;

static const struct {
    const PointGLI ORIGIN;
    // 1d
    const PointGLI RIGHT;
    const PointGLI LEFT;
    const PointGLI UP;
    const PointGLI DOWN;
    const PointGLI FRONT;
    const PointGLI BACK;
    // 2d
    const PointGLI RU;
    const PointGLI LU;
    const PointGLI LD;
    const PointGLI RD;
    const PointGLI UF;
    const PointGLI DF;
    const PointGLI DB;
    const PointGLI UB;

    const PointGLI RF;
    const PointGLI RB;
    const PointGLI LF;
    const PointGLI LB;
    // 3d
    const PointGLI RUF;
    const PointGLI RDF;
    const PointGLI RDB;
    const PointGLI RUB;
    const PointGLI LUF;
    const PointGLI LDF;
    const PointGLI LDB;
    const PointGLI LUB;

} GLI_POSITION = {
    .ORIGIN = { 0, 0, 0 },

    // 1d
    .RIGHT = { 1.0f, 0, 0 },
    .LEFT = { -1.0f, 0, 0 },
    .UP = { 0, 1.0f, 0 },
    .DOWN = { 0, -1.0f, 0 },
    .FRONT = { 0, 0, 1.0f },
    .BACK = { 0, 0, -1.0f },

    // 2d
    .RU = { 1.0f, 1.0f, 0 },
    .LU = { -1.0f, 1.0f, 0 },
    .LD = { -1.0f, -1.0f, 0 },
    .RD = { 1.0f, -1.0f, 0 },
    .UF = { 0, 1.0f, 1.0f },
    .DF = { 0, -1.0f, 1.0f },
    .DB = { 0, -1.0f, -1.0f },
    .UB = { 0, 1.0f, -1.0f },

    .RF = { 1.0f, 0, 1.0f },
    .RB = { 1.0f, 0, -1.0f },
    .LF = { -1.0f, 0, 1.0f },
    .LB = { -1.0f, 0, -1.0f },

    // 3d
    .RUF = { 1.0f, 1.0f, 1.0f },
    .RDF = { 1.0f, -1.0f, 1.0f },
    .RDB = { 1.0f, -1.0f, -1.0f },
    .RUB = { 1.0f, 1.0f, -1.0f },
    .LUF = { -1.0f, 1.0f, 1.0f },
    .LDF = { -1.0f, -1.0f, 1.0f },
    .LDB = { -1.0f, -1.0f, -1.0f },
    .LUB = { -1.0f, 1.0f, -1.0f }
};

static const struct {
    ColorGLI WHITE;
    ColorGLI BLACK;
    ColorGLI RED;
    ColorGLI GREEN;
    ColorGLI BLUE;
    ColorGLI YELLOW;
    ColorGLI CYAN;
    ColorGLI MAGENTE;
    ColorGLI GRAY;
    ColorGLI ORANGE;
    ColorGLI PURPLE;
    ColorGLI BROWN;
} GLI_COLORS = {
    .WHITE = { 1.0f, 1.0f, 1.0f, 1.0f },
    .BLACK = { 0.0f, 0.0f, 0.0f, 1.0f },
    .RED = { 1.0f, 0.0f, 0.0f, 1.0f },
    .GREEN = { 0.0f, 1.0f, 0.0f, 1.0f },
    .BLUE = { 0.0f, 0.0f, 1.0f, 1.0f },
    .YELLOW = { 1.0f, 1.0f, 0.0f, 1.0f },
    .CYAN = { 0.0f, 1.0f, 1.0f, 1.0f },
    .MAGENTE = { 1.0f, 0.0f, 1.0f, 1.0f },
    .GRAY = { 0.5f, 0.5f, 0.5f, 1.0f },
    .ORANGE = { 1.0f, 0.65f, 0.0f, 1.0f },
    .PURPLE = { 0.5f, 0.0f, 0.5f, 1.0f },
    .BROWN = { 0.65f, 0.16f, 0.16f, 1.0f }
};

struct GraphicsDataGLI {
    struct {
        enum DrawMode draw;
        enum ColorMode color;
    } mode;
    unsigned int vertexNums;
    float *vertices;
    float *colors;
    float thickness;
    BoolGLI filled;
};

// --------------------- GLI_API_V ----------------------

// Core API
GLI_API_V void * gli_malloc(unsigned int size);
GLI_API_V void gli_free(void *ptr);

// init/deinit
GLI_API_V void gli_backend_init(void *extend);
GLI_API_V void gli_backend_deinit();

// config
GLI_API_V void gli_viewport(int x, int y, int w, int h);

// gli 2d
#define GLI_2D(size, x, y, code_block) gli_2d(size, x, y); code_block; gli_camera_update()
GLI_API_V void gli_2d(float size, float x, float y);

// gli 3d
GLI_API_V void gli_camera_pos(float x, float y, float z);
GLI_API_V void gli_camera_target(float x, float y, float z);
GLI_API_V void gli_camera_direction(VectorGLI direction);
GLI_API_V void gli_camera_fov(float fov);
GLI_API_V void gli_camera_aspect(float aspect);
GLI_API_V void gli_camera_clipping(float near, float far);
GLI_API_V void gli_camera_update();


// render
GLI_API_V void gli_clear(float r, float g, float b, float a);
GLI_API_V void gli_draw(struct GraphicsDataGLI *gData);
GLI_API_V void gli_render();

// data process
GLI_API_V void * gli_frame_buff();

// math
GLI_API_V float gli_sin(float x);
GLI_API_V float gli_cos(float x);


// --------------------- GLI_API ----------------------


GLI_API void gli_view(VectorGLI pos, VectorGLI target, VectorGLI direction);
GLI_API void gli_projection(float fov, float aspect, float near, float far);
GLI_API void gli_camera_rotation(float angle);


/////// - graphics api
GLI_API void gli_point(PointGLI p, ColorGLI col, float size GLI_IF_CPP(= 1.0));
// line
GLI_API void gli_line(PointGLI p1, PointGLI p2, ColorGLI col, float thickness GLI_IF_CPP(= 1.0));
GLI_API void gli_line_base(PointGLI p1, ColorGLI col1, PointGLI p2, ColorGLI col2, float thickness);
GLI_API void gli_line_strip(PointGLI *points, int pNum, ColorGLI col, float thickness);
GLI_API void gli_line_strip_base(PointGLI *points, int pNum, ColorGLI col, float thickness);

// triangle
GLI_API void gli_triangle(PointGLI p1, PointGLI p2, PointGLI p3, ColorGLI col, float thickness GLI_IF_CPP(= 1.0));
GLI_API void gli_triangle_filled(PointGLI p1, PointGLI p2, PointGLI p3, ColorGLI col);
GLI_API void gli_triangle_base(
    PointGLI p1, ColorGLI col1,
    PointGLI p2, ColorGLI col2,
    PointGLI p3, ColorGLI col3,
    BoolGLI filled GLI_IF_CPP(= false)
);

// rectangle - TODO: test 2d and 3d (0, 1)-(1, 0) and delta(x or y or z) = 0
GLI_API void gli_rectangle(PointGLI p1, PointGLI p2, ColorGLI col, float thickness);
GLI_API void gli_rectangle_filled(PointGLI p1, PointGLI p2, ColorGLI col);
GLI_API void gli_rectangle_base(
    PointGLI p1, PointGLI p2,
    ColorGLI colRD, ColorGLI colRU, ColorGLI colLU, ColorGLI colLD,
    BoolGLI filled GLI_IF_CPP(= false), float thickness GLI_IF_CPP(= 1.0)
);

// circle and ngon
GLI_API void gli_circle(
    PointGLI center, float radius, VectorGLI normal, ColorGLI col,
    int segmentsNum GLI_IF_CPP(= 0), float thickness GLI_IF_CPP(= 1.0)
);
GLI_API void gli_circle_filled(
    PointGLI center, float radius, VectorGLI normal, ColorGLI col,
    int segmentsNum GLI_IF_CPP(= 0)
);
GLI_API void gli_ngon(
    PointGLI center, float radius, ColorGLI col,
    int segmentsNum GLI_IF_CPP(= 5), float thickness GLI_IF_CPP(= 1.0)
);
GLI_API void gli_ngon_filled(
    PointGLI center, float radius, ColorGLI col,
    int segmentsNum GLI_IF_CPP(= 5)
);

GLI_API void gli_polygon(PointGLI *points, int pNums, ColorGLI col, float thickness GLI_IF_CPP(= 1.0));
GLI_API void gli_polygon_filled(PointGLI *points, int pNums, ColorGLI col);

GLI_API void gli_bezier_curve(PointGLI *points, int pNums, ColorGLI col, int segmentsNum, float thickness GLI_IF_CPP(= 1.0));

GLI_API void gli_coordinate();
GLI_API void gli_space();

//// helper
//math
GLI_API float gli_sqrt(float x);
GLI_API float gli_fabs(float x);

// data process
GLI_API void gli_pos_normalization(PointGLI *p1, PointGLI *p2);
GLI_API void gli_generate_circle_vertices(float *vertices, float radius, int numSegments);
GLI_API PointGLI gli_de_casteljau(PointGLI *points, float nums, float t);

// object
GLI_API PointGLI gli_pos_obj(float x, float y, float z);
GLI_API PointGLI gli_pos_plus(PointGLI p1, PointGLI p2);
GLI_API PointGLI gli_pos_minus(PointGLI p1, PointGLI p2);
GLI_API PointGLI gli_pos_scale(PointGLI p1, float scale);
GLI_API PointGLI gli_col_obj(float x, float y, float z);

#ifdef __cplusplus
} /* extern "C" */
#endif

GLI_API void gli_view(VectorGLI pos, VectorGLI target, VectorGLI direction) {
    gli_camera_pos(pos.x, pos.y, pos.z);
    gli_camera_target(target.x, target.y, target.z);
    gli_camera_direction(direction);
}

GLI_API void gli_projection(float fov, float aspect, float near, float far) {
    gli_camera_fov(fov);
    gli_camera_aspect(aspect);
    gli_camera_clipping(near, far);
}

GLI_API void gli_camera_rotation(float angle) {
    static unsigned short currentAngle = 0;
    static float camX, camZ;

    currentAngle = currentAngle + angle;
    camX = gli_sin(currentAngle * 0.01) * 5;
    camZ = gli_cos(currentAngle * 0.01) * 5;

    gli_camera_pos(camX, 3, camZ);
}


GLI_API void gli_point(PointGLI p, ColorGLI col, float size) {
    struct GraphicsDataGLI data;

    data.mode.draw = GLI_POINTS;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = 1;
    data.colors = &col.r;
    data.vertices = &p.x;
    data.thickness = size;
    data.filled = GLI_FALSE;

    gli_draw(&data);
}

GLI_API void gli_line(PointGLI p1, PointGLI p2, ColorGLI col, float thickness) {
    struct GraphicsDataGLI data;

    float vertices[2 * 3] = { p1.x, p1.y, p1.z, p2.x, p2.y, p2.z };

    data.mode.draw = GLI_LINES;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = 2;
    data.colors = &col.r;
    data.vertices = vertices;
    data.thickness = thickness;
    data.filled = GLI_FALSE;

    gli_draw(&data);
}

GLI_API void gli_line_strip(PointGLI *points, int pNum, ColorGLI col, float thickness) {
    struct GraphicsDataGLI data;

    data.mode.draw = GLI_LINE_STRIP;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = pNum;
    data.colors = &col.r;
    data.vertices = &(points->x);
    data.thickness = thickness;
    data.filled = GLI_FALSE;

    gli_draw(&data);
}

GLI_API void gli_rectangle(PointGLI p1, PointGLI p2, ColorGLI col, float thickness) {
    struct GraphicsDataGLI data;

    gli_pos_normalization(&p1, &p2);

    float deltaX = p2.x - p1.x;
    float deltaY = p2.y - p1.y;
    float deltaZ = p2.z - p1.z;

    float vertices[3 * 4] = {
        p1.x, p1.y, p1.z, // LT
        p1.x + deltaX, p1.y, p1.z + (deltaY == 0 ? 0 : deltaZ), // RT
        p2.x, p2.y, p2.z, // RB
        p1.x, p1.y + deltaY, p1.z + (deltaY == 0 ? deltaZ : 0) // LB
    };

    data.mode.draw = GLI_LINE_LOOP;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = 4;
    data.vertices = vertices;
    data.colors = &col.r;
    data.filled = GLI_FALSE;

    gli_draw(&data);
}

GLI_API void gli_rectangle_filled(PointGLI p1, PointGLI p2, ColorGLI col) {
    struct GraphicsDataGLI data;

    gli_pos_normalization(&p1, &p2);

    float deltaX = p2.x - p1.x;
    float deltaY = p2.y - p1.y;
    float deltaZ = p2.z - p1.z;

    float vertices[3 * 3 * 2] = {
        p1.x, p1.y, p1.z, // LT
        p1.x + deltaX, p1.y, p1.z + (deltaY == 0 ? 0 : deltaZ), // RT
        p2.x, p2.y, p2.z, // RB
        p2.x, p2.y, p2.z, // RB
        p1.x, p1.y + deltaY, p1.z + (deltaY == 0 ? deltaZ : 0), // LB
        p1.x, p1.y, p1.z, // LT
    };

    data.mode.draw = GLI_TRIANGLES;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = 6;
    data.vertices = vertices;
    data.colors = &col.r;
    data.filled = GLI_TRUE;

    gli_draw(&data);
}

GLI_API void gli_rectangle_base(
    PointGLI p1, PointGLI p2,
    ColorGLI colRD, ColorGLI colRU, ColorGLI colLU, ColorGLI colLD,
    BoolGLI filled, float thickness
) {
    struct GraphicsDataGLI data;

    ColorGLI colors[4] = { colRD, colRU, colLU, colLD };

    gli_pos_normalization(&p1, &p2);

    float deltaX = p2.x - p1.x;
    float deltaY = p2.y - p1.y;
    float deltaZ = p2.z - p1.z;

    float vertices[3 * 4] = {
        p1.x, p1.y, p1.z, // LT
        p1.x + deltaX, p1.y, p1.z + (deltaY == 0 ? 0 : deltaZ), // RT
        p2.x, p2.y, p2.z, // RB
        p1.x, p1.y + deltaY, p1.z + (deltaY == 0 ? deltaZ : 0) // LB
    };

    data.mode.draw = GLI_LINE_LOOP;
    data.mode.color = GLI_COL_MULTI;
    data.vertexNums = 4;
    data.vertices = vertices;
    data.colors = &colors[0].r;
    data.thickness = thickness;
    data.filled = filled;

    gli_draw(&data);
}

GLI_API void gli_polygon(PointGLI *points, int pNums, ColorGLI col, float thickness) {

    struct GraphicsDataGLI data;

    data.mode.draw = GLI_LINE_LOOP;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = pNums;
    data.vertices = &(points[0].x);
    data.colors = &col.r;
    data.thickness = thickness;
    data.filled = GLI_FALSE;

    gli_draw(&data);
}

GLI_API void gli_triangle(PointGLI p1, PointGLI p2, PointGLI p3, ColorGLI col, float thickness) {
    PointGLI points[3] = { p1, p2, p3 };
    gli_polygon(points, 3, col, 1);  
}

GLI_API void gli_triangle_filled(PointGLI p1, PointGLI p2, PointGLI p3, ColorGLI col) {
    struct GraphicsDataGLI data;

    PointGLI vertices[3] = {p1, p2, p3};

    data.mode.draw = GLI_TRIANGLES;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = 3;
    data.vertices = &(vertices[0].x);
    data.colors = &(col.r);
    data.filled = GLI_TRUE;

    gli_draw(&data);
}

GLI_API void gli_triangle_base(
    PointGLI p1, ColorGLI col1,
    PointGLI p2, ColorGLI col2,
    PointGLI p3, ColorGLI col3,
    BoolGLI filled
) {
    struct GraphicsDataGLI data;

    PointGLI vertices[3] = {p1, p2, p3};
    ColorGLI colors[3] = {col1, col2, col3};

    data.mode.draw = filled ? GLI_TRIANGLES : GLI_LINE_LOOP;
    data.mode.color = GLI_COL_MULTI;
    data.vertexNums = 3;
    data.vertices = &(vertices[0].x);
    data.colors = &(colors[0].r);
    data.filled = GLI_TRUE;

    gli_draw(&data);
}

GLI_API void gli_circle(
    PointGLI center, float radius, PointGLI normal, ColorGLI col,
    int segmentsNum, float thickness
) {
    struct GraphicsDataGLI data;
    float staticBuff[(64 + 1) * 3] = { 0 };
    float *vertices = staticBuff;
    BoolGLI useDynamicBuff = segmentsNum > 64 ? GLI_TRUE : GLI_FALSE;

    if (useDynamicBuff) {
        vertices = (float *) gli_malloc((segmentsNum + 1) * 3 * sizeof(float));
    }

    vertices[0] = center.x;
    vertices[1] = center.y;
    vertices[2] = center.z;

    vertices[3] = normal.x;
    vertices[4] = normal.y;
    vertices[5] = normal.z;

    gli_generate_circle_vertices(vertices, radius, segmentsNum);

    data.mode.draw = GLI_LINE_LOOP;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = segmentsNum;
    data.vertices = vertices;
    data.colors = &(col.r);
    data.thickness = thickness;
    data.filled = GLI_FALSE;

    gli_draw(&data);

    if (useDynamicBuff) {
        gli_free(vertices);
    }
}

GLI_API void gli_circle_filled(
    PointGLI center, float radius, PointGLI normal, ColorGLI col,
    int segmentsNum
) {
    struct GraphicsDataGLI data;
    float staticBuff[(64 + 1) * 3] = { 0 };
    float *vertices = staticBuff;
    BoolGLI useDynamicBuff = segmentsNum > 64 ? GLI_TRUE : GLI_FALSE;

    if (useDynamicBuff) {
        vertices = (float *) gli_malloc((segmentsNum + 1) * 3 * sizeof(float));
    }

    vertices[0] = center.x;
    vertices[1] = center.y;
    vertices[2] = center.z;

    vertices[3] = normal.x;
    vertices[4] = normal.y;
    vertices[5] = normal.z;

    gli_generate_circle_vertices(vertices, radius, segmentsNum);

    data.mode.draw = GLI_TRIANGLE_FAN;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = segmentsNum;
    data.vertices = vertices;
    data.colors = &(col.r);
    data.filled = GLI_TRUE;

    gli_draw(&data);

    if (useDynamicBuff) {
        gli_free(vertices);
    }
}

GLI_API void gli_bezier_curve(
    PointGLI *controlPoints, int pNums,
    ColorGLI col, int segmentsNum,
    float thickness
) {
    struct GraphicsDataGLI data;
    PointGLI *curvePoints = (PointGLI *)gli_malloc(sizeof(PointGLI) * (segmentsNum + 1));

    for (int i = 0; i <= segmentsNum; i++) {
        curvePoints[i] = gli_de_casteljau(controlPoints, pNums, (float)i / segmentsNum);
        //printf("%d - (%f, %f, %f)\n", i, curvePoints[i].x, curvePoints[i].y, curvePoints[i].z);
    }

    data.mode.draw = GLI_LINE_STRIP;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = segmentsNum + 1;
    data.vertices = &(curvePoints[0].x);
    data.colors = &(col.r);
    data.thickness = thickness;
    data.filled = GLI_FALSE;

    gli_draw(&data);

    gli_free(curvePoints);
}

// helper

// Quake III Arena - Fast Inverse Square Root
GLI_API float gli_sqrt(float x) {
    if (x < 0) {
        return -1.0f;
    }

    float xhalf = 0.5f * x;
    int i = *(int*)&x; // float -> bit
    i = 0x5f375a86 - (i >> 1); // magic number
    x = *(float*)&i; // bit -> float
    x = x * (1.5f - xhalf * x * x); // Newton-Raphson
    return 1.0f / x;
}

GLI_API float gli_fabs(float x) {
    return x < 0 ? -x : x;
}

GLI_API void gli_orthogonal_basis(const float* normal, float* u, float* v) {
    if (gli_fabs(normal[0]) > gli_fabs(normal[1])) {
        float invLength = 1.0f / gli_sqrt(normal[0] * normal[0] + normal[2] * normal[2]);
        u[0] = -normal[2] * invLength;
        u[1] = 0.0f;
        u[2] = normal[0] * invLength;

        v[0] = normal[1] * u[2];
        v[1] = normal[2] * u[0] - normal[0] * u[2];
        v[2] = -normal[1] * u[0];
    } else {
        float invLength = 1.0f / gli_sqrt(normal[1] * normal[1] + normal[2] * normal[2]);
        u[0] = 0.0f;
        u[1] = normal[2] * invLength;
        u[2] = -normal[1] * invLength;

        v[0] = normal[1] * u[2] - normal[2] * u[1];
        v[1] = -normal[0] * u[2];
        v[2] = normal[0] * u[1];
    }
}

// TODO: optimize memory
GLI_API PointGLI gli_de_casteljau(PointGLI *points, float nums, float t) {

    PointGLI staticMem[4] = { 0 };
    PointGLI retVal;
    BoolGLI useDynamicMem = nums > 4 ? GLI_TRUE : GLI_FALSE;
    PointGLI *newPoints =
        useDynamicMem ? (PointGLI *)gli_malloc(nums * sizeof(PointGLI)) : staticMem;

    if (nums == 1) {
        retVal = points[0];
    } else {
        for (int i = 0; i < nums - 1; i++) {
            VectorGLI vec = gli_pos_minus(points[i + 1], points[i]);
            newPoints[i] = gli_pos_plus(points[i], gli_pos_scale(vec, t));
            //printf("gli_de_casteljau %d - (%f, %f, %f)\n", i, newPoints[i].x, newPoints[i].y, newPoints[i].z);
        }
        retVal = gli_de_casteljau(newPoints, nums - 1, t);
    }

    if (useDynamicMem) gli_free(newPoints);

    return retVal;
}

GLI_API PointGLI gli_pos_obj(float x, float y, float z) {
    PointGLI p = {x, y, z};
    return p;
}

GLI_API PointGLI gli_pos_plus(PointGLI p1, PointGLI p2) {
    PointGLI p = {p1.x + p2.x, p1.y + p2.y, p1.z + p2.z};
    return p;
}

GLI_API PointGLI gli_pos_minus(PointGLI p1, PointGLI p2) {
    PointGLI p = {p1.x - p2.x, p1.y - p2.y, p1.z - p2.z};
    return p;
}

GLI_API PointGLI gli_pos_scale(PointGLI p1, float scale) {
    PointGLI p = {p1.x * scale, p1.y * scale, p1.z * scale};
    return p;
}

// vertices:
//    (0, 1, 2) is center
//    (3, 4, 5) is normal
GLI_API void gli_generate_circle_vertices(float *vertices, float radius, int numSegments) {

    float angleIncrement = 2.0f * M_PI / numSegments;

    float centerX = vertices[0];
    float centerY = vertices[1];
    float centerZ = vertices[2];

    float u[3], v[3]; // calculated by normal vertor
    gli_orthogonal_basis(vertices + 3, u, v);

    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * angleIncrement;
        float x = radius * gli_cos(angle);
        float y = radius * gli_sin(angle);

        // projectin
        vertices[i * 3 + 0] = centerX + x * u[0] + y * v[0];
        vertices[i * 3 + 1] = centerY + x * u[1] + y * v[1];
        vertices[i * 3 + 2] = centerZ + x * u[2] + y * v[2];
    }
}

GLI_API ColorGLI gli_color_obj(float r, float g, float b, float a) {
    ColorGLI col = {r, g, b, a};
    return col;
}

GLI_API void gli_pos_normalization(PointGLI *p1, PointGLI *p2) {
    PointGLI p;
    // TODO: Optimize
    p.x = p1->x < p2->x ? p1->x : p2->x;
    p.y = p1->y < p2->y ? p1->y : p2->y;
    p.z = p1->z < p2->z ? p1->z : p2->z;
    p2->x = p1->x > p2->x ? p1->x : p2->x;
    p2->y = p1->y > p2->y ? p1->y : p2->y;
    p2->z = p1->z > p2->z ? p1->z : p2->z;
    p1->x = p.x;
    p1->y = p.y;
    p1->z = p.z;
}

GLI_API PointGLI gli_pos_add(PointGLI p1, PointGLI p2) {
    PointGLI p = {p1.x + p2.x, p1.y + p2.y, p1.z + p2.z};
    return p;
}

GLI_API void gli_coordinate() {
    gli_line(GLI_POSITION.ORIGIN, GLI_POSITION.RIGHT, GLI_COLORS.RED, 1);
    gli_line(GLI_POSITION.ORIGIN, GLI_POSITION.UP, GLI_COLORS.GREEN, 1);
    gli_line(GLI_POSITION.ORIGIN, GLI_POSITION.FRONT, GLI_COLORS.BLUE, 1);
}

GLI_API void gli_space() {
    gli_point(GLI_POSITION.ORIGIN, GLI_COLORS.BLACK, 10);

    gli_point(GLI_POSITION.RIGHT, GLI_COLORS.RED, 5);
    gli_point(GLI_POSITION.LEFT, GLI_COLORS.RED, 5);
    gli_point(GLI_POSITION.UP, GLI_COLORS.RED, 5);
    gli_point(GLI_POSITION.DOWN, GLI_COLORS.RED, 5);
    gli_point(GLI_POSITION.FRONT, GLI_COLORS.RED, 5);
    gli_point(GLI_POSITION.BACK, GLI_COLORS.RED, 5);

    gli_point(GLI_POSITION.RU, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.LU, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.LD, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.RD, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.UF, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.DF, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.DB, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.UB, GLI_COLORS.GREEN, 5);

    gli_point(GLI_POSITION.RF, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.RB, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.LF, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.LB, GLI_COLORS.GREEN, 5); 

    gli_point(GLI_POSITION.RUF, GLI_COLORS.BLUE, 5);
    gli_point(GLI_POSITION.RDF, GLI_COLORS.BLUE, 5);
    gli_point(GLI_POSITION.RDB, GLI_COLORS.BLUE, 5);
    gli_point(GLI_POSITION.RUB, GLI_COLORS.BLUE, 5);
    gli_point(GLI_POSITION.LUF, GLI_COLORS.BLUE, 5);
    gli_point(GLI_POSITION.LDF, GLI_COLORS.BLUE, 5);
    gli_point(GLI_POSITION.LDB, GLI_COLORS.BLUE, 5);
    gli_point(GLI_POSITION.LUB, GLI_COLORS.BLUE, 5);
}

#endif /* GL_INTERFACE_H_GLI */