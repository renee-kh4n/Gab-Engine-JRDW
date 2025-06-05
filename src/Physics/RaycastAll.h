#pragma once

#include "PhysicsDatatypes.h"
#include "PhysicsBody.h"

#include <vector>

namespace gbe {
	class PhysicsObject;

	namespace physics {
		struct RaycastAll {
		public:
			bool result;
			std::vector<PhysicsObject*> others;
			PhysicsVector3 intersection;
			float distance;

			RaycastAll(PhysicsVector3 from, PhysicsVector3 dir);
		};
	}
}