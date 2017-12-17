//
// Created by Bogdan Sasko on 12/10/17.
// Copyright (c) 2017 TQOS. All rights reserved.
//

#ifndef OPEN_GL_TEST_PROJ_SHADER_H
#define OPEN_GL_TEST_PROJ_SHADER_H

#include <iostream>
#include <string>

using std::string;

enum class CheckStatusType {
    Compiling,
    Linking
};

class Shader {
public:
    uint ProgramID;

    Shader(const string& vShaderPath, const string& fShaderPath);

    void use();

    void setBool(const string& name, bool value);
    void setInt(const string& name, int value);
    void setFloat(const string& name, float value);

    void setVec3(const string& name, float value0, float value1, float value2);

    static const string getShaderSource(const string& shaderfilename);
    static void showShaderStatus(uint shaderId, CheckStatusType statusType);

private:
    static void checkShaderCompileStatus(uint shaderId);
    static void checkShaderLinkStatus(uint shaderId);
};


#endif //OPEN_GL_TEST_PROJ_SHADER_H
