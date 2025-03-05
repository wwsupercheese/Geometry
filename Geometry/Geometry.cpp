#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

using namespace std;

GLFWwindow* g_window;

struct Object {
    GLuint vbo, ibo, vao;
    GLsizei indexCount;
    GLsizei verticCount;
    GLuint shaderProgram;
};

Object g_object;

string readFile(const string filePath) {
    ifstream file;
    stringstream buffer;

    // Включаем проверку ошибок потока
    file.exceptions(ifstream::failbit | ifstream::badbit);

    try {
        file.open(filePath);
        buffer << file.rdbuf();
        file.close();
        return buffer.str();
    }
    catch (const ifstream::failure& e) {
        cerr << "Error: wasnt read" 
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
    //string fshCode = readFile("FragmentShaderRings.glsl");
    //string fshCode = readFile("FragmentShaderWaves.glsl");
    string fshCode = readFile("FragmentShaderMandelbrot.glsl");

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
       -0.9f, -0.9f, 1.0f, 0.0f, 0.0f,  // 0: левый нижний (красный)
        0.9f, -0.9f, 0.0f, 0.0f, 1.0f,  // 1: правый нижний (синий)
        0.9f,  0.9f, 0.0f, 1.0f, 0.0f,  // 2: правый верхний (зеленый)
       -0.9f,  0.9f, 1.0f, 1.0f, 1.0f   // 3: левый верхний (белый)
    };
    g_object.verticCount = 4;

    const GLuint indixes[] = {
        0, 1, 2,  // Первый треугольник (нижний правый)
        0, 2, 3   // Второй треугольник (верхний левый)
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

bool init()
{
    // Set initial color of color buffer to blue.
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    return createShaderProgram() && createModel();
}

void reshape(GLFWwindow* window, int width, int height)
{

}

void draw()
{
    // Clear color buffer.
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(g_object.shaderProgram);
    glBindVertexArray(g_object.vao);
    glDrawElements(GL_TRIANGLES, g_object.indexCount, GL_UNSIGNED_INT, NULL);
}

void cleanup()
{
    if (g_object.shaderProgram != 0) glDeleteProgram(g_object.shaderProgram);
    if (g_object.vbo != 0) glDeleteBuffers(1, &g_object.vbo);
    if (g_object.ibo != 0) glDeleteBuffers(1, &g_object.ibo);
    if (g_object.vao != 0) glDeleteVertexArrays(1, &g_object.vao);
}

bool initOpenGL()
{
    // Initialize GLFW functions.
    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return false;
    }

    // Request OpenGL 3.3 without obsoleted functions.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window.
    g_window = glfwCreateWindow(800, 600, "OpenGL Test", NULL, NULL);
    if (g_window == NULL)
    {
        cout << "Failed to open GLFW window" << endl;
        glfwTerminate();
        return false;
    }

    // Initialize OpenGL context with.
    glfwMakeContextCurrent(g_window);

    // Set internal GLEW variable to activate OpenGL core profile.
    glewExperimental = true;

    // Initialize GLEW functions.
    if (glewInit() != GLEW_OK)
    {
        cout << "Failed to initialize GLEW" << endl;
        return false;
    }

    // Ensure we can capture the escape key being pressed.
    glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);

    // Set callback for framebuffer resizing event.
    glfwSetFramebufferSizeCallback(g_window, reshape);

    return true;
}

void tearDownOpenGL()
{
    // Terminate GLFW.
    glfwTerminate();
}

int main()
{
    // Initialize OpenGL
    if (!initOpenGL())
        return -1;

    // Initialize graphical resources.
    init();

    // Main loop until window closed or escape pressed.
    while (glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(g_window) == 0)
    {
        // Draw scene.
        draw();

        // Swap buffers.
        glfwSwapBuffers(g_window);
        // Poll window events.
        glfwPollEvents();
    }

    // Cleanup graphical resources.
    cleanup();

    // Tear down OpenGL.
    tearDownOpenGL();

    return 0;
}
