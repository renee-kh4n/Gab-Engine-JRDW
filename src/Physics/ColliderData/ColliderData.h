#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include "../../Datatypes/Matrix4.h"
#include "../PhysicsDatatypes.h"

namespace gbe {
	namespace physics {
		class Rigidbody;

		class ColliderData {
		private:
			Rigidbody* owner;

			Matrix4 local;
			btTransform transform;

			PhysicsVector3 scale;
		public:
			Matrix4& GetLocalMatrix();
			btTransform& GetInternalTransform();

			void UpdateLocalTransformation(Matrix4);
			void UpdateScale(PhysicsVector3);

			void AssignOwner(Rigidbody* owner);
			void RemoveOwner();

			virtual btCollisionShape* GetShape() = 0;
		};
	}
}