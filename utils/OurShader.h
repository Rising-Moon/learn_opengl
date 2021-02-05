//
// Created by 孟昱成 on 2021/1/28.
//

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../head/my_structs.h"

class Shader {
public:
    //程序ID
    unsigned int ID;

    //构造器读取并且构建着色器
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath);

    //使用/激活程序
    void use();

    //uniform工具函数
    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setMatrix4fv(const std::string &name, float *value) const;

    void setVector3(const std::string &name, float *value) const;

    void setVector4(const std::string &name, float *value) const;

    unsigned int setImage(Image image, GLenum textIndex);
};

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath) {
    //从文件中获取着色器
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    //设置异常抛出
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        //打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        //读取文件的缓冲内容到数据流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        //关闭文件处理
        vShaderFile.close();
        fShaderFile.close();
        //转换数据流
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        return;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
//    const char *vShaderCode = readFile(vertexPath);
//    const char *fShaderCode = readFile(fragmentPath);
    //编译着色器
    unsigned int vertex, fragment;
    int success = 0;
    char infoLog[512] = "";

    //顶点着色器
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return;
    }

    //片段着色器
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << fShaderCode << std::endl;
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return;
    }

    //着色器程序
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    //错误
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return;
    }

    //删除着色器
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setInt(const std::string &name, int value) const {
    glProgramUniform1i(ID, glGetUniformLocation(ID, name.c_str()), (int) value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glProgramUniform1f(ID, glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMatrix4fv(const std::string &name, float *value) const {
    glProgramUniformMatrix4fv(ID, glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setVector3(const std::string &name, float *value) const {
    glProgramUniform3fv(ID, glGetUniformLocation(ID, name.c_str()), 1, value);
}

void Shader::setVector4(const std::string &name, float *value) const {
    glProgramUniform4fv(ID, glGetUniformLocation(ID, name.c_str()), 1, value);
}

unsigned int Shader::setImage(Image image, GLenum textIndex) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(textIndex);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture;
}

#endif //SHADER_H
