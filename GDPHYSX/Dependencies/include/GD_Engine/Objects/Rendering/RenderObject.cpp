#include "RenderObject.h"
#include <glm/gtx/matrix_decompose.hpp>

using namespace gde;

RenderObject::RenderObject(rendering::DrawCall* mDrawCall)
{
	this->mDrawCall = mDrawCall;
}

RenderObject::~RenderObject()
{
	this->mDrawCall->calls.erase(this);
}

void RenderObject::InvokeEarlyUpdate()
{
	this->mDrawCall->calls.insert_or_assign(this, this->GetWorldSpaceMatrix());
}
