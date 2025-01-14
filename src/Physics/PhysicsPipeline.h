#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include "Rigidbody.h"

namespace gbe {
	namespace physics {
		class PhysicsPipeline
		{
		private:
			static PhysicsPipeline* Instance;

			btDefaultCollisionConfiguration* collisionConfiguration;
			btCollisionDispatcher* dispatcher;
			btBroadphaseInterface* overlappingPairCache;
			btSequentialImpulseConstraintSolver* solver;
			btDiscreteDynamicsWorld* dynamicsWorld;
		public:
			static PhysicsPipeline* Get_Instance();
			
			bool Init();
			void Tick(double delta);
			void RegisterBody(Rigidbody* body);
			void UnRegisterBody(Rigidbody* body);
		};
	}
}