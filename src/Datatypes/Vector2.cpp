
#include "Vector2.h"

gde::Vector2::Vector2(float x, float y) : glm::vec2(x, y)
{

}

gde::Vector2::Vector2() : glm::vec2(0)
{
}

gde::Vector2::Vector2(glm::vec2 glmvec) : glm::vec2(glmvec)
{

}

const gde::Vector2 gde::Vector2::zero = gde::Vector2(0, 0);