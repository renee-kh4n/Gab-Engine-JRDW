#pragma once

#include "PhysicsDatatypes.h"
#include "PhysicsBody.h"

namespace gbe {
	class PhysicsObject;

	namespace physics {
		struct Raycast {
		public:
			bool result;
			PhysicsObject* other;
			PhysicsVector3 intersection;
			float distance;

			Raycast(PhysicsVector3 from, PhysicsVector3 dir);
		};
	}
}