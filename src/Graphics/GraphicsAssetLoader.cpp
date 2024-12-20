#include "GraphicsAssetLoader.h"

bool gbe::gfx::GraphicsAssetLoader::Load_Texture(asset::Texture* target, asset::Texture::TextureData& data)
{
	std::string pathstr = target->GetPath();

	GLuint texID;
	stbi_uc* tex_bytes;
	int img_width;
	int img_height;
	int colorchannels;

	stbi_set_flip_vertically_on_load(true);
	tex_bytes = stbi_load(pathstr.c_str(), &img_width, &img_height, &colorchannels, 0);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);

	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(tex_bytes);

	data.gl_id = texID;
	data.colorchannels = colorchannels;
	data.dimensions = Vector2Int(img_width, img_height);

	return true;
}
