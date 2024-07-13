#include <stdio.h>

// GL
#include <glad/glad.h>
#include <GL/gl.h>

// GLI
#define GLI_BUILD_LIB
#include <gl_interface.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// m * v * p -> glsl is p * v * m
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec4 aColor;\n"
    "out vec4 posColor;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * vec4(aPos, 1.0);\n"
    "   posColor = aColor;\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "in vec4 posColor;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = posColor;\n"
    "}\n\0";

static unsigned int gShaderProgram;
static unsigned int gVBO, gVAO;
static unsigned int gCmdMapTable[100] = { 0 };

// TODO: optimize gli config
static struct {
    struct {
        float w, h; // window
    } window;
    struct {
        float pointSize;
        float lineThickness;
        float r, g, b, a;
    } graphics; // default config
    glm::vec4 viewport;
    float vertexBuff[7 * 180]; // TODO: use dynamic memory
    // camera
    float fov, aspect, near, far;
    glm::vec3 camPos, camTarget, camDirectionGLI;
    glm::mat4 model, view, projection;
} gGLI;


void * gli_malloc(unsigned int size) {
    return malloc(size);
}

void gli_free(void *ptr) {
    free(ptr);
}

void gli_backend_init(void *extend) {

    if (NULL != extend) {
        if (!gladLoadGLLoader((GLADloadproc)extend))
            printf("Failed to initialize GLAD\n");
        printf("gli_backend_init(%p): glad loader - extend(%p)\n", gli_backend_init, extend);
    }

    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Shader program
    gShaderProgram = glCreateProgram();
    glAttachShader(gShaderProgram, vertexShader);
    glAttachShader(gShaderProgram, fragmentShader);
    glLinkProgram(gShaderProgram);

    GLint success;
    glGetProgramiv(gShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        printf("shader program error\n");
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &gVAO);
    glGenBuffers(1, &gVBO);
    glBindVertexArray(gVAO);

    // config vertex buff
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(gverticesBuff), gVerticesBuff, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gGLI.vertexBuff), gGLI.vertexBuff, GL_DYNAMIC_DRAW);

    // config position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // config color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //glBindVertexArray(0);
    //glBindVertexArray(gVAO);

/*
    glEnable(GL_CULL_FACE);   // 显式开启面剔除
    glCullFace(GL_BACK);      // 指定剔除背面，默认行为
    glDisable(GL_CULL_FACE);  // 然后禁用面剔除
*/

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    // 启用混合模式
    glEnable(GL_BLEND);
    // 设置混合函数，这里使用源 alpha 和目标 1-alpha
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 1 - 10
    //float lineWidthRange[2] = {0.0f, 0.0f};
    //glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidthRange);
    //printf("line width: %f\n", lineWidthRange[1]);

    gGLI.fov = 45.0f; gGLI.aspect = 1;
    gGLI.near = 1, gGLI.far = -1;
    gGLI.camPos = { 0, 0, 5.0f };
    gGLI.camTarget = { 0, 0, 0 };
    gGLI.camDirectionGLI = { 0, 1.0f, 0 };

    gCmdMapTable[GLI_POINTS] = GL_POINTS;
    gCmdMapTable[GLI_LINES] = GL_LINES;
    gCmdMapTable[GLI_LINE_STRIP] = GL_LINE_STRIP;
    gCmdMapTable[GLI_LINE_LOOP] = GL_LINE_LOOP;
    gCmdMapTable[GLI_TRIANGLES] = GL_TRIANGLES;
    gCmdMapTable[GLI_TRIANGLE_FAN] = GL_TRIANGLE_FAN;

// Config Shader - MVP
    glUseProgram(gShaderProgram);
    gGLI.view = glm::mat4(1);
    //gGLI.view = glm::translate(glm::mat4(1), glm::vec3(-2.0f, -3.0f, 0.0f));
    // Projection - orthogonal
    gGLI.projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f);
    // send data to shader
    glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(gGLI.view));
    glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(gGLI.projection));

    printf("gli_backend_init: done\n");
}

void gli_viewport(int x, int y, int w, int h) {
    gGLI.aspect = w * 1.0 / h;
    gGLI.viewport = {x, y, w, h};
    glViewport(x, y, w, h);
}

void gli_clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gli_camera_pos(float x, float y, float z) {
    gGLI.camPos = { x, y, z };
}

