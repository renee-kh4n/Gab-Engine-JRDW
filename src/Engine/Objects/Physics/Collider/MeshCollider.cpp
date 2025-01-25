#include "MeshCollider.h"

gbe::MeshCollider::MeshCollider(gfx::Mesh* mesh)
{
	auto tris = mesh->triangles;

	auto newtris = std::vector<std::vector<Vector3>>();

	for (auto tri : tris)
	{
		newtris.push_back({ tri[0],tri[1], tri[2] });
	}

	this->mData = new physics::MeshColliderData(newtris);
}

gbe::physics::ColliderData* gbe::MeshCollider::GetColliderData()
{
	return this->mData;
}
