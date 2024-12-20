#include "Texture.h"
#include "Asset/AssetLoader.h"

gbe::asset::Texture::Texture(std::string id, const char* path) : BaseAsset(id, path)
{
	AssetLoaderGeneric<Texture, Texture::TextureData>::LoadAsset(this, this->data);
}

unsigned int gbe::asset::Texture::Get_gl_id() {
	return this->data.gl_id;
}
