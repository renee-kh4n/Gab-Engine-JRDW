#pragma once

#include "../RigidObject.h"
#include "../../../Datatypes/Vector3.h"

namespace gde {
	class CollisionContact {
	public:
		RigidObject* objects[2];
		float restitution;
		Vector3 contactNormal;
		void Resolve(float time);

	protected:
		float GetSeperatingSpeed();
		void ResolveVelocity(float time);
	};
}