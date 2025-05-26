#pragma once

#include <iostream>
#include <glad/glad.h>
#include "Math/gbe_math.h"

namespace gbe {
    namespace gfx {
        class Framebuffer {
        public:
            Vector2Int dimensions;
            unsigned int id = 0;
            unsigned int outputId = 0;
            unsigned int rbo = 0;

            unsigned int quadVAO = 0, quadVBO = 0;

            Framebuffer(Vector2Int dimensions) {
                this->dimensions = dimensions;
            }
        };
    }
}