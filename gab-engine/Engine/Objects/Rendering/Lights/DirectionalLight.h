#pragma once

#include "LightObject.h"

namespace gde {
	class DirectionalLight : public gde::LightObject {
	public:
		// Inherited via Light
		virtual rendering::Light* GetData() override;
	};
}