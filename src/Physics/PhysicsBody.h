#pragma once

#include "Math/gbe_math.h"

#include <bullet/btBulletDynamicsCommon.h>
#include "PhysicsDatatypes.h"
#include <list>
#include "ColliderData/ColliderData.h"

namespace gbe {
	namespace physics {
		class PhysicsBody {
		protected:
			btDynamicsWorld* world;

			btTransform transform;
			btCollisionObject* base_data;
			btDefaultMotionState* motionstate;

			btCompoundShape* mMainShape;
			PhysicsBody();
		public:
			void InjectCurrentTransformMatrix(Matrix4);
			void PassTransformationData(Vector3&, Quaternion&);
			void PassTransformationMatrix(Matrix4&);

			btCollisionObject* GetRegistrant(btDynamicsWorld* register_to);
			btCollisionObject* UnRegister();

			void AddCollider(ColliderData*);
			void UpdateColliderTransform(ColliderData*);
			void RemoveCollider(ColliderData*);

			void UpdateAABB();
			virtual void ForceUpdateTransform() = 0;
		};
	}
}