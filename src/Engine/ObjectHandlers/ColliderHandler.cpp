#include "ColliderHandler.h"

gbe::ColliderHandler::ColliderHandler(physics::PhysicsPipeline* pipeline)
{
	this->mPipeline = pipeline;
}

void gbe::ColliderHandler::OnAdd(Collider* col)
{
	this->mPipeline->RegisterCollider(col->GetColliderData());
}

void gbe::ColliderHandler::OnRemove(Collider* col)
{
	this->mPipeline->UnRegisterCollider(col->GetColliderData());
}