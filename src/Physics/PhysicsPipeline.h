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
			std::unordered_map<const btCollisionObject*, PhysicsBody*> body_wrapper_dictionary;
			std::unordered_map<const btCollisionShape*, ColliderData*> collider_wrapper_dictionary;
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
			void RegisterCollider(ColliderData* body);
			void UnRegisterCollider(ColliderData* body);
			void Set_OnFixedUpdate_callback(std::function<void(float physicsdeltatime)>);

			btDiscreteDynamicsWorld* Get_world();

			static PhysicsBody* GetRelatedBody(const btCollisionObject*);
			static ColliderData* GetRelatedCollider(const btCollisionShape*);
		};
	}
}