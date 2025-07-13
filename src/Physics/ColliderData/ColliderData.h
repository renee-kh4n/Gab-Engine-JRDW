#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include "../../Math/Matrix4.h"
#include "../PhysicsDatatypes.h"

namespace gbe {
	class Collider;

	namespace physics {
		class Rigidbody;

		class ColliderData {
		private:
			Matrix4 local;
			PhysicsVector3 scale;

			btTransform transform;

		protected:
			Collider* related_engine_wrapper;
		public:
			ColliderData(Collider* related_engine_wrapper);

			Matrix4& GetLocalMatrix();
			btTransform& GetInternalTransform();

			void UpdateLocalTransformation(Matrix4);
			void UpdateScale(PhysicsVector3);

			virtual btCollisionShape* GetShape() = 0;

			Collider* Get_wrapper();
		};
	}
}