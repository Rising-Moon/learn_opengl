#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

typedef struct image {
    int width, height, nrChannels;
    unsigned char *data;
} Image;

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
//图片
Image image;

int main(void) {
    //读取顶点着色器代码
    readFile("../Matrix/vertex_shader.vert", VERTEX_SHADER);
    //读取片元着色器代码
    readFile("../Matrix/fragment_shader.frag", FRAGMENT_SHADER);
    //加载图片
    stbi_set_flip_vertically_on_load(true);
    image.data = stbi_load("../images/timg.jpeg", &image.width, &image.height, &image.nrChannels, 0);

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

    //销毁图片
    stbi_image_free(image.data);
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
    glUseProgram(program);

    //顶点数据
    float vertices[] = {
//              4---5
//             /|  /|
//            0---1 |
//            | 6-|-7
//            |/  |/
//            2---3
            -0.5f, 0.5f, 0.5f, //0
            0.5f, 0.5f, 0.5f, //1
            -0.5f, -0.5f, 0.5f, //2
            0.5f, -0.5f, 0.5f, //3
            -0.5f, 0.5f, -0.5f, //4
            0.5f, 0.5f, -0.5f, //5
            -0.5f, -0.5f, -0.5f, //6
            0.5f, -0.5f, -0.5f, //7
    };

    //索引
    unsigned int indexes[] = {
            0, 1, 2, 2, 1, 3,
            1, 5, 3, 3, 5, 7,
            5, 4, 7, 7, 4, 6,
            4, 0, 6, 6, 9, 2,
            4, 5, 0, 0, 5, 1,
            6, 7, 2, 2, 7, 3
    };

    //生成VAO、VBO、EBO对象
    unsigned int VAO[1], VBO[1], EBO[1];
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);

    //绑定顶点数据
    glBindVertexArray(VAO[0]);
    //绑定对象到顶点缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    //设置顶点数据到缓冲
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //绑定索引数据到缓冲区
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    //设置索引缓冲
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
    //设置连接缓冲
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    //开启缓冲区
    glEnableVertexAttribArray(0);

    /**
     * 变换
     */
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    unsigned int u_ModelMat = glGetUniformLocation(program, "u_ModelMat");
    glUniformMatrix4fv(u_ModelMat, 1, GL_FALSE, glm::value_ptr(model));

    glm::mat4 view = glm::mat4(1.0f);
    glm::vec3 cameraPos = glm::vec3(glm::sin(time) * 4.0f, 1.0f, 2.0f);
    glm::vec3 targetPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 front = glm::normalize(targetPos - cameraPos);
    glm::vec3 left = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), front));
    glm::vec3 top = glm::normalize(glm::cross(front, left));
    view = glm::lookAt(cameraPos, targetPos, top);
    unsigned int u_ViewMat = glGetUniformLocation(program, "u_ViewMat");
    glUniformMatrix4fv(u_ViewMat, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 proj = glm::perspective(glm::radians(90.0f), (float) 10.0f / (float) 10.0f, 0.1f, 100.0f);
    unsigned int u_ProjectMat = glGetUniformLocation(program, "u_ProjectMat");
    glUniformMatrix4fv(u_ProjectMat, 1, GL_FALSE, glm::value_ptr(proj));


    //背面剔除
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//    glFrontFace(GL_CW);

    //设置WRAP方式
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //设置WRAP颜色
//    float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
//    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //设置放大和缩小时的纹理过滤方式
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    ////绘制
    glBindVertexArray(VAO[0]);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void *) 0);

    //清空
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}