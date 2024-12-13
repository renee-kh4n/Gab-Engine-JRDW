#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include "../../Datatypes/Matrix4.h"
#include "../PhysicsDatatypes.h"

namespace gbe {
	namespace physics {
		class Rigidbody;

		class ColliderData {
		private:
			Matrix4 local;
			PhysicsVector3 scale;

			btTransform transform;

		public:
			ColliderData();

			Matrix4& GetLocalMatrix();
			btTransform& GetInternalTransform();

			void UpdateLocalTransformation(Matrix4);
			void UpdateScale(PhysicsVector3);

			virtual btCollisionShape* GetShape() = 0;
		};
	}
}