#include "Raycast.h"

#include "PhysicsPipeline.h"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"

gbe::physics::Raycast::RaycastCallback::RaycastCallback(PhysicsVector3 from, PhysicsVector3 to) : btCollisionWorld::ClosestRayResultCallback(from, to), hitObject(nullptr), localShapeInfo(nullptr) {}

btScalar gbe::physics::Raycast::RaycastCallback::addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace) {
    hitObject = rayResult.m_collisionObject;
    localShapeInfo = rayResult.m_localShapeInfo;

    // Access shape information using localShapeInfo and hitObject
    if (localShapeInfo) {
        // Example: If the object is a compound shape
        if (hitObject->getCollisionShape()->getShapeType() == COMPOUND_SHAPE_PROXYTYPE) {
            // Cast to btCompoundShape
            const btCompoundShape* compoundShape = static_cast<const btCompoundShape*>(hitObject->getCollisionShape());

            // Access child shape using localShapeInfo->m_shapePart
            int childShapeIndex = localShapeInfo->m_shapePart;
			if (childShapeIndex < 0)
				childShapeIndex = 0;

            this->hitShape = compoundShape->getChildShape(childShapeIndex);
		}
    }

    return btCollisionWorld::ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
}

gbe::physics::Raycast::Raycast(PhysicsVector3 from, PhysicsVector3 dir)
{
	PhysicsVector3 to = (PhysicsVector3)(from + dir);

	RaycastCallback closestResults(from, to);
	closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

	physics::PhysicsPipeline::Get_Instance()->Get_world()->rayTest(from, to, closestResults);

	this->result = closestResults.hasHit();
	
	if (this->result) {
		auto relatedbody = PhysicsPipeline::GetRelatedBody(closestResults.m_collisionObject);
		auto relatedcollider = PhysicsPipeline::GetRelatedCollider(closestResults.hitShape);
		this->other = relatedbody->Get_wrapper();
		this->collider = relatedcollider->Get_wrapper();
		this->intersection = closestResults.m_hitPointWorld;
		this->normal = closestResults.m_hitNormalWorld;
		Vector3 delta = from - this->intersection;
		this->distance = delta.Magnitude();
	}
}
