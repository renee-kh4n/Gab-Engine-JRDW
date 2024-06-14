#pragma once

#include "../ParticleSystem.h"
#include "LerpableParameter.h"

namespace gde {
	namespace particlesystem {
		class FloatParameter : public LerpableParameter<float> {
		public:
			bool random_between_two;

		protected:
			virtual float GetRandomValue() override;
		};
	}
}