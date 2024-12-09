#pragma once

#include <bullet/btBulletDynamicsCommon.h>

namespace gbe {
	namespace physics {
		class PhysicsPipeline
		{
		private:
			btDefaultCollisionConfiguration* collisionConfiguration;
			btCollisionDispatcher* dispatcher;
			btBroadphaseInterface* overlappingPairCache;
			btSequentialImpulseConstraintSolver* solver;
			btDiscreteDynamicsWorld* dynamicsWorld;
		public:
			bool Init();
		};
	}
}