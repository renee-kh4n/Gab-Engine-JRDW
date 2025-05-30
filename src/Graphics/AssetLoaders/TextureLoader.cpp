#include "TextureLoader.h"

#include "../RenderPipeline.h"

gbe::gfx::TextureData gbe::gfx::TextureLoader::LoadAsset_(gbe::asset::Texture* target, const asset::data::TextureImportData& importdata, asset::data::TextureLoadData* loaddata) {
	std::string pathstr = target->Get_asset_directory() + importdata.filename;

	stbi_uc* pixels;
	int tex_width;
	int tex_height;
	int colorchannels;

	stbi_set_flip_vertically_on_load(true);
	pixels = stbi_load(pathstr.c_str(), &tex_width, &tex_height, &colorchannels, 4);
	loaddata->colorchannels = colorchannels;
	loaddata->dimensions = Vector2Int(tex_width, tex_height);

	//VULKAN TEXTURE LOAD
	VkDeviceSize imageSizevk = tex_width * tex_height * 4;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	RenderPipeline::createBuffer(imageSizevk, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* vkdata;
	vkMapMemory(*this->vkdevice, stagingBufferMemory, 0, imageSizevk, 0, &vkdata);
	memcpy(vkdata, pixels, static_cast<size_t>(imageSizevk));
	vkUnmapMemory(*this->vkdevice, stagingBufferMemory);

	stbi_image_free(pixels);

	//IMAGE CREATION
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;

	RenderPipeline::createImage(tex_width, tex_height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

	RenderPipeline::transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	RenderPipeline::copyBufferToImage(stagingBuffer, textureImage, tex_width, tex_width);

	RenderPipeline::transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(*this->vkdevice, stagingBuffer, nullptr);
	vkFreeMemory(*this->vkdevice, stagingBufferMemory, nullptr);

	//CREATE IMAGE VIEW
	VkImageView textureImageView;
	RenderPipeline::createImageView(textureImageView, textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

	//SAMPLING
	VkSampler textureSampler;
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;

	//ANISOTROPY CALCULATION
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(*this->vkphysicaldevice, &properties);
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(*this->vkdevice, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}

	return TextureData{
		textureImage,
		textureImageMemory,
		textureImageView,
		textureSampler
	};
}
void gbe::gfx::TextureLoader::UnLoadAsset_(gbe::asset::Texture* target, const asset::data::TextureImportData& importdata, asset::data::TextureLoadData* data) {
	const auto& imagedata = this->GetAssetData(target);
	
	vkDestroySampler(*this->vkdevice, imagedata.textureSampler, nullptr);
	vkDestroyImageView(*this->vkdevice, imagedata.textureImageView, nullptr);

	vkDestroyImage(*this->vkdevice, imagedata.textureImage, nullptr);
	vkFreeMemory(*this->vkdevice, imagedata.textureImageMemory, nullptr);
}

gbe::gfx::TextureData& gbe::gfx::TextureLoader::GetDefaultImage()
{
	return static_cast<TextureLoader*>(active_instance)->defaultImage;
}

void gbe::gfx::TextureLoader::PassDependencies(VkDevice* vkdevice, VkPhysicalDevice* vkphysicaldevice)
{
	this->vkdevice = vkdevice;
	this->vkphysicaldevice = vkphysicaldevice;

	//CREATING DEFAULT IMAGE
	//IMAGE CREATION

	int tex_width = 1;
	int tex_height = 1;
	int colorchannels = 3;

	stbi_uc* pixels = (stbi_uc*)calloc(tex_width * tex_height * colorchannels, 1);

	//VULKAN TEXTURE LOAD
	VkDeviceSize imageSize = tex_width * tex_height * (colorchannels + 1);

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	RenderPipeline::createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* vkdata;
	vkMapMemory(*this->vkdevice, stagingBufferMemory, 0, imageSize, 0, &vkdata);
	memcpy(vkdata, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(*this->vkdevice, stagingBufferMemory);

	stbi_image_free(pixels);

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;

	RenderPipeline::createImage(tex_width, tex_height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

	RenderPipeline::transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	RenderPipeline::copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(tex_width), static_cast<uint32_t>(tex_height));

	RenderPipeline::transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(*this->vkdevice, stagingBuffer, nullptr);
	vkFreeMemory(*this->vkdevice, stagingBufferMemory, nullptr);

	//CREATE IMAGE VIEW
	VkImageView textureImageView;
	RenderPipeline::createImageView(textureImageView, textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

	//SAMPLING
	VkSampler textureSampler;
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 1.0f;

	if (vkCreateSampler(*this->vkdevice, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}

	this->defaultImage = TextureData{
		textureImage,
		textureImageMemory,
		textureImageView,
		textureSampler
	};
}
