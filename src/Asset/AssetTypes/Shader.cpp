#include "Shader.h"

#include "Asset/AssetLoading/AssetLoader.h"
#include "Asset/AssetTypes/Texture.h"

gbe::asset::Shader::Shader(std::string path) : BaseAsset(path)
{
}

unsigned int gbe::asset::Shader::Get_gl_id() {
	return this->load_data.gl_id;
}

void gbe::asset::Shader::SetTextureOverride(const char* id, asset::Texture* value) {
	this->load_data.overridefunctions.SetOverride_Texture(id, value);
}
void gbe::asset::Shader::SetTextureIdOverride(const char* id, unsigned int value) {
	this->load_data.overridefunctions.SetOverride_TextureId(id, value);
}