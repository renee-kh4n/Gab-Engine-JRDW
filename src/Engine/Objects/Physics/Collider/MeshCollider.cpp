#include "MeshCollider.h"

gbe::MeshCollider::MeshCollider(asset::Mesh* mesh)
{
	std::vector<std::vector<Vector3>> tris;

	throw new std::exception("Not implemented yet.");

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
