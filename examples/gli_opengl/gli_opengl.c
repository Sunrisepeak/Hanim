#include <stdio.h>

// GL
#include <glad/glad.h>
#include <GL/gl.h>

// GLI
#include <gl_interface.h>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n\0";

static float gVertexsBuff[100] = {
        -0.91f, -0.51f, 0.0f,  // Triangle
        -0.0f, -0.5f, 0.0f,
        -0.45f, 0.51f, 0.0f,
         0.1f, -0.5f, 0.0f,  // Rectangle
         0.9f, -0.5f, 0.0f,
         0.9f,  0.5f, 0.0f,
         0.1f,  0.5f, 0.0f
};
static unsigned int gShaderProgram;
static unsigned int gVBO, gVAO;

static unsigned int gCmdMapTable[] = {
    [GLI_LINE_LOOP] = GL_LINE_LOOP,
};

void gli_backend_init(void *plugin) {

    if (!gladLoadGLLoader((GLADloadproc)plugin)) {
        printf("Failed to initialize GLAD\n");
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
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &gVAO);
    glGenBuffers(1, &gVBO);
    glBindVertexArray(gVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexsBuff), gVertexsBuff, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexsBuff), gVertexsBuff, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    // 初始化阶段 或 每次更新
    glUseProgram(gShaderProgram);
    glBindVertexArray(gVAO);
}

void gli_backend_deinit() {
    glDeleteVertexArrays(1, &gVAO);
    glDeleteBuffers(1, &gVBO);
    glDeleteProgram(gShaderProgram);
}

void gli_draw(const struct GraphicsDataGLI *gData) {

    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferSubData(
        GL_ARRAY_BUFFER, 0,
        gData->vertexNums * sizeof(float) * 3,
        gData->vertexs
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(gCmdMapTable[gData->type], 0, gData->vertexNums);
}