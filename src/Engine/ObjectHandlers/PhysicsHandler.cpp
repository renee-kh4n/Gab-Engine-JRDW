#include "PhysicsHandler.h"

#include <iostream>

gbe::PhysicsHandler::PhysicsHandler()
{
	this->subhandlers.push_back(&this->forcevolume_handler);
}

void gbe::PhysicsHandler::Update(double duration)
{
	for (auto ro : this->object_list) {

	}
}
