#include "Raycast.h"

#include "PhysicsPipeline.h"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"

gbe::physics::Raycast::Raycast(PhysicsVector3 from, PhysicsVector3 dir)
{
	PhysicsVector3 to = (PhysicsVector3)(from + dir);

	btCollisionWorld::ClosestRayResultCallback closestResults(from, to);
	closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

	physics::PhysicsPipeline::Get_Instance()->Get_world()->rayTest(from, to, closestResults);

	this->result = closestResults.hasHit();
	
	if (this->result) {
		//this->intersection = (PhysicsVector3)Vector3::Lerp(from, to, closestResults.m_closestHitFraction);
		this->other = nullptr;
	}
}
