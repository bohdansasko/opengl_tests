//
// Created by Bogdan Sasko on 12/10/17.
// Copyright (c) 2017 TQOS. All rights reserved.
//

#ifndef OPENGL_TEST_PROJ_SHADERCONTROLLER_H
#define OPENGL_TEST_PROJ_SHADERCONTROLLER_H

#include <iostream>

using std::string;

enum class CheckStatusType {
    Compiling,
    Linking
};

class ShaderController {
public:
    static const string getShaderSource(const string& shaderfilename);
    static void showShaderStatus(uint shaderId, CheckStatusType statusType);

private:
    static void checkShaderCompileStatus(uint shaderId);
    static void checkShaderLinkStatus(uint shaderId);
};


#endif //OPENGL_TEST_PROJ_SHADERCONTROLLER_H
