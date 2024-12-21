#include "Shader.h"

#include "Asset/AssetLoader.h"
#include "Asset/AssetTypes/Texture.h"

gbe::asset::Shader::Shader(std::string id, const char* vert, const char* frag) : BaseAsset(id, vert, frag)
{
}

unsigned int gbe::asset::Shader::Get_gl_id() {
	return this->data.gl_id;
}

void gbe::asset::Shader::SetTextureOverride(const char* id, asset::Texture* value) {
	this->data.overridefunctions.SetOverride_Texture(id, value);
}
void gbe::asset::Shader::SetTextureIdOverride(const char* id, unsigned int value) {
	this->data.overridefunctions.SetOverride_TextureId(id, value);
}