#pragma once

#include "../Datatypes/Vectors.h"
#include "../Datatypes/Quaternion.h"

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

		class PhysicsQuaternion : public Quaternion {
		public:
			PhysicsQuaternion();
			PhysicsQuaternion(const Quaternion& normalquat);
			PhysicsQuaternion(const btQuaternion& btquat);
			operator btQuaternion() const;
		};
	}
}