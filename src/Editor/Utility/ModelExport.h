#pragma once

#include "Graphics/gbe_graphics.h"
#include "Engine/gbe_engine.h"

#include "Asset/AssetTypes/Mesh.h"

namespace gbe {
	namespace editor {
		class ModelExport {
			std::vector<gbe::Object*> selected;

			struct export_subobject {
				std::vector<asset::data::Vertex> export_vertices;
				std::vector<std::vector<uint16_t>> export_faces;
			};
		public:
			ModelExport(std::vector<gbe::Object*> selected);

			void Export(std::string path);
		};
	}
}