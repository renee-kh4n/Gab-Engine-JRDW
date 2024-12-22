#pragma once

#include "Asset/BaseAsset.h"
#include <string>

namespace gbe {
	namespace asset {
		class AssetDeserializer {
		public:
			internal::BaseAsset_base* DeserializeFile(std::string path);
		};
	}
}