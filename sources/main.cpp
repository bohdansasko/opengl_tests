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
    uint width;
    uint height;
};

Size windowSize { 800, 600 };

enum MAIN_FUNC_STATUS {
    SUCCESS = 0,
    FAIL = -1
};

enum CheckStatusType {
    Compiling,
    Linking
};

float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
         0.5f, -0.5f, 0.0f, // right
         0.0f,  0.5f, 0.0f  // top
};

uint vaoUniqueID1, vboUniqueID1;

uint vertexShaderId;
uint fragmentShaderId;
uint shaderProgramId;

void windowResizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void checkShaderStatus(uint shaderId, CheckStatusType statusType);
void addShaderProgram();

int main(int argc, const char * argv[]) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* mainWindowPtr = glfwCreateWindow(windowSize.width, windowSize.height, "OpenGL Game", nullptr, nullptr);
    if (!mainWindowPtr) {
        cout << "Failed to create GL Window" << endl;
        glfwTerminate();
        return MAIN_FUNC_STATUS::FAIL;
    }
    glfwMakeContextCurrent(mainWindowPtr);
    glfwSetFramebufferSizeCallback(mainWindowPtr, windowResizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return MAIN_FUNC_STATUS ::FAIL;
    }

    addShaderProgram();

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment this call to draw in wireframe polygons.

    while(!glfwWindowShouldClose(mainWindowPtr)) {
        processInput(mainWindowPtr);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgramId);
        glBindVertexArray(vaoUniqueID1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(mainWindowPtr);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vaoUniqueID1);
    glDeleteBuffers(1, &vboUniqueID1);

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
    string content = ShaderController::getShaderSource("vertexShader.vrsh");
    const GLchar* vertexShaderSource = content.c_str();
    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShaderId);
    checkShaderStatus(vertexShaderId, CheckStatusType::Compiling);

    string content1 = ShaderController::getShaderSource("fragShader.frsh");
    const GLchar* fragmentShaderSource = content1.c_str();
    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShaderId);
    checkShaderStatus(fragmentShaderId, CheckStatusType::Compiling);

    shaderProgramId = glCreateProgram();
    glAttachShader(shaderProgramId, vertexShaderId);
    glAttachShader(shaderProgramId, fragmentShaderId);
    glLinkProgram(shaderProgramId);
    checkShaderStatus(shaderProgramId, CheckStatusType::Linking);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    glGenBuffers(1, &vboUniqueID1);
    glGenVertexArrays(1, &vaoUniqueID1);

    glBindVertexArray(vaoUniqueID1);

    glBindBuffer(GL_ARRAY_BUFFER, vboUniqueID1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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
                cout << "ERROR => Shader Linking => status: " << infoLog << endl;
                break;
            case CheckStatusType::Compiling:
            default:
                glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
                cout << "ERROR => Shader Compilation => status: " << infoLog << endl;
                break;
        }
    }
}