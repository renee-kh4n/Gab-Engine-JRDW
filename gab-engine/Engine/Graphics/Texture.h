#pragma once

#include "stb_image.h"

namespace gde {
    namespace rendering {
        struct Texture {
            GLuint texID;
            stbi_uc* tex_bytes;
            int img_width;
            int img_height;
            int colorchannels;

            Texture(const char* path);
        };

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