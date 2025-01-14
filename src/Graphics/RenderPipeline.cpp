#define GLM_ENABLE_EXPERIMENTAL
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "RenderPipeline.h"

using namespace gbe;

gbe::RenderPipeline* gbe::RenderPipeline::Instance;

gbe::RenderPipeline* gbe::RenderPipeline::Get_Instance() {
	return gbe::RenderPipeline::Instance;
}

void gbe::RenderPipeline::Init() {
	this->Instance = this;
}

gbe::RenderPipeline::RenderPipeline(void* (*procaddressfunc)(const char*), Vector2Int dimensions)
{
	this->resolution = dimensions;

	auto thing = procaddressfunc("glGetString");
	if (gladLoadGLLoader(procaddressfunc) == 0)
		throw "ERROR: glad failed to initialize.";

	//Asset Loaders
	this->textureloader.AssignSelfAsLoader();
	this->shaderloader.AssignSelfAsLoader();

	//Framebuffers setup
	mFrameBuffer = new Framebuffer(dimensions);
	mDepthFrameBuffer = new Framebuffer(dimensions);

	//Shaders
	this->depth_shader.Assign(new asset::Shader("DefaultAssets/Shaders/depth.shader.gbe"));
}

void RenderPipeline::SetCameraShader(asset::Shader* camshader) {
	this->camera_shader.Assign(camshader);
}
bool RenderPipeline::TryPushLight(gfx::Light* data, bool priority) {

	if (this->lights_this_frame.size() == this->maxlights)
		return false;

	if (priority) {
		this->lights_this_frame.push_front(data);
		return true;
	}

	this->lights_this_frame.push_back(data);
	return true;
}

void gbe::RenderPipeline::SetResolution(Vector2Int newresolution) {
	this->resolution = newresolution;

	mFrameBuffer = new Framebuffer(newresolution);
	mDepthFrameBuffer = new Framebuffer(newresolution);
}

