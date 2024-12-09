#pragma once

#include "../Datatypes/Vector3.h"
#include "../Datatypes/Matrix4.h"

#include <bullet/btBulletDynamicsCommon.h>

namespace gbe {
	namespace physics {
		class Rigidbody : btRigidBody {
		public:
			void PassCurrentTransformMatrix(Matrix4);

			Rigidbody();
		};
	}
}