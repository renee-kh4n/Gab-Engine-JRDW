#pragma once

namespace gbe {
	namespace asset {
		namespace data {
			struct AudioLoadData {
				unsigned int gl_id;

			};
			struct AudioImportData {
				unsigned int gl_id;

			};
		}

		class Audio : public BaseAsset<Audio, data::AudioImportData, data::AudioLoadData> {
		public:

		};
	}
}