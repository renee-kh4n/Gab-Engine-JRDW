#include "RaycastAll.h"

#include "PhysicsPipeline.h"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"

gbe::physics::RaycastAll::RaycastAll(PhysicsVector3 from, PhysicsVector3 dir)
{
	PhysicsVector3 to = (PhysicsVector3)(from + dir);

	btCollisionWorld::AllHitsRayResultCallback allResults(from, to);
	allResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

	physics::PhysicsPipeline::Get_Instance()->Get_world()->rayTest(from, to, allResults);

	this->result = allResults.hasHit();
	
	if (this->result) {
		for (const auto& other : allResults.m_collisionObjects)
		{

		}

		this->others = PhysicsPipeline::GetRelatedBody(closestResults.m_collisionObject)->Get_wrapper();
		this->intersection = closestResults.m_hitPointWorld;
		Vector3 delta = from - this->intersection;
		this->distance = delta.Magnitude();
	}
}
