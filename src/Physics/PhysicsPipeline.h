#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include "Rigidbody.h"

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
			void Tick(double delta);
			void RegisterBody(Rigidbody* body);
		};
	}
}