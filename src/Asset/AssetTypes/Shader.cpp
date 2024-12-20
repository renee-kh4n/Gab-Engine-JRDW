#include "Shader.h"

#include "Asset/AssetLoader.h"
#include "Asset/AssetTypes/Texture.h"

gbe::asset::Shader::Shader(std::string id, const char* vert, const char* frag) : BaseAsset(id, vert, frag)
{
	AssetLoader<Shader, Shader::ShaderData>::LoadAsset(this, &this->data);
}

unsigned int gbe::asset::Shader::Get_gl_id() {
	return this->data.gl_id;
}

void gbe::asset::Shader::SetFunctions(ShaderData::ShaderOverrideFunctions& funcs) {
	this->data.overridefunctions = funcs;
}

void gbe::asset::Shader::SetTextureOverride(const char* id, asset::Texture* value, int gpu_tex_slot) {
	this->data.overridefunctions.SetOverride_Texture(id, value);
}