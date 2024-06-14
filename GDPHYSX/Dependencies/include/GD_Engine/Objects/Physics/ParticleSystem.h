#pragma once

#include "../Object.h"
#include "RigidObject.h"

#include "ParticleSystem/VectorParameter.h"
#include "ParticleSystem/FloatParameter.h"

#include "../../ObjectFunctions/Update.h"

namespace gde {
	using namespace particlesystem;

	class ParticleSystem : public Object, public Update
	{
	private:
		RigidObject particle;

		float time_last_spawned;
	public:
		float spawns_per_sec;
		bool world_space;

		VectorParameter start_force;
		FloatParameter start_speed;
		FloatParameter start_lifetime;

		ParticleSystem(RigidObject particle);

		void Spawn();

		// Inherited via Update
		virtual void InvokeUpdate(float deltatime) override;
	};
}