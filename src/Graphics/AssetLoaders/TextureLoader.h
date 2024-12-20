#pragma once

#include "Asset/gbe_asset.h"
#include "Math/gbe_math.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace gbe {
	namespace gfx {
		class TextureLoader : public asset::AssetLoader<asset::Texture, asset::Texture::TextureData> {
		protected:
			bool LoadAsset_(asset::Texture* asset, asset::Texture::TextureData* data) override;
		public:
			void AssignSelfAsLoader() override;
		};
	}
}