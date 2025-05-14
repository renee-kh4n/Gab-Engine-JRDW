#pragma once

#include <iostream>
#include <glad/glad.h>
#include "Math/gbe_math.h"

namespace gbe {
    namespace gfx {
        class Framebuffer {
        public:
            Vector2Int dimensions;
            unsigned int id;
            unsigned int outputId;
            unsigned int rbo;

            unsigned int quadVAO, quadVBO;

            Framebuffer(Vector2Int dimensions) {
                this->dimensions = dimensions;

                
            }
        };
    }
}