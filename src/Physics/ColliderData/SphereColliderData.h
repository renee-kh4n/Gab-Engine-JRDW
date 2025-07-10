#pragma once
#include "ColliderData.h"

namespace gbe {
	namespace physics {
		class SphereColliderData : public ColliderData {
		private:
			btSphereShape mShape;
		public:
			SphereColliderData(Collider* related_engine_wrapper);
			virtual btCollisionShape* GetShape() override;
		};
	}
}