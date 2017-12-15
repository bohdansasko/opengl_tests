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

float vertices[] = {
    -0.5f,  0.5f, 0.0f, // top left
    -0.5f, -0.5f, 0.0f, // bottom left
     0.0f,  0.0f, 0.0f // center
};

float vertices1[] = {
     0.0f,  0.0f, 0.0f, // center
     0.5f,  0.5f, 0.0f, // top right
     0.5f, -0.5f, 0.0f // bottom right
};

void windowResizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
uint getShaderProgram(const string& vshFilename, const string& fshFilename);

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

    uint vaoUniqueId1, vaoUniqueId2;
    uint vboUniqueId1, vboUniqueId2;

    glGenBuffers(1, &vboUniqueId1);
    glGenBuffers(1, &vboUniqueId2);

    glGenVertexArrays(1, &vaoUniqueId1);
    glGenVertexArrays(1, &vaoUniqueId2);

    glBindVertexArray(vaoUniqueId1);
    glBindBuffer(GL_ARRAY_BUFFER, vboUniqueId1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(vaoUniqueId2);
    glBindBuffer(GL_ARRAY_BUFFER, vboUniqueId2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    uint shaderProgramId = getShaderProgram("shaders/vertexShader.vsh", "shaders/fragShader.fsh");
    uint shaderProgramId1 = getShaderProgram("shaders/vertexShaderYellowColor.vsh", "shaders/fragShaderYellowColor.fsh");

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment this call to draw in wireframe polygons.

    while(!glfwWindowShouldClose(mainWindowPtr)) {
        processInput(mainWindowPtr);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgramId);
        glBindVertexArray(vaoUniqueId1);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
//
        glUseProgram(shaderProgramId1);
        glBindVertexArray(vaoUniqueId2);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(mainWindowPtr);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vaoUniqueId1);
    glDeleteBuffers(1, &vboUniqueId1);

    glDeleteVertexArrays(1, &vaoUniqueId2);
    glDeleteBuffers(1, &vboUniqueId2);

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

uint getShaderProgram(const string& vshFilename, const string& fshFilename) {
    uint vertexShaderId;
    uint fragmentShaderId;

    string content = ShaderController::getShaderSource(vshFilename);
    const GLchar* vertexShaderSource = content.c_str();
    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShaderId);
    ShaderController::showShaderStatus(vertexShaderId, CheckStatusType::Compiling);

    string content1 = ShaderController::getShaderSource(fshFilename);
    const GLchar* fragmentShaderSource = content1.c_str();
    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShaderId);
    ShaderController::showShaderStatus(fragmentShaderId, CheckStatusType::Compiling);

    uint shaderProgramId = glCreateProgram();
    glAttachShader(shaderProgramId, vertexShaderId);
    glAttachShader(shaderProgramId, fragmentShaderId);
    glLinkProgram(shaderProgramId);
    ShaderController::showShaderStatus(shaderProgramId, CheckStatusType::Linking);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return shaderProgramId;
}

