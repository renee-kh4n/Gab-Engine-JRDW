#include "ShaderLoader.h"

struct ShaderMeta {
	struct TextureMeta {
		std::string name;
		std::string type;
		unsigned int set;
		unsigned int binding;
	};
	struct UboType {
		struct UboTypeMember {
			std::string name;
			std::string type;
		};

		std::string name;
		std::vector<UboTypeMember> members;
	};
	struct UboMeta {
		std::string name;
		std::string type;
		unsigned int block_size;
		unsigned int set;
		unsigned int binding;
	};
	std::vector<UboType> types;
	std::vector<TextureMeta> textures;
	std::vector<UboMeta> ubos;
};

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
	
	ShaderMeta vertMeta;
	ShaderMeta fragMeta;
	
	gbe::asset::serialization::gbeParser::PopulateClass(vertMeta, vertmetapath);
	gbe::asset::serialization::gbeParser::PopulateClass(fragMeta, fragmetapath);

	//============DESCRIPTOR LAYOUT SETUP============//
	VkDescriptorSetLayout descriptorSetLayout;

	//BINDINGS

	//Transform ubo binding
	VkDescriptorSetLayoutBinding transform_ubo_Binding{};
	transform_ubo_Binding.binding = 0;
	transform_ubo_Binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	transform_ubo_Binding.descriptorCount = 1;
	transform_ubo_Binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	transform_ubo_Binding.pImmutableSamplers = nullptr; // Optional

	//Color texture sampler binding
	VkDescriptorSetLayoutBinding color_sampler_Binding{};
	color_sampler_Binding.binding = 1;
	color_sampler_Binding.descriptorCount = 1;
	color_sampler_Binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	color_sampler_Binding.pImmutableSamplers = nullptr;
	color_sampler_Binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	//COMPILE BINDINGS
	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { transform_ubo_Binding, color_sampler_Binding };
	
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(*this->vkdevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
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
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
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
		descriptorSetLayout,
		newpipelineLayout,
		newgraphicsPipeline,
		asset
	};
}

void gbe::gfx::ShaderLoader::UnLoadAsset_(asset::Shader* asset, const asset::data::ShaderImportData& importdata, asset::data::ShaderLoadData* data) {
	auto shaderdata = this->GetAssetData(asset);
	vkDestroyDescriptorSetLayout(*this->vkdevice, shaderdata.descriptorSetLayout, nullptr);
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