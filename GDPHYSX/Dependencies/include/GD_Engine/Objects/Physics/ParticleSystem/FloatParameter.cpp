#include "FloatParameter.h"

float gde::particlesystem::FloatParameter::GetValue()
{
    return 0.0f;
}

float gde::particlesystem::FloatParameter::GetRandomValue()
{
    auto range = this->valueB - this->valueA;
    return ((float)rand() / RAND_MAX) * range;
}
