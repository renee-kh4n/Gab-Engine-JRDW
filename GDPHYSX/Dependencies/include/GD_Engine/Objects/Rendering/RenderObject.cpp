#include "RenderObject.h"
#include <glm/gtx/matrix_decompose.hpp>

gde::RenderObject::RenderObject(DrawCall* mDrawCall)
{
	this->mDrawCall = mDrawCall;
	this->mDrawCall->calls.insert_or_assign(this, this->GetWorldSpaceMatrix());
}

gde::RenderObject::~RenderObject()
{
	this->mDrawCall->calls.erase(this);
}
