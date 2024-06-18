#include "RenderObject.h"
#include <glm/gtx/matrix_decompose.hpp>

gde::RenderObject::RenderObject(DrawCall* mDrawCall)
{
	this->mDrawCall = mDrawCall;
}

gde::RenderObject::~RenderObject()
{
	this->mDrawCall->calls.erase(this);
}

void gde::RenderObject::InvokeEarlyUpdate()
{
	this->mDrawCall->calls.insert_or_assign(this, this->GetWorldSpaceMatrix());
}
