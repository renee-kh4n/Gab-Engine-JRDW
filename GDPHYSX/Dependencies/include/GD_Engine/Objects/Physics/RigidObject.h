#pragma once

#include "../Object.h"

namespace gde {
	class RigidObject : public Object {
	public:
		//Rigidbody stuff
		float mass;
		Vector3 velocity;
		Vector3 frame_impule_force;
		Vector3 frame_continuous_force;
		Vector3 acceleration;
		float damping;

		RigidObject();
		virtual ~RigidObject() override;

		void AddForce(Vector3 force, bool continuous = false);
	};
}