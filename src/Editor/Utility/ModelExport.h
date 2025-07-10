#pragma once

#include "Graphics/gbe_graphics.h"
#include "Engine/gbe_engine.h"

#include "Asset/AssetTypes/Mesh.h"

namespace gbe {
	namespace editor {
		class ModelExport {
			std::vector<gbe::Object*> selected;
		public:
			ModelExport(std::vector<gbe::Object*> selected);

			void Export(std::string path);
		};
	}
}