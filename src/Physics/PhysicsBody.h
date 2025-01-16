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
			float mass;
			btCollisionObject* data;
			btDefaultMotionState* motionstate;

			btCompoundShape* mMainShape;
		};
	}
}