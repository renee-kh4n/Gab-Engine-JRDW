#pragma once

#include "../Datatypes/Vectors.h"
#include "../Component/Transform.h"
#include <list>
#include <functional>
#include <glm/mat4x4.hpp>

namespace gde {

	class Object {
	private:
		bool isDestroyQueued = false;

		std::list<Object*> children;

		Transform world;
		Transform local;

		void MatToTrans(Transform* target, glm::mat4 mat);
	protected:
		Object* parent;
		glm::mat4 transform;
		virtual Object* Copy_self();
	public:
		Object();
		virtual ~Object();
		virtual Object* Copy();

		glm::mat4 GetWorldSpaceMatrix();
		Transform* World();
		Transform* Local();
		void SetPosition(Vector3 vector);
		void TranslateWorld(Vector3 vector);
		void TranslateLocal(Vector3 vector);
		void Rotate(Vector3 axis, float angle);
		void Orient(Vector3 forward, Vector3 Up);
		void Scale(Vector3 vector);

		virtual void OnEnterHierarchy(Object* newChild);
		virtual void OnExitHierarchy(Object* newChild);
		void SetParent(Object* newParent);
		Object* GetChildAt(size_t i);
		size_t GetChildCount();

		void Destroy();
		bool get_isDestroyed();

		void CallRecursively(std::function<void(Object*)> action);
	};
}