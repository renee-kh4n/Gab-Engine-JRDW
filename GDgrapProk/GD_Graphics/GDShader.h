#pragma once

#include "GDutil.h"

class GDShader {
public:
    GLuint shaderID;

    GDShader(std::string vert, std::string frag);

    static GLuint TryCompileShader(std::string path, int compile_type);
    static void check_attached_shaders(GLuint program);
};