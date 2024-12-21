#pragma once

#include "Asset/BaseAsset.h"
#include "Math/gbe_math.h"

namespace gbe {
	namespace asset {

		namespace data {
			struct TextureData {
				Vector2Int dimensions;
				int colorchannels;
			};
		}

		class Texture : public BaseAsset<Texture, data::TextureData> {
		public:
			Texture(std::string id, const char* path);
		};
	}
}