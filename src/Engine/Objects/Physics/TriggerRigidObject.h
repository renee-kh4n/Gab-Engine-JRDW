#pragma once

#include "Engine/Objects/Object.h"
#include "Collider/Collider.h"
#include "Physics/gbe_physics.h"
#include "PhysicsObject.h"
#include <functional>
#include "Engine/ObjectFunctions/PhysicsUpdate.h"
#include <algorithm>

namespace gbe {
	class TriggerRigidObject : public PhysicsObject, public PhysicsUpdate {
	private:
		std::vector<PhysicsObject*> inside_last_Frame;

		std::function<void(PhysicsObject*)> OnEnter;
		std::function<void(PhysicsObject*)> OnExit;
		std::function<void(PhysicsObject*, float)> OnStay;
	public:
		TriggerRigidObject();

		physics::TriggerRigidBody* GetTriggerRigidbody();
		void Set_OnEnter(std::function<void(PhysicsObject*)> func);
		void Set_OnExit(std::function<void(PhysicsObject*)> func);
		void Set_OnStay(std::function<void(PhysicsObject*, float)> func);

		void InvokePhysicsUpdate(float deltatime) override;
	};
}