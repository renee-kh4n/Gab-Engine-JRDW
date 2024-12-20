#pragma once

#include "Texture.h"
#include "Material.h"
#include "Mesh.h"


namespace gbe {
    namespace gfx {
        class DrawCall {
        public:
            std::unordered_map<void*, Matrix4> calls;

            Mesh* m_mesh;
            Material* m_material;

            DrawCall(Mesh* mesh, Material* material);
        };
    }
}