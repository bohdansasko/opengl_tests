//
//  main.cpp
//  opengl_test_proj
//
//  Created by TQOS on 09.12.17.
//  Copyright © 2017 TQOS. All rights reserved.
//

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "ShaderController.h"

using namespace std;

struct Size {
    int width;
    int height;
};

Size windowSize { 1136, 640 };

enum MAIN_FUNC_STATUS {
    SUCCESS = 0,
    FAIL = -1
};

enum CheckStatusType {
    Compiling,
    Linking
};

void windowResizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void checkShaderStatus(uint shaderId, CheckStatusType statusType);
void addShaderProgram();

int main(int argc, const char * argv[]) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindowPtr = glfwCreateWindow(windowSize.width, windowSize.height, "OpenGL Game", nullptr, nullptr);
    if (!mainWindowPtr) {
        cout << "Failed to create GL Window" << endl;
        glfwTerminate();
        return MAIN_FUNC_STATUS::FAIL;
    }
    glfwMakeContextCurrent(mainWindowPtr);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return MAIN_FUNC_STATUS ::FAIL;
    }

    glViewport(0, 0, windowSize.width, windowSize.height);
    glfwSetFramebufferSizeCallback(mainWindowPtr, windowResizeCallback);

    addShaderProgram();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    while(!glfwWindowShouldClose(mainWindowPtr)) {
        processInput(mainWindowPtr);

        glfwSwapBuffers(mainWindowPtr);
        glfwPollEvents();
    }

    glfwTerminate();

    return MAIN_FUNC_STATUS::SUCCESS;
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        glClearColor(1.0f, 0.1f, 0.1f, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void addShaderProgram() {
    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f,
            0.5f, -0.5f, 0.0f
    };

    uint vboUniqueID1;
    glGenBuffers(1, &vboUniqueID1);
    glBindBuffer(GL_ARRAY_BUFFER, vboUniqueID1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const GLchar* vertexShaderSource = ShaderController::getShaderSource("vertexShader.vrsh").c_str();
    uint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShaderId);
    checkShaderStatus(vertexShaderId, CheckStatusType::Compiling);

    const GLchar* fragmentShaderSource = ShaderController::getShaderSource("fragShader.frsh").c_str();
    uint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShaderId);
    checkShaderStatus(fragmentShaderId, CheckStatusType::Compiling);

    uint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShaderId);
    glAttachShader(shaderProgram, fragmentShaderId);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    //checkShaderStatus(shaderProgram, CheckStatusType::Linking);
}

void checkShaderStatus(uint shaderId, CheckStatusType statusType) {
    uint status;
    switch (statusType) {
        case CheckStatusType::Linking:
            status = GL_LINK_STATUS;
            break;
        case CheckStatusType::Compiling:
        default:
            status = GL_COMPILE_STATUS;
            break;
    }

    int success;
    char infoLog[512];
    switch (statusType) {
        case CheckStatusType::Linking:
            glGetProgramiv(shaderId, status, &success);
            break;
        case CheckStatusType::Compiling:
        default:
            glGetShaderiv(shaderId, status, &success);
            break;
    }

    if (!success) {
        switch (statusType) {
            case CheckStatusType::Linking:
                glGetProgramInfoLog(shaderId, 512, nullptr, infoLog);
                break;
            case CheckStatusType::Compiling:
            default:
                glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
                break;
        }
        cout << "ERROR => Shader Compilation => status: " << infoLog << endl;
    }
}