#pragma once

#include "Asset/BaseAsset.h"

namespace gbe {
	namespace asset {

		namespace data {
			struct MeshData {
				unsigned int gl_id;

			};
		}

		class Mesh : public BaseAsset<Mesh, data::MeshData> {
		public:

		};
	}
}