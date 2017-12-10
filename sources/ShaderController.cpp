//
// Created by Bogdan Sasko on 12/10/17.
// Copyright (c) 2017 TQOS. All rights reserved.
//

#include "ShaderController.h"
#include <fstream>
#include <streambuf>

using namespace std;

const string& ShaderController::getShaderSource(const string& shaderFilename) {
    ifstream t(shaderFilename);
    string content(
            (std::istreambuf_iterator<char>(t)),
            std::istreambuf_iterator<char>()
    );

    return content;
}