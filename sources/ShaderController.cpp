//
// Created by Bogdan Sasko on 12/10/17.
// Copyright (c) 2017 TQOS. All rights reserved.
//

#include "ShaderController.h"
#include <fstream>
#include <iostream>
#include "glad/glad.h"

using namespace std;

const string ShaderController::getShaderSource(const string& shaderFilename) {
    ifstream t(shaderFilename);
    if (t.is_open()) {
        string content(
                (std::istreambuf_iterator<char>(t)),
                (std::istreambuf_iterator<char>())
        );
        return content;
    }

    return "";
}

void ShaderController::showShaderStatus(uint shaderId, CheckStatusType statusType) {
    switch (statusType) {
        case CheckStatusType::Linking:
            checkShaderLinkStatus(shaderId);
            break;
        case CheckStatusType::Compiling:
        default:
            checkShaderCompileStatus(shaderId);
            break;
    }
}

void ShaderController::checkShaderCompileStatus(uint shaderId) {
    int success;
    char infoLog[512];

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
        cout << "ERROR => Shader Compilation => status: " << infoLog << endl;
    }
}

void ShaderController::checkShaderLinkStatus(uint shaderId) {
    int success;
    char infoLog[512];

    glGetProgramiv(shaderId, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shaderId, 512, nullptr, infoLog);
        cout << "ERROR => Shader Linking => status: " << infoLog << endl;
    }
}