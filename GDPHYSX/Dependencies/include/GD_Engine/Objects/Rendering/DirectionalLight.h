#pragma once

#include "Light.h"

namespace gde {
	class DirectionalLight : public gde::Light {
	public:
		// Inherited via Light
		virtual rendering::Light* GetData() override;
	};
}