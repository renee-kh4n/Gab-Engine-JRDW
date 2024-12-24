#pragma once

#include "Asset/gbe_asset.h"
#include "Math/gbe_math.h"
#include <glad/glad.h>
#include <stb_image.h>
#include <unordered_map>
#include <stack>

namespace gbe {
	namespace gfx {
		class TextureLoader : public asset::AssetLoader<asset::Texture, asset::data::TextureImportData, asset::data::TextureLoadData> {
		private:
			std::unordered_map<asset::Texture*, unsigned int> textures_in_memory;
			std::unordered_map<unsigned int, unsigned int> buffers_in_renderstack;
			unsigned int texture_renderstack_index = 0;
			unsigned int texture_renderstack_index_locked = 0;
		protected:
			bool LoadAsset_(asset::Texture* asset, const asset::data::TextureImportData& importdata, asset::data::TextureLoadData* data) override;
			void UnLoadAsset_(asset::Texture* asset, const asset::data::TextureImportData& importdata, asset::data::TextureLoadData* data) override;
		public:
			static unsigned int Get_tex_glId(asset::Texture* tex);
			static unsigned int PushGet_texture_stackId(asset::Texture*);
			static unsigned int PushGet_bufferId_stackId(unsigned int bufferid);
			static void Reset_texture_stack();
			static void Lock_stack();
			static void UnLock_stack();
		};
	}
}