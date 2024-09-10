#pragma once

#include "Datatypes/Vectors.h"

namespace gde {
	class Transform {
	public:
		Vector3 Right;
		Vector3 Up;
		Vector3 Forward;
		Vector3 position;
		Vector3 scale;
		Vector3 rotation;
	};
}