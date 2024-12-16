#include "Shader.h"
#include <string>
#include <iostream>

namespace gbe {

	using namespace rendering;

	Shader::Shader(std::string vert, std::string frag) {
		auto vertShader = TryCompileShader(vert, GL_VERTEX_SHADER);
		auto fragShader = TryCompileShader(frag, GL_FRAGMENT_SHADER);

		shaderID = glCreateProgram();
		glAttachShader(shaderID, vertShader);
		glAttachShader(shaderID, fragShader);
		std::cout << "Compiling: " + vert + " | " + frag << "INTO ->" << std::to_string(shaderID) << std::endl;
		
		GLsizei count = 0;
		GLuint shaders[] = { 0, 0, 0, 0 };
		glGetAttachedShaders(shaderID, 4, &count, shaders);
		printf("\tnumber of shaders: %d\n", count);
		for (int i = 0; i < count; i++) printf("\tshader_id: %d\n", shaders[i]);

		glLinkProgram(shaderID);
	}

	void Shader::SetTextureOverride(const char* id, Texture* value, int gpu_tex_slot) {
		glUseProgram(shaderID);
		GLuint texAddress = glGetUniformLocation(shaderID, id);

		glBindTexture(GL_TEXTURE_2D, value->texID);
		glUniform1i(texAddress, gpu_tex_slot);
	}

	void Shader::SetTextureOverride(const char* id, TextureCubeMap* value, int gpu_tex_slot) {
		glUseProgram(shaderID);
		GLuint texAddress = glGetUniformLocation(shaderID, id);

		glBindTexture(GL_TEXTURE_2D, value->texID);
		glUniform1i(texAddress, gpu_tex_slot);
	}

	void Shader::SetTextureOverride(const char* id, Framebuffer* value, int gpu_tex_slot) {
		glUseProgram(shaderID);
		GLuint texAddress = glGetUniformLocation(shaderID, id);

		glBindTexture(GL_TEXTURE_2D, value->textureColorbuffer);
		glUniform1i(texAddress, gpu_tex_slot);
	}

	GLuint Shader::TryCompileShader(std::string path, int compile_type) {
		//Load the text file
		auto shaderStr = [](std::string path) {
			std::fstream fsrc(path);
			std::stringstream fbuff;
			fbuff << fsrc.rdbuf();

			return fbuff.str();
		}(path);

		if (shaderStr.length() <= 1)
			throw "empty shader!";

		const char* shaderptr = shaderStr.c_str();

		//Compile the shader through OpenGL
		GLuint shaderint = glCreateShader(compile_type);
		glShaderSource(shaderint, 1, &shaderptr, NULL);
		glCompileShader(shaderint);

		//Handle errors if there is any
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
}