#include "GenericObject.h"

gbe::GenericObject::GenericObject(std::function<void(GenericObject*, float delta)> n_Update)
{
	this->onUpdate = n_Update;
}

void gbe::GenericObject::InvokeUpdate(float delta)
{
	this->onUpdate(this, delta);
}
