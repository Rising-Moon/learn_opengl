#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

using namespace std;

void framebuffer_size_callback(GLFWwindow *, int, int);

void processInput(GLFWwindow *);

void draw();

void readFile(const char *, char *);


int main(void) {
    char content[1024];
    readFile("../DrawTriangle/vertex_shader.vert", content);
    cout << content << endl;

//    readFile("vertex_shader.vert", content);
//    cout << content << endl;

    /*glfwInit();
    //配置glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if not defined(__WIN32__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //创建窗口
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpengl", NULL, NULL);
    if (window == NULL) {
        cout << "Fail To Create GLFW Window" << endl;
        glfwTerminate();
        return -1;
    }
    //设置窗口上下文
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cout << "Fail To Initialize GLAD" << endl;
        glfwTerminate();
        return -1;
    }

    //设置窗口大小
    glViewport(0, 0, 800, 600);

    //注册窗口大小修改回调
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //渲染循环
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();*/

    return 0;
}

//读取文件
void readFile(const char *path, char *content) {
    ifstream infile;
    int length = 0;
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

//绘制
void draw() {
    //顶点数据
    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
    };
    //生成VBO对象
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    //绑定对象到顶点缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //设置顶点数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}