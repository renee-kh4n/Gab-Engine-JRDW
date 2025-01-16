#pragma once

#include "Engine/Objects/Object.h"
#include "Collider/Collider.h"
#include "Physics/gbe_physics.h"
#include "PhysicsObject.h"

namespace gbe {
	class RigidObject : public PhysicsObject {
	public:
		RigidObject(bool is_static = false);
		~RigidObject();

		physics::Rigidbody* GetRigidbody();
	};
}