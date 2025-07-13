#pragma once
#include "ColliderData.h"
#include "../PhysicsDatatypes.h"

namespace gbe {
	namespace physics {
		class BoxColliderData : public ColliderData {
		private:
			btBoxShape mShape;
		public:
			BoxColliderData(Collider* related_engine_wrapper);
			virtual btCollisionShape* GetShape() override;
		};
	}
}