void gbe::RenderPipeline::RenderFrame(Vector3& from, const Vector3& forward, Matrix4& _frustrum, float& nearclip, float& farclip)
{
#pragma region Rendering
	//PRIVATE MEMBERS NEEDED BY LOCAL FUNCTION
	auto& lights_mframe = this->lights_this_frame;
	auto& tex_loader = this->textureloader;
	auto& shader_loader = this->shaderloader;

	/* =============== Render here =============== */
	const auto commit_object = [](std::function<void()> draw_func) {
		draw_func();
		TextureLoader::Reset_texture_stack();
		};

	const auto render_scene_to_active_buffer = [&](Matrix4 tmat_PV, asset::Shader* overrideShader = nullptr) {
		//Loop through all objects
		for (auto& drawcall : drawcalls)
		{
			auto curshader = drawcall->m_material->m_shader.Get_asset();

			//Enable chosen shader program
			if (overrideShader != nullptr)
				curshader = overrideShader;

			glUseProgram(curshader->Get_gl_id());

			//PASS MATERIAL OVERRIDES
			if (overrideShader == nullptr) {
				//LOAD ALL TEXTURES INTO RENDER STACK
				for (int i = 0; i < drawcall->m_material->textureOverrides.size(); i++)
				{
					auto& textureOverride = drawcall->m_material->textureOverrides[i];

					curshader->SetTextureOverride(textureOverride.parameterName, textureOverride.textureRef.Get_asset());
				}
				//LOCK RENDER STACK AT THIS INDEX FOR REUSING
				TextureLoader::Lock_stack();

				for (auto& iterator : drawcall->m_material->overrides) {
					switch (iterator.second.type)
					{
					case BOOL:
						curshader->SetOverride(iterator.first, iterator.second.value_bool);
						break;
					case FLOAT:
						curshader->SetOverride(iterator.first, iterator.second.value_float);
						break;
					case VEC2:
						curshader->SetOverride(iterator.first, iterator.second.value_vec2);
						break;
					case VEC3:
						curshader->SetOverride(iterator.first, iterator.second.value_vec3);
						break;
					case MAT4:
						curshader->SetOverride(iterator.first, iterator.second.value_mat4);
						break;
					default:
						break;
					}
				}
			}

			//Pass the necessary CPU-computed data to the object shader
			for (auto& call : drawcall->calls) {

				//check if shader has light array
				auto checkloc = glGetUniformLocation(curshader->Get_gl_id(), "lights[0].enabled");

				if (checkloc >= 0) {
					size_t index = 0;
					int group_index = 0;
					for (auto lightdata : lights_mframe)
					{
						if (lightdata->changed == false)
							continue;

						auto index_fix = [&index]() {return "[" + std::to_string(index) + "]"; };

						if (lightdata->GetType() == Light::DIRECTION) {
							auto dirlight = (DirLight*)lightdata;

							auto prev_split = 0.0f;

							for (size_t i = 0; i < dirlight->shadowmaps.size(); i++)
							{
								auto next_split = 1.0f;

								if (i < dirlight->cascade_splits.size())
									next_split = dirlight->cascade_splits[i];

								prev_split = next_split;

								curshader->SetOverride(("lights" + index_fix() + ".enabled").c_str(), true);
								curshader->SetOverride(("lights" + index_fix() + ".group_id").c_str(), group_index);
								curshader->SetOverride(("lights" + index_fix() + ".type").c_str(), (int)Light::DIRECTION);
								curshader->SetOverride(("lights" + index_fix() + ".color").c_str(), (Vector3)(dirlight->color * dirlight->intensity));
								curshader->SetOverride(("lights" + index_fix() + ".dir").c_str(), dirlight->dir);


								curshader->SetOverride(("light_view_matrixes" + index_fix()).c_str(), dirlight->cascade_projections[i]);

								curshader->SetTextureIdOverride(("lightviews" + index_fix()).c_str(), dirlight->shadowmaps[i]->outputId);

								index++;
							}
						}

						group_index++;
					}
					for (size_t cont = index; cont < 10; cont++)
					{
						std::string index_fix = "[" + std::to_string(cont) + "]";
						curshader->SetOverride(("lights" + index_fix + ".enabled").c_str(), false);
					}
				}

				//Transform data
				Matrix4 tmat = call.second;
				curshader->SetOverride("transform_model", tmat);
				curshader->SetOverride("transform_projection", tmat_PV);
				curshader->SetOverride("cameraPos", from);
				curshader->SetOverride("near_clip", nearclip);
				curshader->SetOverride("far_clip", farclip);

				//Draw the current object
				glBindVertexArray(drawcall->m_mesh->VAO);
				commit_object([drawcall]() {
					glDrawArrays(GL_TRIANGLES, 0, (GLsizei)drawcall->m_mesh->fullVertexData.size() / 8);
					});
			}
		}

		//UNLOCK RENDER STACK AT THIS INDEX FOR NEXT BATCH OF OVERRIDES
		TextureLoader::UnLock_stack();

		};

	auto SelectBuffer = [=, &lights_mframe](Framebuffer* buffer) {
		glViewport(0, 0, buffer->dimensions.x, buffer->dimensions.y);

		//Initialize the buffer
		glBindFramebuffer(GL_FRAMEBUFFER, buffer->id);
		glEnable(GL_DEPTH_TEST); //Enable depthtest again
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		};

	auto DeSelectBuffer = []() {
		//De-initialize the current frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default render target
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		};

	/*SHADOW PASS*/

	const auto get_frustrum_corners = [_frustrum]() {
		const auto inv = glm::inverse(_frustrum);

		std::vector<Vector4> frustumCorners;
		for (unsigned int z = 0; z < 2; ++z)
			for (unsigned int y = 0; y < 2; ++y)
				for (unsigned int x = 0; x < 2; ++x)
				{
					const glm::vec4 pt =
						inv * glm::vec4(
							2.0f * x - 1.0f,
							2.0f * y - 1.0f,
							2.0f * z - 1.0f,
							1.0f);
					frustumCorners.push_back(pt / pt.w);
				}
		return frustumCorners;
		};

	const auto get_frustrum_center = [](std::vector<Vector4> corners) {
		glm::vec3 center = glm::vec3(0, 0, 0);
		for (const auto& v : corners)
		{
			center += glm::vec3(v);
		}
		center /= corners.size();

		return center;
		};

	std::vector<Vector4> frustrum_corners = get_frustrum_corners();
	auto frustrum_basis = std::vector<Vector3>();

	for (size_t i = 0; i < 4; i++)
	{
		frustrum_basis.push_back(-(Vector3)(frustrum_corners[i] - frustrum_corners[i + 4]));
	}

	auto get_subfrustrum_corners = [frustrum_basis, frustrum_corners](float from, float to) {
		auto subfrustrum_corners = std::vector<Vector4>();

		for (size_t i = 0; i < 4; i++)
		{
			Vector3 delta = frustrum_basis[i] * from;
			Vector3 final = (Vector3)frustrum_corners[i] + delta;
			subfrustrum_corners.push_back(Vector4(final.x, final.y, final.z, 1.0f));
		}

		for (size_t i = 0; i < 4; i++)
		{
			Vector3 delta = frustrum_basis[i] * to;
			Vector3 final = (Vector3)frustrum_corners[i] + delta;
			subfrustrum_corners.push_back(Vector4(final.x, final.y, final.z, 1.0f));
		}

		return subfrustrum_corners;
		};
	auto frustrum_center = get_frustrum_center(frustrum_corners);

	//DEBUG
	DirLight* mainlight;

	for (auto light : lights_this_frame)
	{
		if (light->GetType() == Light::DIRECTION) {
			auto dirlight = static_cast<DirLight*>(light);

			auto backtrack_dist = 20.0f;
			auto overshoot_dist = 100.0f;

			auto prev_split = 0.0f;
			Matrix4 higherResolution_avoid;

			for (size_t i = 0; i < dirlight->shadowmaps.size(); i++)
			{
				auto next_split = 1.0f;

				if (i < dirlight->cascade_splits.size())
					next_split = dirlight->cascade_splits[i];

				auto subfrustrum_corners = get_subfrustrum_corners(prev_split, next_split);

				if (i < dirlight->cascade_splits.size())
					prev_split = dirlight->cascade_splits[i];

				Matrix4 lightView = glm::lookAt(
					frustrum_center - (dirlight->dir * backtrack_dist),
					frustrum_center,
					forward
				);

				float minX = std::numeric_limits<float>::max();
				float maxX = std::numeric_limits<float>::lowest();
				float minY = std::numeric_limits<float>::max();
				float maxY = std::numeric_limits<float>::lowest();
				float maxZ = std::numeric_limits<float>::lowest();
				for (const auto& v : subfrustrum_corners)
				{
					const auto trf = lightView * v;
					minX = std::min(minX, trf.x);
					maxX = std::max(maxX, trf.x);
					minY = std::min(minY, trf.y);
					maxY = std::max(maxY, trf.y);
					maxZ = std::max(maxZ, trf.z);
				}

				const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, 0.0f, maxZ + overshoot_dist);

				dirlight->cascade_projections[i] = lightProjection * lightView;

				SelectBuffer(dirlight->shadowmaps[i]);
				if (i > 0) {
					depth_shader.Get_asset()->SetOverride("avoid_enabled", true);
					depth_shader.Get_asset()->SetOverride("avoid_matrix", higherResolution_avoid);
				}

				render_scene_to_active_buffer(lightProjection * lightView, depth_shader.Get_asset());

				depth_shader.Get_asset()->SetOverride("avoid_enabled", false);

				DeSelectBuffer();

				higherResolution_avoid = lightProjection * lightView;

				//DEBUG
				mainlight = dirlight;
			}
		}
	}

	/*MAIN PASS*/

	//Draw to the main display buffer
	SelectBuffer(mFrameBuffer);
	render_scene_to_active_buffer(_frustrum);
	DeSelectBuffer();
	
	//Draw to the depth buffer using a depth shader
	SelectBuffer(mDepthFrameBuffer);
	render_scene_to_active_buffer(_frustrum, depth_shader.Get_asset());

	DeSelectBuffer();

	//Assign camera shader as post-processing
	auto camShader = camera_shader.Get_asset();
	glUseProgram(camShader->Get_gl_id());
	glBindVertexArray(mFrameBuffer->quadVAO);
	glDisable(GL_DEPTH_TEST); //Temporarily disable depth test

	//Attach the color texture to the post-process shader
	camShader->SetTextureIdOverride("colorBufferTexture", mFrameBuffer->outputId);
	camShader->SetTextureIdOverride("depthBufferTexture", mDepthFrameBuffer->outputId);

	commit_object([]() {
		glDrawArrays(GL_TRIANGLES, 0, 6);
		});
	this->lights_this_frame.clear();
#pragma endregion
}

void gbe::RenderPipeline::RegisterDrawCall(DrawCall* drawcall)
{
	this->drawcalls.push_back(drawcall);
}

void gbe::RenderPipeline::CleanUp()
{
	for (size_t o_i = 0; o_i < drawcalls.size(); o_i++)
	{
		glDeleteVertexArrays(1, &drawcalls[o_i]->m_mesh->VAO);
		glDeleteBuffers(1, &drawcalls[o_i]->m_mesh->VBO);
	}
}
