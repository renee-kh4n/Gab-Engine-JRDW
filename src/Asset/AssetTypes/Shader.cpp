#include "Shader.h"

#include "Asset/AssetLoader.h"

gbe::asset::Shader::Shader(std::string id, const char* vert, const char* frag) : BaseAsset(id, vert, frag)
{
	AssetLoaderGeneric<Shader, Shader::ShaderData>::LoadAsset(this, this->data);
}

unsigned int gbe::asset::Shader::Get_gl_id() {
	return this->data.gl_id;
}