#pragma once

#include "../ParticleSystem.h"
#include "LerpableParameter.h"

namespace gde {
	namespace particlesystem {
		class VectorParameter : public LerpableParameter<Vector3>{
		public:
			bool random_between_two;

		protected:
			virtual Vector3 GetRandomValue() override;
		};
	}
}