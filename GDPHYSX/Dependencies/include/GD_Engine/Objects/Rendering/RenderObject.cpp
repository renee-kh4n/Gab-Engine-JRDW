#include "RenderObject.h"
#include <glm/gtx/matrix_decompose.hpp>

gde::RenderObject::RenderObject(DrawCall* mDrawCall)
{
	this->mDrawCall = mDrawCall;
}

gde::RenderObject::~RenderObject()
{
}

void gde::RenderObject::InvokeEarlyUpdate()
{
	auto transform = this->World();// your transformation matrix.

	this->mDrawCall->pos = transform->position;
	this->mDrawCall->scale = transform->scale;
	this->mDrawCall->rot = transform->rotation;
}
