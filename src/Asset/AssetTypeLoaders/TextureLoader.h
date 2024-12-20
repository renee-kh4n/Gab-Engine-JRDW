#pragma once

#include "Asset/AssetLoader.h"
#include "Asset/AssetTypes/Texture.h"

namespace gbe {
	namespace asset {
		typedef AssetLoaderGeneric<Texture, Texture::TextureData> TextureLoader;
	}
}