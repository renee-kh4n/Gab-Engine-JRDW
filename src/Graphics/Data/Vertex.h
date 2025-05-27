#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <array>
#include <vulkan/vulkan.h>

namespace gbe {
    namespace gfx {
        struct Vertex {
            glm::vec3 pos;
            glm::vec3 color;
            glm::vec2 texCoord;

            static VkVertexInputBindingDescription getBindingDescription();
            static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
        };
    }
}