#pragma once

#include "../Object.h"
#include "Collider/Collider.h"
#include "../Physics/Rigidbody.h"

namespace gbe {
	class RigidObject : public Object {
	public:
		physics::Rigidbody linked_body;

		RigidObject();
		virtual ~RigidObject();

		void AddForce(Vector3 force);
		float Get_mass();
		Vector3 Get_angularVelocity();
		void Set_angularVelocity(Vector3);
		Vector3 Get_velocity();
		void Set_velocity(Vector3);
		void AddForceAtPoint(Vector3 force, Vector3 relativeWorldPoint);
		
		std::list<Collider*> colliders;

		virtual void OnEnterHierarchy(Object* newChild);
	};
}