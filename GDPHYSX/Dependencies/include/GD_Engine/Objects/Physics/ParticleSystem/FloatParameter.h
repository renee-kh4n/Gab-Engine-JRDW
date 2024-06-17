#pragma once

#include <cstdlib>
#include "LerpableParameter.h"

namespace gde {
	namespace particlesystem {
		class FloatParameter : public LerpableParameter<float> {
		protected:
			float GetRandomValue() override {
				auto range = this->valueB - this->valueA;
				return this->valueA + (((float)rand() / RAND_MAX) * range);
			}
		};
	}
}