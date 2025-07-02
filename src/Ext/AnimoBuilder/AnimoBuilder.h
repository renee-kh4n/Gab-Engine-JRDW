#pragma once

#include "Math/gbe_math.h"

#include <string>
#include <vector>

namespace gbe {
	namespace ext {
		namespace AnimoBuilder {
			//INPUT PARAMETERS
			struct GenerationParams {
				Vector3 from = Vector3(-20, 0, 0);
				Vector3 to = Vector3(100, 0, 0);
				Vector3 up = Vector3(0, 1, 0);
				float base_height = 12;
				float base_width = 6;
				float height = 20;
				float pillarInterval = 16;
				float pillarThickness = 2.5f;
				float wallThickness = 0.5f;
				float beamInterval = 9;
				float beamThickness = 1.8f;
				float windowHeight = 3;
				Vector3 windowSize = Vector3(2.5, 4.5, 0.6f);
				float roofHeight = 5.f;
				float roofThickness = 3.0f;
			};
			//OUTPUT MESH
			struct SubMesh {
				std::string type;
				Vector3 position;
				Vector3 scale;
				Quaternion rotation;
			};
			//OUTPUT DATA
			struct GenerationResult {
				std::vector<SubMesh> meshes;
			};

			class AnimoBuilder
			{
			private:
				
			public:
				AnimoBuilder();
				~AnimoBuilder();

				static GenerationResult Generate(GenerationParams params);
			};
		}
	}
}