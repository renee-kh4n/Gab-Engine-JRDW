#include "DrawCall.h"

namespace gde {
    namespace rendering {
        DrawCall::DrawCall(Mesh* mesh, Material* material)
        {
            this->m_mesh = mesh;
            this->m_material = material;
        }
    }
}