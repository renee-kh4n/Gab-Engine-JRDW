#include "Matrix4.h"

gbe::Matrix4::Matrix4() : glm::mat4(1.0f)
{
	
}

gbe::Matrix4::Matrix4(const glm::mat4x4& from) : glm::mat4x4(from)
{
}
