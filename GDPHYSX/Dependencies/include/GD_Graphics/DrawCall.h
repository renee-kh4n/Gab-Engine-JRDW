#pragma once

#include "Texture.h"
#include "Material.h"
#include "Mesh.h"


namespace gde {
    namespace rendering {
        class DrawCall {
        public:
            std::unordered_map<void*, glm::mat4> calls;

            Mesh* m_mesh;
            Material* m_material;

            DrawCall(Mesh* mesh, Material* material);
        };
    }
}