#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

namespace gde {
    namespace rendering {
        class Shader {
        public:
            GLuint shaderID;

            Shader(std::string vert, std::string frag);
            
            template <typename TValue>
            void SetOverride(const char* id, TValue value) {}

			template<>
			void SetOverride<bool>(const char* id, bool value) {
			}
			template<>
			void SetOverride<float>(const char* id, float value) {
				glUseProgram(shaderID);
				glUniform1f(glGetUniformLocation(shaderID, id), value);
			}
			template<>
			void SetOverride<glm::vec2>(const char* id, glm::vec2 value) {
				glUseProgram(shaderID);
				glUniform2fv(glGetUniformLocation(shaderID, id), 1, glm::value_ptr(value));
			}
			template<>
			void SetOverride<glm::vec3>(const char* id, glm::vec3 value) {
				glUseProgram(shaderID);
				glUniform3fv(glGetUniformLocation(shaderID, id), 1, glm::value_ptr(value));
			}
			template<>
			void SetOverride<glm::vec4>(const char* id, glm::vec4 value) {
				glUseProgram(shaderID);
				glUniform4fv(glGetUniformLocation(shaderID, id), 1, glm::value_ptr(value));
			}
			template<>
			void SetOverride<glm::mat4>(const char* id, glm::mat4 value) {
				glUseProgram(shaderID);
				glUniformMatrix4fv(glGetUniformLocation(shaderID, id), 1, GL_FALSE, glm::value_ptr(value));
			}

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