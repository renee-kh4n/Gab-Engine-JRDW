#pragma once

#include "Asset/BaseAsset.h"

namespace gbe {
	namespace asset {

		namespace data {
			struct MeshImportData {
				std::string path;
			};
			struct MeshLoadData {
				unsigned int gl_id;

			};
		}

		class Mesh : public BaseAsset<Mesh, data::MeshImportData, data::MeshLoadData> {
		public:
			Mesh(std::string path);
		};
	}
}