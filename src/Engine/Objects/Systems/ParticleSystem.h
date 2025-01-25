#pragma once

#include "../Object.h"
#include "../Physics/RigidObject.h"
#include "../../ObjectFunctions/Update.h"

#include <unordered_map>
#include <functional>

namespace gbe {
	class ParticleSystem : public Object, public Update{
	private:
		std::unordered_map<RigidObject*, float> particles;
		Vector3 continuous_force;

		Vector3 localbound_from;
		Vector3 localbound_to;
		std::function<RigidObject*(ParticleSystem*)> particle_create;
		float time_lastspawned = 0.0f;
		float time_perspawn = 0.2f;
		float life_time = 5.0f;

		bool enabled = true;
	public:
		ParticleSystem(std::function<RigidObject* (ParticleSystem*)>);
		void InvokeUpdate(float deltatime) override;
		void SetBounds(Vector3 from, Vector3 to);
		void Set_force(Vector3 newforce);
		void Set_rate(float rate);
		void Set_lifetime(float len);

		void Set_enabled(bool);
	};
}