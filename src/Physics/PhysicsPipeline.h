#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include "Rigidbody.h"
#include <unordered_map>
#include <functional>

namespace gbe {
	namespace physics {
		class PhysicsPipeline
		{
		private:
			static PhysicsPipeline* Instance;
			std::unordered_map<btCollisionObject*, PhysicsBody*> data_wrapper_dictionary;
			std::function<void(float physicsdeltatime)> OnFixedUpdate_callback;

			btDefaultCollisionConfiguration* collisionConfiguration;
			btCollisionDispatcher* dispatcher;
			btBroadphaseInterface* overlappingPairCache;
			btSequentialImpulseConstraintSolver* solver;
			btDiscreteDynamicsWorld* dynamicsWorld;

			void internal_physics_callback(btDynamicsWorld* world, btScalar timeStep);
		public:
			static PhysicsPipeline* Get_Instance();
			
			bool Init();
			void Tick(double delta);
			void RegisterBody(PhysicsBody* body);
			void UnRegisterBody(PhysicsBody* body);
			void Set_OnFixedUpdate_callback(std::function<void(float physicsdeltatime)>);

			btDiscreteDynamicsWorld* Get_world();

			PhysicsBody* GetRelatedBody(btCollisionObject*);
		};
	}
}