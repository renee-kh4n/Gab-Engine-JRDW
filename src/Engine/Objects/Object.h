#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Component/Transform.h"
#include "Component/TransformChangeType.h"
#include <list>
#include <functional>

#include "../Math/Matrix4.h"

namespace gbe {

	class Object {
	private:
		bool isDestroyQueued = false;

		std::list<Object*> children;

		Transform local = Transform([this](TransformChangeType type) {this->OnLocalTransformationChange(type); });
		Transform world = Transform([](TransformChangeType type) {});

		void MatToTrans(Transform* target, Matrix4 mat);
		
		Matrix4 parent_matrix;
	protected:
		Object* parent;
		virtual Object* Copy_self();
		virtual void OnLocalTransformationChange(TransformChangeType changetype);
		virtual void OnExternalTransformationChange(TransformChangeType changetype, Matrix4 newparentmatrix);
	public:
		Object();
		virtual ~Object();
		virtual Object* Copy();

		Transform& World();
		Transform& Local();
		Matrix4 GetWorldMatrix(bool include_local_scale = true);
		void SetLocalMatrix(Matrix4 mat);
		void SetWorldPosition(Vector3 vector);
		void TranslateWorld(Vector3 vector);

		virtual void OnEnterHierarchy(Object* newChild);
		virtual void OnExitHierarchy(Object* newChild);
		Object* GetParent();
		virtual void SetParent(Object* newParent);
		Object* GetChildAt(size_t i);
		size_t GetChildCount();

		void Destroy();
		bool get_isDestroyed();

		void CallRecursively(std::function<void(Object*)> action);
	};
}