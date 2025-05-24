#include "Shader.h"

#include "Asset/AssetLoading/AssetLoader.h"
#include "Asset/AssetTypes/Texture.h"

gbe::asset::Shader::Shader(std::string path) : BaseAsset(path)
{
}

void gbe::asset::Shader::SetTextureOverride(const char* id, asset::Texture* value) {
	this->load_data.overridefunctions.SetOverride_Texture(id, value);
}
void gbe::asset::Shader::SetTextureIdOverride(const char* id, unsigned int value) {
	this->load_data.overridefunctions.SetOverride_TextureId(id, value);
}