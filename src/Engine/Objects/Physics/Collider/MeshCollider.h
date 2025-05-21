#pragma once

#include "Collider.h"

#include "Physics/ColliderData/MeshColliderData.h"
#include "Asset/gbe_asset.h"

namespace gbe {
	class MeshCollider : public Collider{
	private:
		physics::MeshColliderData *mData;
	public:
		MeshCollider(asset::Mesh* mesh);

		// Inherited via Collider
		physics::ColliderData* GetColliderData() override;


	};
}