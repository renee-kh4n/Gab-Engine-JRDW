#include "RenderObject.h"
#include <glm/gtx/matrix_decompose.hpp>

using namespace gbe::rendering;

gbe::RenderObject::RenderObject(DrawCall* mDrawCall)
{
	this->mDrawCall = mDrawCall;
	this->mDrawCall->calls.insert_or_assign(this, this->GetWorldMatrix());
	to_update = &this->mDrawCall->calls[this];
}

gbe::RenderObject::~RenderObject()
{
	this->mDrawCall->calls.erase(this);
}

void gbe::RenderObject::InvokeEarlyUpdate()
{
	*to_update = this->GetWorldMatrix();
}
