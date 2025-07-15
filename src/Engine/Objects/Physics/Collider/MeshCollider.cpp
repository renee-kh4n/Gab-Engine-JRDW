#include "MeshCollider.h"

gbe::MeshCollider::MeshCollider(asset::Mesh* mesh)
{
	auto newtris = std::vector<std::vector<Vector3>>();
	auto& verts = mesh->Get_load_data().vertices;

	for (const auto& tri : mesh->Get_load_data().faces)
	{
		newtris.push_back({ verts[tri[0]].pos,verts[tri[1]].pos, verts[tri[2]].pos });
	}

	this->mData = new physics::MeshColliderData(newtris, this);
}

gbe::physics::ColliderData* gbe::MeshCollider::GetColliderData()
{
	return this->mData;
}

