#pragma once
#include "ColliderData.h"

namespace gbe {
	namespace physics {
		class SphereColliderData : public ColliderData {
		private:
			btSphereShape mShape;
		public:
			SphereColliderData();
			virtual btCollisionShape* GetShape() override;
		};
	}
}