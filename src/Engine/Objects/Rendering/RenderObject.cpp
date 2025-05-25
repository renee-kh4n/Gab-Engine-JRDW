#include "RenderObject.h"
#include <glm/gtx/matrix_decompose.hpp>

using namespace gbe::gfx;

gbe::RenderObject::RenderObject(DrawCall* mDrawCall)
{
	this->mDrawCall = mDrawCall;
	to_update = this->mDrawCall->RegisterCall(this, this->GetWorldMatrix());
}

gbe::RenderObject::~RenderObject()
{
	this->mDrawCall->UnRegisterCall(this);
}

void gbe::RenderObject::InvokeEarlyUpdate()
{
	*to_update = this->GetWorldMatrix();
}
