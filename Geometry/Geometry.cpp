#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

GLFWwindow* g_window;

// Переменные для управления
float offsetX = 0.0f;
float offsetY = 0.0f;
float scale = 1.0f;
const float moveSpeed = 0.01f;
const float zoomSpeed = 0.01f;

struct Object {
    GLuint vbo, ibo, vao;
    GLsizei indexCount;
    GLsizei verticCount;
    GLuint shaderProgram;
};

Object g_object;
static float clamp(float value, float _min, float _max) {
    return fmax(_min, fmin(value, _max));
}
static string readFile(const string filePath) {
    ifstream file;
    stringstream buffer;

    try {
        file.open(filePath);
        buffer << file.rdbuf();
        file.close();
        return buffer.str();
    }
    catch (const ifstream::failure& e) {
        cerr << "Error: wasnt read "
            << filePath << "': " << e.what() << endl;
        return "";
    }
}

GLuint createShader(const GLchar* code, GLenum type) {
    GLuint id = glCreateShader(type);

    glShaderSource(id, 1, &code, NULL);
    glCompileShader(id);

    GLint compiled;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        if (len > 0) {
            char* log = (char*)alloca(len);
            glGetShaderInfoLog(id, len, NULL, log);
            cout << "Shader compile error: " << endl << log << endl;
        }
        glDeleteShader(id);
        return 0;
    }
    else {
        return id;
    }
}

GLuint createProgram(GLuint vS, GLuint fS) {
    GLuint id = glCreateProgram();

    glAttachShader(id, vS);
    glAttachShader(id, fS);

    glLinkProgram(id);

    GLint linked;
    glGetProgramiv(id, GL_LINK_STATUS, &linked);

    if (!linked) {
        GLint len;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
        if (len > 0) {
            char* log = (char*)alloca(len);
            glGetProgramInfoLog(id, len, NULL, log);
            cout << "Program link error: " << endl << log << endl;
        }
        glDeleteProgram(id);
        return 0;
    }
    else {
        return id;
    }
}


bool createShaderProgram() {
    g_object.shaderProgram = 0;

    string vshCode = readFile("VertexShader.glsl");

    // Используем фрагментный шейдер
    string fshCode = readFile("FragmentShaderMandelbrot.glsl");
    //string fshCode = readFile("FragmentShaderJulia.glsl");
    //string fshCode = readFile("FragmentShaderHeart.glsl");

    const GLchar* vsh = vshCode.c_str();
    const GLchar* fsh = fshCode.c_str();

    GLuint vS, fS;
    vS = createShader(vsh, GL_VERTEX_SHADER);
    fS = createShader(fsh, GL_FRAGMENT_SHADER);

    g_object.shaderProgram = createProgram(vS, fS);

    glDeleteShader(vS);
    glDeleteShader(fS);

    return g_object.shaderProgram != 0;
}

bool createModel() {
    const GLfloat vertices[] = {
       -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
       -1.0f,  1.0f, 1.0f, 1.0f, 1.0f
    };
    g_object.verticCount = 4;

    const GLuint indixes[] = {
        0, 1, 2,
        0, 2, 3
    };
    g_object.indexCount = 6;

    glGenVertexArrays(1, &g_object.vao);
    glBindVertexArray(g_object.vao);

    glGenBuffers(1, &g_object.vbo);
    glGenBuffers(1, &g_object.ibo);

    glBindBuffer(GL_ARRAY_BUFFER, g_object.vbo);
    glBufferData(GL_ARRAY_BUFFER, 5 * g_object.verticCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_object.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_object.indexCount * sizeof(GLuint), indixes, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const GLfloat*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const GLfloat*)(2 * sizeof(GLfloat)));

    return g_object.vbo != 0 && g_object.ibo != 0 && g_object.vao != 0;
}

// Функция обработки ввода
void processInput(GLFWwindow* window) {
    // Перемещение (скорость постоянная относительно экрана)
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        offsetX -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        offsetX += moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        offsetY -= moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        offsetY += moveSpeed;

    // Масштабирование с коррекцией смещения для зума к центру
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
        scale *= (1.0f + zoomSpeed);
        offsetX *= (1.0f + zoomSpeed);
        offsetY *= (1.0f + zoomSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        scale *= (1.0f - zoomSpeed);
        offsetX *= (1.0f - zoomSpeed);
        offsetY *= (1.0f - zoomSpeed);
    }

    // Ограничиваем смещение и зум
    scale = fmax(scale , 1);
    offsetX = clamp(offsetX, -scale + 1, scale - 1);
    offsetY = clamp(offsetY, -scale + 1, scale - 1);

}

bool init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    return createShaderProgram() && createModel();
}

void reshape(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(g_object.shaderProgram);

    // Передаем параметры в шейдер

    GLuint offsetLoc = glGetUniformLocation(g_object.shaderProgram, "uOffset");
    GLuint scaleLoc = glGetUniformLocation(g_object.shaderProgram, "uScale");

    glUniform2f(offsetLoc, offsetX, offsetY);
    glUniform1f(scaleLoc, scale);

    glBindVertexArray(g_object.vao);
    glDrawElements(GL_TRIANGLES, g_object.indexCount, GL_UNSIGNED_INT, NULL);
}

void cleanup() {
    if (g_object.shaderProgram != 0) glDeleteProgram(g_object.shaderProgram);
    if (g_object.vbo != 0) glDeleteBuffers(1, &g_object.vbo);
    if (g_object.ibo != 0) glDeleteBuffers(1, &g_object.ibo);
    if (g_object.vao != 0) glDeleteVertexArrays(1, &g_object.vao);
}

bool initOpenGL() {
    if (!glfwInit()) {
        cout << "Failed to initialize GLFW" << endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    g_window = glfwCreateWindow(800, 600, "OpenGL Test", NULL, NULL);
    if (g_window == NULL) {
        cout << "Failed to open GLFW window" << endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(g_window);
    glewExperimental = true;

    if (glewInit() != GLEW_OK) {
        cout << "Failed to initialize GLEW" << endl;
        return false;
    }

    glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetFramebufferSizeCallback(g_window, reshape);

    return true;
}

void tearDownOpenGL() {
    glfwTerminate();
}

int main() {
    if (!initOpenGL())
        return -1;

    init();

    while (glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(g_window) == 0) {

        //Обработка ввода
        processInput(g_window);
        draw();
        glfwSwapBuffers(g_window);
        glfwPollEvents();
        //cout << scale << '\n';
    }

    cleanup();
    tearDownOpenGL();

    return 0;
}