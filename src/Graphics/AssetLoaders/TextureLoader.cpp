#include "TextureLoader.h"

bool gbe::gfx::TextureLoader::LoadAsset_(gbe::asset::Texture* target, const asset::data::TextureImportData& importdata, asset::data::TextureLoadData* data) {
	std::string pathstr = target->Get_asset_directory() + importdata.filename;

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

	data->colorchannels = colorchannels;
	data->dimensions = Vector2Int(img_width, img_height);

	const auto active_instance_cast = static_cast<TextureLoader*>(TextureLoader::active_instance);
	active_instance_cast->textures_in_memory.insert({ target , texID });

	return true;
}
void gbe::gfx::TextureLoader::UnLoadAsset_(gbe::asset::Texture* target, const asset::data::TextureImportData& importdata, asset::data::TextureLoadData* data) {

}

unsigned int gbe::gfx::TextureLoader::Get_tex_glId(asset::Texture* tex) {
	const auto active_instance_cast = static_cast<TextureLoader*>(TextureLoader::active_instance);

	auto mem_ite = active_instance_cast->textures_in_memory.find(tex);
	if (mem_ite == active_instance_cast->textures_in_memory.end())
		return -1;

	return mem_ite->second;
}

unsigned int gbe::gfx::TextureLoader::PushGet_texture_stackId(asset::Texture* tex) {
	const auto active_instance_cast = static_cast<TextureLoader*>(TextureLoader::active_instance);

	unsigned int mem_id = TextureLoader::Get_tex_glId(tex);
	if (mem_id == -1)
		throw "Texture is not even loaded in gpu yet!";

	return active_instance_cast->PushGet_bufferId_stackId(mem_id);
}
unsigned int gbe::gfx::TextureLoader::PushGet_bufferId_stackId(unsigned int bufferid) {
	const auto active_instance_cast = static_cast<TextureLoader*>(TextureLoader::active_instance);
	
	auto stack_ite = active_instance_cast->buffers_in_renderstack.find(bufferid);

	if (stack_ite != active_instance_cast->buffers_in_renderstack.end())
		return stack_ite->second;

	//CREATE NEW AND ASSIGN
	unsigned int stack_id = active_instance_cast->texture_renderstack_index;
	if (stack_id >= 32)
		throw "Texture Render stack is full!";

	active_instance_cast->buffers_in_renderstack.insert({ bufferid, stack_id });
	glActiveTexture(GL_TEXTURE0 + stack_id);
	glBindTexture(GL_TEXTURE_2D, bufferid);

	//INCREMENT
	active_instance_cast->texture_renderstack_index++;

	return stack_id;
}
void gbe::gfx::TextureLoader::Reset_texture_stack() {
	const auto active_instance_cast = static_cast<TextureLoader*>(TextureLoader::active_instance);
	active_instance_cast->texture_renderstack_index = active_instance_cast->texture_renderstack_index_locked;
	
	std::stack<unsigned int> keys_to_remove;

	for (auto bufferId_stackId : active_instance_cast->buffers_in_renderstack)
		if (bufferId_stackId.second >= active_instance_cast->texture_renderstack_index)
			keys_to_remove.push(bufferId_stackId.first);

	while (!keys_to_remove.empty())
	{
		active_instance_cast->buffers_in_renderstack.erase(keys_to_remove.top());
		keys_to_remove.pop();
	}
}
void gbe::gfx::TextureLoader::Lock_stack() {
	const auto active_instance_cast = static_cast<TextureLoader*>(TextureLoader::active_instance);
	active_instance_cast->texture_renderstack_index_locked = active_instance_cast->texture_renderstack_index;
}
void gbe::gfx::TextureLoader::UnLock_stack() {
	const auto active_instance_cast = static_cast<TextureLoader*>(TextureLoader::active_instance);
	active_instance_cast->texture_renderstack_index_locked = 0;
}
