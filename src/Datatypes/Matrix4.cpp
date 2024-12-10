#include "Matrix4.h"

#include <glm/gtc/type_ptr.inl>

gbe::Matrix4::Matrix4() : glm::mat4(1.0f)
{
	
}

gbe::Matrix4::Matrix4(const glm::mat4x4& from) : glm::mat4x4(from)
{
}

const float* gbe::Matrix4::Get_Ptr() {
	return glm::value_ptr(*this);
}
