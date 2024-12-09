#pragma once

#include "../Object.h"
#include "Collider/Collider.h"
#include "../Physics/Rigidbody.h"

namespace gbe {
	class RigidObject : public Object {
	private:
		std::list<Collider*> colliders;
	public:
		physics::Rigidbody body;

		RigidObject();
		virtual ~RigidObject();

		virtual void OnEnterHierarchy(Object* newChild);
	};
}