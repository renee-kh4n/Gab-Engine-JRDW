#pragma once

#include "../RigidObject.h"

namespace gde {
	class ParticleClass : public RigidObject {
	private:
		float radius;
	
	public:
		ParticleClass(float radius = 1);

		float GetRadius();
	};
}