#pragma once

#include "../Object.h"
#include "Collider/Collider.h"

namespace gde {
	class RigidObject : public Object {
	public:
		//Rigidbody stuff
		float mass;
		Vector3 velocity;
		Vector3 frame_force;
		Vector3 acceleration;
		float damping;

		RigidObject();
		virtual ~RigidObject();

		void AddForce(Vector3 force);
		std::list<Collider*> colliders;

		virtual void OnEnterHierarchy(Object* newChild);
	};
}