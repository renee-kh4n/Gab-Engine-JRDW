#pragma once

#include "../Datatypes/Vector3.h"
#include "../Datatypes/Matrix4.h"

#include <bullet/btBulletDynamicsCommon.h>
#include "PhysicsVector.h"

namespace gbe {
	namespace physics {
		class Rigidbody {
		private:
			btTransform transform;
			btRigidBody* data;
			btDefaultMotionState* motionstate;
		public:
			void PassCurrentTransformMatrix(Matrix4);
			void GetCalculatedMatrix(Matrix4*);

			Rigidbody();
			btRigidBody* GetRegistrant();

			void AddForce(PhysicsVector3 force);
			float Get_mass();
			PhysicsVector3 Get_angularVelocity();
			void Set_angularVelocity(PhysicsVector3);
			PhysicsVector3 Get_velocity();
			void Set_velocity(PhysicsVector3);
			void AddForceAtPoint(PhysicsVector3 force, PhysicsVector3 relativeWorldPoint);
		};
	}
}