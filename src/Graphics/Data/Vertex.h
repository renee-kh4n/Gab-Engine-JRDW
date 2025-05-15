#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <array>
#include <vulkan/vulkan.h>

namespace gbe {
    namespace gfx {
        struct Vertex {
            glm::vec2 pos;
            glm::vec3 color;

            static VkVertexInputBindingDescription getBindingDescription();
            static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
        };
    }
}