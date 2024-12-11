#pragma once

#include "Collider.h"

#include "Physics/ColliderData/BoxColliderData.h"

namespace gbe {
	class BoxCollider : public Collider {
	private:
		physics::BoxColliderData* mData;
	public:
		BoxCollider();

		// Inherited via Collider
		physics::ColliderData* GetColliderData() override;

		
	};
}