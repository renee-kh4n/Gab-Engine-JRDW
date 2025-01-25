#pragma once

#include "ColliderData.h"

#include <vector>
#include "Math/gbe_math.h"

namespace gbe {
	namespace physics {
		class MeshColliderData : public ColliderData{
		private:
			btCollisionShape* trimeshShape;
		public:
			MeshColliderData(std::vector<std::vector<Vector3>>);
			virtual btCollisionShape* GetShape() override;
		};
	}
}