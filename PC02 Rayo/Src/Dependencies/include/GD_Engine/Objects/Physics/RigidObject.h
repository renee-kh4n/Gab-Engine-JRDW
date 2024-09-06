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

		Vector3 angularVelocity = Vector3::zero;
		float amgularDamp = 1.0f;
		Vector3 accumulatedTorque = Vector3::zero;
		virtual float MomentOfInertia();
		void AddForceAtPoint(Vector3 force, Vector3 relativeWorldPoint);

		virtual void OnEnterHierarchy(Object* newChild);
	};
}