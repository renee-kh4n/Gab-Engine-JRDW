
#include "Vector2.h"

gde::Vector2::Vector2(float x, float y) : glm::vec2(x, y)
{

}

gde::Vector2::Vector2() : glm::vec2(0)
{
}

gde::Vector2::Vector2(const glm::vec2& glmvec) : glm::vec2(glmvec)
{

}

gde::Vector2& gde::Vector2::operator+=(const gde::Vector2& b) {
	(glm::vec2)*this += (glm::vec2)b;

	return *this;
}

gde::Vector2& gde::Vector2::operator-=(const gde::Vector2& b) {
	(glm::vec2)*this -= (glm::vec2)b;

	return *this;
}

const gde::Vector2 gde::Vector2::zero = gde::Vector2(0, 0);