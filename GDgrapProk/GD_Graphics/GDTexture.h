#pragma once

#include "GDutil.h"
#include "stb_image.h"

struct GDTexture {
    GLuint texID;
    stbi_uc* tex_bytes;
    int img_width;
    int img_height;
    int colorchannels;

    GDTexture(const char* path);
};

struct GDTextureCubeMap {
    GLuint texID;

    /// <summary>
    /// Helper function to automatically generate a cubemap with a specified partial path and filetype.
    /// </summary>
    /// <param name="semiPath"></param>
    /// <param name="fileExtension"></param>
    GDTextureCubeMap(std::string semiPath, std::string fileExtension);
};