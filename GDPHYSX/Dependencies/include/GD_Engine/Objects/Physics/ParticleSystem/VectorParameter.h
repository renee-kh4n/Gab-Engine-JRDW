#pragma once

#include "../../../Vector.h"
#include "LerpableParameter.h"

namespace gde {
	namespace particlesystem {
		class VectorParameter : public LerpableParameter<Vector3> {
		protected:
			virtual Vector3 GetRandomValue() override
			{
				auto deltavec = this->valueB - this->valueA;
				auto randomvec = Vector3::zero;

				auto getrandf = []() {
					return (float)rand() / RAND_MAX;
				};

				randomvec.x = getrandf() * deltavec.x;
				randomvec.y = getrandf() * deltavec.y;
				randomvec.z = getrandf() * deltavec.z;

				return randomvec;
			}
		};
	}
}