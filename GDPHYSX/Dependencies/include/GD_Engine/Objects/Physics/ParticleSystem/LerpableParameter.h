#pragma once

namespace gde {
	namespace particlesystem {
		template <typename TValue>
		class LerpableParameter {
		public:
			bool random_between_two;

			TValue valueA;
			TValue valueB;

			TValue GetValue() {
				if (random_between_two == false)
					return valueA;

				return GetRandomValue();
			}
		protected:
			virtual TValue GetRandomValue() = 0;
		};
	}
}