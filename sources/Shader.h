//
// Created by Bogdan Sasko on 12/10/17.
// Copyright (c) 2017 TQOS. All rights reserved.
//

#ifndef OPEN_GL_TEST_PROJ_SHADER_H
#define OPEN_GL_TEST_PROJ_SHADER_H

#include <iostream>

using std::string;

enum class CheckStatusType {
    Compiling,
    Linking
};

class Shader {
public:
    static const string getShaderSource(const string& shaderfilename);
    static void showShaderStatus(uint shaderId, CheckStatusType statusType);

private:
    static void checkShaderCompileStatus(uint shaderId);
    static void checkShaderLinkStatus(uint shaderId);
};


#endif //OPEN_GL_TEST_PROJ_SHADER_H
