#pragma once

#include "Engine/Objects/Object.h"
#include "Collider/Collider.h"
#include "Physics/gbe_physics.h"
#include "PhysicsObject.h"

namespace gbe {
	class TriggerRigidObject : PhysicsObject {
	public:
		void OnAddCollider(Collider* what) override;
		void OnRemoveCollider(Collider* what) override;
		void UpdateCollider(Collider* what) override;

		// Inherited via PhysicsObject
		void UpdatePhysicsTransformationMatrix() override;
	};
}