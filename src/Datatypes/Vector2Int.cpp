#include "Vector2Int.h"

gde::Vector2Int::Vector2Int(int x, int y) : glm::ivec2(x, y)
{

}

gde::Vector2Int::Vector2Int() : glm::ivec2(0)
{
}

gde::Vector2Int::Vector2Int(glm::ivec2 glmvec) : glm::ivec2(glmvec)
{

}

const gde::Vector2Int gde::Vector2Int::zero = gde::Vector2Int(0, 0);