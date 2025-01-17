#pragma once

#include "Math/gbe_math.h"

#include <bullet/btBulletDynamicsCommon.h>
#include "PhysicsDatatypes.h"
#include <list>
#include "ColliderData/ColliderData.h"

#include "PhysicsBody.h"


namespace gbe {
	namespace physics {
		class TriggerRigidBody : public PhysicsBody {
		public:
			TriggerRigidBody();

			void Register(btDynamicsWorld* register_to) override;
			void UnRegister() override;

			int Get_numInside();
			PhysicsBody* Get_inside(int index);

			void ForceUpdateTransform() override;
		};
	}
}