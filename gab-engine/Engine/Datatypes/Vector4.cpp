#include "Vector4.h"

namespace gde {
	Vector4::Vector4() : Vector<4, float>({ &x, &y, &z, &w })
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	Vector4::Vector4(const Vector4& from) : Vector<4, float>({ &x, &y, &z, &w })
	{
		this->x = from.x;
		this->y = from.y;
		this->z = from.z;
	}

	gde::Vector4::Vector4(float _x, float _y, float _z, float _w) : Vector<4, float>({ &x, &y, &z, &w })
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = _w;
	}
	Vector4::Vector4(glm::vec4 glmvec) : Vector<4, float>({ &x, &y, &z, &w })
	{
		this->x = glmvec.x;
		this->y = glmvec.y;
		this->z = glmvec.z;
	}
	Vector4::operator glm::vec4() const {
		return glm::vec4(this->x, this->y, this->z, this->w);
	}
}
