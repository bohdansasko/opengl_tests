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

void windowResizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

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
        glfwSetWindowShouldClose(window, true);
    }
}