#pragma once

#include "Asset/BaseAsset.h"

namespace gbe {
	namespace asset {
		namespace data {
			struct AudioLoadData {
				
			};
			struct AudioImportData {
				std::string filename;
			};
		}

		class Audio : public BaseAsset<Audio, data::AudioImportData, data::AudioLoadData> {
		public:
			Audio(std::string path);
		};
	}
}