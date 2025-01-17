#pragma once

#include "Engine/Objects/Object.h"
#include "Collider/Collider.h"
#include "Physics/gbe_physics.h"
#include "PhysicsObject.h"
#include <functional>
#include "Engine/ObjectFunctions/PhysicsUpdate.h"

namespace gbe {
	class TriggerRigidObject : public PhysicsObject, public PhysicsUpdate {
	private:
		std::function<void(PhysicsObject*)> OnEnter;
	public:
		TriggerRigidObject();

		physics::TriggerRigidBody* GetTriggerRigidbody();
		void Set_OnEnter(std::function<void(PhysicsObject*)> func);

		void InvokePhysicsUpdate(float deltatime) override;
	};
}