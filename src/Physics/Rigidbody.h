#pragma once

#include "../Datatypes/Vector3.h"
#include "../Datatypes/Matrix4.h"

#include <bullet/btBulletDynamicsCommon.h>

namespace gbe {
	namespace physics {
		class Rigidbody {
		private:
			btTransform transform;
			btRigidBody* data;
		public:
			void PassCurrentTransformMatrix(Matrix4);

			Rigidbody();
			btRigidBody* GetRegistrant();

			void AddForce(Vector3 force);
			float Get_mass();
			Vector3 Get_angularVelocity();
			void Set_angularVelocity(Vector3);
			Vector3 Get_velocity();
			void Set_velocity(Vector3);
			void AddForceAtPoint(Vector3 force, Vector3 relativeWorldPoint);
		};
	}
}