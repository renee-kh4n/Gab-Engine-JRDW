#pragma once

#include "../Object.h"
#include "RigidObject.h"

namespace gde {
	class ForceVolume : public Object {
	public:
		enum Shape {
			GLOBAL,
			BOX,
			SPHERE
		};
		enum Mode {
			DIRECTIONAL,
			RADIAL,
			ORBITAL
		};
		enum ForceMode {
			FORCE,
			VELOCITY
		};

		Shape shape;
		Vector3 half_bounds;
		float radius;
		
		Mode mode;
		//for directional
		Vector3 vector;
		//for radial and orbital
		float scalar;

		ForceMode forceMode;

		void TryApply(RigidObject* object);
	};
}