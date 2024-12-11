#pragma once

#include "../Datatypes/Vector3.h"
#include "../Datatypes/Quaternion.h"
#include "../Datatypes/Matrix4.h"
#include "../Datatypes/TrackedVariable.h"
#include <functional>

namespace gbe {
	class Transform {
	private:
		Matrix4 updated_matrix_without_scale;
		Matrix4 updated_matrix_with_scale;

		std::function<void()> onChange;

		void UpdateAxisVectors();
		void OnComponentChange();
	public:

		//AXIS VECTORS
		TrackedVariable<Vector3> Right = TrackedVariable<Vector3>([this](auto var) {this->OnComponentChange(); });
		TrackedVariable<Vector3> Up = TrackedVariable<Vector3>([this](auto var) {this->OnComponentChange(); });
		TrackedVariable<Vector3> Forward = TrackedVariable<Vector3>([this](auto var) {this->OnComponentChange(); });

		TrackedVariable<Vector3> position = TrackedVariable<Vector3>([this](auto var) {this->OnComponentChange(); });
		TrackedVariable<Vector3> scale = TrackedVariable<Vector3>([this](auto var) {this->OnComponentChange(); });
		TrackedVariable<Quaternion> rotation = TrackedVariable<Quaternion>([this](auto var) {
			this->OnComponentChange();
			this->UpdateAxisVectors();
			});

		Transform();
		Transform(std::function<void()> onChange);
		Matrix4 GetMatrix(bool include_scale = true);
	};
}