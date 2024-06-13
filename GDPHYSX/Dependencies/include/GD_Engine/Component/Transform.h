#pragma once

#include "../Vector.h"

namespace gde {
	class Transform {
	public:
		Vector3 position;
		Vector3 scale;
		Vector3 rotation;
	};
}