#include "DrawCall.h"

namespace gde {
    DrawCall::DrawCall(Mesh* mesh, Material* material, glm::vec3 pos, glm::vec3 scale, glm::vec3 rot)
    {
        this->pos = pos;
        this->scale = scale;
        this->rot = rot;

        this->m_mesh = mesh;
        this->m_material = material;
    }
}