#pragma once

#include <iostream>
#include <string>

namespace gde {
	namespace internal {
		template<int L, typename T>
		struct Vector {
		protected:
			std::vector<T*> value_pointers;
		public:
			Vector(std::vector<T*> value_pointers) {
				this->value_pointers = value_pointers;
			}

			void operator=(const Vector<L, T>& right)
			{
				for (int i = 0; i < L; i++)
				{
					*this->value_pointers[i] = *right.value_pointers[i];
				}
			}
			void operator+=(const Vector<L, T>& right)
			{
				for (int i = 0; i < L; i++)
				{
					*this->value_pointers[i] += *right.value_pointers[i];
				}
			}
			void operator-=(const Vector<L, T>& right)
			{
				for (int i = 0; i < L; i++)
				{
					*this->value_pointers[i] -= *right.value_pointers[i];
				}
			}

			void operator*=(const T& right)
			{
				for (int i = 0; i < L; i++)
				{
					*this->value_pointers[i] *= right;
				}
			}

			T SqrMagnitude()
			{
				T result = 0.0f;

				for (auto component : value_pointers)
				{
					result += (*component) * (*component);
				}

				return result;
			}

			T Magnitude()
			{
				return sqrt(this->SqrMagnitude());
			}

			
			T Dot(const Vector<L, T>& right)
			{
				T result = 0;

				for (int i = 0; i < L; i++)
				{
					result += (*this->value_pointers[i]) * (*right.value_pointers[i]);
				}

				return result;
			}

			std::string ToString()
			{
				std::string tostring = "";

				for (int i = 0; i < L; i++)
				{
					tostring += std::to_string(*this->value_pointers[i]);

					if (i + 1 < L) {
						tostring += " , ";
					}
				}

				return tostring;
			}
		};
	}
}