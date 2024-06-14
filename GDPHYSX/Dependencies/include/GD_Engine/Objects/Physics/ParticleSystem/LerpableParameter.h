#pragma once

namespace gde {
	namespace particlesystem {
		template<typename T>
		class LerpableParameter {
		public:
			bool random_between_two;
			T valueA;
			T valueB;

			T GetValue() {
				if (random_between_two == false)
					return valueA;

				return this->GetRandomValue();
			}
		protected:
			virtual T GetRandomValue() = 0;
		};
	}
}