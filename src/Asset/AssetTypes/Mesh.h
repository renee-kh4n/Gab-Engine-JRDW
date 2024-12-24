#pragma once

#include "Asset/BaseAsset.h"

namespace gbe {
	namespace asset {

		namespace data {
			struct MeshLoadData {
				unsigned int gl_id;

			};
			struct MeshImportData {
				unsigned int gl_id;

			};
		}

		class Mesh : public BaseAsset<Mesh, data::MeshImportData, data::MeshLoadData> {
		public:

		};
	}
}