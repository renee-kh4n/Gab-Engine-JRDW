#pragma once

#include "Asset/BaseAsset.h"

#include "Math/gbe_math.h"

namespace gbe {
	namespace asset {

		namespace data {
			struct MeshImportData {
				std::string path;
			};
			struct Vertex {
				Vector3 pos;
				Vector3 normal;
				Vector3 color;
				Vector2 texCoord;
			};
			struct MeshLoadData {
				std::vector<Vertex> vertices;
				std::vector<uint16_t> indices;
			};
		}

		class Mesh : public BaseAsset<Mesh, data::MeshImportData, data::MeshLoadData> {
		public:
			Mesh(std::string path);
		};
	}
}