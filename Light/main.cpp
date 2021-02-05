#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <stb_image.h>
#include <OurShader.h>
#include "../head/my_structs.h"

using namespace std;

float g_CameraPos[3] = {0.0f, 0.0f, 0.0f};
float g_MoveSpeed = 0.1f;

void framebuffer_size_callback(GLFWwindow *, int, int);

void processInput(GLFWwindow *);

void draw(Shader *);

Image loadImage(const char *path);

//顶点着色器
char VERTEX_SHADER[1024] = "";
//片元着色器
char FRAGMENT_SHADER[1024] = "";
//关照片元着色器
char LIGHT_SHADER[1024] = "";
//图片
Image ambient_texture;
Image specular_texture;

int main(void) {
    //读取顶点着色器代码
//    readFile("../Light/vertex_shader.vert", VERTEX_SHADER);
    //读取片元着色器代码
//    readFile("../Light/fragment_shader.frag", FRAGMENT_SHADER);
    //读取光照着色器
//    readFile("../Light/light_fshader.frag", LIGHT_SHADER);
    //设置图片垂直翻转（适应glsl的y坐标方向）
    stbi_set_flip_vertically_on_load(true);
    //加载图片
    ambient_texture = loadImage("../Light/container2.png");
    specular_texture = loadImage("../Light/container2_specular.png");

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

    //着色器
    Shader shader[] = {
            Shader("../Light/vertex_shader.vert", "../Light/fragment_shader.frag"),
            Shader("../Light/vertex_shader.vert", "../Light/light_fshader.frag")
    };

    //渲染循环
    while (!glfwWindowShouldClose(window)) {
        //限时最多执行5分钟避免卡顿
        if (glfwGetTime() > 20) {
            glfwSetWindowShouldClose(window, true);
        }

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ambient_texture.drop();
    specular_texture.drop();
    glfwTerminate();

    return 0;
}

//加载图片
Image loadImage(const char *path) {
    Image image;
    image.data = stbi_load(path, &image.width, &image.height, &image.nrChannels, 0);
    return image;
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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        g_CameraPos[0] -= g_MoveSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        g_CameraPos[0] += g_MoveSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        g_CameraPos[1] -= g_MoveSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        g_CameraPos[1] += g_MoveSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        g_CameraPos[2] += g_MoveSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        g_CameraPos[2] -= g_MoveSpeed;
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

//绘制
void draw(Shader *shaders) {
    //运行时间
    double time = glfwGetTime();

    Shader modelShader = shaders[0];
    Shader lightShader = shaders[1];

    /**
     * 正方体数据
     */

    //顶点数据
    /*  4-----5
       /|    /|
      0-----1 |
      | 6---|-7
      |/    |/
      2-----3*/
    float vertices[] = {
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,//0
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,//1
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,//2
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,//3
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, //4
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, //5
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //6
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f //7
    };
    int perCountOfInfo = 5;

    //索引
    unsigned int indexes[] = {
            0, 1, 2, 2, 1, 3,
            1, 5, 3, 3, 5, 7,
            5, 4, 7, 7, 4, 6,
            4, 0, 6, 6, 0, 2,
            4, 5, 0, 0, 5, 1,
            2, 3, 6, 6, 3, 7
    };

    //计算出完整顶点信息
    float completeVertices[180];
    int cvIndex = 0;
    for (int i = 0; i < 36; ++i) {
        int index = indexes[i];
        completeVertices[cvIndex] = vertices[index * perCountOfInfo];
        cvIndex++;
        completeVertices[cvIndex] = vertices[index * perCountOfInfo + 1];
        cvIndex++;
        completeVertices[cvIndex] = vertices[index * perCountOfInfo + 2];
        cvIndex++;
        completeVertices[cvIndex] = vertices[index * perCountOfInfo + 3];
        cvIndex++;
        completeVertices[cvIndex] = vertices[index * perCountOfInfo + 4];
        cvIndex++;
    }

    /*for (int i = 0; i < sizeof(completeVertices) / sizeof(float); ++i) {
        cout << completeVertices[i] << " ";
        if ((i + 1) % 5 == 0) {
            cout << endl;
        }
    }

    cout << endl << endl;*/

    //计算出法线信息
    float normals[108];
    for (int i = 0; i < 12; ++i) {
        int index1 = indexes[i * 3];
        int index2 = indexes[i * 3 + 1];
        int index3 = indexes[i * 3 + 2];
        glm::vec3 p1 = glm::vec3(vertices[index1 * perCountOfInfo], vertices[index1 * perCountOfInfo + 1],
                                 vertices[index1 * perCountOfInfo + 2]);
        glm::vec3 p2 = glm::vec3(vertices[index2 * perCountOfInfo], vertices[index2 * perCountOfInfo + 1],
                                 vertices[index2 * perCountOfInfo + 2]);
        glm::vec3 p3 = glm::vec3(vertices[index3 * perCountOfInfo], vertices[index3 * perCountOfInfo + 1],
                                 vertices[index3 * perCountOfInfo + 2]);
        glm::vec3 normal = glm::normalize(glm::cross(p3 - p1, p2 - p1));
        normals[i * 9] = normals[i * 9 + 3] = normals[i * 9 + 6] = normal.x;
        normals[i * 9 + 1] = normals[i * 9 + 4] = normals[i * 9 + 7] = normal.y;
        normals[i * 9 + 2] = normals[i * 9 + 5] = normals[i * 9 + 8] = normal.z;
    }

    //生成VAO、VBO
    unsigned int VAO[1], VBO[3];

    //背面剔除
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    //设置WRAP方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //设置WRAP颜色
    float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //设置放大和缩小时的纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /**
     * 绘制正方体
     */

    modelShader.use();
    glGenVertexArrays(1, VAO);
    glGenBuffers(3, VBO);

    //绑定顶点数据
    glBindVertexArray(VAO[0]);
    //绑定对象到顶点缓冲x
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    //设置顶点数据到缓冲
    glBufferData(GL_ARRAY_BUFFER, sizeof(completeVertices), completeVertices, GL_STATIC_DRAW);
    //设置连接缓冲
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *) 0);
    //开启缓冲区
    glEnableVertexAttribArray(0);
    //绑定缓冲区到
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    //设置法线数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    //设置连接缓冲区
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
    //开启缓冲区
    glEnableVertexAttribArray(1);
    //绑定缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    //设置UV坐标
    glBufferData(GL_ARRAY_BUFFER, sizeof(completeVertices), completeVertices, GL_STATIC_DRAW);
    //设置连接缓冲
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *) (3 * sizeof(float)));
    //开启缓冲区
    glEnableVertexAttribArray(2);

    //模型矩阵
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    modelShader.setMatrix4fv("u_ModelMat", glm::value_ptr(model));

    //视图矩阵
    glm::mat4 view = glm::mat4(1.0f);
    //计算相机位置
    float distance = g_CameraPos[0];
    float x = glm::sin(g_CameraPos[1]) * distance;
    float z = glm::cos(g_CameraPos[1]) * distance;
    glm::vec3 cameraPos = glm::vec3(x, g_CameraPos[2], z);
    //计算相机上方向
    glm::vec3 targetPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 front = glm::normalize(targetPos - cameraPos);
    glm::vec3 left = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), front));
    glm::vec3 top = glm::normalize(glm::cross(front, left));
    view = glm::lookAt(cameraPos, targetPos, top);
    modelShader.setMatrix4fv("u_ViewMat", glm::value_ptr(view));

    //投影矩阵
    glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
    modelShader.setMatrix4fv("u_ProjectMat", glm::value_ptr(proj));

    //设置光源位置
    glm::vec3 lightPos = glm::vec3(sin(time) * 1.2f, 1.0f, 2.0f);
    modelShader.setVector3("light.position", glm::value_ptr(lightPos));

    //设置摄像机位置
    modelShader.setVector3("u_EyePos", glm::value_ptr(cameraPos));

    //设置光源颜色
    modelShader.setVector3("light.ambient", (float[]) {0.2f, 0.2f, 0.2f});
    modelShader.setVector3("light.diffuse", (float[]) {1.0f, 1.0f, 1.0f});
    modelShader.setVector3("light.specular", (float[]) {0.8f, 0.8f, 0.8f});

    //设置模型颜色
