#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.h>

namespace gbe {
    namespace gfx {
        struct Vertex {
            glm::vec3 pos;
            glm::vec3 normal;
            glm::vec3 color;
            glm::vec2 texCoord;
            unsigned int obj_id;

            static VkVertexInputBindingDescription getBindingDescription();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };
    }
}