#pragma once

#include "Math/gbe_math.h"

#include <bullet/btBulletDynamicsCommon.h>

#include "PhysicsDatatypes.h"
#include <list>
#include "ColliderData/ColliderData.h"

#include "PhysicsBody.h"

namespace gbe {
	namespace physics {
		class Rigidbody : public PhysicsBody{
		private:
			float mass;
		public:
			Rigidbody(bool is_static = false);

			void Register(btDynamicsWorld* register_to) override;
			void UnRegister() override;

			void SetStatic(bool);
			void AddForce(PhysicsVector3 force);
			float Get_mass();
			PhysicsVector3 Get_angularVelocity();
			void Set_angularVelocity(PhysicsVector3);
			PhysicsVector3 Get_velocity();
			void Set_velocity(PhysicsVector3);
			void AddForceAtPoint(PhysicsVector3 force, PhysicsVector3 relativeWorldPoint);			

			void ForceUpdateTransform() override;
		};
	}
}