#pragma once

#include "../Object.h"
#include "RigidObject.h"

namespace gde {
	class ParticleSystem : public Object
	{
	public:


	private:
		float spawns_per_sec;
		RigidObject* particle;
		bool world_space;
	};
}