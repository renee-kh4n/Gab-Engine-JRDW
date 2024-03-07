#pragma once

#include "GDutil.h"

class Shader {
public:
    GLuint shaderID;

    Shader(std::string vert, std::string frag) {
        auto vertShader = TryCompileShader(vert, GL_VERTEX_SHADER);
        auto fragShader = TryCompileShader(frag, GL_FRAGMENT_SHADER);

        shaderID = glCreateProgram();
        glAttachShader(shaderID, vertShader);
        glAttachShader(shaderID, fragShader);
        check_attached_shaders(shaderID);

        glLinkProgram(shaderID);
    }

    static GLuint TryCompileShader(std::string path, int compile_type) {
        auto shaderStr = LoadTxtFile(path);
        const char* shaderptr = shaderStr.c_str();

        GLuint shaderint = glCreateShader(compile_type);
        glShaderSource(shaderint, 1, &shaderptr, NULL);
        glCompileShader(shaderint);

        GLint isCompiled = 0;
        glGetShaderiv(shaderint, GL_COMPILE_STATUS, &isCompiled);

        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shaderint, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(shaderint, maxLength, &maxLength, &errorLog[0]);

            for (size_t e_i = 0; e_i < errorLog.size(); e_i++)
            {
                std::cout << errorLog[e_i];
            }

            glDeleteShader(shaderint);
        }

        return shaderint;
    }
    static void check_attached_shaders(GLuint program)
    {
        GLsizei count = 0;
        GLuint shaders[] = { 0, 0, 0, 0 };
        glGetAttachedShaders(program, 4, &count, shaders);
        printf("\tnumber of shaders: %d\n", count);
        for (int i = 0; i < count; i++) printf("\tshader_id: %d\n", shaders[i]);
    }
};