#pragma once
#include "Asset/gbe_asset.h"

namespace gbe {
    namespace gfx {
        class DrawCall {
        public:
            std::unordered_map<void*, Matrix4> calls;

            int order;
            asset::Mesh* m_mesh;
            asset::Material* m_material;

            DrawCall(asset::Mesh* mesh, asset::Material* material, int order = 0);
        };
    }
}