#pragma once

#include "../Object.h"
#include <GD_Graphics/Light.h>

namespace gde {
	class Light : public Object{
	protected:
		rendering::Light* mLight;
	public:
		Vector3 Color;
		float intensity;

		virtual rendering::Light* GetData() = 0;
	};
}