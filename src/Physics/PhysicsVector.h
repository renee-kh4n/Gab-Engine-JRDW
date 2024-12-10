#pragma once

#include "../Datatypes/Vectors.h"

#include <bullet/btBulletDynamicsCommon.h>

namespace gbe {
	namespace physics {
		class PhysicsVector3 : public Vector3 {
		public:
			PhysicsVector3();
			PhysicsVector3(const Vector3& normalvec);
			PhysicsVector3(const btVector3& btvec);
			operator btVector3() const;
		};
	}
}