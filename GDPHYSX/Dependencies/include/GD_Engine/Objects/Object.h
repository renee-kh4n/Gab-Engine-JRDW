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

		Object* parent;
		std::list<Object*> children;

		Transform world;
		Transform local;
		glm::mat4 transform;
	protected:
		virtual Object* Copy_self();
	public:
		Object();
		virtual ~Object();
		virtual Object* Copy();

		glm::mat4 GetWorldSpaceMatrix();
		Transform* World();
		Transform* Local();
		void Translate(Vector3 vector);
		void Rotate(Vector3 vector);
		void Scale(Vector3 vector);

		virtual void OnEnterHierarchy(Object* newChild);
		virtual void OnExitHierarchy(Object* newChild);
		void SetParent(Object* newParent);
		Object* GetChildAt(int i);
		int GetChildCount();

		void Destroy();
		bool get_isDestroyed();

		void CallRecursively(std::function<void(Object*)> action) {
			int childcount = GetChildCount();
			for (int i = 0; i < childcount; i++)
			{
				this->GetChildAt(i)->CallRecursively(action);
			}

			action(this);
		}
	};
}