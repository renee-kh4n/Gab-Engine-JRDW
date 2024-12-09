#pragma once

#include <btBulletDynamicsCommon.h>
#include "../Datatypes/Vector3.h"
#include "../Datatypes/Matrix4.h"

namespace gbe {
	namespace physics {
		class Rigidbody : btRigidBody {
		public:
			void PassCurrentTransformMatrix(Matrix4);
		};
	}
}