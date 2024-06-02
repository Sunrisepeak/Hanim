// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/HAnim
//

#ifndef GL_INTERFACE_H_GLI
#define GL_INTERFACE_H_GLI

enum {
    GLI_LINES = 1,
    GLI_LINE_STRIP = 11,
    GLI_LINE_LOOP = 111,
};

struct PointGLI {
    float x, y, z;
};

struct GraphicsDataGLI {
    unsigned int type;
    unsigned int vertexNums;
    float *vertexs;
};


void gli_backend_init(void *plugin);
void gli_backend_deinit();

void gli_draw(const struct GraphicsDataGLI *gData);

static inline void gli_line_2d(struct PointGLI p1, struct PointGLI p2) {
    struct GraphicsDataGLI data;
    float vertexs[2 * 3] = { p1.x, p1.y, 0, p2.x, p2.y, 0 };
    data.type = GLI_LINES;
    data.vertexs = vertexs;
    gli_draw(&data);
}

static inline void gli_rectange_2d(struct PointGLI p1, struct PointGLI p2) {
    struct GraphicsDataGLI data;

    float deltaX = p2.x - p1.x;
    float deltaY = p2.y - p1.y;

    float vertexs[3 * 4] = {
        p1.x, p1.y, 0,
        p1.x + deltaX, p1.y, 0,
        p2.x, p2.y, 0,
        p2.x - deltaX, p2.y, 0
    };

    data.type = GLI_LINE_LOOP;
    data.vertexs = vertexs;
    data.vertexNums = 4;

    gli_draw(&data);
}

#endif