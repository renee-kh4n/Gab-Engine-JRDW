#pragma once

#include "Asset/BaseAsset.h"
#include <functional>

#include "Math/gbe_math.h"

namespace gbe {
	namespace asset {
		namespace data {
			struct AudioLoadData {
				std::function<void(Vector3, float, int)> play;
			};
			struct AudioImportData {
				std::string filename;
			};
		}

		class Audio : public BaseAsset<Audio, data::AudioImportData, data::AudioLoadData> {
		public:
			Audio(std::string path);
			void Play(Vector3 position = Vector3::zero, float volume = 1.0f, int channel = 0);
		};
	}
}