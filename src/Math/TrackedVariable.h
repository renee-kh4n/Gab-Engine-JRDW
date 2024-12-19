#pragma once

#include "Vectors.h"
#include <functional>

namespace gbe {
	template<typename T>
	class TrackedVariable {
	private:
		T variable;
		std::function<void(T)> doOnChange;
	public:
		TrackedVariable(std::function<void(T)> func) {
			doOnChange = func;
		}

		T& Get() {
			return variable;
		}
		void Set(T value) {
			this->variable = value;

			if(this->doOnChange)
				doOnChange(value);
		}
	};
}