void gli_camera_target(float x, float y, float z) {
    gGLI.camTarget = { x, y, z };
}

void gli_camera_direction(VectorGLI direction) {
    gGLI.camDirectionGLI = { direction.x, direction.y, direction.z };
}

void gli_camera_fov(float fov) {
    gGLI.fov = fov;
}

GLI_API_V void gli_camera_aspect(float aspect) {
    gGLI.aspect = aspect;
}

GLI_API_V void gli_camera_clipping(float near, float far) {
    gGLI.near = near;
    gGLI.near = far;
}


GLI_API_V void gli_2d(float size, float x, float y) {
    // send data to shader
    glUniformMatrix4fv(
        glGetUniformLocation(gShaderProgram, "view"),
        1, GL_FALSE,
        glm::value_ptr(glm::mat4(1))
    );
    glUniformMatrix4fv(
        glGetUniformLocation(gShaderProgram, "projection"),
        1, GL_FALSE,
        glm::value_ptr(glm::ortho(
            -size * gGLI.aspect + x, size * gGLI.aspect + x,
            -size + y, size + y,
            -size, size
        ))
    );
}

void gli_camera_update() {

    glUseProgram(gShaderProgram);

    // 局部坐标 -> 世界坐标 -> 观察坐标系
    // Transform坐标变换矩阵
    // Model
    //glm::mat4 model(1);
    //model = glm::translate(model, glm::vec3(0.0,0.0,0.0));
    //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    //model = glm::scale(model, glm::vec3(1.0f,1.0f,1.0f));

    // View
    gGLI.view = glm::lookAt(gGLI.camPos, gGLI.camTarget, gGLI.camDirectionGLI);

    // Projection
    gGLI.projection = glm::perspective(
        glm::radians(gGLI.fov),
        gGLI.aspect, // viewport w/h(default) or aspect
        gGLI.near, gGLI.far
    );

    // update shader mvp matrix
    //glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(
        glGetUniformLocation(gShaderProgram, "view"),
        1, GL_FALSE, glm::value_ptr(gGLI.view)
    );
    glUniformMatrix4fv(
        glGetUniformLocation(gShaderProgram, "projection"),
        1, GL_FALSE, glm::value_ptr(gGLI.projection)
    );
}

void gli_backend_deinit() {
    glDeleteVertexArrays(1, &gVAO);
    glDeleteBuffers(1, &gVBO);
    glDeleteProgram(gShaderProgram);
    printf("gli_backend_deinit: done\n");
}

void gli_draw(struct GraphicsDataGLI *gData) {

    if (GLI_FALSE == gData->filled) {
        glPointSize(gData->thickness);
        glLineWidth(gData->thickness);
    }

    for (int i = 0; i < gData->vertexNums; i++) {
        // pos
        gGLI.vertexBuff[7 * i] = gData->vertices[3 * i];
        gGLI.vertexBuff[7 * i + 1] = gData->vertices[3 * i + 1];
        gGLI.vertexBuff[7 * i + 2] = gData->vertices[3 * i + 2];

        // color
        if (gData->mode.color == GLI_COL_ONE) {
            gGLI.vertexBuff[7 * i + 3] = gData->colors[0];
            gGLI.vertexBuff[7 * i + 4] = gData->colors[1];
            gGLI.vertexBuff[7 * i + 5] = gData->colors[2];
            gGLI.vertexBuff[7 * i + 6] = gData->colors[3];
        } else if (gData->mode.color == GLI_COL_MULTI) {
            gGLI.vertexBuff[7 * i + 3] = gData->colors[4 * i];
            gGLI.vertexBuff[7 * i + 4] = gData->colors[4 * i + 1];
            gGLI.vertexBuff[7 * i + 5] = gData->colors[4 * i + 2];
            gGLI.vertexBuff[7 * i + 6] = gData->colors[4 * i + 3];
        }
    }

    // update buff
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferSubData(
        GL_ARRAY_BUFFER, 0,
        gData->vertexNums * sizeof(float) * 7,
        gGLI.vertexBuff
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // send draw cmd
    glDrawArrays(gCmdMapTable[gData->mode.draw], 0, gData->vertexNums);
}

GLI_API_V void gli_render() {
    
}

GLI_API_V float gli_sin(float x) {
    return sin(x);
}

GLI_API_V float gli_cos(float x) {
    return cos(x);
}