#include "Texture.h"
#include "Asset/AssetLoader.h"

gbe::asset::Texture::Texture(std::string id, const char* path) : BaseAsset(id, path)
{
	AssetLoader<Texture, Texture::TextureData>::LoadAsset(this, &this->data);
}

unsigned int gbe::asset::Texture::Get_gl_id() {
	return this->data.gl_id;
}

bool gbe::asset::Texture::CheckGpuLoaded(int& address) {
	address = this->data.gpu_texture_address;
	return this->data.loaded_in_gpu;
}