//    glm::vec3 toyColor = glm::vec3(sin(time * 2.0f) * 2.0f - 0.5f, sin(time * 0.7f) * 2.0f - 0.5f,
//                                   sin(time * 1.3f) * 2.0f - 0.5f);
    glm::vec3 toyColor = glm::vec3(1.0f, 1.0f, 1.0f);
    modelShader.setVector3("u_ToyColor", glm::value_ptr(toyColor));

    //环境光
    modelShader.setVector3("material.ambient", (float[]) {1.0f, 0.5f, 0.31f});
    //漫反射
    modelShader.setInt("material.diffuse", 1);
    modelShader.setImage(ambient_texture, GL_TEXTURE1);
    //镜面反射
    modelShader.setInt("material.specular", 0);
    modelShader.setImage(specular_texture,GL_TEXTURE0);
    //反射系数
    modelShader.setFloat("material.shininess", 32.0f);

    ////绘制
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(indexes) / sizeof(unsigned int));
//    glDrawArrays(GL_TRIANGLES, 0, 6);

    //清空
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, VAO);
    glDeleteBuffers(2, VBO);

    /**
     * 绘制光源
     */

    lightShader.use();
    //生成VAO、VBO
    glGenVertexArrays(1, VAO);
    glGenBuffers(2, VBO);

    //绑定顶点数据
    glBindVertexArray(VAO[0]);
    //绑定对象到顶点缓冲x
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    //设置顶点数据到缓冲
    glBufferData(GL_ARRAY_BUFFER, sizeof(completeVertices), completeVertices, GL_STATIC_DRAW);
    //设置连接缓冲
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *) 0);
    //开启缓冲区
    glEnableVertexAttribArray(0);
    //绑定缓冲区到
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    //设置法线数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    //设置连接缓冲区
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
    //开启缓冲区
    glEnableVertexAttribArray(1);

    //模型矩阵
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f, 0.2f, 0.2f));

    lightShader.setMatrix4fv("u_ModelMat", glm::value_ptr(lightModel));
    lightShader.setMatrix4fv("u_ViewMat", glm::value_ptr(view));
    lightShader.setMatrix4fv("u_ProjectMat", glm::value_ptr(proj));

    ////绘制
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(indexes) / sizeof(unsigned int));

    //清空
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, VAO);
    glDeleteBuffers(2, VBO);
}