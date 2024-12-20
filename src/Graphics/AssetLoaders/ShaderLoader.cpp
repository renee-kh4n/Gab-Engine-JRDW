#include "ShaderLoader.h"
#include <iostream>
#include <fstream>

bool gbe::gfx::ShaderLoader::LoadAsset_(asset::Shader* asset, asset::Shader::ShaderData* data) {
	auto vertShader = TryCompileShader(asset->GetPath(0), GL_VERTEX_SHADER);
	auto fragShader = TryCompileShader(asset->GetPath(1), GL_FRAGMENT_SHADER);

	unsigned int shaderID = glCreateProgram();
	glAttachShader(shaderID, vertShader);
	glAttachShader(shaderID, fragShader);
	std::cout << "Compiling: " + asset->GetPath(0) + " | " + asset->GetPath(1) << "INTO ->" << std::to_string(shaderID) << std::endl;

	GLsizei count = 0;
	GLuint shaders[] = { 0, 0, 0, 0 };
	glGetAttachedShaders(shaderID, 4, &count, shaders);
	printf("\tnumber of shaders: %d\n", count);
	for (int i = 0; i < count; i++) printf("\tshader_id: %d\n", shaders[i]);

	glLinkProgram(shaderID);

	auto override_funcs = asset::Shader::ShaderData::ShaderOverrideFunctions{
		
	};

	auto SetOverride_bool = [shaderID](const char* id, bool value) {
		glUseProgram(shaderID);
		auto loc = glGetUniformLocation(shaderID, id);
		glUniform1i(loc, value);
		};
	auto SetOverride_int = [shaderID](const char* id, int value) {
		glUseProgram(shaderID);
		auto loc = glGetUniformLocation(shaderID, id);
		glUniform1i(loc, value);
		};
	auto SetOverride_float = [shaderID](const char* id, float value) {
		glUseProgram(shaderID);
		auto loc = glGetUniformLocation(shaderID, id);
		glUniform1f(loc, value);
		};
	auto SetOverride_Vector2 = [shaderID](const char* id, Vector2 value) {
		glUseProgram(shaderID);
		glUniform2fv(glGetUniformLocation(shaderID, id), 1, value.Get_Ptr());
		};
	auto SetOverride_Vector3 = [shaderID](const char* id, Vector3 value) {
		glUseProgram(shaderID);
		auto loc = glGetUniformLocation(shaderID, id);
		glUniform3fv(loc, 1, value.Get_Ptr());
		};
	auto SetOverride_Vector4 = [shaderID](const char* id, Vector4 value) {
		glUseProgram(shaderID);
		glUniform4fv(glGetUniformLocation(shaderID, id), 1, value.Get_Ptr());
		};
	auto SetOverride_Matrix4 = [shaderID](const char* id, Matrix4 value) {
		glUseProgram(shaderID);
		auto loc = glGetUniformLocation(shaderID, id);
		glUniformMatrix4fv(loc, 1, GL_FALSE, value.Get_Ptr());
		};
	auto SetOverride_Texture = [shaderID](const char* id, asset::Texture* value) {
		int gpu_tex_slot = -1;
		if (!value->CheckGpuLoaded(gpu_tex_slot))
			throw "Texture is not loaded in the GPU!";

		glUseProgram(shaderID);
		GLuint texAddress = glGetUniformLocation(shaderID, id);

		glBindTexture(GL_TEXTURE_2D, value->Get_gl_id());
		glUniform1i(texAddress, gpu_tex_slot);
		};


}

void gbe::gfx::ShaderLoader::AssignSelfAsLoader()
{
	this->load_func = [this](auto target, auto data) {return this->LoadAsset_(target, data); };
}

unsigned int gbe::gfx::ShaderLoader::TryCompileShader(std::string path, int compile_type) {
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