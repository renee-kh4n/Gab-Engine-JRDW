#include "DrawCall.h"

namespace gde {
    DrawCall::DrawCall(Mesh* mesh, Material* material)
    {
        this->m_mesh = mesh;
        this->m_material = material;
    }
}