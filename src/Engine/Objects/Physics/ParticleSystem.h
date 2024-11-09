#pragma once

#include <unordered_map>

#include "../Object.h"
#include "RigidObject.h"

#include "ParticleSystem/VectorParameter.h"
#include "ParticleSystem/FloatParameter.h"

#include "../../ObjectFunctions/Update.h"

namespace gbe {
	using namespace particlesystem;

	class ParticleSystem : public Object, public Update
	{
	private:
		std::unordered_map<RigidObject*, float> particles;

		std::function<RigidObject*(void)> particle_function;

		float time_last_spawned = 0;
	public:
		float spawns_per_sec = 1;
		bool world_space = false;

		VectorParameter start_force;
		FloatParameter start_speed;
		FloatParameter start_lifetime;

		ParticleSystem(std::function<RigidObject* (void)> particle_function);

		void Spawn();
		void Spawn(int count);

		// Inherited via Update
		virtual void InvokeUpdate(float deltatime) override;
	};
}