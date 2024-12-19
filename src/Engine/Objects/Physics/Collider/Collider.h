#pragma once
#include "Physics/ColliderData/ColliderData.h"
#include "Engine/Objects/Object.h"

namespace gbe {
	class RigidObject;

	class Collider : public Object {
	private:
		RigidObject* rigidparent;
	public:
		virtual physics::ColliderData* GetColliderData() = 0;

		RigidObject* GetRigidbody();
		void AssignToRigidbody(RigidObject*);
		void UnAssignRigidbody();

		void OnLocalTransformationChange(TransformChangeType) override;
		void OnExternalTransformationChange(TransformChangeType, Matrix4 newparentmat) override;
	};
}