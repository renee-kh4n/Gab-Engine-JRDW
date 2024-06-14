#pragma once

#include "../Vector.h"
#include "../Component/Transform.h"
#include <list>
#include <functional>
#include <glm/mat4x4.hpp>

namespace gde {
	class Object {
	private:
		bool isDestroyed = false;

		Object* parent;
		std::list<Object*> children;

		Transform world;
		Transform local;
		glm::mat4 transform;
	public:
		Object();
		virtual ~Object();

		glm::mat4 GetWorldSpaceMatrix();

		Transform* World();
		Transform* Local();

		void Translate(Vector3 vector);
		void Rotate(Vector3 vector);
		void Scale(Vector3 vector);

		void SetParent(Object* newParent);
		Object* GetChildAt(int i);
		int GetChildCount();

		void Destroy();
		bool get_isDestroyed();

		void CallRecursively(std::function<void(Object*)> action) {
			action(this);

			for (auto child : children)
			{
				child->CallRecursively(action);
			}
		}
	};
}