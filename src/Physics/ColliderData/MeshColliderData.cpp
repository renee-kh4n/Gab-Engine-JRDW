#include "MeshColliderData.h"

gbe::physics::MeshColliderData::MeshColliderData(std::vector<std::vector<Vector3>> tris, Collider* related_engine_wrapper) : ColliderData(related_engine_wrapper)
{
	btTriangleMesh* trimesh = new btTriangleMesh();

	for (auto tri : tris)
	{
		trimesh->addTriangle((PhysicsVector3)tri[0], (PhysicsVector3)tri[1], (PhysicsVector3)tri[2]);
	}
	
	btTransform	trans;
	trans.setIdentity();

	this->trimeshShape = new btBvhTriangleMeshShape( trimesh, false );
}

btCollisionShape* gbe::physics::MeshColliderData::GetShape()
{
	return this->trimeshShape;
}
