#include "PhysicsHandler.h"

#include <iostream>

gbe::PhysicsHandler::PhysicsHandler(physics::PhysicsPipeline* pipeline)
{
	auto map_ptr = &this->map;
	this->lookup_func = [=](physics::PhysicsBody* key) {
		auto it = map_ptr->find(key);
		PhysicsObject* toreturn = nullptr;

		if (it != this->map.end())
			toreturn = it->second;

		return toreturn;
	};

	this->subhandlers.push_back(&this->forcevolume_handler);
	this->mPipeline = pipeline;
}

void gbe::PhysicsHandler::Update()
{
	for (auto po : this->object_list) {

		auto ro = dynamic_cast<RigidObject*>(po);

		if (ro == nullptr)
			continue;

		Vector3 newpos;
		Quaternion newrot;

		ro->Get_data()->PassTransformationData(newpos, newrot);
		ro->Local().position.Set(newpos);
		ro->Local().rotation.Set(newrot);

		for (auto fv : this->forcevolume_handler.object_list)
		{
			fv->TryApply(ro);
		}
	}
}

void gbe::PhysicsHandler::OnAdd(PhysicsObject* ro)
{
	map.insert_or_assign(ro->Get_data(), ro);
	ro->Set_lookup_func(&this->lookup_func);
	this->mPipeline->RegisterBody(ro->Get_data());
}

void gbe::PhysicsHandler::OnRemove(PhysicsObject* ro)
{
	map.erase(ro->Get_data());
	ro->Set_lookup_func(nullptr);
	this->mPipeline->UnRegisterBody(ro->Get_data());
}