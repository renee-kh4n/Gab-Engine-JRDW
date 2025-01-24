#pragma once

#include "PhysicsDatatypes.h"
#include "PhysicsBody.h"

namespace gbe {
	namespace physics {
		struct Raycast {
		public:
			bool result;
			PhysicsBody* other;
			PhysicsVector3 intersection;

			Raycast(PhysicsVector3 from, PhysicsVector3 dir);
		};
	}
}