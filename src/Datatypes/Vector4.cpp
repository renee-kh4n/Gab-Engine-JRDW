
#include "Vector4.h"

gde::Vector4::Vector4(float x, float y, float z, float w) : glm::vec4(x, y, z, w)
{

}

gde::Vector4::Vector4() : glm::vec4(0)
{
}

gde::Vector4::Vector4(glm::vec4 glmvec) : glm::vec4(glmvec)
{

}

const gde::Vector4 gde::Vector4::zero = gde::Vector4(0, 0, 0, 0);