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

		RigidObject(bool is_static = false);
		virtual ~RigidObject();

		virtual void OnEnterHierarchy(Object* newChild);
		virtual void OnExitHierarchy(Object* newChild);

		void OnLocalTransformationChange(TransformChangeType) override;
		void OnExternalTransformationChange(TransformChangeType, Matrix4 parentmat) override;

		void UpdateCollider(Collider* what);
	};
}