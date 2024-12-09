#pragma once

#include "../Datatypes/Vector3.h"
#include "../Datatypes/Matrix4.h"

namespace gbe {
	namespace physics {
		class Rigidbody {
		public:
			void PassCurrentTransformMatrix(Matrix4);
		};
	}
}