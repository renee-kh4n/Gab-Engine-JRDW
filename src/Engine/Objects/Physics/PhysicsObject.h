#pragma once

#include "Engine/Objects/Object.h"
#include "Collider/Collider.h"
#include "Physics/gbe_physics.h"

namespace gbe {
	class PhysicsObject : public Object {
	protected:
		std::list<Collider*> colliders;
	public:
		virtual ~PhysicsObject();

		virtual void OnEnterHierarchy(Object* newChild);
		virtual void OnExitHierarchy(Object* newChild);

		void OnLocalTransformationChange(TransformChangeType) override;
		void OnExternalTransformationChange(TransformChangeType, Matrix4 parentmat) override;

		virtual void OnAddCollider(Collider* what) = 0;
		virtual void OnRemoveCollider(Collider* what) = 0;
		virtual void UpdatePhysicsTransformationMatrix() = 0;
		virtual void UpdateCollider(Collider* what) = 0;
	};
}