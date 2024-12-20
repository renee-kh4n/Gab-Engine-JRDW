#pragma once

#include "Asset/BaseAsset.h"
#include "Math/gbe_math.h"

namespace gbe {
	namespace asset {

		class Texture : public BaseAsset {
		public:
			struct TextureData {
				unsigned int gl_id = 0;
				Vector2Int dimensions;
				int colorchannels;
				bool loaded_in_gpu = false;
				unsigned int gpu_texture_address;
			};
		private:
			TextureData data;
		public:
			Texture(std::string id, const char* path);

			unsigned int Get_gl_id();

			bool CheckGpuLoaded(int& address);
		};
	}
}