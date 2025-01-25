#pragma once

#include "Collider.h"

#include "Physics/ColliderData/MeshColliderData.h"
#include "Graphics/Data/Mesh.h"

namespace gbe {
	class MeshCollider : public Collider{
	private:
		physics::MeshColliderData *mData;
	public:
		MeshCollider(gfx::Mesh* mesh);

		// Inherited via Collider
		physics::ColliderData* GetColliderData() override;


	};
}