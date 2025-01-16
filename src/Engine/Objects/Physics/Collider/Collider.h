#pragma once
#include "Physics/ColliderData/ColliderData.h"
#include "Engine/Objects/Object.h"

namespace gbe {
	class PhysicsObject;

	class Collider : public Object {
	private:
		PhysicsObject* holder;
	public:
		virtual physics::ColliderData* GetColliderData() = 0;

		PhysicsObject* GetBody();
		void AssignToBody(PhysicsObject*);
		void UnAssignBody();

		void OnLocalTransformationChange(TransformChangeType) override;
		void OnExternalTransformationChange(TransformChangeType, Matrix4 newparentmat) override;
	};
}