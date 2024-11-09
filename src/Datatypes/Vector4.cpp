
#include "Vector4.h"

gbe::Vector4::Vector4(float x, float y, float z, float w) : glm::vec4(x, y, z, w)
{

}

gbe::Vector4::Vector4() : glm::vec4(0)
{
}

gbe::Vector4::Vector4(glm::vec4 glmvec) : glm::vec4(glmvec)
{

}

const gbe::Vector4 gbe::Vector4::zero = gbe::Vector4(0, 0, 0, 0);