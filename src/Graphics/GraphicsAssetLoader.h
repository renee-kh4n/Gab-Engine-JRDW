#pragma once

#include "Asset/gbe_asset.h"
#include "Math/gbe_math.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace gbe {
	namespace gfx {
		class GraphicsAssetLoader {
		public:
			static bool Load_Texture(asset::Texture* target, asset::Texture::TextureData& data);
		};
	}
}