#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include "../Datatypes/Matrix4.h"
#include "../Datatypes/Vectors.h"
#include "Framebuffer.h"

#include "Texture.h"

namespace gbe {
    namespace rendering {
        class Shader {
		private:
        public:
            GLuint shaderID;

            Shader(std::string vert, std::string frag);
            
			template <typename TValue>
			void SetOverride(const char* id, TValue value) {
				throw "Creating new version of restricted template!";
			}
			template<>
			void SetOverride<bool>(const char* id, bool value) {
				glUseProgram(shaderID);
				auto loc = glGetUniformLocation(shaderID, id);
				glUniform1i(loc, value);
			}
			template<>
			void SetOverride<int>(const char* id, int value) {
				glUseProgram(shaderID);
				auto loc = glGetUniformLocation(shaderID, id);
				glUniform1i(loc, value);
			}
			template<>
			void SetOverride<float>(const char* id, float value) {
				glUseProgram(shaderID);
				auto loc = glGetUniformLocation(shaderID, id);
				glUniform1f(loc, value);
			}
			template<>
			void SetOverride<Vector2>(const char* id, Vector2 value) {
				glUseProgram(shaderID);
				glUniform2fv(glGetUniformLocation(shaderID, id), 1, value.Get_Ptr());
			}
			template<>
			void SetOverride<Vector3>(const char* id, Vector3 value) {
				glUseProgram(shaderID);
				auto loc = glGetUniformLocation(shaderID, id);
				glUniform3fv(loc, 1, value.Get_Ptr());
			}
			template<>
			void SetOverride<Vector4>(const char* id, Vector4 value) {
				glUseProgram(shaderID);
				glUniform4fv(glGetUniformLocation(shaderID, id), 1, value.Get_Ptr());
			}
			template<>
			void SetOverride<Matrix4>(const char* id, Matrix4 value) {
				glUseProgram(shaderID);
				auto loc = glGetUniformLocation(shaderID, id);
				glUniformMatrix4fv(loc, 1, GL_FALSE, value.Get_Ptr());
			}

			void SetTextureOverride(const char* id, Texture* value, int gpu_tex_slot);
			void SetTextureOverride(const char* id, TextureCubeMap* value, int gpu_tex_slot);
			void SetTextureOverride(const char* id, Framebuffer* value, int gpu_tex_slot);

            /// <summary>
            /// Helper function to try to compile a shader through OpenGL with error handling.
            /// </summary>
            /// <param name="path"></param>
            /// <param name="compile_type"></param>
            /// <returns></returns>
            static GLuint TryCompileShader(std::string path, int compile_type);
        };
    }
}