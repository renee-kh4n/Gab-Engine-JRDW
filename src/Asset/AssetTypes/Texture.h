#pragma once

#include "Asset/BaseAsset.h"
#include "Math/gbe_math.h"

namespace gbe {
	namespace asset {

		namespace data {
			struct TextureImportData
			{
				std::string filename;
			};
			struct TextureLoadData {
				Vector2Int dimensions;
				int colorchannels;
			};
		}

		class Texture : public BaseAsset<Texture, data::TextureImportData, data::TextureLoadData> {
		public:
			Texture(std::string asset_path);
		};
	}
}