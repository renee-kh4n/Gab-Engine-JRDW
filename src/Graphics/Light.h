#pragma once


#include "../Datatypes/Vectors.h"
#include "../Datatypes/Matrix4.h"
#include "Framebuffer.h"

namespace gbe {
	namespace rendering {
		struct Light {
			virtual enum Type {
				DIRECTION = 0,
				POINT,
				CONE
			} GetType() = 0;

			Vector3 color;
			float intensity;
			Vector3 pos;

			bool changed = true;
			size_t previous_render_index = 0;

			virtual void SetShadowmapResolution(int res) = 0;
		};

		struct PointLight : public Light {
			// Inherited via Light
			virtual Type GetType() override;
		};

		struct ConeLight : public Light {
			// Inherited via Light
			virtual Type GetType() override;

			Vector3 dir;
			Vector2 angle;
		};

		struct DirLight : public Light {
			std::vector<Framebuffer*> shadowmaps;
			std::vector<float> cascade_splits;
			std::vector<Matrix4> cascade_projections;

			// Inherited via Light
			virtual Type GetType() override;

			Vector3 dir;

			DirLight();
			virtual void SetShadowmapResolution(int res);
		};
	}
}