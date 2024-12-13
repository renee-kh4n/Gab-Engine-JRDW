#pragma once


#include "../Datatypes/Vectors.h"
#include "Framebuffer.h"

namespace gbe {
	namespace rendering {
		struct Light {
			virtual enum Type {
				POINT,
				CONE,
				DIRECTION
			} GetType() = 0;

			Vector3 color;
			float intensity;
			Vector3 pos;

			bool changed = true;
			size_t previous_render_index = 0;
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
			Framebuffer* shadowmap;

			// Inherited via Light
			virtual Type GetType() override;

			Vector3 dir;

			DirLight();
		};
	}
}