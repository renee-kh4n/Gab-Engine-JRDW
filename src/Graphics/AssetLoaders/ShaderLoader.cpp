#include "ShaderLoader.h"

gbe::gfx::ShaderData gbe::gfx::ShaderLoader::LoadAsset_(asset::Shader* asset, const asset::data::ShaderImportData& importdata, asset::data::ShaderLoadData* data) {
	//============READING============//
	auto vertpath = asset->Get_asset_directory() + importdata.vert;
	auto fragpath = asset->Get_asset_directory() + importdata.frag;
	auto vertmetapath = asset->Get_asset_directory() + importdata.vert_meta;
	auto fragmetapath = asset->Get_asset_directory() + importdata.frag_meta;

	auto readfile = [](std::string path) {
		std::ifstream file(path, std::ios::ate | std::ios::binary);
		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
		};

	auto vertShaderCode = readfile(vertpath);
	auto fragShaderCode = readfile(fragpath);
	
	ShaderStageMeta vertMeta;
	ShaderStageMeta fragMeta;
	
	gbe::asset::serialization::gbeParser::PopulateClass(vertMeta, vertmetapath);
	gbe::asset::serialization::gbeParser::PopulateClass(fragMeta, fragmetapath);

	//============DESCRIPTOR LAYOUT SETUP============//
	std::vector<VkDescriptorSetLayout> descriptorSetLayouts;

	//BINDINGS
	std::unordered_map<unsigned int, std::vector<VkDescriptorSetLayoutBinding>> binding_sets;
	std::vector<ShaderData::ShaderBlock> uniformblocks;
	std::vector<ShaderData::ShaderField> uniformfields;

	const auto AddUboBinding = [&](const ShaderStageMeta& stagemeta, const ShaderStageMeta::UboMeta& ubo, VkShaderStageFlags flags) {
		VkDescriptorSetLayoutBinding ubo_Binding{};
		ubo_Binding.binding = ubo.binding;
		ubo_Binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		ubo_Binding.descriptorCount = 1;
		ubo_Binding.stageFlags = flags;
		ubo_Binding.pImmutableSamplers = nullptr; // Optional

		if (binding_sets.find(ubo.set) == binding_sets.end())
			binding_sets.insert_or_assign(ubo.set, std::vector<VkDescriptorSetLayoutBinding>());

		auto& bset = binding_sets[ubo.set];
		bset.push_back(ubo_Binding);

		ShaderData::ShaderBlock block{};
		block.binding = ubo.binding;
		block.set = ubo.set;
		block.name = ubo.name;
		block.block_size = ubo.block_size;

		ShaderData::ShaderField* prevfield = nullptr; //To set the size correctly
		for (const auto& member : stagemeta.types.at(ubo.type).members)
		{
			ShaderData::ShaderField field{};
			field.name = member.name;
			field.block = ubo.name;
			field.type = gbe::asset::Shader::UniformFieldType::MAT4; // Default to MAT4, can be changed later
			field.offset = member.offset;

			//Calculate previous field size
			if (prevfield != nullptr)
				prevfield->size = member.offset - prevfield->offset;

			if (member.type == "bool") {
				field.type = gbe::asset::Shader::UniformFieldType::BOOL;
			}
			else if (member.type == "int") {
				field.type = gbe::asset::Shader::UniformFieldType::INT;
			}
			else if (member.type == "float") {
				field.type = gbe::asset::Shader::UniformFieldType::FLOAT;
			}
			else if (member.type == "vec2") {
				field.type = gbe::asset::Shader::UniformFieldType::VEC2;
			}
			else if (member.type == "vec3") {
				field.type = gbe::asset::Shader::UniformFieldType::VEC3;
			}
			else if (member.type == "vec4") {
				field.type = gbe::asset::Shader::UniformFieldType::VEC4;
			}
			else if (member.type == "mat4") {
				field.type = gbe::asset::Shader::UniformFieldType::MAT4;
			}
			
			//test if there are other fields with the same name
			for (const auto& existing_field : uniformfields)
			{
				if (existing_field.name == field.name && existing_field.block == field.block) {
					throw std::runtime_error("Duplicate uniform field found: " + field.name + " in block " + field.block);
				}
			}
			
			uniformfields.push_back(field);

			prevfield = &uniformfields.back();
		}

		//for the last element with no next element, set the size to the block size
		if (prevfield != nullptr) {
			prevfield->size = ubo.block_size - prevfield->offset;
		}

		uniformblocks.push_back(block);
	};

	const auto AddTextureBinding = [&](const ShaderStageMeta::TextureMeta& meta, VkShaderStageFlags flags) {
		VkDescriptorSetLayoutBinding color_sampler_Binding{};
		color_sampler_Binding.binding = meta.binding;
		color_sampler_Binding.descriptorCount = 1;
		color_sampler_Binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		color_sampler_Binding.pImmutableSamplers = nullptr;
		color_sampler_Binding.stageFlags = flags;

		if (binding_sets.find(meta.set) == binding_sets.end())
			binding_sets.insert_or_assign(meta.set, std::vector<VkDescriptorSetLayoutBinding>());

		auto& bset = binding_sets[meta.set];
		bset.push_back(color_sampler_Binding);

		ShaderData::ShaderBlock block{};
		block.binding = meta.binding;
		block.set = meta.set;
		block.name = meta.name;

		ShaderData::ShaderField field{};
		field.name = meta.name;
		field.block = block.name;
		field.type = gbe::asset::Shader::UniformFieldType::TEXTURE; // Default to TEXTURE, can be changed later
		field.offset = 0; // Offset is not applicable for textures, set to 0
		uniformfields.push_back(field);
	};

	//LOOP THROUGH UBO BINDINGS
	for (const auto& ubo : vertMeta.ubos)
		AddUboBinding(vertMeta, ubo, VK_SHADER_STAGE_VERTEX_BIT);
	for (const auto& ubo : fragMeta.ubos)
		AddUboBinding(fragMeta, ubo, VK_SHADER_STAGE_FRAGMENT_BIT);

	//LOOP THROUGH TEXTURE BINDINGS
	for (const auto& meta : vertMeta.textures)
		AddTextureBinding(meta, VK_SHADER_STAGE_VERTEX_BIT);
	for (const auto& meta : fragMeta.textures)
		AddTextureBinding(meta, VK_SHADER_STAGE_FRAGMENT_BIT);

	//COMPILE BINDINGS
	for (auto& pair : binding_sets)
	{
		auto& setindex = pair.first;
		auto& setlist = pair.second;

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(setlist.size());
		layoutInfo.pBindings = setlist.data();

		VkDescriptorSetLayout newsetlayout;

		if (vkCreateDescriptorSetLayout(*this->vkdevice, &layoutInfo, nullptr, &newsetlayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}

		for (size_t i = descriptorSetLayouts.size(); i <= setindex; i++)
		{
			descriptorSetLayouts.push_back({});
		}

		descriptorSetLayouts[setindex] = newsetlayout;
	}

	

	//============SHADER COMPILING============//
	auto vertShader = TryCompileShader(vertShaderCode);
	auto fragShader = TryCompileShader(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;

	vertShaderStageInfo.module = vertShader;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShader;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	//Dynamic state
	std::vector<VkDynamicState> dynamicStates = {
	VK_DYNAMIC_STATE_VIEWPORT,
	VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	//Vertex input
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	
	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	//Input assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	//Viewports and scissors
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)(*this->vkextent).width;
	viewport.height = (float)(*this->vkextent).height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = (*this->vkextent);

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	//Rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	//Multisampling
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	//Color blending
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	//Pipeline layout
	VkPipelineLayout newpipelineLayout;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout((*this->vkdevice), &pipelineLayoutInfo, nullptr, &newpipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	//Pipeline creation
	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;

	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;

	pipelineInfo.layout = newpipelineLayout;

	pipelineInfo.renderPass = (*this->vkrenderpass);
	pipelineInfo.subpass = 0;

	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	VkPipeline newgraphicsPipeline;
	VkResult newgraphicsPipeline_result = vkCreateGraphicsPipelines((*this->vkdevice), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &newgraphicsPipeline);
	if (newgraphicsPipeline_result != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	//OVERRIDE FUNCTIONS
	data->overridefunctions.SetOverride_bool = [=](const char* id, bool value) {

		};
	data->overridefunctions.SetOverride_int = [=](const char* id, int value) {

		};
	data->overridefunctions.SetOverride_float = [=](const char* id, float value) {

		};
	data->overridefunctions.SetOverride_Vector2 = [=](const char* id, Vector2 value) {

		};
	data->overridefunctions.SetOverride_Vector3 = [=](const char* id, Vector3 value) {

		};
	data->overridefunctions.SetOverride_Vector4 = [=](const char* id, Vector4 value) {

		};
	data->overridefunctions.SetOverride_Matrix4 = [=](const char* id, Matrix4 value) {

		};
	data->overridefunctions.SetOverride_Texture = [=](const char* id, asset::Texture* value) {

		};
	data->overridefunctions.SetOverride_TextureId = [=](const char* id, int texid) {
		
		};


	//Cleanup
	vkDestroyShaderModule((*this->vkdevice), vertShader, nullptr);
	vkDestroyShaderModule((*this->vkdevice), fragShader, nullptr);

	return ShaderData{
		binding_sets,
		descriptorSetLayouts,
		uniformblocks,
		uniformfields,
		newpipelineLayout,
		newgraphicsPipeline,
		asset
	};
}

void gbe::gfx::ShaderLoader::UnLoadAsset_(asset::Shader* asset, const asset::data::ShaderImportData& importdata, asset::data::ShaderLoadData* data) {
	auto shaderdata = this->GetAssetData(asset);
	for (const auto& setlayout : shaderdata.descriptorSetLayouts)
	{
		vkDestroyDescriptorSetLayout(*this->vkdevice, setlayout, nullptr);
	}
	vkDestroyPipelineLayout((*this->vkdevice), shaderdata.pipelineLayout, nullptr);
	vkDestroyPipeline((*this->vkdevice), shaderdata.pipeline, nullptr);
}

void gbe::gfx::ShaderLoader::PassDependencies(VkDevice* vkdevice, VkExtent2D* vkextent, VkRenderPass* vkrenderpass) {
	this->vkdevice = vkdevice;
	this->vkextent = vkextent;
	this->vkrenderpass = vkrenderpass;
}

VkShaderModule gbe::gfx::ShaderLoader::TryCompileShader(const std::vector<char>& code) {
	VkShaderModuleCreateInfo shaderModuleInfo{};
	shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleInfo.codeSize = code.size();
	shaderModuleInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule((*this->vkdevice), &shaderModuleInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}

bool gbe::gfx::ShaderData::FindUniformField(std::string id, ShaderField& out_field, ShaderBlock& out_block)
{
	for (const auto& field : this->uniformfields)
	{
		if (field.name == id) {
			for (const auto& block : this->uniformblocks)
			{
				if(block.name == field.block)
					out_block = block;
			}
			out_field = field;
			return true;
		}
	}


	return false;
}
