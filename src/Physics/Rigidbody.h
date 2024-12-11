#pragma once

#include "../Datatypes/Vector3.h"
#include "../Datatypes/Matrix4.h"
#include "../Datatypes/Quaternion.h"

#include <bullet/btBulletDynamicsCommon.h>
#include "PhysicsDatatypes.h"
#include <list>
#include "ColliderData/ColliderData.h"

namespace gbe {
	namespace physics {
		class Rigidbody {
		private:
			btDynamicsWorld* world;

			btTransform transform;
			float mass;
			btRigidBody* data;
			btDefaultMotionState* motionstate;

			btCompoundShape* mMainShape;
			std::list<ColliderData*> colliders;
		public:
			void InjectCurrentTransformMatrix(Matrix4);
			void PassTransformationData(Vector3&, Quaternion&);
			void PassTransformationMatrix(Matrix4&);

			Rigidbody(bool is_static = false);
			btRigidBody* GetRegistrant(btDynamicsWorld* register_to);
			btRigidBody* UnRegister();

			void SetStatic(bool);
			void AddForce(PhysicsVector3 force);
			float Get_mass();
			PhysicsVector3 Get_angularVelocity();
			void Set_angularVelocity(PhysicsVector3);
			PhysicsVector3 Get_velocity();
			void Set_velocity(PhysicsVector3);
			void AddForceAtPoint(PhysicsVector3 force, PhysicsVector3 relativeWorldPoint);

			void AddCollider(ColliderData*);
			void UpdateColliderTransform(ColliderData*);
			void RemoveCollider(ColliderData*);

			void UpdateAABB();
		};
	}
}