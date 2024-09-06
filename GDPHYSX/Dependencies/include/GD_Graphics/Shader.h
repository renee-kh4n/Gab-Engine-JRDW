#pragma once

#include "util.h"
#include <string>

namespace gde {
    namespace rendering {
        class Shader {
        public:
            GLuint shaderID;

            Shader(std::string vert, std::string frag);

            /// <summary>
            /// Helper function to try to compile a shader through OpenGL with error handling.
            /// </summary>
            /// <param name="path"></param>
            /// <param name="compile_type"></param>
            /// <returns></returns>
            static GLuint TryCompileShader(std::string path, int compile_type);

            /// <summary>
            /// Utility function to check if shaders are compiled correctly
            /// </summary>
            /// <param name="program"></param>
            static void check_attached_shaders(GLuint program);
        };
    }
}