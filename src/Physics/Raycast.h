#pragma once

#include "PhysicsDatatypes.h"
#include "PhysicsBody.h"

#include <bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h>

namespace gbe {
	class PhysicsObject;
	class Collider;

	namespace physics {
		struct Raycast {
		private:
			class RaycastCallback : public btCollisionWorld::ClosestRayResultCallback {
			public:
				const btCollisionShape* hitShape;
				const btCollisionObject* hitObject;
				btCollisionWorld::LocalShapeInfo* localShapeInfo;

				RaycastCallback(PhysicsVector3 from, PhysicsVector3 to);

				btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace) override;
			};
		
		public:
			bool result;
			PhysicsObject* other;
			Collider* collider;
			PhysicsVector3 intersection;
			float distance;

			Raycast(PhysicsVector3 from, PhysicsVector3 dir);
			
		};
	}
}