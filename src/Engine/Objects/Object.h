#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Component/Transform.h"
#include <list>
#include <functional>

#include "../Datatypes/Matrix4.h"

namespace gbe {

	class Object {
	private:
		bool isDestroyQueued = false;

		std::list<Object*> children;

		Transform world;
		Transform local;

		void UpdateTransform();

		void MatToTrans(Transform* target, Matrix4 mat);
	protected:
		Object* parent;
		Matrix4 parent_matrix;
		Matrix4 local_matrix;
		virtual Object* Copy_self();
	public:
		Object();
		virtual ~Object();
		virtual Object* Copy();

		Transform* World();
		Transform* Local();
		void SetPosition(Vector3 vector);
		void TranslateWorld(Vector3 vector);
		void TranslateLocal(Vector3 vector);
		void Rotate(Vector3 axis, float angle);
		void SetRotation(Vector3 euler);
		void Orient(Vector3 forward, Vector3 Up);
		void Scale(Vector3 vector);
		void SetScale(Vector3 vector);

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