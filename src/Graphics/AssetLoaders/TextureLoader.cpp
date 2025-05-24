#include "TextureLoader.h"

#include "../RenderPipeline.h"

gbe::gfx::TextureData gbe::gfx::TextureLoader::LoadAsset_(gbe::asset::Texture* target, const asset::data::TextureImportData& importdata, asset::data::TextureLoadData* loaddata) {
	std::string pathstr = target->Get_asset_directory() + importdata.filename;

	stbi_uc* pixels;
	int img_width;
	int img_height;
	int colorchannels;

	stbi_set_flip_vertically_on_load(true);
	pixels = stbi_load(pathstr.c_str(), &img_width, &img_height, &colorchannels, 0);
	loaddata->colorchannels = colorchannels;
	loaddata->dimensions = Vector2Int(img_width, img_height);

	//VULKAN TEXTURE LOAD
	VkDeviceSize imageSize = img_width * img_height * 4;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	RenderPipeline::createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* vkdata;
	vkMapMemory(*this->vkdevice, stagingBufferMemory, 0, imageSize, 0, &vkdata);
	memcpy(loaddata, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(*this->vkdevice, stagingBufferMemory);

	//END
	stbi_image_free(pixels);

	return TextureData{
		
	};
}
void gbe::gfx::TextureLoader::UnLoadAsset_(gbe::asset::Texture* target, const asset::data::TextureImportData& importdata, asset::data::TextureLoadData* data) {

}

void gbe::gfx::TextureLoader::PassDependencies(VkDevice* vkdevice)
{
	this->vkdevice = vkdevice;
}
