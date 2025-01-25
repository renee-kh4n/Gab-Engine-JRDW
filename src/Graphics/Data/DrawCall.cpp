#include "DrawCall.h"

namespace gbe {
    using namespace gfx;

    DrawCall::DrawCall(Mesh* mesh, Material* material, int order)
    {
        this->order = order;
        this->m_mesh = mesh;
        this->m_material = material;
    }
}