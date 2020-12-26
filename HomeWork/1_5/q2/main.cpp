#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow *, int, int);

void processInput(GLFWwindow *);

void draw(unsigned int);

void readFile(const char *, char *);

unsigned int createProgram(unsigned int, unsigned int);

unsigned int createShader(char *, int);

//顶点着色器
char VERTEX_SHADER[1024] = "";
//片元着色器
char FRAGMENT_SHADER[1024] = "";

int main(void) {
    //读取顶点着色器代码
    readFile("../HomeWork/1_5/q2/vertex_shader.vert", VERTEX_SHADER);
    //读取片元着色器代码
    readFile("../HomeWork/1_5/q2/fragment_shader.frag", FRAGMENT_SHADER);

    //初始化glfw
    glfwInit();
    //配置glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //创建窗口
    GLFWwindow *window = glfwCreateWindow(600, 600, "LearnOpengl", NULL, NULL);
    if (window == NULL) {
        cout << "Fail To Create GLFW Window" << endl;
        glfwTerminate();
        return -1;
    }
    //设置窗口上下文
    glfwMakeContextCurrent(window);

    //初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cout << "Fail To Initialize GLAD" << endl;
        glfwTerminate();
        return -1;
    }

    //设置窗口大小
    glViewport(0, 0, 600, 600);
    //注册窗口大小修改回调
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //初始化顶点着色器
    unsigned int vertexShader = createShader(VERTEX_SHADER, GL_VERTEX_SHADER);
    //初始化片元着色器
    unsigned int fragmentShader = createShader(FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
    //创建着色器程序
    unsigned int program = createProgram(vertexShader, fragmentShader);
    //卸载着色器
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    //渲染循环
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        draw(program);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

//读取文件
void readFile(const char *path, char *content) {
    ifstream infile;
    int length;
    infile.open(path, ios::in);
    infile.seekg(0, ios::end);
    length = infile.tellg();
    infile.seekg(0, ios::beg);
    infile.read(content, length);
    infile.close();
}

//窗口大小改变回调
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

//处理输入
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

//创建着色程序
unsigned int createProgram(unsigned int vertex, unsigned int fragment) {
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);

    //连接程序
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR::PROGRAM::LINKER_FAILED\n" << infoLog << endl;
        return 0;
    }

    return shaderProgram;
}

/**
 * 创建一个着色器
 * @param text 着色器文本
 * @param type 着色器类型 (GL_VERTEX_SHADER | GL_FRAGMENT_SHADER)
 * @return 创建的着色器
 */
unsigned int createShader(char *text, int type) {
    unsigned int shader;
    shader = glCreateShader(type);
    const char *context = text;
    glShaderSource(shader, 1, &context, NULL);
    glCompileShader(shader);

    //是否编译成功
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER) {
            cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        } else if (type == GL_FRAGMENT_SHADER) {
            cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        }
        return 0;
    }

    return shader;
}

//绘制
void draw(unsigned int program) {
    double time = glfwGetTime();
    float greenValue = sin(time) / 2.0f + 0.5f;
    glUseProgram(program);
    int fragColorLocation = glGetUniformLocation(program, "u_FragColor");
    glUniform4f(fragColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    int uOffsetLocation = glGetUniformLocation(program, "u_Offset");
    glUniform1f(uOffsetLocation, sin(time) / 2);

    //顶点数据
    float vertices[] = {
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    //生成VAO、VBO、EBO对象
    unsigned int VAO[1], VBO[1];
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    ////设置第一个三角形
    //绑定顶点数据
    glBindVertexArray(VAO[0]);
    //绑定对象到顶点缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    //设置顶点数据到缓冲
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //设置连接缓冲
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *) (3 * sizeof(float)));
    //开启缓冲区
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //背面剔除
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//    glFrontFace(GL_CW);

    ////绘制
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    //清空
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}