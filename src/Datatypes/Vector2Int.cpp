#include "Vector2Int.h"

gbe::Vector2Int::Vector2Int(int x, int y) : glm::ivec2(x, y)
{

}

gbe::Vector2Int::Vector2Int() : glm::ivec2(0)
{
}

gbe::Vector2Int::Vector2Int(glm::ivec2 glmvec) : glm::ivec2(glmvec)
{

}

const gbe::Vector2Int gbe::Vector2Int::zero = gbe::Vector2Int(0, 0);