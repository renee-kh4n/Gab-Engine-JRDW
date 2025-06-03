
#include "Vector4.h"

gbe::Vector4::Vector4(float x, float y, float z, float w) : glm::vec4(x, y, z, w)
{

}

gbe::Vector4::Vector4(float x) : glm::vec4(x) {

}
gbe::Vector4::Vector4() : glm::vec4(0)
{
}

gbe::Vector4::Vector4(Vector3 vec3, float w) : glm::vec4(vec3.x, vec3.y, vec3.z, w) {
	 
}

gbe::Vector4::Vector4(glm::vec4 glmvec) : glm::vec4(glmvec)
{

}

const float* gbe::Vector4::Get_Ptr() {
	return &((*this).x);
}

const gbe::Vector4 gbe::Vector4::zero = gbe::Vector4(0, 0, 0, 0);