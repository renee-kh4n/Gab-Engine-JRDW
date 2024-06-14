#include "VectorParameter.h"


gde::Vector3 gde::particlesystem::VectorParameter::GetRandomValue()
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
