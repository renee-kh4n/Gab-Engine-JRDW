#pragma once

#include "Vector.h"
#include <GD_Graphics/DrawCall.h>

namespace gde {
	class Object {
	private:
		bool isDestroyed = false;
	public:
		Object();

		Vector3 position;
		Vector3 scale;
		Vector3 rotation;

		//Rendering stuff
		DrawCall* mDrawCall;
		void UpdateDrawCall();

		//Rigidbody stuff
		float mass;
		Vector3 velocity;
		Vector3 frame_impule_force;
		Vector3 frame_continuous_force;
		Vector3 acceleration;
		float damping;

		void AddForce(Vector3 force, bool continuous = false);

		void Destroy();
		bool get_isDestroyed();
	};
}