//
// Created by Bogdan Sasko on 12/10/17.
// Copyright (c) 2017 TQOS. All rights reserved.
//

#include "Shader.h"
#include <fstream>
#include <iostream>
#include "glad/glad.h"

using namespace std;

Shader::Shader(const string& vShaderPath, const string& fShaderPath) {
    const string& vShaderSourceCode = Shader::getShaderSource(vShaderPath);
    auto vShaderId = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vSource = vShaderSourceCode.c_str();
    glShaderSource(vShaderId, 1, &vSource, nullptr);
    glCompileShader(vShaderId);
    Shader::showShaderStatus(vShaderId, CheckStatusType::Compiling);

    const string& fragmentShaderSource = Shader::getShaderSource(fShaderPath);
    const GLchar* fSource = fragmentShaderSource.c_str();
    auto fShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShaderId, 1, &fSource, nullptr);
    glCompileShader(fShaderId);
    Shader::showShaderStatus(fShaderId, CheckStatusType::Compiling);

    ProgramID = glCreateProgram();
    glAttachShader(ProgramID, vShaderId);
    glAttachShader(ProgramID, fShaderId);
    glLinkProgram(ProgramID);
    Shader::showShaderStatus(ProgramID, CheckStatusType::Linking);

    glDeleteShader(vShaderId);
    glDeleteShader(fShaderId);
}

void Shader::use() {
    glUseProgram(ProgramID);
}

void Shader::setBool(const string &name, bool value) {
    auto layoutId = glGetUniformLocation(ProgramID, name.c_str());
    glUniform1i(layoutId, static_cast<int>(value));
}

void Shader::setInt(const string &name, int value) {
    auto layoutId = glGetUniformLocation(ProgramID, name.c_str());
    glUniform1i(layoutId, value);
}

void Shader::setFloat(const string &name, float value) {
    auto layoutId = glGetUniformLocation(ProgramID, name.c_str());
    glUniform1f(layoutId, value);
}

void Shader::setVec3(const string& name, float value0, float value1, float value2) {
    auto layoutId = glGetUniformLocation(ProgramID, name.c_str());
    glUniform3f(layoutId, value0, value1, value2);
}

const string Shader::getShaderSource(const string& shaderFilename) {
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

void Shader::showShaderStatus(uint shaderId, CheckStatusType statusType) {
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

void Shader::checkShaderCompileStatus(uint shaderId) {
    int success;
    char infoLog[512];

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
        cout << "ERROR => Shader Compilation => status: " << infoLog << endl;
    }
}

void Shader::checkShaderLinkStatus(uint shaderId) {
    int success;
    char infoLog[512];

    glGetProgramiv(shaderId, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shaderId, 512, nullptr, infoLog);
        cout << "ERROR => Shader Linking => status: " << infoLog << endl;
    }
}
