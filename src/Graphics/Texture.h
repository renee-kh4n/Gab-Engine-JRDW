#pragma once

#include <glad/glad.h>
#include <stb_image.h>
#include <string>

namespace gbe {
    namespace gfx {

        struct TextureCubeMap {
            GLuint texID;

            /// <summary>
            /// Helper function to automatically generate a cubemap with a specified partial path and filetype.
            /// </summary>
            /// <param name="semiPath"></param>
            /// <param name="fileExtension"></param>
            TextureCubeMap(std::string semiPath, std::string fileExtension);
        };
    }
}