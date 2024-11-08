#include "Matrix4.h"

gde::Matrix4::Matrix4() : glm::mat4(1.0f)
{
	
}

gde::Matrix4::Matrix4(const glm::mat4x4& from) : glm::mat4x4(from)
{
}
