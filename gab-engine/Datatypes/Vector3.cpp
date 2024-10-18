#include "Vector3.h"

gde::Vector3::Vector3() : glm::vec3(0)
{
}

gde::Vector3::Vector3(float x, float y, float z) : glm::vec3(x, y, z)
{

}

gde::Vector3::Vector3(glm::vec3 glmvec) : glm::vec3(glmvec)
{
	
}

const gde::Vector3 gde::Vector3::zero = gde::Vector3(0, 0, 0);
