#include "RenderObject.h"
#include <glm/gtx/matrix_decompose.hpp>

using namespace gde::rendering;

gde::RenderObject::RenderObject(DrawCall* mDrawCall)
{
	this->mDrawCall = mDrawCall;
	this->mDrawCall->calls.insert_or_assign(this, this->parent_matrix * this->local_matrix);
	to_update = &this->mDrawCall->calls[this];
}

gde::RenderObject::~RenderObject()
{
	this->mDrawCall->calls.erase(this);
}

void gde::RenderObject::InvokeEarlyUpdate()
{
	*to_update = this->parent_matrix * this->local_matrix;
}
