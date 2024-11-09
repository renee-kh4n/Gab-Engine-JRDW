#include "DrawCall.h"

namespace gbe {
    using namespace rendering;

    DrawCall::DrawCall(Mesh* mesh, Material* material)
    {
        this->m_mesh = mesh;
        this->m_material = material;
    }
}